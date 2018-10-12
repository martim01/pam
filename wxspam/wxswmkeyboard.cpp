#include "wxswmkeyboard.h"
#include "wmkeyboard.h"
#include <prep.h>
#include "icons/wmkeyboard16.xpm"
#include "icons/wmkeyboard32.xpm"

namespace
{
    wxsRegisterItem<wxswmKeyboard> Reg(_T("wmKeyboard"), wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pKbd"),
                                     wxsCPP,
                                     1,0,
                                     wmkeyboard32_xpm,
                                     wmkeyboard16_xpm
                                     );

    WXS_ST_BEGIN(wxswmKeyboardStyles,_T(""))
        WXS_ST_CATEGORY("wmKeyboard")
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmKeyboardEvents)
    WXS_EV_END()
}

wxswmKeyboard::wxswmKeyboard(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmKeyboardEvents,
        wxswmKeyboardStyles),
        m_nType(wmKeyboard::KBS_NORMAL),
        m_bHints(false)
{}

void wxswmKeyboard::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmkeyboard.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S, %d, %d);\n"),m_nType, m_bHints);
            //if ( IsDefault ) Codef( _T("%ASetDefault();\n"));
            BuildSetupWindowCode();
            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmKeyboard::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmKeyboard::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmKeyboard* Preview = new wmKeyboard(Parent,GetId(),Pos(Parent),Size(Parent),m_nType, m_bHints);
    //if ( IsDefault ) Preview->SetDefault();
    return SetupWindow(Preview,Flags);
}

void wxswmKeyboard::OnEnumWidgetProperties(cb_unused long Flags)
{
    static const long Values[] = {wmKeyboard::KBS_NORMAL, wmKeyboard::KBS_NUMPAD, wmKeyboard::KBS_PHONE, wmKeyboard::KBS_NAVIGATE, wmKeyboard::KBS_CAPS, wmKeyboard::KBS_CALC};
    static const wxChar* Names[] = {wxT("Normal"), wxT("NumPad"), wxT("Phone"), wxT("Navigate"), wxT("Capitals"), wxT("Calc"), NULL };

    WXS_ENUM(wxswmKeyboard, m_nType, _("Type"), _T("type"), Values, Names, 0);

    WXS_BOOL(wxswmKeyboard,m_bHints,_("Show Hints"),_T("hints"),false);
}
