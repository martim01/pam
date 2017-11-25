#include "pnlSettingsPlugins.h"
#include "settings.h"
#include <wx/dir.h>
#include <wx/dynlib.h>
#include "monitorpluginfactory.h"
#include <wx/filename.h>
//(*InternalHeaders(pnlSettingsPlugins)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using   namespace std;

//(*IdInit(pnlSettingsPlugins)
const long pnlSettingsPlugins::ID_M_PLST1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL2 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLST2 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN2 = wxNewId();
const long pnlSettingsPlugins::ID_PANEL1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsPlugins,wxPanel)
	//(*EventTable(pnlSettingsPlugins)
	//*)
END_EVENT_TABLE()

pnlSettingsPlugins::pnlSettingsPlugins(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsPlugins)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_pswpPlugins = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,5), wxSize(600,400), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpPlugins->SetPageNameStyle(3);
	m_pswpPlugins->SetBackgroundColour(wxColour(0,0,0));
	Panel1 = new wxPanel(m_pswpPlugins, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_plstPossible = new wmList(Panel1, ID_M_PLST1, wxPoint(10,40), wxSize(200,350), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
	m_plstPossible->SetBackgroundColour(wxColour(0,0,0));
	m_plstPossible->SetButtonColour(wxColour(wxT("#008000")));
	m_plstPossible->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pLbl1 = new wmLabel(Panel1, ID_M_PLBL1, _("Possible Plugins"), wxPoint(10,10), wxSize(200,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,160));
	m_pLbl2 = new wmLabel(Panel1, ID_M_PLBL2, _("Current Plugins"), wxPoint(390,10), wxSize(200,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,160));
	m_plstCurrent = new wmList(Panel1, ID_M_PLST2, wxPoint(390,40), wxSize(200,350), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
	m_plstCurrent->SetBackgroundColour(wxColour(0,0,0));
	m_plstCurrent->SetButtonColour(wxColour(wxT("#008000")));
	m_plstCurrent->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pbtnSet = new wmButton(Panel1, ID_M_PBTN1, _("Hold To Set"), wxPoint(250,30), wxSize(100,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSet->Disable();
	m_pbtnSet->SetBackgroundColour(wxColour(0,155,0));
	m_pbtnSet->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnClear = new wmButton(Panel1, ID_M_PBTN2, _("Hold To Clear"), wxPoint(250,100), wxSize(100,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnClear->Disable();
	m_pbtnClear->SetBackgroundColour(wxColour(155,0,0));
	m_pbtnClear->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pswpPlugins->AddPage(Panel1, _("Monitor"), false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsPlugins::OnlstPossibleSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsPlugins::OnlstCurrentSelected);
	Connect(ID_M_PBTN1,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsPlugins::OnpbtnSetHeld);
	Connect(ID_M_PBTN2,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnClearHeld);
	//*)
    SetBackgroundColour(*wxBLACK);
    #ifdef __WXDEBUG__
    wxString sLibDir = wxT("lib/debug");
    #else
    wxString sLibDir = wxT("lib");
    #endif // __WXDEBUG__

    size_t nButton = 0;
    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(wxT("Monitor Plugins"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("Monitor Plugins"), itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            wxString sPlugin = MonitorPluginFactory::Get()->GetPluginName(sLibDir, itBegin->second);
            if(sPlugin != wxEmptyString)
            {
                m_plstCurrent->AddButton(sPlugin);
            }
        }
    }

    nButton = m_plstCurrent->AddButton(wxT("---"));
    m_plstCurrent->SetButtonColour(nButton, wxColour(150,150,150));




    wxArrayString asLibs;
    wxDir::GetAllFiles(sLibDir, &asLibs);

    for(size_t i = 0; i < asLibs.GetCount(); i++)
    {
        wxFileName fnLib(asLibs[i]);

        wxString sPlugin = MonitorPluginFactory::Get()->GetPluginName(sLibDir, fnLib.GetName().Mid(3));
        if(sPlugin != wxEmptyString)
        {
            m_mPossible.insert(make_pair(sPlugin, fnLib.GetName()));
            m_plstPossible->AddButton(sPlugin);
        }
    }

}

pnlSettingsPlugins::~pnlSettingsPlugins()
{
	//(*Destroy(pnlSettingsPlugins)
	//*)
}


void pnlSettingsPlugins::OnlstPossibleSelected(wxCommandEvent& event)
{
    m_itPossible = m_mPossible.find(event.GetString());

    m_pbtnSet->Enable((m_plstCurrent->GetSelectionCount() == 1) && m_plstPossible->GetSelectionCount() == 1);
}

void pnlSettingsPlugins::OnlstCurrentSelected(wxCommandEvent& event)
{
    m_nSelected = event.GetInt();
    m_pbtnSet->Enable((m_plstCurrent->GetSelectionCount() == 1) && m_plstPossible->GetSelectionCount() == 1);
    m_pbtnClear->Enable((m_plstCurrent->GetSelectionCount() == 1));
}

void pnlSettingsPlugins::OnpbtnSetHeld(wxCommandEvent& event)
{
    if(m_itPossible != m_mPossible.end())
    {
        Settings::Get().Write(wxT("Monitor Plugins"), wxString::Format(wxT("%04d"), m_nSelected+1), m_itPossible->second);

        wxString sCurrent(m_plstCurrent->GetButtonText(m_nSelected));
        if(sCurrent != wxT("---"))
        {
            // @todo remove plugin
        }

        m_plstCurrent->SetButtonText(m_nSelected, m_itPossible->first);
        m_plstCurrent->SetButtonColour(m_nSelected, wxColour(0,128,0));

        if(m_nSelected == m_plstCurrent->GetItemCount()-1)
        {
            int nButton = m_plstCurrent->AddButton(wxT("---"));
            m_plstCurrent->SetButtonColour(nButton, wxColour(150,150,150));
        }

        // @todo load plugin
    }
}

void pnlSettingsPlugins::OnbtnClearHeld(wxCommandEvent& event)
{
}
