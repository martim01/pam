#include "wxswmswipebutton.h"
#include "wmswipebutton.h"
#include <prep.h>

#include "icons/wmbutton16.xpm"
#include "icons/wmbutton32.xpm"

namespace
{
    wxsRegisterItem<wxswmSwipeButton> Reg(_T("wmSwipeButton"), wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("wammacs"),
                                     340,
                                     wxT("m_pBtn"),
                                     wxsCPP,
                                     1,0,
                                     wmbutton32_xpm,
                                     wmbutton16_xpm
                                     );

    WXS_ST_BEGIN(wxswmSwipeButtonStyles,_T(""))
        WXS_ST_CATEGORY("wmSwipeButton")
        WXS_ST(wmSwipeButton::STYLE_VERTICAL)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmSwipeButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
    WXS_EV_END()
}

wxswmSwipeButton::wxswmSwipeButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmSwipeButtonEvents,
        wxswmSwipeButtonStyles),
    Label(_("Slide")),
    IsDefault(false)
{}

void wxswmSwipeButton::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmswipebutton.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %V, %N);\n"),Label.wx_str());
            //if ( IsDefault ) Codef( _T("%ASetDefault();\n"));
            BuildSetupWindowCode();
            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmSwipeButton::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmSwipeButton::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmSwipeButton* Preview = new wmSwipeButton(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    //if ( IsDefault ) Preview->SetDefault();
    return SetupWindow(Preview,Flags);
}

void wxswmSwipeButton::OnEnumWidgetProperties(cb_unused long Flags)
{
    WXS_STRING(wxswmSwipeButton,Label,_("Label"),_T("label"),_T(""),false)
}
