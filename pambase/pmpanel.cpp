#include "pmpanel.h"
#include "settingevent.h"
#include "settings.h"

BEGIN_EVENT_TABLE(pmPanel, wxPanel)
END_EVENT_TABLE()
 wxIMPLEMENT_DYNAMIC_CLASS(pmPanel, wxPanel);

pmPanel::pmPanel()
{
    Settings::Get().AddHandler(wxT("General"), wxT("Cursor"), this);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pmPanel::OnSettingEvent);

    if(Settings::Get().Read(wxT("General"), wxT("Cursor"),1) == 0)
    {
        SetCursor(wxCURSOR_BLANK);
    }
}


void pmPanel::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == wxT("General") && event.GetKey() == wxT("Cursor"))
    {
        if(event.GetValue(false)==false)
        {
            SetCursor(wxCURSOR_BLANK);
        }
        else
        {
            SetCursor(wxCURSOR_DEFAULT);
        }
    }
}
