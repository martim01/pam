#pragma once
#include "wxwidgets/wxswidget.h"

/** \brief Class for wxsStaticText widget */
class wxswmLabel: public wxsWidget
{
    public:

        wxswmLabel(wxsItemResData* Data);

    private:

        virtual void      OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void      OnEnumWidgetProperties(long Flags);

        static const long BORDER_VALUES[];
        static const wxChar* BORDER_NAMES[];

        static const long GRADIENT_VALUES[];
        static const wxChar* GRADIENT_NAMES[];

        wxString Label;
        unsigned int m_nBorder;
        unsigned int m_nGradient;
};

