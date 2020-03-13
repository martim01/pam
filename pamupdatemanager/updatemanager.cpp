#include "updatemanager.h"
#include "settings.h"
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/filename.h>
#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"
#include <wx/dir.h>


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
    m_mUpdates.clear();
    if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("HTTP"))
    {
        // @todo add the profiles
        return GetUpdateListFromWebServer();
    }
    else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("Share"))
    {
        return GetUpdateListFromShare();
    }
    else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("USB"))
    {
        return GetUpdateListFromUSB();
    }
    else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("Local"))
    {
        return GetUpdateListFromLocal(Settings::Get().Read(wxT("Update"), wxT("Local"), wxT(".")));
    }
    else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("FTP"))
    {
        // @todo add the profiles and the rest...
        return GetUpdateListFromFTP();
    }

    return false;
}
bool UpdateManager::GetUpdateListFromFTP()
{
    return false;
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
        #ifdef __WXMSW__
        sPage.Printf(wxT("%s:8080/updatelist/windows"), Settings::Get().Read(wxT("Update"), wxT("Server"), wxT("127.0.0.1")).c_str());
        #else
        sPage.Printf(wxT("%s:8080/updatelist/linux"), Settings::Get().Read(wxT("Update"), wxT("Server"), wxT("127.0.0.1")).c_str());
        #endif

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
    //unmount any previous share
#ifdef __WXGNU__
    if(wxDirExists(wxT("/mnt/share")) == false)
    {
        wxMkdir(wxT("/mnt/share"));
    }
    wxExecute(wxT("sudo umount /mnt/share"), wxEXEC_SYNC);
    wxExecute(wxString::Format(wxT("sudo mount -t cifs -o user=pam,password=10653045 %s /mnt/share"), Settings::Get().Read(wxT("Update"), wxT("Share"), wxEmptyString)), wxEXEC_SYNC);

    return GetUpdateListFromLocal(wxT("/mnt/share"));
#else
    return DecodeUpdateList(wxString::Format(wxT("%s/manifest.xml"), Settings::Get().Read(wxT("Update"), wxT("Share"), wxT(".")).c_str()));
#endif
}

bool UpdateManager::GetUpdateListFromUSB()
{
    //unmount any previous share
#ifdef __WXGNU__
    if(wxDirExists(wxT("/mnt/share")) == false)
    {
        wxMkdir(wxT("/mnt/share"));
    }
    wxExecute(wxT("sudo umount /mnt/share"), wxEXEC_SYNC);
    wxExecute(wxString::Format(wxT("sudo mount -o umask=000 /dev/%s /mnt/share"), Settings::Get().Read(wxT("Update"), wxT("USB"), wxEmptyString)), wxEXEC_SYNC);

    return GetUpdateListFromLocal(wxT("/mnt/share"));
#else
    return DecodeUpdateList(wxString::Format(wxT("%s/manifest.xml"), Settings::Get().Read(wxT("Update"), wxT("Share"), wxT(".")).c_str()));
#endif
}

bool UpdateManager::GetUpdateListFromLocal(const wxString& sPath)
{
    wxArrayString asFiles;
    wxDir::GetAllFiles(sPath, &asFiles, wxT("*_manifest.xml"));
    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        DecodeUpdateList(asFiles[i]);
    }

    UpdateObject ppmObject;
    ppmObject.sName = wxT("ppmtypes");
    ppmObject.nType = UpdateObject::CONFIG;
    m_mUpdates.insert(make_pair(ppmObject.sName, ppmObject));


    UpdateObject aoipObject;
    aoipObject.sName = wxT("aoipsources");
    aoipObject.nType = UpdateObject::CONFIG;
    m_mUpdates.insert(make_pair(aoipObject.sName, aoipObject));

    //Add the profiles
    wxArrayString asProfiles;
    wxDir::GetAllFiles(sPath, &asProfiles, wxT("*.ini"));
    for(size_t i = 0; i < asProfiles.GetCount(); i++)
    {
        wxFileName fn(asProfiles[i]);
        UpdateObject anObject;
        anObject.sName = fn.GetName();
        anObject.nType = UpdateObject::PROFILES;
        m_mUpdates.insert(make_pair(anObject.sName, anObject));
    }


    return true;
}

bool UpdateManager::DecodeUpdateList(const wxXmlDocument& xmlDoc)
{
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
    else
    {
        wxLogDebug(wxT("UpdateManager Xml is not correct"));
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
            else if(pNode->GetNodeContent().CmpNoCase(wxT("lib")) == 0)
            {
                anUpdate.nType = UpdateObject::PLUGIN_MONITOR;
            }
            else if(pNode->GetNodeContent().CmpNoCase(wxT("docs")) == 0)
            {
                anUpdate.nType = UpdateObject::DOCUMENTATION;
            }
            else
            {
                anUpdate.nType = UpdateObject::UNKNOWN;
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
    wxString sName, sVersion;
    for(wxXmlNode* pNode = pDepNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase(wxT("name")) == 0)
        {
            sName = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("version")) == 0)
        {
            sVersion = pNode->GetNodeContent();
        }
    }
    if(sName.empty() == false)
    {

        anObject.mDependsOn.insert(make_pair(sName, sVersion));
    }
}



