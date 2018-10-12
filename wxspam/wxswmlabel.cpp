#include "wxswmlabel.h"
#include "wmlabel.h"
#include "icons/wmlabel16.xpm"
#include "icons/wmlabel32.xpm"
namespace
{
    wxsRegisterItem<wxswmLabel> Reg(_T("wmLabel"), wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pLbl"),
                                     wxsCPP,
                                     1,0,
                                     wmlabel32_xpm,
                                     wmlabel16_xpm
                                     );

    WXS_ST_BEGIN(wxswmLabelStyles,_T(""))
        WXS_ST_CATEGORY("wmLabel")
        WXS_ST(wxST_NO_AUTORESIZE)
        WXS_ST(wxALIGN_LEFT)
        WXS_ST(wxALIGN_RIGHT)
        WXS_ST(wxALIGN_CENTRE)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmLabelEvents)
    WXS_EV_END()

}

const long wxswmLabel::BORDER_VALUES[] = {uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE};
const wxChar* wxswmLabel::BORDER_NAMES[] = {wxT("uiRect::BORDER_UP"), wxT("uiRect::BORDER_DOWN"), wxT("uiRect::BORDER_FLAT"), wxT("uiRect::BORDER_NONE"), NULL};

const long wxswmLabel::GRADIENT_VALUES[] = {0, 1,2, 3, 4};
const wxChar* wxswmLabel::GRADIENT_NAMES[] = {wxT("0"), wxT("wxWEST"), wxT("wxEAST"), wxT("wxNORTH"), wxT("wxSOUTH"), NULL};

wxswmLabel::wxswmLabel(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmLabelEvents,
        wxswmLabelStyles),
    Label(_("Label")),
    m_nBorder(uiRect::BORDER_NONE)

{}


void wxswmLabel::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmlabel.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %N);\n"),Label.wx_str());

            Codef(wxString::Format(_T("%ASetBorderState(%s);\n"), BORDER_NAMES[m_nBorder]));
            Codef(wxString::Format(_T("%AGetUiRect().SetGradient(%s);\n"), GRADIENT_NAMES[m_nGradient]));
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmLabel::OnBuildCreatingCode"),GetLanguage());
        }
    }
}


wxObject* wxswmLabel::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmLabel* Preview = new wmLabel(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    Preview->SetBorderState(m_nBorder);
    Preview->GetUiRect().SetGradient(m_nGradient);
    return SetupWindow(Preview,Flags);
}


void wxswmLabel::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxswmLabel,Label,_("Label"),_T("label"),_T(""),true);
    WXS_ENUM(wxswmLabel,m_nBorder,_("Border"),_T("border"), BORDER_VALUES, BORDER_NAMES, uiRect::BORDER_NONE);
    WXS_ENUM(wxswmLabel,m_nGradient,_("Gradient"),_T("gradient"), GRADIENT_VALUES, GRADIENT_NAMES, 0);
}
