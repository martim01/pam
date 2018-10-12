#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxwidgets/properties/wxscolourproperty.h"
#include "dlgLevels.h"


/** \brief Class for wxButton widget */
class wxsLevelMeter: public wxsWidget
{
    public:

        wxsLevelMeter(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString m_sText;
        long m_nMin;
        bool m_bLevelOnly;

        levelDetails m_details;

        wxsColourData m_clrNormal;
        wxsColourData m_clrOver;

};




