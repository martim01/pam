#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxwidgets/properties/wxscolourproperty.h"

/** \brief Class for wxButton widget */
class wxswmSlider: public wxsWidget
{
    public:

        wxswmSlider(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString m_sLabel;
        wxsColourData m_clrSlider;
        wxsColourData m_clrButton;

        long m_nMin;
        long m_nMax;
        long m_nStart;
};



