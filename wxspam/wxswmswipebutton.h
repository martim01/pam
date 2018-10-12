#pragma once
#include "wxwidgets\wxswidget.h"

/** \brief Class for wxButton widget */
class wxswmSwipeButton: public wxsWidget
{
    public:

        wxswmSwipeButton(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString Label;
        bool IsDefault;
};



