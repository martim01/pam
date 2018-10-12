#include "wxswmbutton.h"
#include "wmbutton.h"
#include <prep.h>

#include "icons/wmbutton16.xpm"
#include "icons/wmbutton32.xpm"

namespace
{
    /*
    wxString ClassName,
            wxsItemType Type,
            wxString License,
            wxString Author,
            wxString Email,
            wxString Site,
            wxString Category,
            long Priority,
            wxString DefaultVarName,
            long Languages,
            unsigned short VerHi,
            unsigned short VerLo,
            wxString Bmp32FileName,
            wxString Bmp16FileName
            */
    wxsRegisterItem<wxswmButton> Reg(_T("wmButton"),
                                     wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pBtn"),
                                     wxsCPP,
                                     1,0,
                                     wmbutton32_xpm,
                                     wmbutton16_xpm
                                     );

    WXS_ST_BEGIN(wxswmButtonStyles,_T(""))
        WXS_ST_CATEGORY("wmButton")
        WXS_ST(wmButton::STYLE_NORMAL)
        WXS_ST(wmButton::STYLE_ARM)
        WXS_ST(wmButton::STYLE_HOLD)
        WXS_ST(wmButton::STYLE_REPEAT)
        WXS_ST(wmButton::STYLE_SELECT)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
        WXS_EVI(EVT_BUTTON_HELD,wxEVT_BUTTON_HELD, wxCommandEvent,Held)
        WXS_EVI(EVT_BUTTON_PRESSED,wxEVT_BUTTON_PRESSED,wxCommandEvent,Pressed)
        WXS_EVI(EVT_BUTTON_RELEASED,wxEVT_BUTTON_RELEASED,wxCommandEvent,Released)
    WXS_EV_END()
}

wxswmButton::wxswmButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmButtonEvents,
        wxswmButtonStyles),
    Label(_("Button")),
    IsDefault(false),
    m_nPercent(33)
{}

void wxswmButton::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmbutton.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %V, %N);\n"),Label.wx_str());

            BuildSetupWindowCode();


            if(m_clrPressed.GetColour().IsOk())
            {
                Codef(_T("%ASetColourSelected(wxColour(wxT(\"%s\")));\n"), m_clrPressed.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrArmed.GetColour().IsOk())
            {
                Codef(_T("%ASetColourArmed(wxColour(wxT(\"%s\")));\n"), m_clrArmed.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrFlash.GetColour().IsOk())
            {
                Codef(_T("%ASetColourFlash(wxColour(wxT(\"%s\")));\n"), m_clrFlash.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrDisabled.GetColour().IsOk())
            {
                Codef(_T("%ASetColourDisabled(wxColour(wxT(\"%s\")));\n"), m_clrDisabled.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }

            if(m_clrPressedText.GetColour().IsOk())
            {
                Codef(_T("%ASetTextColourSelected(wxColour(wxT(\"%s\")));\n"), m_clrPressedText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrArmedText.GetColour().IsOk())
            {
                Codef(_T("%ASetTextColourArmed(wxColour(wxT(\"%s\")));\n"), m_clrArmedText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrFlashText.GetColour().IsOk())
            {
                Codef(_T("%ASetTextColourFlash(wxColour(wxT(\"%s\")));\n"), m_clrFlashText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrDisabledText.GetColour().IsOk())
            {
                Codef(_T("%ASetTextColourDisabled(wxColour(wxT(\"%s\")));\n"), m_clrDisabledText.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }

            if(m_bToggle)
            {
                Codef(_T("%ASetToggleLook(true, wxT(\"%s\"), wxT(\"%s\"), %d);\n"), m_sLeft.c_str(), m_sRight.c_str(), m_nPercent);
            }


            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmButton::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmButton::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmButton* Preview = new wmButton(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    if(m_clrPressed.GetColour().IsOk())
    {
        Preview->SetColourSelected(m_clrPressed.GetColour());
    }
    if(m_clrArmed.GetColour().IsOk())
    {
        Preview->SetColourArmed(m_clrArmed.GetColour());
    }
    if(m_clrFlash.GetColour().IsOk())
    {
        Preview->SetColourFlash(m_clrFlash.GetColour());
    }
    if(m_clrDisabled.GetColour().IsOk())
    {
        Preview->SetColourDisabled(m_clrDisabled.GetColour());
    }

    if(m_clrPressedText.GetColour().IsOk())
    {
        Preview->SetTextColourSelected(m_clrPressedText.GetColour());
    }
    if(m_clrArmedText.GetColour().IsOk())
    {
        Preview->SetTextColourArmed(m_clrArmedText.GetColour());
    }
    if(m_clrFlashText.GetColour().IsOk())
    {
        Preview->SetTextColourFlash(m_clrFlashText.GetColour());
    }
    if(m_clrDisabledText.GetColour().IsOk())
    {
        Preview->SetTextColourDisabled(m_clrDisabledText.GetColour());
    }
    if(m_bToggle)
    {
        Preview->SetToggleLook(m_bToggle, m_sLeft, m_sRight, m_nPercent);
    }


    return SetupWindow(Preview,Flags);
}

void wxswmButton::OnEnumWidgetProperties(cb_unused long Flags)
{
    WXS_STRING(wxswmButton,Label,_("Label"),_T("label"),_T(""),false)

    WXS_COLOUR(wxswmButton, m_clrPressed, wxT("Button: Pressed"), wxT("button_pressed"));
    WXS_COLOUR(wxswmButton, m_clrArmed, wxT("Button: Armed"), wxT("button_armed"));
    WXS_COLOUR(wxswmButton, m_clrFlash, wxT("Button: Flash"), wxT("button_flash"));
    WXS_COLOUR(wxswmButton, m_clrDisabled, wxT("Button: Disabled"), wxT("button_disabled"));

    WXS_COLOUR(wxswmButton, m_clrPressedText, wxT("Text: Pressed"), wxT("text_pressed"));
    WXS_COLOUR(wxswmButton, m_clrArmedText, wxT("Text: Armed"), wxT("text_armed"));
    WXS_COLOUR(wxswmButton, m_clrFlashText, wxT("Text: Flash"), wxT("text_flash"));
    WXS_COLOUR(wxswmButton, m_clrDisabledText, wxT("Text: Disabled"), wxT("text_disabled"));

    WXS_BOOL(wxswmButton, m_bToggle, wxT("Toggle Look"), wxT("toggle_look"), false);
    WXS_STRING(wxswmButton, m_sLeft, wxT("Toggle Left"), wxT("toggle_left"), _T(""), false);
    WXS_STRING(wxswmButton, m_sRight, wxT("Toggle Right"), wxT("toggle_right"), _T(""), false);
    WXS_LONG(wxswmButton, m_nPercent, wxT("Toggle Percent"), wxT("toggle_percent"), 33);
}
