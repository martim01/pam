#pragma once
#include "wxwidgets\wxswidget.h"

/** \brief Class for wxButton widget */
class wxswmKeyboard: public wxsWidget
{
    public:

        wxswmKeyboard(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        unsigned long m_nType;
        bool m_bHints;
};



