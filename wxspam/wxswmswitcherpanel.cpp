#include "wxswmswitcherpanel.h"
#include "wxsadvqppchild.h"
#include "wxwidgets/wxsitemresdata.h"
#include "wmswitcherpanel.h"
#include <wx/menu.h>
#include <wx/textdlg.h>
#include "wxwidgets/wxsflags.h"

#include <prep.h>

#include "icons/wmswitcherpanel16.xpm"
#include "icons/wmswitcherpanel32.xpm"

using namespace wxsFlags;

//(*Headers(wxsNotebookParentQP)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
//*)

//(*InternalHeaders(wxsNotebookParentQP)
#include <wx/intl.h>
#include <wx/string.h>
//*)

// TODO: Add notebook images
namespace
{
    wxsRegisterItem<wxswmSwitcherPanel> Reg(_T("wmSwitcherPanel"),wxsTContainer,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pSwp"),
                                     wxsCPP,
                                     1,0,
                                     wmswitcherpanel32_xpm,
                                     wmswitcherpanel16_xpm
                                     );

    /** \brief Extra parameters for wmSwitcherPanel's children */
    class wxswmSwitcherPanelExtra: public wxsPropertyContainer
    {
        public:

            wxswmSwitcherPanelExtra():
                m_Label(_("Page name")),
                m_Selected(false)
            {}

            wxString m_Label;
            bool m_Selected;

        protected:

            virtual void OnEnumProperties(cb_unused long Flags)
            {
                WXS_SHORT_STRING(wxswmSwitcherPanelExtra,m_Label,_("Page name"),_T("label"),_T(""),false);
                WXS_BOOL(wxswmSwitcherPanelExtra,m_Selected,_("Page selected"),_T("selected"),false);
            }
    };

    /** \brief Inernal Quick properties panel */
    class wxswmSwitcherPanelParentQP: public wxsAdvQPPChild
    {
        public:

