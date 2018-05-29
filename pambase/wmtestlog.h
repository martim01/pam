#pragma once

#include <wx/event.h>
#include <wx/datetime.h>
#include "dlldefine.h"
#include <map>


class wxFile;
/** Singleton class that is called to send wmLogEvents
**/
class PAMBASE_IMPEXPORT wmTestLog
{
    public:
        static wmTestLog& Get();
        void SetTarget(wxEvtHandler* pHandler);
        void Log(const wxString& sTest, const wxString& sMessage, bool bSend=true);
        ~wmTestLog();

    private:
        wmTestLog() : m_pHandler(0){}
        std::map<wxString, wxFile*>::iterator OpenLogFile(const wxString& sTest, bool bOpen);
        wxEvtHandler* m_pHandler;

        std::map<wxString, wxFile*> m_mFileLog;
        wxDateTime m_dtLog;
};

