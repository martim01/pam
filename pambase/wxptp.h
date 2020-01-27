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
class PtpV2Clock;

class PAMBASE_IMPEXPORT wxPtp : public wxEvtHandler
{
    public:
        static wxPtp& Get();

        void RunDomain(const wxString& sInterface, unsigned char nDomain);
        void StopDomain(unsigned char nDomain);

        wxString GetMasterClockId(unsigned char nDomain);
        timeval GetPtpTime(unsigned char nDomain);
        timeval GetPtpOffset(unsigned char nDomain);
        std::shared_ptr<const PtpV2Clock> GetPtpClock(unsigned char nDomain, const wxString& sClockId);
        std::shared_ptr<const PtpV2Clock> GetMasterClock(unsigned char nDomain);
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksBegin(unsigned char nDomain) const;
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksEnd(unsigned char nDomain) const;

        void AddHandler(wxEvtHandler* pHandler);

    private:
        wxPtp();
        std::shared_ptr<wxPtpEventHandler> m_pNotifier;
        std::map<unsigned char, std::shared_ptr<PtpMonkey>> m_mDomain;
        std::map<std::string, std::shared_ptr<PtpV2Clock> > m_mEmpty;
};


wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_ADDED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_UPDATED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_REMOVED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_TIME,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MASTER,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_SLAVE,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_REMOVED,wxCommandEvent);
#endif



