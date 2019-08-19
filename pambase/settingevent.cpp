#include "settingevent.h"


#ifdef WXSPAM
DEFINE_EVENT_TYPE(wxEVT_SETTING_CHANGED)
#else
wxDEFINE_EVENT(wxEVT_SETTING_CHANGED, SettingEvent);
#endif // WXSPAM

//wxDECLARE_DYNAMIC_CLASS(SettingEvent, wxCommandEvent)

SettingEvent::SettingEvent() : wxCommandEvent(wxEVT_SETTING_CHANGED, wxID_ANY)
{

}

SettingEvent::SettingEvent(const wxString& sSection, const wxString& sKey, const wxString& sValue) : wxCommandEvent(wxEVT_SETTING_CHANGED,wxID_ANY),
m_sSection(sSection),
m_sKey(sKey),
m_sValue(sValue)
{

}


SettingEvent::SettingEvent(const SettingEvent& event) : wxCommandEvent(event), 
	m_sSection(event.GetSection()),
	m_sKey(event.GetKey()),
	m_sValue(event.GetValue())
{
}


const wxString& SettingEvent::GetSection() const
{
    return m_sSection;
}

const wxString& SettingEvent::GetKey() const
{
    return m_sKey;
}

wxString SettingEvent::GetValue() const
{
    return m_sValue;
}
long SettingEvent::GetValue(long nDefault) const
{
    m_sValue.ToLong(&nDefault);
    return nDefault;
}

double SettingEvent::GetValue(double dDefault) const
{
    m_sValue.ToDouble(&dDefault);
    return dDefault;
}

bool SettingEvent::GetValue(bool bDefault) const
{
    if(m_sValue==wxT("0"))
    {
        return false;
    }
    if(m_sValue==wxT("1"))
    {
        return true;
    }
    return bDefault;
}
