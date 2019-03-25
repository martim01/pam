#include "dlgWiFi.h"

#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/log.h>
#include "networkcontrol.h"
#include "settings.h"

BEGIN_EVENT_TABLE(dlgWiFi,wxDialog)
END_EVENT_TABLE()

dlgWiFi::dlgWiFi(wxWindow* parent,const wxString& sWiFi, const wxString& sInterface, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_sWifi(sWiFi),
m_sInterface(sInterface)
{
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxPoint(0,0));
	SetForegroundColour(wxColour(255,255,255));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(14,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pKbd1 = new wmKeyboard(this, wxNewId(), wxPoint(10,190), wxDefaultSize, 0, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	m_plblPassword= new wmLabel(this, wxNewId(), wxT("Password"), wxPoint(10, 140), wxSize(80,40));
	m_plblPassword->SetForegroundColour(*wxWHITE);

	m_pedtPassword = new wmEdit(this, wxNewId(), wxEmptyString, wxPoint(90,140), wxSize(400,40), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtPassword->SetBackgroundColour(wxColour(255,255,255));
	m_pedtPassword->SetBorderStyle(1,1);

	m_pbtnScan = new wmButton(this, wxNewId(), _("Scan"), wxPoint(690,300), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));

	m_pbtnSave = new wmButton(this, wxNewId(), _("Save"), wxPoint(690,380), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSave->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnSave->SetColourDisabled(wxColour(120,120,120));
	m_pbtnSave->Disable();
	m_pbtnCancel = new wmButton(this, wxNewId(), _("Cancel"), wxPoint(690,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));

	m_plstWiFi = new wmList(this, wxNewId(), wxPoint(10,10), wxSize(780,120), wmList::STYLE_SELECT, wmList::SCROLL_VERTICAL, wxSize(-1,30), 4, wxSize(-1,-1));
	m_plstWiFi->SetBackgroundColour(wxColour(0,0,0));

	Connect(m_pedtPassword->GetId(),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgWiFi::OnedtPasswordText);
	Connect(m_pedtPassword->GetId(),wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgWiFi::OnedtPasswordTextEnter);
	Connect(m_pbtnSave->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWiFi::OnbtnSaveClick);
	Connect(m_pbtnCancel->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWiFi::OnbtnCancelClick);
	Connect(m_pbtnScan->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWiFi::OnbtnScanClick);
	Connect(m_plstWiFi->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgWiFi::OnlstWiFiSelected);
	//*)

	m_pbtnSave->Enable(false);
	m_pedtPassword->Show(false);
	m_pKbd1->Show(false);

    Scan();

}

dlgWiFi::~dlgWiFi()
{
	//(*Destroy(dlgWiFi)
	//*)
}


void dlgWiFi::OnedtPasswordText(wxCommandEvent& event)
{
    m_pbtnSave->Enable(m_pedtPassword->GetValue()!=wxEmptyString);
}

void dlgWiFi::OnedtPasswordTextEnter(wxCommandEvent& event)
{
    if(m_pedtPassword->GetValue() != wxEmptyString)
    {
        EndModal(wxID_OK);
    }
}

void dlgWiFi::OnbtnSaveClick(wxCommandEvent& event)
{
    //@todo save the settings
    NetworkControl::Get().ChangeWiFiNetwork(m_sWifi, m_pedtPassword->GetValue(), m_sInterface);
    EndModal(wxID_OK);
}

void dlgWiFi::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void dlgWiFi::OnlstWiFiSelected(wxCommandEvent& event)
{
    m_sWifi = event.GetString();
    std::map<wxString, wifi_cell>::const_iterator itCell = NetworkControl::Get().FindWiFiCell(event.GetString());
    if(itCell != NetworkControl::Get().GetWiFiCellEnd())
    {
        if(itCell->second.bEncryption)
        {
            m_pedtPassword->Show(true);
            m_pKbd1->Show();
            m_pedtPassword->SetValue(Settings::Get().Read(wxT("WiFi"), m_sWifi, wxEmptyString));
            m_pedtPassword->SetFocus();
            m_pbtnSave->Enable((m_pedtPassword->GetValue().empty()==false));
        }
        else
        {
            m_pedtPassword->Hide();
            m_pKbd1->Hide();
            m_pbtnSave->Enable();
        }
    }
}


void dlgWiFi::Scan()
{
    // Scan
    m_plstWiFi->Clear();
    NetworkControl::Get().ScanWiFi(m_sInterface);
    for(std::map<wxString, wifi_cell>::const_iterator itCell = NetworkControl::Get().GetWiFiCellBegin(); itCell != NetworkControl::Get().GetWiFiCellEnd(); ++itCell)
    {

        size_t nIndex(0);
        if(itCell->second.bEncryption)
        {
            nIndex = m_plstWiFi->AddButton(itCell->second.sESSID, wxNullBitmap, 0,wmList::wmENABLED, wxColour(0,128,0));
        }
        else
        {
            nIndex = m_plstWiFi->AddButton(itCell->second.sESSID, wxNullBitmap, 0,wmList::wmENABLED, wxColour(128,64,0));
        }
        if(itCell->second.nNetwork != -1)
        {
            m_plstWiFi->SetTextButtonColour(nIndex, *wxBLACK);
        }
    }

    m_plstWiFi->SelectButton(m_sWifi);
}

void dlgWiFi::OnbtnScanClick(wxCommandEvent& event)
{
    m_pbtnSave->Enable(false);
    m_pedtPassword->Show(false);
    m_pKbd1->Show(false);
    Scan();
}