            wxswmSwitcherPanelParentQP(wxsAdvQPP* parent,wxswmSwitcherPanelExtra* Extra,wxWindowID id = -1):
                wxsAdvQPPChild(parent,_("wmSwitcherPanel")),
                m_Extra(Extra)
            {
                //(*Initialize(wxswmSwitcherPanelParentQP)
                wxStaticBoxSizer* StaticBoxSizer2;
                wxStaticBoxSizer* StaticBoxSizer1;
                wxFlexGridSizer* FlexGridSizer1;

                Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
                FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
                StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Label"));
                Label = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
                StaticBoxSizer1->Add(Label, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
                FlexGridSizer1->Add(StaticBoxSizer1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
                StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Selection"));
                Selected = new wxCheckBox(this, ID_CHECKBOX1, _("Selected"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
                Selected->SetValue(false);
                StaticBoxSizer2->Add(Selected, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
                FlexGridSizer1->Add(StaticBoxSizer2, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
                SetSizer(FlexGridSizer1);
                FlexGridSizer1->Fit(this);
                FlexGridSizer1->SetSizeHints(this);

                Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&wxswmSwitcherPanelParentQP::OnLabelText);
                Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&wxswmSwitcherPanelParentQP::OnSelectionChange);
                //*)
                ReadData();

                Label->Connect(-1,wxEVT_KILL_FOCUS,(wxObjectEventFunction)&wxswmSwitcherPanelParentQP::OnLabelKillFocus,0,this);
            }

            virtual ~wxswmSwitcherPanelParentQP()
            {
                //(*Destroy(wxswmSwitcherPanelParentQP)
                //*)
            }

        private:

            virtual void Update()
            {
                ReadData();
            }

            void ReadData()
            {
                if ( !GetPropertyContainer() || !m_Extra ) return;
                Label->SetValue(m_Extra->m_Label);
                Selected->SetValue(m_Extra->m_Selected);
            }

            void SaveData()
            {
                if ( !GetPropertyContainer() || !m_Extra ) return;
                m_Extra->m_Label = Label->GetValue();
                m_Extra->m_Selected = Selected->GetValue();
                NotifyChange();
            }

            //(*Identifiers(wxswmSwitcherPanelParentQP)
            static const long ID_TEXTCTRL1;
            static const long ID_CHECKBOX1;
            //*)

            //(*Handlers(wxswmSwitcherPanelParentQP)
            void OnLabelText(wxCommandEvent& event);
            void OnLabelKillFocus(wxFocusEvent& event);
            void OnSelectionChange(wxCommandEvent& event);
            //*)

            //(*Declarations(wxswmSwitcherPanelParentQP)
            wxCheckBox* Selected;
            wxTextCtrl* Label;
            //*)

            wxswmSwitcherPanelExtra* m_Extra;

            DECLARE_EVENT_TABLE()
    };

    //(*IdInit(wxswmSwitcherPanelParentQP)
    const long wxswmSwitcherPanelParentQP::ID_TEXTCTRL1 = wxNewId();
    const long wxswmSwitcherPanelParentQP::ID_CHECKBOX1 = wxNewId();
    //*)

    BEGIN_EVENT_TABLE(wxswmSwitcherPanelParentQP,wxPanel)
        //(*EventTable(wxswmSwitcherPanelParentQP)
        //*)
    END_EVENT_TABLE()

    void wxswmSwitcherPanelParentQP::OnLabelText(cb_unused wxCommandEvent& event)       { SaveData(); }
    void wxswmSwitcherPanelParentQP::OnLabelKillFocus(wxFocusEvent& event)              { SaveData(); event.Skip(); }
    void wxswmSwitcherPanelParentQP::OnSelectionChange(cb_unused wxCommandEvent& event) { SaveData(); }

    WXS_ST_BEGIN(wxswmSwitcherPanelStyles,_T(""))
        WXS_ST_CATEGORY("wxwmSwitcherPanel")
        WXS_ST(wmSwitcherPanel::STYLE_NOSWIPE)
        WXS_ST(wmSwitcherPanel::STYLE_NOANIMATION)
        WXS_ST(wmSwitcherPanel::STYLE_VERTICAL)
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmSwitcherPanelEvents)
        WXS_EVI(EVT_NOTEBOOK_PAGE_CHANGED,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,wxNotebookEvent,PageChanged)
        WXS_EVI(EVT_NOTEBOOK_PAGE_CHANGING,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,wxNotebookEvent,PageChanging)
    WXS_EV_END()

    const long popupNewPageId = wxNewId();
    const long popupPrevPageId = wxNewId();
    const long popupNextPageId = wxNewId();
    const long popupFirstId = wxNewId();
    const long popupLastId = wxNewId();
}


wxswmSwitcherPanel::wxswmSwitcherPanel(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxswmSwitcherPanelEvents,
        wxswmSwitcherPanelStyles),
    m_CurrentSelection(0)
{
}

void wxswmSwitcherPanel::OnEnumContainerProperties(cb_unused long Flags)
{
    static const long Values[] = {wmSwitcherPanel::PAGE_HIDDEN,wmSwitcherPanel::PAGE_NONE,wmSwitcherPanel::PAGE_FLAT,wmSwitcherPanel::PAGE_BUTTON};
    static const wxChar* Names[] = {wxT("Hide"), wxT("Text"), wxT("Flat"), wxT("Button"), NULL};

    WXS_ENUM(wxswmSwitcherPanel, m_nPageName, _("Page Name"), _T("pagename"), Values, Names, 0);
}

bool wxswmSwitcherPanel::OnCanAddChild(wxsItem* Item,bool ShowMessage)
{
    if ( Item->GetType() == wxsTSizer )
    {
        if ( ShowMessage )
        {
            wxMessageBox(_("Can not add sizer into wmSwitcherPanel.\nAdd panels first"));
        }
        return false;
    }

    return wxsContainer::OnCanAddChild(Item,ShowMessage);
}

wxsPropertyContainer* wxswmSwitcherPanel::OnBuildExtra()
{
    return new wxswmSwitcherPanelExtra();
}

wxString wxswmSwitcherPanel::OnXmlGetExtraObjectClass()
{
    return _T("wmSwitcherPanelpage");
}

void wxswmSwitcherPanel::OnAddChildQPP(wxsItem* Child,wxsAdvQPP* QPP)
{
    wxswmSwitcherPanelExtra* NBExtra = (wxswmSwitcherPanelExtra*)GetChildExtra(GetChildIndex(Child));
    if ( NBExtra )
    {
        QPP->Register(new wxswmSwitcherPanelParentQP(QPP,NBExtra),_("wmSwitcherPanel"));
    }
}

wxObject* wxswmSwitcherPanel::OnBuildPreview(wxWindow* Parent,long PreviewFlags)
{
    UpdateCurrentSelection();
    wmSwitcherPanel* Notebook = new wmSwitcherPanel(Parent,-1,Pos(Parent),Size(Parent),Style());

    if ( !GetChildCount() && !(PreviewFlags&pfExact) )
    {
        // Adding additional empty notebook to prevent from having zero-sized notebook
        Notebook->AddPage(
            new wxWindow(Notebook,-1,wxDefaultPosition,wxSize(50,50)),
            _("No pages"));
    }

    AddChildrenPreview(Notebook,PreviewFlags);

    for ( int i=0; i<GetChildCount(); i++ )
    {
        wxsItem* Child = GetChild(i);
        wxswmSwitcherPanelExtra* NBExtra = (wxswmSwitcherPanelExtra*)GetChildExtra(i);

        wxPanel* ChildPreview = wxDynamicCast(GetChild(i)->GetLastPreview(),wxPanel);
        if ( !ChildPreview ) continue;

        bool Selected = (Child == m_CurrentSelection);
        if ( PreviewFlags & pfExact ) Selected = NBExtra->m_Selected;

        Notebook->AddPage(ChildPreview,NBExtra->m_Label,Selected);
    }

    Notebook->SetPageNameStyle(m_nPageName);

    return Notebook;
}

void wxswmSwitcherPanel::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmswitcherpanel.h\""),GetInfo().ClassName,0);
            AddHeader(_T("<wx/notebook.h>"),_T("wxNotebookEvent"),0);
            Codef(_T("%C(%W, %I, %P, %S, %T, %N);\n"));
            Codef(_T("%ASetPageNameStyle(%d);\n"),m_nPageName);
            BuildSetupWindowCode();
            AddChildrenCode();

