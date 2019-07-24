#include "pmcontrol.h"
#ifndef WXSPAM
#include "settingevent.h"
#include "settings.h"
#endif // WXSPAM
#include <wx/log.h>


#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(pmControl, wxWindow)
#else
wxIMPLEMENT_DYNAMIC_CLASS(pmControl, wxWindow);
#endif // WXSPAM

BEGIN_EVENT_TABLE(pmControl, wxWindow)
END_EVENT_TABLE()


pmControl::pmControl()
{
    InitCursor();
}


void pmControl::InitCursor()
{
  //  Settings::Get().AddHandler(wxT("General"), wxT("Cursor"), this);
  //  Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pmControl::OnSettingEvent);
    #ifndef WXSPAM
    if(Settings::Get().Read(wxT("General"), wxT("Cursor"),1) == 0)
    {
        SetCursor(wxCURSOR_BLANK);
    }
    #endif // WXSPAM
}

#ifndef WXSPAM
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
#endif // WXSPAM

