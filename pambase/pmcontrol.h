#pragma once
#include <wx/window.h>
#include "dlldefine.h"

class SettingEvent;
class PAMBASE_IMPEXPORT pmControl : public wxWindow
{
    DECLARE_EVENT_TABLE()
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(pmControl)
    #else
    wxDECLARE_DYNAMIC_CLASS(pmControl);
    #endif // WXSPAM
    public:

        pmControl();
        virtual ~pmControl(){}


  protected:

        virtual void InitCursor();
        #ifndef WXSPAM
        void OnSettingEvent(SettingEvent& event);
        #endif // WXSPAM


};


