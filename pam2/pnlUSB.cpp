#include "pnlUSB.h"
#include "settings.h"

//(*InternalHeaders(pnlUSB)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlUSB)
const long pnlUSB::ID_M_PLBL13 = wxNewId();
const long pnlUSB::ID_M_PLBL12 = wxNewId();
const long pnlUSB::ID_M_PBTN6 = wxNewId();
const long pnlUSB::ID_M_PLST3 = wxNewId();
//*)

using namespace std;

BEGIN_EVENT_TABLE(pnlUSB,wxPanel)
	//(*EventTable(pnlUSB)
	//*)
END_EVENT_TABLE()

pnlUSB::pnlUSB(wxWindow* parent, wxWindowID id,const wxPoint& pos,const wxSize& size, int nStyle, const wxString& sn)
{
	//(*Initialize(pnlUSB)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl7 = new wmLabel(this, ID_M_PLBL13, _("Connect a USB Drive and press Detect button"), wxPoint(0,0), wxSize(600,60), 0, _T("ID_M_PLBL13"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	wxFont m_pLbl7Font(18,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	m_plblUSB = new wmLabel(this, ID_M_PLBL12, wxEmptyString, wxPoint(0,100), wxSize(600,100), 0, _T("ID_M_PLBL12"));
	m_plblUSB->SetBorderState(uiRect::BORDER_NONE);
	m_plblUSB->GetUiRect().SetGradient(0);
	m_plblUSB->SetForegroundColour(wxColour(0,255,0));
	m_plblUSB->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	wxFont m_plblUSBFont(18,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblUSB->SetFont(m_plblUSBFont);
	m_pbtnDetect = new wmButton(this, ID_M_PBTN6, _("Detect USB Drive"), wxPoint(200,60), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnDetect->SetBackgroundColour(wxColour(0,0,255));
	wxFont m_pbtnDetectFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnDetect->SetFont(m_pbtnDetectFont);
	m_plstUsb = new wmList(this, ID_M_PLST3, wxPoint(0,200), wxSize(600,120), wmList::STYLE_SELECT, 0, wxSize(-1,40), 6, wxSize(5,5));
	m_plstUsb->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlUSB::OnbtnDetectClick);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlUSB::OnlstUsbSelected);
	//*)
	SetPosition(pos);
    m_plblUSB->SetBackgroundColour(*wxBLACK);
	SetSize(size);
    SetBackgroundColour(*wxBLACK);
	m_timerUSB.SetOwner(this, wxNewId());
    Connect(m_timerUSB.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&pnlUSB::OnTimerUSB);
}

pnlUSB::~pnlUSB()
{
	//(*Destroy(pnlUSB)
	//*)
}

void pnlUSB::SetSection(const wxString& sSection)
{
    m_sSection = sSection;
}


void pnlUSB::OnbtnDetectClick(wxCommandEvent& event)
{
    CheckUSB();
}



void pnlUSB::StartCheck()
{
    if(IsShownOnScreen())
    {
        Settings::Get().Write(m_sSection, wxT("USB"), wxEmptyString);
        CheckUSB();
        m_timerUSB.Start(2000, true);
    }
}

void pnlUSB::StopCheck()
{
    m_timerUSB.Stop();
}


void pnlUSB::CheckUSB()
{
    m_plstUsb->Freeze();
    m_plstUsb->Clear();
    wxArrayString asFiles;
    #ifdef __WXGNU__
    wxExecute(wxT("sh -c \"lsblk -l -o name,label | grep sd \""), asFiles);
    #endif // __WXGNU__

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        //NAME="sda1" LABEL="KINGSTON"
        wxString sDevice(asFiles[i].BeforeFirst(wxT(' ')));
        wxString sLabel(asFiles[i].AfterFirst(wxT(' ')).Trim(false));
        if(sLabel.empty() == false)
        {
            m_mUsb.insert(make_pair(sLabel, sDevice));
            m_plstUsb->AddButton(sLabel);
        }
    }
    if(m_plstUsb->GetItemCount() == 0)
    {
        m_plblUSB->SetLabel(wxT("No USB Drives Detected"));
        m_plblUSB->SetForegroundColour(*wxRED);
        m_pbtnDetect->Show();
        m_pLbl7->Show();
    }
    else if(m_plstUsb->GetItemCount() == 1)
    {
        m_plblUSB->SetLabel(wxT("USB Drive Detected"));
        m_plblUSB->SetForegroundColour(*wxGREEN);
        m_plstUsb->SelectButton(0, true);
        m_plstUsb->Hide();
        m_pbtnDetect->Hide();
        m_pLbl7->Hide();
    }
    else
    {
        m_plblUSB->SetLabel(wxString::Format(wxT("%d USB Drives Detected.\nPlease select from the list."), m_plstUsb->GetItemCount()));
        m_plblUSB->SetForegroundColour(*wxGREEN);
        m_plstUsb->Show();
        m_pbtnDetect->Hide();
        for(map<wxString, wxString>::iterator itUSB = m_mUsb.begin(); itUSB != m_mUsb.end(); ++itUSB)
        {
            if(itUSB->second == Settings::Get().Read(wxT("Update"), wxT("USB"), wxEmptyString))
            {
                m_plstUsb->SelectButton(itUSB->first, true);
                break;
            }
        }

        m_pLbl7->Hide();
    }

    m_plstUsb->Thaw();

    if(IsShownOnScreen())
    {
        m_timerUSB.Start(2000, true);
    }
}

void pnlUSB::OnlstUsbSelected(wxCommandEvent& event)
{
    map<wxString, wxString>::iterator itDevice = m_mUsb.find(event.GetString());
    if(itDevice != m_mUsb.end())
    {
        Settings::Get().Write(m_sSection, wxT("USB"), itDevice->second);
    }
}


void pnlUSB::OnTimerUSB(wxTimerEvent& event)
{
    CheckUSB();
}
