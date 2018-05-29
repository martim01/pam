#pragma once

#include <wx/event.h>
#include <wx/datetime.h>
#include "dlldefine.h"

class wxFile;
/** Singleton class that is called to send wmLogEvents
**/
class PAMBASE_IMPEXPORT wmLog
{
    public:
        static wmLog* Get();
        void SetTarget(wxEvtHandler* pHandler);
        void Log(const wxString& sDevice, const wxString& sMessage, bool bSend=true);
        void Log(const wxString& sMessage);


    private:
        ~wmLog();
        wmLog() : m_pHandler(0),m_pFileLog(0){}
        void OpenLogFile(bool bOpen);
        wxEvtHandler* m_pHandler;

        wxFile* m_pFileLog;
        wxDateTime m_dtLog;
};

class PAMBASE_IMPEXPORT wmLogEvent : public wxCommandEvent
{

public:
    /** Constructor
    *   @param commandType should be wxEVT_NI
    *   @param id the ID of the button list control
    **/
    wmLogEvent(const wxString& sDevice, const wxString& sMessage, bool bDebug=false);

    wmLogEvent() : wxCommandEvent(){}

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

    DECLARE_DYNAMIC_CLASS(wmLogEvent)



private:
    wxDateTime m_dtTimestamp;
    wxString m_sDevice;
    wxString m_sMessage;
    bool m_bDebug;

};

typedef void (wxEvtHandler::*wmLogEventFunction)(wmLogEvent&);


BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT,wxEVT_WMLOG,-1)
END_DECLARE_EVENT_TYPES()

#define wmLogEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wmLogEventFunction, &func)

//#define EVT_NI(id,func) wx__DECLARE_EVT0(wxEVT_NI, wxNIEventHandler(func))
#define EVT_WMLOG(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_WMLOG,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wmLogEventFunction) &fn, (wxObject*) NULL),



