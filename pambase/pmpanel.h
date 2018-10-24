#pragma once
#include <wx/panel.h>
#include "dlldefine.h"

class SettingEvent;
class PAMBASE_IMPEXPORT pmPanel : public wxPanel
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(pmPanel);

    public:

        pmPanel();

        virtual ~pmPanel(){}

  protected:

        void OnSettingEvent(SettingEvent& event);



};



