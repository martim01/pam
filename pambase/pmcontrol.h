#pragma once
#include <wx/window.h>
#include "dlldefine.h"

class SettingEvent;
class PAMBASE_IMPEXPORT pmControl : public wxWindow
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(pmControl);

    public:

        pmControl();
        virtual ~pmControl(){}


  protected:

        virtual void InitCursor();
        void OnSettingEvent(SettingEvent& event);



};


