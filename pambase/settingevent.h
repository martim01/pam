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
    SettingEvent(const wxString& sSection, const wxString& sKey, const wxString& sValue);

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

    //wxDECLARE_DYNAMIC_CLASS(SettingEvent)

    const wxString& GetSection() const;
    const wxString& GetKey() const;
    wxString GetValue() const;
    long GetValue(long nDefault) const;
    double GetValue(double dDefault) const;
    bool GetValue(bool bDefault) const;


protected:

    wxString m_sSection;
    wxString m_sKey;
    wxString m_sValue;
};

typedef void (wxEvtHandler::*SettingEventFunction)(SettingEvent&);


#ifdef WXSPAM
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_SETTING_CHANGED, -1)
#else
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_SETTING_CHANGED,SettingEvent);
#endif

