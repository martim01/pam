#pragma once

#include <wx/event.h>
#include <wx/timer.h>
#include <wx/datetime.h>
#include "dlldefine.h"
#include <queue>
#include <wx/thread.h>
class wxFile;

class PAMBASE_IMPEXPORT wmLogEvent : public wxCommandEvent
{

public:
    /** Constructor
    *   @param commandType should be wxEVT_NI
    *   @param id the ID of the button list control
    **/
    wmLogEvent(const wxString& sDevice, const wxString& sMessage, int nType, bool bDebug=false);

    wmLogEvent() : wxCommandEvent(), m_bDebug(false){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/
    wmLogEvent(const wmLogEvent& event);

    /** Destructor
    **/
    ~wmLogEvent(){}

    /** Creates a copy of the wmLogEvent
    *   @return <i>wmLogEvent</i>
    **/
    virtual wxEvent *Clone() const { return new wmLogEvent(*this); }

    const wxDateTime& GetTimestamp() const;
    const wxString& GetDevice() const;
    const wxString& GetLogMessage() const;
    bool IsDebug() const;
    int GetLogType() const;

    wxDECLARE_DYNAMIC_CLASS(wmLogEvent);



private:
    wxDateTime m_dtTimestamp;
    wxString m_sDevice;
    wxString m_sMessage;
    bool m_bDebug;

};


/** Singleton class that is called to send wmLogEvents
**/
class PAMBASE_IMPEXPORT wmLog : public wxEvtHandler
{
    public:
        enum {LOG_SYSTEM=1, LOG_TEST_INFO=2, LOG_TEST_ALARM=4, LOG_TEST_OK=8};
        static wmLog* Get();
        void SetTarget(wxEvtHandler* pHandler);
        void Log(wxString sDevice, wxString sMessage, int nType=LOG_SYSTEM, bool bSend=true);
        void Log(const wxString& sMessage, int nType=LOG_SYSTEM);


    private:
        ~wmLog();
        wmLog();
        void OpenLogFile(bool bOpen);

        void OnTimerSave(wxTimerEvent& event);
        wxEvtHandler* m_pHandler;

        std::queue<wmLogEvent*> m_queueEvents;
        wxFile* m_pFileLog;
        wxDateTime m_dtLog;

        std::queue<wxString> m_queueFile;
        wxTimer m_timerSave;

        wxMutex m_mutex;
};


typedef void (wxEvtHandler::*wmLogEventFunction)(wmLogEvent&);


BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT,wxEVT_WMLOG,-1)
END_DECLARE_EVENT_TYPES()

#define wmLogEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wmLogEventFunction, &func)

//#define EVT_NI(id,func) wx__DECLARE_EVT0(wxEVT_NI, wxNIEventHandler(func))
#define EVT_WMLOG(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_WMLOG,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wmLogEventFunction) &fn, (wxObject*) NULL),



