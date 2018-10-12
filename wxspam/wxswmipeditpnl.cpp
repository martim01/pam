#include "wxswmipeditpnl.h"
#include "wmipeditpnl.h"
#include <prep.h>

#include "icons/wmipeditpnl16.xpm"
#include "icons/wmipeditpnl32.xpm"

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
    wxsRegisterItem<wxswmIpEditPnl> Reg(_T("wmIpEditPnl"),
                                     wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pIP"),
                                     wxsCPP,
                                     1,0,
                                     wmipeditpnl32_xpm,
                                     wmipeditpnl16_xpm
                                     );

    WXS_ST_BEGIN(wxswmIpEditPnlStyles,_T(""))

    WXS_ST_END()

    WXS_EV_BEGIN(wxswmIpEditPnlEvents)
    WXS_EV_END()
}

wxswmIpEditPnl::wxswmIpEditPnl(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmIpEditPnlEvents,
        wxswmIpEditPnlStyles)
{}

void wxswmIpEditPnl::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmipeditpnl.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S);\n"));
            Codef(_T("%ASetValue(%t);\n"), m_sText.wx_str());

            BuildSetupWindowCode();

            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmIpEditPnl::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmIpEditPnl::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmipeditpnl* Preview = new wmipeditpnl(Parent,GetId(),Pos(Parent),Size(Parent));
    Preview->SetValue(m_sText);


    return SetupWindow(Preview,Flags);
}

void wxswmIpEditPnl::OnEnumWidgetProperties(cb_unused long Flags)
{
    WXS_STRING(wxswmIpEditPnl,m_sText,_("Address"),_T("address"),_T(""),false)

}
