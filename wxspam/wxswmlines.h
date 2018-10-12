#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxspropertyaddlines.h"

/** \brief Class for wxButton widget */
class wxswmLines : public wxsWidget
{
    public:

        wxswmLines(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        void OnAddExtraProperties(wxsPropertyGridManager* pGrid);
        void OnExtraPropertyChanged(wxsPropertyGridManager* pGrid, wxPGId id);
        bool OnXmlRead(TiXmlElement* pElement, bool bIsXrc, bool bIsExtra);
        bool OnXmlWrite(TiXmlElement* pElement, bool bIsXrc, bool bIsExtra);

        mLines_t m_mLines;
};




