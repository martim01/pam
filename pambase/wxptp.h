#pragma once
#include <wx/event.h>
#include <wx/string.h>
#include <map>
#include <memory>
#include <list>
#include "dlldefine.h"

#ifdef PTPMONKEY
class PtpMonkey;
class wxPtpEventHandler;

class PAMBASE_IMPEXPORT wxPtp : public wxEvtHandler
{
    public:
        static wxPtp& Get();

        void RunDomain(const wxString& sIpAddress, unsigned char nDomain);
        void StopDomain(unsigned char nDomain);

        wxString GetMasterClockId(unsigned char nDomain);
        timeval GetPtpTime(unsigned char nDomain);
        timeval GetPtpOffset(unsigned char nDomain);
        std::shared_ptr<PtpMonkey> GetPtpMonkey(nDomain);
        void AddHandler(wxEvtHandler* pHandler);

    private:
        wxPtp();
        std::shared_ptr<wxPtpEventHandler> m_pNotifier;
        std::map<unsigned char, std::shared_ptr<PtpMonkey>> m_mDomain;
};


wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_ADDED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_UPDATED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_REMOVED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_TIME,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MASTER,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_SLAVE,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_REMOVED,wxCommandEvent);
#endif



