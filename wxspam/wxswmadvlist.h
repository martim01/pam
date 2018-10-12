#pragma once
#include "wxwidgets\wxswidget.h"


/** \brief Class for wxButton widget */
class wxswmListAdv: public wxsWidget
{
    public:

        wxswmListAdv(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        unsigned long m_nScroll;
        wxsSizeData m_szButtons;
        wxsSizeData m_szGap;

};




