#include "wxswmadvlist.h"
#include "wmlistadv.h"
#include <prep.h>
#include "icons/wmlist16.xpm"
#include "icons/wmlist32.xpm"
using namespace std;


namespace
{
    wxsRegisterItem<wxswmListAdv> Reg(_T("wmAdvList"), wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("wammacs"),
                                     340,
                                     wxT("m_pLst"),
                                     wxsCPP,
                                     1,0,
                                     wmlist32_xpm,
                                     wmlist16_xpm
                                     );
    WXS_ST_BEGIN(wxswmListAdvStyles,_T(""))
        WXS_ST_CATEGORY("wmListAdv")
        WXS_ST_DEFAULTS()
    WXS_ST_END()
    WXS_EV_BEGIN(wxswmListAdvEvents)
        WXS_EVI(EVT_LISTADV_SELECTED,wxEVT_LISTADV_SELECTED,wxCommandEvent,Selected)
        WXS_EVI(EVT_LISTADV_HELD,wxEVT_LISTADV_HELD,wxCommandEvent,Held)
        WXS_EVI(EVT_LISTADV_PAGED,wxEVT_LISTADV_PAGED,wxCommandEvent,Paged)
        WXS_EVI(EVT_LISTADV_MOVED,wxEVT_LISTADV_MOVED,wxCommandEvent,Moved)
    WXS_EV_END()
}

wxswmListAdv::wxswmListAdv(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmListAdvEvents,
        wxswmListAdvStyles),
        m_nScroll(wmListAdv::SCROLL_VERTICAL)
{

}

void wxswmListAdv::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmlistadv.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S, %T, %d, wxSize(%d,%d), %d, wxSize(%d,%d));\n"),m_nScroll, m_szButtons.X,m_szButtons.Y, 1, m_szGap.X, m_szGap.Y);

            BuildSetupWindowCode();

            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmListAdv::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmListAdv::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmListAdv* Preview = new wmListAdv(Parent,GetId(),Pos(Parent),Size(Parent),Style(), m_nScroll, m_szButtons.GetSize(Parent), 1, m_szGap.GetSize(Parent));

    return SetupWindow(Preview,Flags);
}

void wxswmListAdv::OnEnumWidgetProperties(cb_unused long Flags)
{

    static const long Values[] = {wmListAdv::SCROLL_NONE, wmListAdv::SCROLL_VERTICAL, wmListAdv::SCROLL_HORIZONTAL};
    static const wxChar* Names[] = {wxT("None"), wxT("Vertical"), wxT("Horizontal"), NULL };

    WXS_ENUM(wxswmListAdv, m_nScroll, _("Scrolling"), _T("scrolling"), Values, Names, wmListAdv::SCROLL_VERTICAL);
    WXS_SIZE(wxswmListAdv,m_szGap,_("Default Button Gap"),wxT("Gap: Width"),wxT("Gap: Height"),wxT("In Dialog Units"),wxT("button_gap"));
    WXS_SIZE(wxswmListAdv,m_szButtons,_("Default Button Size"),wxT("Button: Width"),wxT("Button: Height"),wxT("In Dialog Units"),wxT("button_size"));

//    static wxsPropertyAddItems _Property(wxsOFFSET(wxswmListAdv, m_lstItems), wxsOFFSET(wxswmListAdv,m_sColour),  wxT("Items"), wxT("items"));
//    Property(_Property);
    //WXS_ADDITEMS(wxswmListAdv, m_lstItems, m_sColour, wxT("Items"), wxT("items"));

}


