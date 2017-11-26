#pragma once

#include <wx/event.h>
#include "dlldefine.h"



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

    //DECLARE_DYNAMIC_CLASS(SettingEvent)

    const wxString& GetSection() const;
    const wxString& GetKey() const;

protected:

    wxString m_sSection;
    wxString m_sKey;
};

typedef void (wxEvtHandler::*SettingEventFunction)(SettingEvent&);


wxDECLARE_EVENT(wxEVT_SETTING_CHANGED,SettingEvent);


