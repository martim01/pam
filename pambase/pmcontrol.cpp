#include "pmcontrol.h"
#include "settingevent.h"
#include "settings.h"
#include <wx/log.h>

BEGIN_EVENT_TABLE(pmControl, wxWindow)
END_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(pmControl, wxWindow);

pmControl::pmControl()
{
    InitCursor();
}


void pmControl::InitCursor()
{
  //  Settings::Get().AddHandler(wxT("General"), wxT("Cursor"), this);
  //  Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pmControl::OnSettingEvent);

    if(Settings::Get().Read(wxT("General"), wxT("Cursor"),1) == 0)
    {
        SetCursor(wxCURSOR_BLANK);
    }
}

void pmControl::OnSettingEvent(SettingEvent& event)
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


