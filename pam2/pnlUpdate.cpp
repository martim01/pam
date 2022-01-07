#include "pnlUpdate.h"

//(*InternalHeaders(pnlUpdate)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "settings.h"
#include <wx/dir.h>
#include "folder.xpm"
#include <wx/log.h>

#ifdef __WXMSW__
#include <wx/volume.h>
#endif // __WXMSW__

using namespace std;

//(*IdInit(pnlUpdate)
const long pnlUpdate::ID_M_PLBL3 = wxNewId();
const long pnlUpdate::ID_M_PLST2 = wxNewId();
const long pnlUpdate::ID_PANEL4 = wxNewId();
const long pnlUpdate::ID_M_PSWP1 = wxNewId();
const long pnlUpdate::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlUpdate,wxPanel)
	//(*EventTable(pnlUpdate)
	//*)
END_EVENT_TABLE()

pnlUpdate::pnlUpdate(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, unsigned long n, const wxString& s)
{
	//(*Initialize(pnlUpdate)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Manage Update Settings"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_plstType = new wmList(this, ID_M_PLST2, wxPoint(0,36), wxSize(600,34), wmList::STYLE_SELECT, 0, wxSize(100,30), 3, wxSize(-1,-1));
	m_plstType->SetButtonColour(wxColour(wxT("#400080")));
	m_plstType->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pswpType = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,70), wxSize(600,315), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpType->SetPageNameStyle(0);
	m_pnlUSB = new pnlUSB(m_pswpType, ID_PANEL4, wxPoint(0,0), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	m_pnlUSB->SetBackgroundColour(wxColour(0,0,0));
	m_pswpType->AddPage(m_pnlUSB, _("USB"), false);
	m_pbtnCheck = new wmButton(this, ID_M_PBTN1, _("Check For Updates"), wxPoint(200,390), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnCheck->SetBackgroundColour(wxColour(67,167,69));
	wxFont m_pbtnCheckFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnCheck->SetFont(m_pbtnCheckFont);

	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlUpdate::OnlstTypeSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlUpdate::OnbtnCheckClick);
	//*)
	m_pbtnCheck->SetColourDisabled(wxColour(120,120,120));
    m_plstFolders->SetGradient(0);
    m_plstFolders->SetTextAlign(wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL);
    m_plstFolders->SetBitmapAlign(wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);
	SetSize(size);
	SetPosition(pos);

    m_plstType->AddButton(wxT("USB"));

	m_plstType->SelectButton("USB", true);

	m_plblLocation->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblLocation->GetUiRect().Pad(10,0);


    Connect(wxEVT_SHOW, (wxObjectEventFunction)&pnlUpdate::OnShown);

}

pnlUpdate::~pnlUpdate()
{
	//(*Destroy(pnlUpdate)
	//*)
}


void pnlUpdate::OnlstTypeSelected(wxCommandEvent& event)
{
    m_pswpType->ChangeSelection(event.GetString());
    if(event.GetString() == wxT("USB"))
    {
        m_pnlUSB->StartCheck();
    }
    else
    {
        m_pnlUSB->StopCheck();
        m_pbtnCheck->Enable();
    }

    Settings::Get().Write(wxT("Update"), wxT("Type"), event.GetString());
}
void pnlUpdate::OnbtnCheckClick(wxCommandEvent& event)
{
     #ifndef NDEBUG
        #ifdef __WXMSW__
            wxString sUpdate = wxT("C:\\developer\\matt\\pam2\\pamupdatemanager\\bin\\Debug\\pamupdatemanager");
        #else
          wxString sUpdate = wxT("pamupdatemanager");
        #endif
    #else
        #ifdef __WXMSW__
            wxString sUpdate = wxT("pamupdatemanager");
        #else
        wxString sUpdate = wxT("sudo pamupdatemanager");
        #endif // __WXMSW__
    #endif

    wxExecute(sUpdate);
}

void pnlUpdate::OnShown(wxShowEvent& event)
{
    if(event.IsShown() == false)
    {
        m_pnlUSB->StopCheck();
    }
    else if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Shared")) == wxT("USB"))
    {
        m_pnlUSB->StartCheck();
    }
}
