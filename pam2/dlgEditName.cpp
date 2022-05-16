#include "dlgEditName.h"

//(*InternalHeaders(dlgEditName)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgEditName)
const long dlgEditName::ID_M_PKBD1 = wxNewId();
const long dlgEditName::ID_M_PEDT1 = wxNewId();
const long dlgEditName::ID_M_PBTN1 = wxNewId();
const long dlgEditName::ID_M_PBTN2 = wxNewId();
const long dlgEditName::ID_M_PLST1 = wxNewId();
const long dlgEditName::ID_M_PLBL1 = wxNewId();
const long dlgEditName::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgEditName,wxDialog)
	//(*EventTable(dlgEditName)
	//*)
END_EVENT_TABLE()

dlgEditName::dlgEditName(wxWindow* parent,wxString sName, long nChannelCount, long nSelectedChannel, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_nSelectedChannel(nSelectedChannel)
{
	//(*Initialize(dlgEditName)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxPoint(0,0));
	SetForegroundColour(wxColour(255,255,255));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(14,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pKbd1 = new wmKeyboard(this, ID_M_PKBD1, wxPoint(10,90), wxDefaultSize, 0, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	m_pedtName = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(10,30), wxSize(600,50), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtName->SetValidation(3);
	m_pedtName->SetBackgroundColour(wxColour(255,255,255));
	m_pedtName->SetBorderStyle(1,1);
	m_pbtnSave = new wmButton(this, ID_M_PBTN1, _("Save"), wxPoint(160,400), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSave->Disable();
	m_pbtnCancel = new wmButton(this, ID_M_PBTN2, _("Cancel"), wxPoint(500,400), wxSize(100,0), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_ppnlChannel = new wxPanel(this, ID_PANEL1, wxPoint(664,30), wxSize(100,440), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlChannel->Hide();
	m_ppnlChannel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	m_plstChannels = new wmList(m_ppnlChannel, ID_M_PLST1, wxPoint(0,30), wxSize(100,410), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 1, wxSize(-1,-1));
	m_plstChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plblChannels = new wmLabel(m_ppnlChannel, ID_M_PLBL1, _("Channels"), wxPoint(0,0), wxSize(100,30), 0, _T("ID_M_PLBL1"));
	m_plblChannels->SetBorderState(uiRect::BORDER_NONE);
	m_plblChannels->GetUiRect().SetGradient(0);
	m_plblChannels->SetForegroundColour(wxColour(0,128,192));
	m_plblChannels->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblChannelsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_plblChannels->SetFont(m_plblChannelsFont);

	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgEditName::OnedtNameText);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgEditName::OnedtNameTextEnter);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEditName::OnbtnSaveClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEditName::OnbtnCancelClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgEditName::OnlstChannelsSelected);
	//*)
	m_pedtName->SetValue(sName);
	m_pedtName->SetFocus();

	if(nChannelCount == 1)
    {
        m_plstChannels->AddButton(wxT("Left"));
        m_ppnlChannel->Show();

        if(m_nSelectedChannel == 0)
        {
            m_plstChannels->SelectButton(0);
        }
    }
    else if(nChannelCount == 2)
    {
        m_plstChannels->AddButton(wxT("Left"));
        m_plstChannels->AddButton(wxT("Right"));
        m_ppnlChannel->Show();

        m_plstChannels->SelectButton(m_nSelectedChannel);
    }
    else if(nChannelCount > 2)
    {
        for(int i = 0; i < nChannelCount; i ++)
        {
            m_plstChannels->AddButton(wxString::Format("%d", i));
        }
        m_ppnlChannel->Show();
        m_plstChannels->SelectButton(m_nSelectedChannel);
    }
	else if(nChannelCount == CH_SERVER || nChannelCount == CH_POOL)
    {
        m_plstChannels->AddButton("Server");
        m_plstChannels->AddButton("Pool");
        m_ppnlChannel->Show();
        m_plstChannels->SelectButton(nChannelCount-CH_SERVER);
        m_pedtName->SetValidation(wmEdit::wmEdit::ASCII);
    }
	m_pbtnSave->Enable(m_pedtName->GetValue().empty()==false);

}

dlgEditName::~dlgEditName()
{
	//(*Destroy(dlgEditName)
	//*)
}


void dlgEditName::OnedtNameText(wxCommandEvent& event)
{
    m_pbtnSave->Enable(m_pedtName->GetValue()!=wxEmptyString);
}

void dlgEditName::OnedtNameTextEnter(wxCommandEvent& event)
{
    if(m_pedtName->GetValue() != wxEmptyString)
    {
        EndModal(wxID_OK);
    }
}

void dlgEditName::OnbtnSaveClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void dlgEditName::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void dlgEditName::OnlstChannelsSelected(wxCommandEvent& event)
{
    m_nSelectedChannel = event.GetInt();
}
