#include "pnlAoIPSelection.h"
#include "settings.h"
#include <wx/app.h>
#include "pam2Main.h"

//(*InternalHeaders(pnlAoIPSelection)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlAoIPSelection)
const long pnlAoIPSelection::ID_M_PLBL3 = wxNewId();
const long pnlAoIPSelection::ID_M_PLST1 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN1 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN2 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN4 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN5 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlAoIPSelection,wxPanel)
	//(*EventTable(pnlAoIPSelection)
	//*)
END_EVENT_TABLE()

using namespace std;

pnlAoIPSelection::pnlAoIPSelection(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, int n, const wxString& s)
{
	//(*Initialize(pnlAoIPSelection)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("AES67 Source"), wxPoint(0,5), wxSize(800,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_plstDevices = new wmList(this, ID_M_PLST1, wxPoint(0,40), wxSize(800,390), wmList::STYLE_SELECT, 2, wxSize(-1,40), 5, wxSize(5,5));
	m_plstDevices->SetBackgroundColour(wxColour(0,0,0));
	m_plstDevices->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstDevices->SetDisabledColour(wxColour(wxT("#808080")));
	m_pbtnHome = new wmButton(this, ID_M_PBTN1, wxEmptyString, wxPoint(70,435), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnHome->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnPrevious = new wmButton(this, ID_M_PBTN2, wxEmptyString, wxPoint(190,435), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnPrevious->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNext = new wmButton(this, ID_M_PBTN4, wxEmptyString, wxPoint(310,435), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnNext->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnEnd = new wmButton(this, ID_M_PBTN5, wxEmptyString, wxPoint(430,435), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnEnd->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnBack = new wmButton(this, ID_M_PBTN3, _("Back"), wxPoint(690,435), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnBack->SetForegroundColour(wxColour(255,255,255));
	m_pbtnBack->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnBackFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnBack->SetFont(m_pbtnBackFont);
	m_pbtnBack->SetColourDisabled(wxColour(wxT("#808080")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstDevicesSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnHomeClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnPreviousClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnNextClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnEndClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnBackClick);
	//*)
	Connect(wxEVT_SHOW, (wxObjectEventFunction)&pnlAoIPSelection::OnShown);

	SetPosition(pos);
	SetSize(size);
}

pnlAoIPSelection::~pnlAoIPSelection()
{
	//(*Destroy(pnlAoIPSelection)
	//*)
}

void pnlAoIPSelection::OnShown(wxShowEvent& event)
{
    if(event.IsShown())
    {
        m_plstDevices->Freeze();
        m_plstDevices->Clear();
        int i = 0;
        map<wxString, wxString>::const_iterator itBegin, itEnd;
        if(Settings::Get().GetSectionDataBegin(wxT("AoIP"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("AoIP"), itEnd))
        {
            for(map<wxString, wxString>::const_iterator itSource = itBegin; itSource != itEnd; ++itSource)
            {
                m_plstDevices->AddButton(itSource->first, wxNullBitmap, (void*)i);
                ++i;
            }
        }

        m_plstDevices->Thaw();

        ShowPagingButtons();

        m_plstDevices->SelectButton(Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString),false);

    }
}


void pnlAoIPSelection::OnlstDevicesSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Input"), wxT("AoIP"), event.GetString());
    ShowMainScreen();
}

void pnlAoIPSelection::OnbtnHomeClick(wxCommandEvent& event)
{
    m_plstDevices->ShowFirstPage(false,false);
}

void pnlAoIPSelection::OnbtnPreviousClick(wxCommandEvent& event)
{
    m_plstDevices->ShowPreviousPage(false, false);
}

void pnlAoIPSelection::OnbtnNextClick(wxCommandEvent& event)
{
    m_plstDevices->ShowNextPage(false, false);
}

void pnlAoIPSelection::OnbtnEndClick(wxCommandEvent& event)
{
    m_plstDevices->ShowLastPage(false, false);
}

void pnlAoIPSelection::ShowPagingButtons()
{
    m_pbtnEnd->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnHome->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnPrevious->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnNext->Show(m_plstDevices->GetPageCount() > 1);
}

void pnlAoIPSelection::OnbtnBackClick(wxCommandEvent& event)
{
    ShowMainScreen();
}

void pnlAoIPSelection::ShowMainScreen()
{
    wmSwitcherPanel* pPanel = dynamic_cast<wmSwitcherPanel*>(GetParent());
    if(pPanel)
    {
        pPanel->ChangeSelection("Main");
    }
}