            for ( int i=0; i<GetChildCount(); i++ )
            {
                wxswmSwitcherPanelExtra* NBExtra = (wxswmSwitcherPanelExtra*)GetChildExtra(i);
                Codef(_T("%AAddPage(%o, %t, %b);\n"),i,NBExtra->m_Label.wx_str(),NBExtra->m_Selected);
            }

            break;
        }

        case wxsUnknownLanguage: // fall-through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmSwitcherPanel::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

bool wxswmSwitcherPanel::OnMouseClick(wxWindow* Preview,int PosX,int PosY)
{
    UpdateCurrentSelection();
    wmSwitcherPanel* Notebook = (wmSwitcherPanel*)Preview;
    int Hit = Notebook->HitTest(wxPoint(PosX,PosY));
    if ( Hit != wxNOT_FOUND )
    {
        wxsItem* OldSel = m_CurrentSelection;
        m_CurrentSelection = GetChild(Hit);
        GetResourceData()->SelectItem(m_CurrentSelection,true);
        return OldSel != m_CurrentSelection;
    }
    return false;
}

bool wxswmSwitcherPanel::OnIsChildPreviewVisible(wxsItem* Child)
{
    UpdateCurrentSelection();
    return Child == m_CurrentSelection;
}

bool wxswmSwitcherPanel::OnEnsureChildPreviewVisible(wxsItem* Child)
{
    if ( IsChildPreviewVisible(Child) ) return false;
    m_CurrentSelection = Child;
    UpdateCurrentSelection();
    return true;
}

