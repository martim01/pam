#include "pmpanel.h"
#ifndef WXSPAM
#include "settingevent.h"
#include "settings.h"
wxIMPLEMENT_DYNAMIC_CLASS(pmPanel, wxPanel);
#else
IMPLEMENT_DYNAMIC_CLASS(pmPanel, wxPanel)
#endif // WXSPAM

BEGIN_EVENT_TABLE(pmPanel, wxPanel)
END_EVENT_TABLE()


pmPanel::pmPanel()
{
    #ifndef WXSPAM
    Settings::Get().AddHandler(wxT("General"), wxT("Cursor"), this);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pmPanel::OnSettingEvent);

    if(Settings::Get().Read(wxT("General"), wxT("Cursor"),1) == 0)
    {
        SetCursor(wxCURSOR_BLANK);
    }
    #endif // WXSPAM
}


#ifndef WXSPAM
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
#endif
