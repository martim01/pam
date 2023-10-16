#pragma once
#include <wx/event.h>
#include <wx/string.h>
#include <map>
#include <memory>
#include <list>
#include "dlldefine.h"
#include "ptpmonkey.h"


class wxPtpEventHandler;
class PAMBASE_IMPEXPORT wxPtp : public wxEvtHandler
{
    public:
        static wxPtp& Get();

        void Run(const wxString& sInterface, unsigned char nDomain, pml::ptpmonkey::Mode mode);
        void Stop();
        void ChangeDomain(unsigned char nDomain);

        wxString GetMasterClockId();
        timeval GetPtpTime();
        timespec GetPtpTimeSpec();
        timeval GetPtpOffset();
        timeval GetLastPtpOffset();
        bool IsSyncedToMaster() const;
        void ResyncToMaster();
        std::shared_ptr<const pml::ptpmonkey::PtpV2Clock> GetPtpClock(const wxString& sClockId);
        std::shared_ptr<const pml::ptpmonkey::PtpV2Clock> GetSyncMasterClock();
        std::shared_ptr<const pml::ptpmonkey::PtpV2Clock> GetLocalClock();
        const std::map<std::string, std::shared_ptr<pml::ptpmonkey::PtpV2Clock> >& GetClocks() const;
        void ResetLocalClockStats();
        void AddHandler(wxEvtHandler* pHandler);
        void RemoveHandler(wxEvtHandler* pHandler);

    private:
        wxPtp();
        IpInterface m_interface;
        std::shared_ptr<wxPtpEventHandler> m_pNotifier;
        std::unique_ptr<pml::ptpmonkey::PtpMonkey> m_pMonkey;
        std::map<std::string, std::shared_ptr<pml::ptpmonkey::PtpV2Clock> > m_mEmpty;

};



wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_ADDED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_UPDATED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_REMOVED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_TIME,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MASTER,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_SLAVE,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_REMOVED,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MSG_ANNOUNCE,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MSG_SYNC,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MSG_FOLLOWUP,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MSG_DELAY_REQUEST,wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_CLOCK_MSG_DELAY_RESPONSE,wxCommandEvent);



