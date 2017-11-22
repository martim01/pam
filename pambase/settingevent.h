#pragma once

#include <wx/event.h>
#include "dlldefine.h"

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT,wxEVT_SETTING_CHANGED,-1)

class PAMBASE_IMPEXPORT SettingEvent : public wxCommandEvent
{
    friend class Settings;

public:
    /** @brief	Constructor
    *   @param commandType should be wxEVT_NI...
    *   @param id the ID of the button list control
    **/
    SettingEvent(const wxString& sSection, const wxString& sKey);

    SettingEvent();
    /** @brief	Copy Constructor
    *   @param event a wxNIEvent
    **/
    SettingEvent(const SettingEvent& event);

    /** @brief	Destructor
    **/
    virtual ~SettingEvent(){}

    /** @brief	Creates a copy of the wxNIEvent
    *   @return <i>wxNIEvent</i>
    **/
    virtual wxEvent *Clone() const { return new SettingEvent(*this); }

    DECLARE_DYNAMIC_CLASS(SettingEvent)

    const wxString& GetSection() const;
    const wxString& GetKey() const;

protected:

    wxString m_sSection;
    wxString m_sKey;
};

typedef void (wxEvtHandler::*SettingEventFunction)(SettingEvent&);




#define SettingEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(SettingEventFunction, &func)

//#define EVT_NI(id,func) wx__DECLARE_EVT0(wxEVT_NI, wxNIEventHandler(func))
#define EVT_SETTING_EVENT(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SETTING_EVENT,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (SettingEventFunction) &fn, (wxObject*) NULL)

