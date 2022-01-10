#include "pamupdatemanagerMain.h"
#include "version.h"
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"
#include "settings.h"
#include "updatemanager.h"
#include <wx/log.h>
#include <wx/wfstream.h>
#include <wx/tarstrm.h>

//(*InternalHeaders(pamupdatemanagerDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pamupdatemanagerDialog)
const long pamupdatemanagerDialog::ID_M_PLBL2 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL3 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL2 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PEDT1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PKBD1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL1 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL3 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PSWP1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN4 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN1 = wxNewId();
const long pamupdatemanagerDialog::ID_TIMER2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pamupdatemanagerDialog,wxDialog)
	//(*EventTable(pamupdatemanagerDialog)
	//*)
END_EVENT_TABLE()

pamupdatemanagerDialog::pamupdatemanagerDialog(wxWindow* parent,const wxString& sDevice, const wxFileName& fnUpdate, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_sDevice(sDevice), m_fnUpdate(fnUpdate)
{
	//(*Initialize(pamupdatemanagerDialog)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(255,255,255));
	wxFont thisFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_plblTitle = new wmLabel(this, ID_M_PLBL2, _("PAM Update Manager"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL2"));
	m_plblTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblTitle->GetUiRect().SetGradient(0);
	m_plblTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblTitle->SetBackgroundColour(wxColour(45,36,81));
	wxFont m_plblTitleFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblTitle->SetFont(m_plblTitleFont);
	m_pswpMain = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,40), wxSize(800,400), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpMain->SetPageNameStyle(0);
	m_ppnlProgress = new wxPanel(m_pswpMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlRelease = new wxPanel(m_pswpMain, ID_PANEL2, wxDefaultPosition, wxSize(0,0), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_pLbl2 = new wmLabel(m_ppnlRelease, ID_M_PLBL3, _("Release Notes"), wxPoint(0,5), wxSize(800,35), 0, _T("ID_M_PLBL3"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl2Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_ppnlPassword = new wxPanel(m_pswpMain, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_ppnlPassword->SetBackgroundColour(wxColour(0,0,0));
	m_pedtPassword = new wmEdit(m_ppnlPassword, ID_M_PEDT1, wxEmptyString, wxPoint(110,40), wxSize(600,45), wxTE_PROCESS_ENTER|wxTE_PASSWORD, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtPassword->SetValidation(3);
	m_pedtPassword->SetBackgroundColour(wxColour(255,255,255));
	m_pedtPassword->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
	m_pedtPassword->SetFocusedForeground(wxColour(wxT("#000000")));
	m_pedtPassword->SetBorderStyle(1,1);
	m_pkeyboard = new wmKeyboard(m_ppnlPassword, ID_M_PKBD1, wxPoint(110,90), wxDefaultSize, 0, 0);
	m_pkeyboard->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1 = new wmLabel(m_ppnlPassword, ID_M_PLBL1, _("Enter Password"), wxPoint(0,5), wxSize(800,35), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(0,255,0));
	wxFont m_pLbl1Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pswpMain->AddPage(m_ppnlProgress, _("Progress"), false);
	m_pswpMain->AddPage(m_ppnlRelease, _("Release"), true);
	m_pswpMain->AddPage(m_ppnlPassword, _("Password"), false);
	m_pbtnCancel = new wmButton(this, ID_M_PBTN4, _("Cancel Update"), wxPoint(240,443), wxSize(100,35), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnCancel->Disable();
	m_pbtnCancel->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnCancel->SetColourSelected(wxColour(wxT("#FF0000")));
	m_pbtnCancel->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnUpdate = new wmButton(this, ID_M_PBTN1, _("Update"), wxPoint(460,443), wxSize(100,35), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnUpdate->Disable();
	m_pbtnUpdate->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnUpdate->SetColourSelected(wxColour(wxT("#00FF00")));
	m_pbtnUpdate->SetColourDisabled(wxColour(wxT("#909090")));
	m_timerStart.SetOwner(this, ID_TIMER2);
	m_timerStart.Start(100, true);

	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pamupdatemanagerDialog::OnedtPasswordTextEnter);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnCancelClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnUpdateClick);
	Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&pamupdatemanagerDialog::OntimerStartTrigger);
	//*)

    Settings::Get().Write(wxT("Version"), wxT("pamupdatemanager"), wxString::Format(wxT("%d.%d.%d.%d"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD, AutoVersion::REVISION));

    m_plstRelease = new wmListAdv(this, wxNewId(), wxPoint(0,0), wxSize(600,480), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
	m_plstRelease->SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));
	m_plstRelease->SetBackgroundColour(wxColour(100,100,180));

	m_plblTitle->SetLabel("PAM Update Manager - " + m_fnUpdate.GetName());
}

pamupdatemanagerDialog::~pamupdatemanagerDialog()
{
	//(*Destroy(pamupdatemanagerDialog)
	//*)
	#ifdef __WXGNU__
	wxExecute(wxT("sudo umount /mnt/share"), wxEXEC_SYNC);
	#endif // __WXGNU__
}


void pamupdatemanagerDialog::OnedtPasswordTextEnter(wxCommandEvent& event)
{
}

void pamupdatemanagerDialog::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void pamupdatemanagerDialog::OnbtnUpdateClick(wxCommandEvent& event)
{

}

void pamupdatemanagerDialog::OntimerStartTrigger(wxTimerEvent& event)
{
    if(UsbChecker::MountDevice(m_sDevice) != 0)
    {

    }
    else
    {
        wxFileInputStream in(m_fnUpdate.GetFullPath());
        wxTarInputStream tar(in);

        wxTarEntry* pEntry = nullptr;
        do
        {
            pEntry = tar.GetNextEntry();
            if(pEntry && pEntry->GetName().CmpNoCase("release.md") == 0)
            {
                wxString str;
                str << tar;
                break;
            }
        }while(pEntry);
    }
}
