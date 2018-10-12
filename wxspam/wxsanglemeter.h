#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxwidgets/properties/wxscolourproperty.h"
#include "dlgLevels.h"

/** \brief Class for wxButton widget */
class wxsAngleMeter: public wxsWidget
{
    public:

        wxsAngleMeter(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString m_sText;
        long m_nMin;
        long m_nRouting;

        wxsColourData m_clrMeter1;
        wxsColourData m_clrMeter2;

        levelDetails m_details;


};



