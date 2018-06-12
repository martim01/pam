/***************************************************************
 * Name:      pamupdateserverMain.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-11
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PAMUPDATESERVERMAIN_H
#define PAMUPDATESERVERMAIN_H

//(*Headers(pamupdateserverDialog)
#include <wx/listctrl.h>
#include <wx/dialog.h>
//*)

#include <wx/xml/xml.h>


struct MHD_Daemon;

static int ahc_echo (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr);

class pamupdateserverDialog: public wxDialog
{
    public:

        pamupdateserverDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~pamupdateserverDialog();

        int Connection(MHD_Connection *pConnection, const wxString& sUrl, const wxString& sMethod, const wxString& sVersion, void** ptr);

    private:

        //(*Handlers(pamupdateserverDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        int SendUpdateList(MHD_Connection *pConnection, const wxString& sPlatform, void** ptr);
        int SendUpdateFile(MHD_Connection *pConnection, const wxString& sPlatform, void** ptr);
        int SendNotFoundError(MHD_Connection *pConnection, void** ptr, const wxString& sMessage);


        wxString GetUpdateManifests(const wxString& sPlatform);
        void AppendManifest(wxXmlNode* pRoot, const wxString& sFile);


        void Log(const wxString& sMessage);

        //(*Identifiers(pamupdateserverDialog)
        static const long ID_LISTCTRL1;
        //*)

        //(*Declarations(pamupdateserverDialog)
        wxListCtrl* m_plstLog;
        //*)

        DECLARE_EVENT_TABLE()

        wxString m_sPath;
        unsigned long m_nPort;
        int m_nAPtr;
        MHD_Daemon* m_pmhd;

        static const wxString STR_FNF;
        static const wxString STR_FNO;
        static const wxString STR_PNF;
};

#endif // PAMUPDATESERVERMAIN_H
