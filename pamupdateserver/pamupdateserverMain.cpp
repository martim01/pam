/***************************************************************
 * Name:      pamupdateserverMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-11
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pamupdateserverMain.h"
#include <wx/msgdlg.h>
#include <microhttpd.h>
#include <wx/dir.h>
#include <wx/sstream.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <wx/file.h>


//(*InternalHeaders(pamupdateserverDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

const wxString pamupdateserverDialog::STR_FNF = wxT("<html><head><title>File not found</title></head><body>File not found</body></html>");
const wxString pamupdateserverDialog::STR_FNO = wxT("<html><head><title>File not opened</title></head><body>File not opened</body></html>");
const wxString pamupdateserverDialog::STR_PNF = wxT("<html><head><title>Page not found</title></head><body>Page not found</body></html>");


int ahc_echo (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr)
{

    pamupdateserverDialog* pDlg = reinterpret_cast<pamupdateserverDialog*>(cls);
    return pDlg->Connection(connection, wxString::FromAscii(url), wxString::FromAscii(method), wxString::FromAscii(version), ptr);
}

static ssize_t file_reader (void *cls, uint64_t pos, char *buf, size_t nMax)
{
    wxLogDebug(wxT("Read %d"), nMax);
    wxFile* pFile = reinterpret_cast<wxFile*>(cls);

    pFile->Seek(pos);
    return pFile->Read(buf,nMax);
}


static void free_callback (void *cls)
{
  wxFile* pFile = reinterpret_cast<wxFile*>(cls);
  pFile->Close();
  delete pFile;

}



//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(pamupdateserverDialog)
const long pamupdateserverDialog::ID_LISTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pamupdateserverDialog,wxDialog)
    //(*EventTable(pamupdateserverDialog)
    //*)
END_EVENT_TABLE()

pamupdateserverDialog::pamupdateserverDialog(wxWindow* parent,wxWindowID id) : m_sPath(wxT(".")), m_nPort(80)
{
    //(*Initialize(pamupdateserverDialog)
    Create(parent, id, _("PAM Update Web Server"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
    m_plstLog = new wxListCtrl(this, ID_LISTCTRL1, wxPoint(160,256), wxDefaultSize, wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
    //*)

    m_plstLog->InsertColumn(0, wxT("Time"), wxLIST_FORMAT_LEFT, 200);
    m_plstLog->InsertColumn(1, wxT("Message"), wxLIST_FORMAT_LEFT, 400);

    m_pmhd = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, m_nPort, NULL, NULL, &ahc_echo, this, MHD_OPTION_END);
}

pamupdateserverDialog::~pamupdateserverDialog()
{
    //(*Destroy(pamupdateserverDialog)
    //*)
}

void pamupdateserverDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void pamupdateserverDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


int pamupdateserverDialog::Connection(struct MHD_Connection *pConnection, const wxString& sUrl, const wxString& sMethod, const wxString& sVersion, void** ptr)
{
    Log(wxString::Format(wxT("Connection '%s' Method '%s' Version '%s'"), sUrl.c_str(), sMethod.c_str(), sVersion.c_str()));
    if (sMethod.CmpNoCase(wxT("GET")) != 0)
    {
        Log(wxString::Format(wxT("Invalid connection method %s"), sMethod.c_str()));
        return MHD_NO;              /* unexpected method */
    }

    if (&m_nAPtr != *ptr)
    {
        /* do never respond on first call */
        *ptr = &m_nAPtr;
        return MHD_YES;
    }


    if(sUrl.BeforeLast(wxT('/')).CmpNoCase(wxT("/updatelist")) == 0)
    {
        return SendUpdateList(pConnection, sUrl.AfterLast(wxT('/')), ptr);
    }
    else if(sUrl.BeforeLast(wxT('/')).CmpNoCase(wxT("/update")) == 0)
    {
        return SendUpdateFile(pConnection, sUrl.AfterLast(wxT('/')), ptr);
    }

    return SendNotFoundError(pConnection, ptr, STR_PNF);
}


