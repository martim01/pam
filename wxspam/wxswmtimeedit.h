#pragma once
#include "wxwidgets/wxswidget.h"

/** \brief Class for wxsTextCtrl widget */
class wxswmTimeEdit: public wxsWidget
{
    public:

        wxswmTimeEdit(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        unsigned long m_nDateFormat;
        unsigned long m_nTimeFormat;
        unsigned long m_nFrames;
        bool m_bTimeFirst;

        wxString m_sSeparatorDate;
        wxString m_sSeparatorTime;

};

