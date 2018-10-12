#pragma once
#include "wxwidgets\wxswidget.h"
#include "wxspropertyadditems.h"


/** \brief Class for wxButton widget */
class wxswmList: public wxsWidget
{
    public:

        wxswmList(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);

        void CreateColourString();

        wxString m_sColour;

        unsigned long m_nScroll;
        unsigned long m_nColumns;
        wxsSizeData m_szButtons;
        wxsSizeData m_szGap;

        unsigned long m_nShading;
        unsigned long m_nBorder;

        wxsColourData m_clrNormal;
        wxsColourData m_clrPressed;
        wxsColourData m_clrSelected;
        wxsColourData m_clrFlash;
        wxsColourData m_clrDisabled;

        wxsColourData m_clrNormalText;
        wxsColourData m_clrPressedText;
        wxsColourData m_clrSelectedText;
        wxsColourData m_clrFlashText;
        wxsColourData m_clrDisabledText;

        lItems_t m_lstItems;

};




