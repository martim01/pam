#include "wxswmlist.h"
#include "wmlist.h"
#include <prep.h>
#include "icons/wmlist16.xpm"
#include "icons/wmlist32.xpm"
using namespace std;


namespace
{
    wxsRegisterItem<wxswmList> Reg(_T("wmList"), wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pLst"),
                                     wxsCPP,
                                     1,0,
                                     wmlist32_xpm,
                                     wmlist16_xpm
                                     );

    WXS_ST_BEGIN(wxswmListStyles,_T(""))
        WXS_ST_CATEGORY("wmList")
        WXS_ST(wmList::STYLE_NORMAL)
        WXS_ST(wmList::STYLE_SELECT)
        WXS_ST(wmList::STYLE_HOLD)
        WXS_ST(wmList::STYLE_RESIZE_BUTTONS)
        WXS_ST(wmList::STYLE_SELECT_MULTI)
        WXS_ST(wmList::STYLE_SELECT_ROW)
        WXS_ST(wmList::STYLE_CIRCULAR)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmListEvents)
        WXS_EVI(EVT_LIST_SELECTED,wxEVT_LIST_SELECTED,wxCommandEvent,Selected)
        WXS_EVI(EVT_LIST_HELD,wxEVT_LIST_HELD,wxCommandEvent,Held)
        WXS_EVI(EVT_LIST_PAGED,wxEVT_LIST_PAGED,wxCommandEvent,Paged)
        WXS_EVI(EVT_LIST_SLID,wxEVT_LIST_SLID,wxCommandEvent,Slid)
    WXS_EV_END()
}

wxswmList::wxswmList(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmListEvents,
        wxswmListStyles),
        m_nScroll(wmList::SCROLL_VERTICAL),
        m_nColumns(1)
{
    m_szButtons.X = -1;
    m_szButtons.Y = 40;
    m_szGap.X = 1;
    m_szGap.Y = 1;
    m_nBorder = uiRect::BORDER_UP;
    m_nScroll = wmList::SCROLL_NONE;
    m_nShading = wxEAST;
}

