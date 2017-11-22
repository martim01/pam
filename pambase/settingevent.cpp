#include "settingevent.h"

DEFINE_EVENT_TYPE(wxEVT_SETTING_CHANGED)

IMPLEMENT_DYNAMIC_CLASS(SettingEvent, wxCommandEvent)

SettingEvent::SettingEvent() : wxCommandEvent(wxEVT_SETTING_CHANGED, wxID_ANY)
{

}

SettingEvent::SettingEvent(const wxString& sSection, const wxString& sKey) : wxCommandEvent(wxEVT_SETTING_CHANGED,wxID_ANY),
m_sSection(sSection),
m_sKey(sKey)
{

}


SettingEvent::SettingEvent(const SettingEvent& event) : wxCommandEvent(event)
{
    m_sSection = event.GetSection();
    m_sKey = event.GetKey();

}


const wxString& SettingEvent::GetSection() const
{
    return m_sSection;
}

const wxString& SettingEvent::GetKey() const
{
    return m_sKey;
}
