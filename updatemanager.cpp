#include "updatemanager.h"
#include "settings.h"
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/filename.h>
#include <wx/dynlib.h>
#include <wx/log.h>
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"



using namespace std;

static size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *pChunk = reinterpret_cast<MemoryStruct*>(userp);

    pChunk->pMemory = reinterpret_cast<char*>(realloc(pChunk->pMemory, pChunk->nSize + realsize + 1));
    if(pChunk->pMemory == NULL)
    {
        /* out of memory! */
        return 0;
    }

    memcpy(&(pChunk->pMemory[pChunk->nSize]), contents, realsize);
    pChunk->nSize += realsize;
    pChunk->pMemory[pChunk->nSize] = 0;

    return realsize;
}


int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr)
{
    #ifdef __WXMSW__
  //  wxLogMessage(wxString::FromAscii(data));
    #endif // __WXMSW__
    //wxLogDebug(wxString::FromAscii(data));
    return size;
}

UpdateManager::UpdateManager()
{

}


UpdateManager::~UpdateManager()
{

}

UpdateManager& UpdateManager::Get()
{
    static UpdateManager um;
    return um;
}

bool UpdateManager::GetUpdateList()
{
    if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("HTTP"))
    {
        return GetUpdateListFromWebServer();
    }
    else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("Share"))
    {
        return GetUpdateListFromShare();
    }
}

bool UpdateManager::GetUpdateListFromWebServer()
{
    CURL *pCurl;
    CURLcode res;
    MemoryStruct chunk;
    char chError[CURL_ERROR_SIZE];

    pCurl = curl_easy_init();
    if(pCurl)
    {
        wxString sPage;
        sPage.Printf(wxT("%s/updatelist"), Settings::Get().Read(wxT("Update"), wxT("Server"), wxT("127.0.0.1")).c_str());


        std::string url(sPage.mb_str());

        curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &chunk);
        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(pCurl, CURLOPT_ERRORBUFFER, chError);
        curl_easy_setopt(pCurl, CURLOPT_DEBUGFUNCTION, debug_callback);
        curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 1);

        if(Settings::Get().Read(wxT("Update"), wxT("Secure"),0) == 1)
        {
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(pCurl, CURLOPT_CAPATH, "./");
        }

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(pCurl);

        m_nCurlResponseCode = 0;
        curl_easy_getinfo (pCurl, CURLINFO_RESPONSE_CODE, &m_nCurlResponseCode);

        /* Check for errors */
        if(res != CURLE_OK)
        {
            //Can't connect to server
            std::string sLastError = curl_easy_strerror(res);
            m_sCurlError = wxString::FromAscii(sLastError.c_str());
            m_sCurlErrorBuffer = wxString::FromAscii(chError);
        }
        else
        {
            std::string sBuff;
            sBuff.assign(chunk.pMemory, chunk.nSize);

            wxStringInputStream sis(wxString::FromAscii(sBuff.c_str()));
            DecodeUpdateList(wxXmlDocument(sis));
            m_sCurlError = wxEmptyString;
            m_sCurlErrorBuffer = wxEmptyString;

        }

        /* always cleanup */
        curl_easy_cleanup(pCurl);
    }
    return (res == CURLE_OK);
}


bool UpdateManager::GetUpdateListFromShare()
{
    return DecodeUpdateList(wxString::Format(wxT("%s/manifest.xml"), Settings::Get().Read(wxT("Update"), wxT("Share"), wxT(".")).c_str()));

}

bool UpdateManager::DecodeUpdateList(const wxXmlDocument& xmlDoc)
{
    m_mUpdates.clear();

    if(xmlDoc.IsOk() && xmlDoc.GetRoot())
    {
        for(wxXmlNode* pNode = xmlDoc.GetRoot()->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            if(pNode->GetName().CmpNoCase(wxT("update")) == 0)
            {
                AddUpdateToList(pNode);
            }
        }
        return true;
    }
    return false;
}


void UpdateManager::AddUpdateToList(wxXmlNode* pUpdateNode)
{
    UpdateObject anUpdate;


    for(wxXmlNode* pNode = pUpdateNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase(wxT("type")) == 0)
        {
            if(pNode->GetNodeContent().CmpNoCase(wxT("app")) == 0)
            {
                anUpdate.nType = UpdateObject::APP;
            }
            else if(pNode->GetNodeContent().CmpNoCase(wxT("core")) == 0)
            {
                anUpdate.nType = UpdateObject::CORE_DLL;
            }
            else if(pNode->GetNodeContent().CmpNoCase(wxT("monitor")) == 0)
            {
                anUpdate.nType = UpdateObject::PLUGIN_MONITOR;
            }
            else if(pNode->GetNodeContent().CmpNoCase(wxT("test")) == 0)
            {
                anUpdate.nType = UpdateObject::PLUGIN_TEST;
            }
            if(pNode->GetNodeContent().CmpNoCase(wxT("docs")) == 0)
            {
                anUpdate.nType = UpdateObject::DOCUMENTATION;
            }
        }
        else if(pNode->GetName().CmpNoCase(wxT("version")) == 0)
        {
            anUpdate.sVersion = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("name")) == 0)
        {
            anUpdate.sName = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("changelog")) == 0)
        {
            anUpdate.sChangelog = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("depends_on")) == 0)
        {
            AddDependencyToList(anUpdate, pNode);
        }
    }

    if(anUpdate.nType != UpdateObject::UNKNOWN)
    {
        m_mUpdates.insert(make_pair(anUpdate.sName, anUpdate));
    }
}