void wxswmList::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmlist.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S, %T, %d, wxSize(%d,%d), %d, wxSize(%d,%d));\n"),m_nScroll, m_szButtons.X,m_szButtons.Y, m_nColumns, m_szGap.X, m_szGap.Y);

            BuildSetupWindowCode();

            if(m_nShading != wxEAST)
            {
                Codef(_T("%ASetGradient(%d);\n"),m_nShading);
            }
            if(m_nBorder != uiRect::BORDER_UP)
            {
                Codef(_T("%ASetBorderStyle(%d);\n"), m_nBorder);
            }

            if(m_clrNormal.GetColour().IsOk())
            {
                 Codef(_T("%ASetButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrNormal.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrPressed.GetColour().IsOk())
            {
                 Codef(_T("%ASetPressedButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrPressed.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrSelected.GetColour().IsOk())
            {
                 Codef(_T("%ASetSelectedButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrSelected.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrFlash.GetColour().IsOk())
            {
                 Codef(_T("%ASetFlashButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrFlash.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrDisabled.GetColour().IsOk())
            {
                 Codef(_T("%ASetDisabledColour(wxColour(wxT(\"%s\")));\n"), m_clrDisabled.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }

            if(m_clrNormalText.GetColour().IsOk())
            {
                 Codef(_T("%ASetTextButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrNormalText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrPressedText.GetColour().IsOk())
            {
                 Codef(_T("%ASetTextPressedButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrPressedText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrSelectedText.GetColour().IsOk())
            {
                 Codef(_T("%ASetTextSelectedButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrSelectedText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrFlashText.GetColour().IsOk())
            {
                 Codef(_T("%ASetTextFlashButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrFlashText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrDisabledText.GetColour().IsOk())
            {
                 Codef(_T("%ASetTextDisabledButtonColour(wxColour(wxT(\"%s\")));\n"), m_clrDisabledText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }

            if(m_lstItems.empty() == false)
            {
                Codef(_T("size_t nIndex;\n"));
            }
            for(list<button_wxs>::const_iterator itItem = m_lstItems.begin(); itItem != m_lstItems.end(); ++itItem)
            {
                Codef(_T("nIndex = %AAddButton(wxT(\"%s\"), wxNullBitmap, 0, %d, wxColour(wxT(\"%s\")), wxColour(wxT(\"%s\")),wxColour(wxT(\"%s\")));\n"), (*itItem).sText.c_str(), (*itItem).nEnabled, (*itItem).clrButton.GetAsString(wxC2S_HTML_SYNTAX).c_str(), (*itItem).clrPressed.GetAsString(wxC2S_HTML_SYNTAX).c_str(), (*itItem).clrSelected.GetAsString(wxC2S_HTML_SYNTAX).c_str());

                Codef(_T("%ASetTextButtonColour(nIndex, wxColour(wxT(\"%s\")));\n"), (*itItem).clrText.GetAsString(wxC2S_HTML_SYNTAX).c_str());
                Codef(_T("%ASetTextPressedButtonColour(nIndex, wxColour(wxT(\"%s\")));\n"), (*itItem).clrTextPressed.GetAsString(wxC2S_HTML_SYNTAX).c_str());
                Codef(_T("%ASetTextSelectedButtonColour(nIndex, wxColour(wxT(\"%s\")));\n"), (*itItem).clrTextSelected.GetAsString(wxC2S_HTML_SYNTAX).c_str());
                if((*itItem).nSlideAllowed != wmList::SLID_NONE)
                {
                    Codef(_T("%ASetSlideAllowed(nIndex,%d,wxT(\"%s\"), false);\n"), (*itItem).nSlideAllowed, (*itItem).sSlide.c_str());
                }
            }
            CreateColourString();

            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmList::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmList::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmList* Preview = new wmList(Parent,GetId(),Pos(Parent),Size(Parent),Style(), m_nScroll, m_szButtons.GetSize(Parent), m_nColumns, m_szGap.GetSize(Parent));



    if(m_nShading != wxEAST)
    {
        Preview->SetGradient(m_nShading);
    }
    if(m_nBorder != uiRect::BORDER_UP)
    {
        Preview->SetBorderStyle(m_nBorder);
    }

    if(m_clrNormal.GetColour().IsOk())
    {
         Preview->SetButtonColour(m_clrNormal.GetColour());
    }
    if(m_clrPressed.GetColour().IsOk())
    {
         Preview->SetPressedButtonColour(m_clrPressed.GetColour());
    }
    if(m_clrSelected.GetColour().IsOk())
    {
         Preview->SetSelectedButtonColour(m_clrSelected.GetColour());
    }
    if(m_clrFlash.GetColour().IsOk())
    {
         Preview->SetFlashButtonColour(m_clrFlash.GetColour());
    }
    if(m_clrDisabled.GetColour().IsOk())
    {
         Preview->SetDisabledColour(m_clrDisabled.GetColour());
    }

    if(m_clrNormalText.GetColour().IsOk())
    {
         Preview->SetTextButtonColour(m_clrNormalText.GetColour());
    }
    if(m_clrPressedText.GetColour().IsOk())
    {
         Preview->SetTextPressedButtonColour(m_clrPressedText.GetColour());
    }
    if(m_clrSelectedText.GetColour().IsOk())
    {
         Preview->SetTextSelectedButtonColour(m_clrSelectedText.GetColour());
    }
    if(m_clrFlashText.GetColour().IsOk())
    {
         Preview->SetTextFlashButtonColour(m_clrFlashText.GetColour());
    }
    if(m_clrDisabledText.GetColour().IsOk())
    {
        // Preview->SetTextDisabledColour(m_clrDisabledText.GetColour());
    }

    if(m_lstItems.empty())
    {
        Preview->AddButton(wxT("Example"));
        Preview->AddButton(wxT("Buttons"));
    }
    else
    {
        for(list<button_wxs>::const_iterator itItem = m_lstItems.begin(); itItem != m_lstItems.end(); ++itItem)
        {
            size_t nIndex = Preview->AddButton((*itItem).sText, wxNullBitmap, 0, (*itItem).nEnabled, (*itItem).clrButton, (*itItem).clrPressed, (*itItem).clrSelected);
            Preview->SetTextButtonColour(nIndex, (*itItem).clrText);
            Preview->SetTextPressedButtonColour(nIndex, (*itItem).clrTextPressed);
            Preview->SetTextSelectedButtonColour(nIndex, (*itItem).clrTextSelected);
            Preview->SetSlideAllowed(nIndex, (*itItem).nSlideAllowed, (*itItem).sSlide, false);
        }
    }

    CreateColourString();

    return SetupWindow(Preview,Flags);
}

void wxswmList::OnEnumWidgetProperties(cb_unused long Flags)
{

    static const long Values[] = {wmList::SCROLL_NONE, wmList::SCROLL_VERTICAL, wmList::SCROLL_HORIZONTAL};
    static const wxChar* Names[] = {wxT("None"), wxT("Vertical"), wxT("Horizontal"), NULL };

    WXS_ENUM(wxswmList, m_nScroll, _("Scrolling"), _T("scrolling"), Values, Names, wmList::SCROLL_VERTICAL);
    WXS_SIZE(wxswmList,m_szGap,_("Default Button Gap"),wxT("Gap: Width"),wxT("Gap: Height"),wxT("In Dialog Units"),wxT("button_gap"));
    WXS_SIZE(wxswmList,m_szButtons,_("Default Button Size"),wxT("Button: Width"),wxT("Button: Height"),wxT("In Dialog Units"),wxT("button_size"));
    WXS_LONG(wxswmList, m_nColumns, wxT("Columns"), wxT("columns"), 1);

    static const long Values2[] = {wxNORTH, wxEAST, wxSOUTH, wxWEST};
    static const wxChar* Names2[] = {wxT("North"), wxT("East"), wxT("South"), wxT("West"), NULL };
    WXS_ENUM(wxswmList, m_nShading, _("Shading"), _T("shading"), Values2, Names2, wxEAST);


    static const long Values3[] = {uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE};
    static const wxChar* Names3[] = {wxT("Up"), wxT("Down"), wxT("Flat"), wxT("None"), NULL };
    WXS_ENUM(wxswmList, m_nBorder, _("Item Border"), _T("item_border"), Values3, Names3, uiRect::BORDER_UP);


    WXS_COLOUR(wxswmList, m_clrNormal, wxT("Button"), wxT("button"));
    WXS_COLOUR(wxswmList, m_clrPressed, wxT("Button: Pressed"), wxT("button_pressed"));
    WXS_COLOUR(wxswmList, m_clrSelected, wxT("Button: Selected"), wxT("button_selected"));
    WXS_COLOUR(wxswmList, m_clrFlash, wxT("Button: Flash"), wxT("button_flash"));
    WXS_COLOUR(wxswmList, m_clrDisabled, wxT("Button: Disabled"), wxT("button_disabled"));

    WXS_COLOUR(wxswmList, m_clrNormalText, wxT("Text"), wxT("text"));
    WXS_COLOUR(wxswmList, m_clrPressedText, wxT("Text: Pressed"), wxT("text_pressed"));
    WXS_COLOUR(wxswmList, m_clrSelectedText, wxT("Text: Selected"), wxT("text_selected"));
    WXS_COLOUR(wxswmList, m_clrFlashText, wxT("Text: Flash"), wxT("text_flash"));
    WXS_COLOUR(wxswmList, m_clrDisabledText, wxT("Text: Disabled"), wxT("text_disabled"));

    static wxsPropertyAddItems _Property(wxsOFFSET(wxswmList, m_lstItems), wxsOFFSET(wxswmList,m_sColour),  wxT("Items"), wxT("items"));
    Property(_Property);
    //WXS_ADDITEMS(wxswmList, m_lstItems, m_sColour, wxT("Items"), wxT("items"));

}


void wxswmList::CreateColourString()
{
    m_sColour = wxEmptyString;
    if(m_clrNormal.GetColour().IsOk())
    {
        m_sColour << m_clrNormal.GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    }
    m_sColour << wxT(",");

    if(m_clrPressed.GetColour().IsOk())
    {
        m_sColour << m_clrPressed.GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    }
    m_sColour << wxT(",");

    if(m_clrSelected.GetColour().IsOk())
    {
        m_sColour << m_clrSelected.GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    }
    m_sColour << wxT(",");

    if(m_clrNormalText.GetColour().IsOk())
    {
        m_sColour << m_clrNormalText.GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    }
    m_sColour << wxT(",");

    if(m_clrPressedText.GetColour().IsOk())
    {
        m_sColour << m_clrPressedText.GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    }
    m_sColour << wxT(",");

    if(m_clrSelectedText.GetColour().IsOk())
    {
        m_sColour << m_clrSelectedText.GetColour().GetAsString(wxC2S_HTML_SYNTAX);
    }

}