void wxswmSwitcherPanel::UpdateCurrentSelection()
{
    wxsItem* NewCurrentSelection = 0;
    for ( int i=0; i<GetChildCount(); i++ )
    {
        if ( m_CurrentSelection == GetChild(i) ) return;
        wxswmSwitcherPanelExtra* NBExtra = (wxswmSwitcherPanelExtra*)GetChildExtra(i);
        if ( (i==0) || NBExtra->m_Selected )
        {
            NewCurrentSelection = GetChild(i);
        }
    }
    m_CurrentSelection = NewCurrentSelection;
}

void wxswmSwitcherPanel::OnPreparePopup(wxMenu* Menu)
{
    Menu->Append(popupNewPageId,_("Add new page"));
    Menu->AppendSeparator();
    // This require some extra fixing
    //wxMenuItem* Item1 = Menu->Append(popupPrevPageId,_("Go to previous page"));
    //wxMenuItem* Item2 = Menu->Append(popupNextPageId,_("Go to next page"));
    //Menu->AppendSeparator();
    wxMenuItem* Item3 = Menu->Append(popupFirstId,_("Make current page the first one"));
    wxMenuItem* Item4 = Menu->Append(popupLastId,_("Make current page the last one"));
    if ( !m_CurrentSelection || GetChildIndex(m_CurrentSelection)==0 )
    {
        //Item1->Enable(false);
        Item3->Enable(false);
    }
    if ( !m_CurrentSelection || GetChildIndex(m_CurrentSelection)==GetChildCount()-1 )
    {
        //Item2->Enable(false);
        Item4->Enable(false);
    }
}

bool wxswmSwitcherPanel::OnPopup(long Id)
{
    if ( Id == popupNewPageId )
    {
        wxTextEntryDialog Dlg(0,_("Enter name of new page"),_("Adding page"),_("New page"));
        if ( Dlg.ShowModal() == wxID_OK )
        {
            wxsItem* Panel = wxsItemFactory::Build(_T("wxPanel"),GetResourceData());
            if ( Panel )
            {
                GetResourceData()->BeginChange();
                if ( AddChild(Panel) )
                {
                    wxswmSwitcherPanelExtra* NBExtra = (wxswmSwitcherPanelExtra*)GetChildExtra(GetChildCount()-1);
                    if ( NBExtra )
                    {
                        NBExtra->m_Label = Dlg.GetValue();
                    }
                    m_CurrentSelection = Panel;
                }
                else
                {
                    delete Panel;
                }
                GetResourceData()->EndChange();
            }
        }
    }
    else if ( Id == popupNextPageId )
    {
        GetResourceData()->BeginChange();
        int Index = GetChildIndex(m_CurrentSelection);
        m_CurrentSelection = GetChild(Index-1);
        UpdateCurrentSelection();
        GetResourceData()->EndChange();
    }
    else if ( Id == popupPrevPageId )
    {
        GetResourceData()->BeginChange();
        int Index = GetChildIndex(m_CurrentSelection);
        m_CurrentSelection = GetChild(Index+1);
        UpdateCurrentSelection();
        GetResourceData()->EndChange();
    }
    else if ( Id == popupFirstId )
    {
        GetResourceData()->BeginChange();
        MoveChild(GetChildIndex(m_CurrentSelection),0);
        GetResourceData()->EndChange();
    }
    else if ( Id == popupLastId )
    {
        GetResourceData()->BeginChange();
        MoveChild(GetChildIndex(m_CurrentSelection),GetChildCount()-1);
        GetResourceData()->EndChange();
    }
    else
    {
        return wxsContainer::OnPopup(Id);
    }
    return true;
}
