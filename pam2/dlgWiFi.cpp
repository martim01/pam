#include "dlgWiFi.h"

#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>


BEGIN_EVENT_TABLE(dlgWiFi,wxDialog)
END_EVENT_TABLE()

dlgWiFi::dlgWiFi(wxWindow* parent,const wxString& sWiFi, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_nChannels(0)
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

	m_pbtnSave = new wmButton(this, wxNewId(), _("Save"), wxPoint(690,380), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSave->Disable();
	m_pbtnCancel = new wmButton(this, wxNewId(), _("Cancel"), wxPoint(690,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));

	m_plstWiFi = new wmList(this, wxNewId(), wxPoint(10,10), wxSize(780,120), wmList::STYLE_SELECT, wmList::SCROLL_VERTICAL, wxSize(-1,30), 8, wxSize(-1,-1));
	m_plstWiFi->SetBackgroundColour(wxColour(0,0,0));

	Connect(m_pedtPassword->GetId(),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgWiFi::OnedtPasswordText);
	Connect(m_pedtPassword->GetId(),wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgWiFi::OnedtPasswordTextEnter);
	Connect(m_pbtnSave->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWiFi::OnbtnSaveClick);
	Connect(m_pbtnCancel->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWiFi::OnbtnCancelClick);
	Connect(m_plstWiFi->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgWiFi::OnlstWiFiSelected);
	//*)

	m_pbtnSave->Enable(false);
	m_pedtPassword->Enable(false);

    // Scan
    wxArrayString asAccessPoints;
    wxExecute(wxT("iwlist wlan0 scan"), asAccessPoints);
    for(size_t i = 0; i < asAccessPoints.size(); i++)
    {
        if(asAccessPoints[i].Find(wxT("ESSID")) != wxNOT_FOUND)
        {
            m_plstWiFi->AddButton(asAccessPoints[i].AfterFirst(wxT('"')).BeforeFirst(wxT('"')));
        }
    }


    m_plstWiFi->SelectButton(sWiFi);

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

    EndModal(wxID_OK);
}

void dlgWiFi::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void dlgWiFi::OnlstWiFiSelected(wxCommandEvent& event)
{
    m_pedtPassword->Enable(true);
    m_pedtPassword->SetFocus();
}