int pamupdateserverDialog::SendUpdateList(MHD_Connection *pConnection, const wxString& sPlatform, void** ptr)
{
    wxString sXml(GetUpdateManifests(sPlatform).mb_str());

    Log(wxString::Format(wxT("Send update list for platform '%s'"), sPlatform.c_str()));



    std::string sPost(sXml);
    const char *pResponse = sPost.c_str();

    MHD_Response *response;

    *ptr = NULL;                  /* reset when done */
    response = MHD_create_response_from_buffer (strlen (pResponse), (void *) pResponse, MHD_RESPMEM_PERSISTENT);
    int nResult = MHD_queue_response (pConnection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);
    return nResult;
}

int pamupdateserverDialog::SendUpdateFile(MHD_Connection *pConnection, const wxString& sPlatform, void** ptr)
{
    wxString sFile(wxString::FromAscii(MHD_lookup_connection_value (pConnection, MHD_GET_ARGUMENT_KIND, "update")));
    wxString sFilePath(wxString::Format(wxT("%s/%s/%s"), m_sPath.c_str(), sPlatform.c_str(), sFile));

    Log(wxString::Format(wxT("Send file '%s' for platform '%s'"), sFile.c_str(), sPlatform.c_str()));

    int nResult(MHD_NO);
    if(wxFileExists(sFilePath) == false)
    {   //File does not exist
        Log(wxString::Format(wxT("ERROR: File not found '%s' for platform '%s'"), sFile.c_str(), sPlatform.c_str()));
        return SendNotFoundError(pConnection, ptr, STR_FNF);
    }
    else
    {
        wxFile* pFile = new wxFile();
        if(pFile->Open(sFilePath) == false)
        {
            delete pFile;
            Log(wxString::Format(wxT("ERROR: File could not be opened '%s' for platform '%s'"), sFile.c_str(), sPlatform.c_str()));
            return SendNotFoundError(pConnection, ptr, STR_FNO);
        }
        else
        {
            Log(wxString::Format(wxT("File '%s opened"), sFile.c_str()));

            MHD_Response* pResponse = MHD_create_response_from_callback (pFile->Length(), 32 * 1024, &file_reader, pFile, &free_callback);
            if (NULL == pResponse)
            {
                Log(wxString::Format(wxT("Could not create response")));
            }
            else
            {
                nResult = MHD_queue_response (pConnection, MHD_HTTP_OK, pResponse);
                MHD_destroy_response (pResponse);
            }
        }
    }
    return nResult;
}

int pamupdateserverDialog::SendNotFoundError(MHD_Connection *pConnection, void** ptr, const wxString& sMessage)
{
    std::string sPost(sMessage.mb_str());
    const char *pResponse = sPost.c_str();

    MHD_Response *response;
    *ptr = NULL;                  /* reset when done */
    response = MHD_create_response_from_buffer (strlen (pResponse), (void *) pResponse, MHD_RESPMEM_PERSISTENT);
    int nResult = MHD_queue_response (pConnection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response (response);
    return nResult;
}


wxString pamupdateserverDialog::GetUpdateManifests(const wxString& sPlatform)
{
    wxArrayString asFiles;
    wxDir::GetAllFiles(wxString::Format(wxT("%s/%s"), m_sPath.c_str(), sPlatform.c_str()), &asFiles, wxT("*_manifest.xml"));

    wxXmlDocument doc;
    doc.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pam")));

    for(int i = 0; i < asFiles.GetCount(); i++)
    {
        AppendManifest(doc.GetRoot(), asFiles[i]);
    }

    wxStringOutputStream oss;
    doc.Save(oss);
    return oss.GetString();
}


void pamupdateserverDialog::AppendManifest(wxXmlNode* pRoot, const wxString& sFile)
{
    wxXmlDocument doc;
    if(doc.Load(sFile) && doc.GetRoot())
    {
        for(wxXmlNode* pNode = doc.GetRoot()->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            if(pNode->GetName().CmpNoCase(wxT("update")) == 0)
            {
                pRoot->AddChild(new wxXmlNode(*pNode));
            }
        }
    }
}


void pamupdateserverDialog::Log(const wxString& sMessage)
{
    int nIndex = m_plstLog->InsertItem(m_plstLog->GetItemCount(), wxDateTime::Now().Format(wxT("%Y-%m-%d %H:%M:%S")));
    m_plstLog->SetItem(nIndex, 1, sMessage);

    if(m_plstLog->GetItemCount() > 100000)
    {
        m_plstLog->DeleteItem(0);
    }
}
