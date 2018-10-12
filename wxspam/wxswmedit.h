#pragma once
#include "wxwidgets/wxswidget.h"

/** \brief Class for wxsTextCtrl widget */
class wxswmEdit: public wxsWidget
{
    public:

        wxswmEdit(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString Text;
        long MaxLength;
        short m_nValidation;

        wxsColourData m_clrFocusedBackground;
        wxsColourData m_clrFocusedForeground;
        long m_nBorderStyle;
        long m_nFocusedBorderStyle;
};
