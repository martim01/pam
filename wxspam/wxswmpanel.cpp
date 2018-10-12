
#include <wx/settings.h> // wxSystemSettings, wxSYS_COLOUR_BTNFACE
#include "wxswmPanel.h"
#include "wxwidgets/wxsgridpanel.h"
#include "icons/wmpanel16.xpm"
#include "icons/wmpanel32.xpm"
#include "wmpanel.h"

namespace
{
    wxsRegisterItem<wxswmPanel> Reg(_T("wmPanel"),wxsTContainer,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("wammacs"),
                                     340,
                                     wxT("m_pSwp"),
                                     wxsCPP,
                                     1,0,
                                     wmpanel32_xpm,
                                     wmpanel16_xpm
                                     );

    WXS_ST_BEGIN(wxswmPanelStyles,_T("wxTAB_TRAVERSAL"))
        WXS_ST_CATEGORY("wxPanel")
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmPanelEvents)
        WXS_EV_DEFAULTS()
    WXS_EV_END()

    class PanelPreview: public wxsGridPanel
    {
        public:

            PanelPreview(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size,long style,bool IsRoot):
                wxsGridPanel(parent,id,pos,size,style),
                m_IsRoot(IsRoot)
            {}

        private:

            bool DrawBorder()
            {
                return !m_IsRoot;
            }

            bool m_IsRoot;
    };
}

wxswmPanel::wxswmPanel(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxswmPanelEvents,
        wxswmPanelStyles)
{}

void wxswmPanel::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmpanel.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S);\n"));
            BuildSetupWindowCode();
            AddChildrenCode();
            return;
        }

        case wxsUnknownLanguage: // fall-through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmPanel::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmPanel::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxWindow* NewItem = 0;
    if ( Flags & pfExact )
    {
        NewItem = new wmPanel(Parent,GetId(),Pos(Parent),Size(Parent));
    }
    else
    {
        NewItem = new PanelPreview(Parent,GetId(),Pos(Parent),Size(Parent),Style(),IsRootItem());
    }
    NewItem->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    SetupWindow(NewItem,Flags);
    AddChildrenPreview(NewItem,Flags);
    return NewItem;
}

void wxswmPanel::OnEnumContainerProperties(long Flags)
{
}