bool UpdateManager::Update(wxString sName)
{
    //wxLogNull ln;

    wxFileName fileOld;

    map<wxString, UpdateObject>::iterator itUpdate = m_mUpdates.find(sName);
    if(itUpdate != m_mUpdates.end())
    {
        switch(itUpdate->second.nType)
        {
            case UpdateObject::APP:
                #ifdef __WXMSW__
                sName << wxT(".exe");
                #endif // __WXMSW__

                fileOld.SetPath(Settings::Get().GetExecutableDirectory());
                fileOld.SetName(sName);

                break;
            case UpdateObject::CORE_DLL:
                #ifdef __WXMSW__
                sName << wxT(".dll");
                #else
                    #ifdef __WXGNU__
                    sName  = wxT("lib")+sName+wxT(".so");
                    #endif // __WXGNU__
                #endif // __WXMSW__

                fileOld.SetPath(Settings::Get().GetCoreLibDirectory());
                fileOld.SetName(sName);
                break;

            case UpdateObject::PLUGIN_MONITOR:
                #ifdef __WXMSW__
                sName << wxT(".dll");
                #else
                #ifdef __WXGNU__
                sName  = wxT("lib")+sName+wxT(".so");
                #endif
                #endif // __WXMSW__

                fileOld.SetPath(Settings::Get().GetMonitorPluginDirectory());
                fileOld.SetName(sName);
                break;
            case UpdateObject::PLUGIN_TEST:
                #ifdef __WXMSW__
                sName << wxT(".dll");
                #else
                #ifdef __WXGNU__
                sName  = wxT("lib")+sName+wxT(".so");
                #endif // __WXGNU__
                #endif // __WXMSW__
                fileOld.SetPath(Settings::Get().GetTestPluginDirectory());
                fileOld.SetName(sName);
                break;
            case UpdateObject::DOCUMENTATION:
                sName << wxT(".html");
                fileOld.SetPath(Settings::Get().GetDocumentDirectory());
                fileOld.SetName(sName);
                break;
            case UpdateObject::PROFILES:
                sName << wxT(".ini");
                fileOld.SetPath(Settings::Get().GetDocumentDirectory());
                fileOld.SetName(sName);
                break;
            case UpdateObject::CONFIG:
                sName << wxT(".xml");
                fileOld.SetPath(Settings::Get().GetDocumentDirectory());
                fileOld.SetName(sName);
                break;
            case UpdateObject::UNKNOWN:
                fileOld.SetPath(Settings::Get().GetDocumentDirectory());
                fileOld.SetName(sName);
                break;


        }
        wxFileName fileTo(Settings::Get().GetTempDirectory(), sName);

        bool bCopied(false);
        if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("HTTP"))
        {
             bCopied = UpdateFromWebServer(sName, fileTo.GetFullPath());
        }
        else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("Share"))
        {
            bCopied = UpdateFromShare(sName, fileTo.GetFullPath(), itUpdate->second.nType);
        }
        else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Share")) == wxT("Local"))
        {
            bCopied = UpdateFromLocal(sName, fileTo.GetFullPath(), itUpdate->second.nType);
        }

        if(bCopied)
        {
            if(!wxFileExists(fileOld.GetFullPath()) || wxRenameFile(fileOld.GetFullPath(), wxString::Format(wxT("%s.bak"), fileOld.GetFullPath().c_str())))
            {
                if(wxRenameFile(fileTo.GetFullPath(), fileOld.GetFullPath()))
                {
                    wxString sRemove(Settings::Get().Read(wxT("Startup"), wxT("Remove"), wxEmptyString));
                    if(sRemove.empty() == false)
                    {
                        sRemove << wxT(",");
                    }
                    sRemove << wxString::Format(wxT("%s.bak"), fileOld.GetFullPath().c_str());
                    Settings::Get().Write(wxT("Startup"), wxT("Remove"), sRemove);
                    return true;
                }
                else
                {
                    wxRenameFile(wxString::Format(wxT("%s.bak"), fileOld.GetFullPath().c_str()), fileOld.GetFullPath());
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
        #ifdef __WXMSW__
        sPage.Printf(wxT("%s/update/windows?update=%s"), Settings::Get().Read(wxT("Update"), wxT("Server"), wxT("127.0.0.1")).c_str(), sName.c_str());
        #else
        sPage.Printf(wxT("%s/update/pi?update=%s"), Settings::Get().Read(wxT("Update"), wxT("Server"), wxT("127.0.0.1")).c_str(), sName.c_str());
        #endif

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

                wxLogMessage(m_sCurlError+wxT("\n")+m_sCurlErrorBuffer);
            }
            else
            {
                if(m_nCurlResponseCode != 200)
                {
                    m_sCurlError = wxT("File not found");
                }
            }
            /* close the header file */
            fclose(local_file);
        }
    }
    curl_easy_cleanup(pCurl);

    return ((res == CURLE_OK)&(m_nCurlResponseCode==200));
}

