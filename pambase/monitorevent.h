#pragma once

#include <wx/event.h>
#include "dlldefine.h"
#include <vector>

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT,wxEVT_MONITOR_REQUEST,-1)

class PAMBASE_IMPEXPORT MonitorEvent : public wxCommandEvent
{

public:
    /** @brief	Constructor
    *   @param commandType should be wxEVT_NI...
    *   @param id the ID of the button list control
    **/
    MonitorEvent(const std::vector<char>& vChannels);

    MonitorEvent();
    /** @brief	Copy Constructor
    *   @param event a wxNIEvent
    **/
    MonitorEvent(const MonitorEvent& event);

    /** @brief	Destructor
    **/
    virtual ~MonitorEvent(){}

    /** @brief	Creates a copy of the wxNIEvent
    *   @return <i>wxNIEvent</i>
    **/
    virtual wxEvent *Clone() const { return new MonitorEvent(*this); }

    DECLARE_DYNAMIC_CLASS(MonitorEvent)

    const std::vector<char>& GetChannels() const;

protected:
    std::vector<char> m_vChannels;
};

typedef void (wxEvtHandler::*MonitorEventFunction)(MonitorEvent&);




#define MonitorEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(MonitorEventFunction, &func)

//#define EVT_NI(id,func) wx__DECLARE_EVT0(wxEVT_NI, wxNIEventHandler(func))
#define EVT_MONITOR_EVENT(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_MONITOR_REQUEST,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (MonitorEventFunction) &fn, (wxObject*) NULL)


