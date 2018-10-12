#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxwidgets/properties/wxscolourproperty.h"

/** \brief Class for wxButton widget */
class wxswmButton: public wxsWidget
{
    public:

        wxswmButton(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString Label;
        bool IsDefault;

        wxsColourData m_clrPressed;
        wxsColourData m_clrArmed;
        wxsColourData m_clrFlash;
        wxsColourData m_clrDisabled;

        wxsColourData m_clrPressedText;
        wxsColourData m_clrArmedText;
        wxsColourData m_clrFlashText;
        wxsColourData m_clrDisabledText;

        bool m_bToggle;
        wxString m_sLeft;
        wxString m_sRight;
        unsigned long m_nPercent;
};


