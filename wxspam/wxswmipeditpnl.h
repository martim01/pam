#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxwidgets/properties/wxscolourproperty.h"

/** \brief Class for wxButton widget */
class wxswmIpEditPnl: public wxsWidget
{
    public:

        wxswmIpEditPnl(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        wxString m_sText;
};


