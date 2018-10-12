#include "wxswmedit.h"
#include "wmedit.h"
#include "uirect.h"
#include "icons/wmedit16.xpm"
#include "icons/wmedit32.xpm"

namespace
{
    wxsRegisterItem<wxswmEdit> Reg(_T("wmEdit"),
                                    wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     40,
                                     wxT("m_pEdt"),
                                     wxsCPP,
                                     1,0,
                                     wmedit32_xpm,
                                     wmedit16_xpm
                                     );

    WXS_ST_BEGIN(wxswmEditStyles,_T(""))
        WXS_ST(wxTE_PROCESS_ENTER)
        WXS_ST(wxTE_PROCESS_TAB)
        WXS_ST(wxTE_PASSWORD)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmEditEvents)
        WXS_EVI(EVT_TEXT,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEvent,Text)
        WXS_EVI(EVT_TEXT_ENTER,wxEVT_COMMAND_TEXT_ENTER,wxCommandEvent,TextEnter)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
    WXS_EV_END()

}

wxswmEdit::wxswmEdit(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmEditEvents,
        wxswmEditStyles),
    Text(_("Text")),
    MaxLength(0),
    m_nBorderStyle(uiRect::BORDER_DOWN),
    m_nFocusedBorderStyle(uiRect::BORDER_DOWN)
{}

void wxswmEdit::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmedit.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %V, %N);\n"),Text.wx_str());
            if ( MaxLength > 0 ) Codef(_T("%ASetMaxLength(%d);\n"),MaxLength);
            Codef(_T("%ASetValidation(%d);\n"),m_nValidation);
            BuildSetupWindowCode();

            if(m_clrFocusedBackground.GetColour().IsOk())
            {
                Codef(_T("%ASetFocusedBackground(wxColour(wxT(\"%s\")));\n"), m_clrFocusedBackground.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }

            if(m_clrFocusedForeground.GetColour().IsOk())
            {
                Codef(_T("%ASetFocusedForeground(wxColour(wxT(\"%s\")));\n"), m_clrFocusedForeground.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }

            Codef(_T("%ASetBorderStyle(%d,%d);\n"),m_nBorderStyle,m_nFocusedBorderStyle);

            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmEdit::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmEdit::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmEdit* Preview = new wmEdit(Parent,GetId(),Text,Pos(Parent),Size(Parent),Style());
    wxObject* pObject = SetupWindow(Preview,Flags);

    Preview->SetNonFocusedStyle(Preview->GetBackgroundColour(), Preview->GetForegroundColour(), m_nBorderStyle);

    wxColour clrBack, clrFore;
    if(m_clrFocusedBackground.GetColour().IsOk())
    {
        clrBack = m_clrFocusedBackground.GetColour();
    }
    else
    {
        clrBack = Preview->GetBackgroundColour();
    }

    if(m_clrFocusedForeground.GetColour().IsOk())
    {
        clrBack = m_clrFocusedForeground.GetColour();
    }
    else
    {
        clrBack = Preview->GetForegroundColour();
    }

    Preview->SetFocusedStyle(clrBack,clrFore,m_nFocusedBorderStyle);


    return pObject;
}

void wxswmEdit::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxswmEdit,Text,_("Text"),_T("value"),_T(""),false);
    WXS_LONG(wxswmEdit,MaxLength,_("Max Length"),_T("maxlength"),0);

    static const long Values[] = {wmEdit::ASCII, wmEdit::ALPHA, wmEdit::ALPHANUMERIC, wmEdit::INTEGER, wmEdit::REAL_NUMBER};
    static const wxChar* Names[] = {wxT("ASCII"), wxT("Alpha"), wxT("Alphanumeric"), wxT("Integer"), wxT("Real"), NULL };

    WXS_ENUM(wxswmEdit, m_nValidation, _("Validation"), _T("validation"), Values, Names, 0);

    static const long Border_Values[] = {uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE};
    static const wxChar* Border_Names[] = {wxT("uiRect::BORDER_UP"), wxT("uiRect::BORDER_DOWN"), wxT("uiRect::BORDER_FLAT"), wxT("uiRect::BORDER_NONE"), NULL};

    WXS_ENUM(wxswmEdit,m_nBorderStyle,_("Border"),_T("border"), Border_Values, Border_Names, 0);
    WXS_ENUM(wxswmEdit,m_nFocusedBorderStyle,_("Focused Border"),_T("border_focused"), Border_Values, Border_Names,0);
    WXS_COLOUR(wxswmEdit, m_clrFocusedBackground, wxT("Focused Background"), wxT("background_focused"));
    WXS_COLOUR(wxswmEdit, m_clrFocusedForeground, wxT("Focused Foreground"), wxT("foreground_focused"));
}