void UpdateManager::AddDependencyToList(UpdateObject& anObject, wxXmlNode* pDepNode)
{
    UpdateObject dependsObject;
    dependsObject.nType = UpdateObject::CORE_DLL;

    for(wxXmlNode* pNode = pDepNode->GetChildren(); pNode; pNode = pNode->GetChildren())
    {
        if(pNode->GetName().CmpNoCase(wxT("name")) == 0)
        {
            dependsObject.sName = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("version")) == 0)
        {
            dependsObject.sVersion = pNode->GetNodeContent();
        }
    }
    anObject.lstDependsOn.push_back(dependsObject);
}



bool UpdateManager::Update(const wxString& sName)
{
    map<wxString, UpdateObject>::iterator itUpdate = m_mUpdates.find(sName);
    if(itUpdate != m_mUpdates.end())
    {
        wxFileName fileTo(Settings::Get().GetTempDirectory(), sName);

        bool bCopied(false);
        if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("HTTP"))
        {
             bCopied = UpdateFromWebServer(sName, fileTo.GetFullPath());
        }
        else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("Share"))
        {
            bCopied = UpdateFromShare(sName, fileTo.GetFullPath());
        }

        if(bCopied)
        {
            wxFileName fileOld;

            switch(itUpdate->second.nType)
            {
                case UpdateObject::APP:
                    fileOld.SetPath(Settings::Get().GetExecutableDirectory());
                    fileOld.SetName(wxT("pam2"));
                    break;
                case UpdateObject::CORE_DLL:
                    fileOld.SetPath(Settings::Get().GetCoreLibDirectory());
                    #ifdef __WXGNU__
                        fileOld.SetName(wxString::Format(wxT("lib%s.so")));
                    #else
                        fileOld.SetName(wxString::Format(wxT("%s.dll"), sName.c_str()));
                    #endif
                    break;
                case UpdateObject::PLUGIN_MONITOR:
                    fileOld.SetPath(Settings::Get().GetMonitorPluginDirectory());
                    #ifdef __WXGNU__
                        fileOld.SetName(wxString::Format(wxT("lib%s.so")));
                    #else
                        fileOld.SetName(wxString::Format(wxT("%s.dll"), sName.c_str()));
                    #endif
                    break;
                case UpdateObject::PLUGIN_TEST:
                    fileOld.SetPath(Settings::Get().GetTestPluginDirectory());
                    #ifdef __WXGNU__
                        fileOld.SetName(wxString::Format(wxT("lib%s.so")));
                    #else
                        fileOld.SetName(wxString::Format(wxT("%s.dll"), sName.c_str()));
                    #endif
                    break;
                case UpdateObject::DOCUMENTATION:
                    fileOld.SetPath(Settings::Get().GetDocumentDirectory());
                    fileOld.SetName(wxString::Format(wxT("%s.html"), sName.c_str()));
                    break;

            }
            if(wxRenameFile(wxString::Format(wxT("%s.bak"), fileOld.GetFullPath().c_str()), fileOld.GetFullPath()))
            {
                if(wxRenameFile(fileTo.GetFullPath(), fileOld.GetFullPath()))
                {
                    wxRemoveFile(wxString::Format(wxT("%s.bak"), fileOld.GetFullPath().c_str()));
                    return true;
                }
                else
                {
                    wxRenameFile(fileOld.GetFullPath(), wxString::Format(wxT("%s.bak"), fileOld.GetFullPath().c_str()));
                    return false;
                }
            }
        }
    }
    return false;
}


bool UpdateManager::UpdateFromWebServer(const wxString& sName, const wxString& sTempFile)
{
    CURL *pCurl;
    CURLcode res;
    FILE *local_file;
    char chError[CURL_ERROR_SIZE];

    pCurl = curl_easy_init();
    if(pCurl)
    {

        wxString sPage;
        sPage.Printf(wxT("%s/update/?update=%s"), Settings::Get().Read(wxT("Update"), wxT("Server"), wxT("127.0.0.1")).c_str(), sName.c_str());

        std::string url(sPage.mb_str());
        std::string local(sTempFile.mb_str());

        curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(pCurl, CURLOPT_ERRORBUFFER, chError);
        curl_easy_setopt(pCurl, CURLOPT_DEBUGFUNCTION, debug_callback);
        curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 1);
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);

        //curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1);


        local_file = fopen(local.c_str(), "wb");
        if(local_file)
        {
            /* write the page body to this file handle */
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, local_file);

            /* get it! */
            res = curl_easy_perform(pCurl);

            m_nCurlResponseCode = 0;
            curl_easy_getinfo (pCurl, CURLINFO_RESPONSE_CODE, &m_nCurlResponseCode);

            /* Check for errors */
            if(res != CURLE_OK)
            {
                //Can't connect to server
                std::string sLastError = curl_easy_strerror(res);
                m_sCurlError = wxString::FromAscii(sLastError.c_str());
                m_sCurlErrorBuffer = wxString::FromAscii(chError);

                //wxMessageBox(m_sCurlError+wxT("\n")+m_sCurlErrorBuffer);

            }
            else
            {
                m_sCurlError = wxEmptyString;
                m_sCurlErrorBuffer = wxEmptyString;
            }
            /* close the header file */
            fclose(local_file);
        }
    }

    curl_easy_cleanup(pCurl);

    return (res == CURLE_OK);
}

bool UpdateManager::UpdateFromShare(const wxString& sName, const wxString& sTempFile)
{
    wxFileName fileFrom(Settings::Get().Read(wxT("Update"), wxT("Share"), wxT(".")), sName);

    //Copy the file from the share to our temp location
    return wxCopyFile(fileFrom.GetFullPath(), sTempFile);
}