bool UpdateManager::UpdateFromUSB(const wxString& sName, const wxString& sTempFile, int nType)
{
    return UpdateFromShare(sName, sTempFile, nType);
}

bool UpdateManager::UpdateFromShare(const wxString& sName, const wxString& sTempFile, int nType)
{
    wxString sPath(wxT("/mnt/share/"));
    switch(nType)
    {
    case UpdateObject::APP:
        sPath << wxT("bin");
        break;
    case UpdateObject::CORE_DLL:
        sPath << wxT("lib");
        break;
    case UpdateObject::DOCUMENTATION:
        sPath << wxT("docs");
        break;
    case UpdateObject::PLUGIN_MONITOR:
        sPath << wxT("lib/monitor");
        break;
    case UpdateObject::PLUGIN_TEST:
        sPath << wxT("lib/test");
        break;

    }
    wxFileName fileFrom(sPath, sName);
    //Copy the file from the share to our temp location
    return wxCopyFile(fileFrom.GetFullPath(), sTempFile);
}

bool UpdateManager::UpdateFromLocal(const wxString& sName, const wxString& sTempFile, int nType)
{
    wxFileName fileFrom(Settings::Get().Read(wxT("Update"), wxT("Local"), wxT(".")), sName);

    //Copy the file from the share to our temp location
    return wxCopyFile(fileFrom.GetFullPath(), sTempFile);
}


map<wxString, UpdateObject>::const_iterator UpdateManager::GetUpdateListBegin() const
{
    return m_mUpdates.begin();
}

map<wxString, UpdateObject>::const_iterator UpdateManager::GetUpdateListEnd() const
{
    return m_mUpdates.end();
}

wxString UpdateManager::GetChangelog(const wxString& sName) const
{
    map<wxString, UpdateObject>::const_iterator itObject = m_mUpdates.find(sName);
    if(itObject != m_mUpdates.end())
    {
        return itObject->second.sChangelog;
    }
    return wxEmptyString;
}

wxString UpdateManager::GetVersion(const wxString& sName) const
{
    map<wxString, UpdateObject>::const_iterator itObject = m_mUpdates.find(sName);
    if(itObject != m_mUpdates.end())
    {
        return itObject->second.sVersion;
    }
    return wxEmptyString;
}

map<wxString, wxString> UpdateManager::GetDependencies(const wxString& sName)
{
    map<wxString, UpdateObject>::const_iterator itObject = m_mUpdates.find(sName);
    if(itObject != m_mUpdates.end())
    {
        return itObject->second.mDependsOn;
    }
    return map<wxString,wxString>();
}


bool UpdateManager::UpdateIsNewer(const wxString& sUpdate, const wxString& sLocalVersion)
{
    wxString sUpdateVersion(GetVersion(sUpdate));
    return UpdateVersionIsNewer(sUpdateVersion, sLocalVersion);
}

bool UpdateManager::UpdateVersionIsNewer(const wxString& sUpdateVersion, const wxString& sLocalVersion)
{
    //return true;

    wxArrayString asUpdate(wxStringTokenize(sUpdateVersion, wxT(".")));

    wxArrayString asLocal(wxStringTokenize(sLocalVersion, wxT(".")));

    if(asUpdate.GetCount() != 4)
    {
        return false;
    }
    if(asLocal.GetCount() != 4)
    {
        return true;
    }

    unsigned long nUpdate[4];
    unsigned long nLocal[4];

    for(int i = 0; i < 4; i++)
    {
        asUpdate[i].ToULong(&nUpdate[i]);
        asLocal[i].ToULong(&nLocal[i]);
    }

    return ( (nUpdate[0] > nLocal[0]) || (nUpdate[0] == nLocal[0] && nUpdate[1] > nLocal[1]) || (nUpdate[0] == nLocal[0] && nUpdate[1] == nLocal[1] && nUpdate[2] > nLocal[2]) || (nUpdate[0] == nLocal[0] && nUpdate[1] == nLocal[1] && nUpdate[2] == nLocal[2] && nUpdate[3] > nLocal[3]));
}
