#pragma once
#include <wx/panel.h>
#include "dlldefine.h"

class SettingEvent;
class PAMBASE_IMPEXPORT pmPanel : public wxPanel
{
    DECLARE_EVENT_TABLE()
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(pmPanel)
    #else
    wxDECLARE_DYNAMIC_CLASS(pmPanel);
    #endif // WXSPAM


    public:

        pmPanel();

        virtual ~pmPanel(){}

  protected:
        #ifndef WXSPAM
        void OnSettingEvent(SettingEvent& event);
        #endif // WXSPAM


};



