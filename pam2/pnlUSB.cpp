#include "pnlUSB.h"
#include "settings.h"
#include "log.h"
//(*InternalHeaders(pnlUSB)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)


using namespace std;

BEGIN_EVENT_TABLE(pnlUSB,wxPanel)
	//(*EventTable(pnlUSB)
	//*)
END_EVENT_TABLE()

pnlUSB::pnlUSB(wxWindow* parent, wxWindowID id,const wxPoint& pos,const wxSize& size, int nStyle, const wxString& sn)
{
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(*wxWHITE);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	m_pstHostname = new wmLabel(this, wxNewId(), sHostname, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT30"));
	m_pstHostname->SetMinSize(wxSize(800,40));
	m_pstHostname->SetForegroundColour(wxColour(255,255,255));
	m_pstHostname->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pstHostnameFont(24,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pstHostname->SetFont(m_pstHostnameFont);
	BoxSizer1->Add(m_pstHostname, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);

	m_plblUSB = new wmLabel(this, wxNewId(), _("Checking USB Drives For Files"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT30"));
	m_plblUSB->SetMinSize(wxSize(800,40));
	m_plblUSB->SetForegroundColour(*wxBLACK);
	m_plblUSB->SetBackgroundColour(*wxWHITE);
	m_plblUSB->SetTextAlign(wxALIGN_CENTER);
	m_plblUSB->SetFont(wxFont(20,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
	BoxSizer1->Add(m_plblUSB, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);

    wxBoxSizer* BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    m_plstLog = new wmList(this, wxNewId(), wxDefaultPosition, wxSize(380,300), 0, wmList::SCROLL_VERTICAL, wxSize(-1,40), 1, wxSize(-1,-1));
	BoxSizer3->Add(m_plstLog, 0, wxALL|wxEXPAND, 5);
	m_plstFiles = new wmList(this, ID_M_PLST1, wxDefaultPosition, wxSize(380,300), wmList::STYLE_SELECT, wmList::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(-1,-1));
	BoxSizer3->Add(m_plstFiles, 0, wxALL|wxEXPAND, 5);


	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);

	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_pbtnCheck = new wmButton(this, wxNewId(), _("Search Again"), wxDefaultPosition, wxSize(120,40), 0, wxDefaultValidator, _T("ID_BUTTON_UPLOAD"));
	BoxSizer2->Add(m_pbtnCheck, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_pbtnUpload = new wmButton(this, ID_BUTTON_UPLOAD, _("Upload"), wxDefaultPosition, wxSize(120,40), 0, wxDefaultValidator, _T("ID_BUTTON_UPLOAD"));
	BoxSizer2->Add(m_pbtnUpload, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_pbtnCancel = new wmButton(this, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(120,40), 0, wxDefaultValidator, _T("ID_BUTTON_CANCEL"));
	BoxSizer2->Add(m_pbtnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	m_pbtnUpload->SetBackgroundColour(wxColour(0,200,0));
	m_pbtnUpload->SetColourDisabled(wxColour(128,128,128));

	m_pbtnCheck->SetBackgroundColour(wxColour(0,0,200));
	m_pbtnCheck->SetColourDisabled(wxColour(128,128,128));

	m_pbtnUpload->Enable(false);

	Connect(m_pbtnCheck->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlUSB::OnbtnCheckClick);
	Connect(ID_BUTTON_UPLOAD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlUSB::OnbtnUploadClick);
	Connect(ID_BUTTON_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlUSB::OnbtnCancelClick);
    Connect(ID_M_PLST1, wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlUSB::OnFileSelected);

	Connect(wxID_ANY, wxEVT_USB_FOUND, (wxObjectEventFunction)&pnlUSB::OnUsbFound);
	Connect(wxID_ANY, wxEVT_USB_FILE_FOUND, (wxObjectEventFunction)&pnlUSB::OnUsbFileFound);
	Connect(wxID_ANY, wxEVT_USB_FINISHED, (wxObjectEventFunction)&pnlUSB::OnUsbFinished);
	Connect(wxID_ANY, wxEVT_USB_ERROR, (wxObjectEventFunction)&pnlUSB::OnUsbError);

	m_plstLog->SetButtonColour(*wxWHITE);
	m_plstLog->SetTextAlign(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	m_plstLog->SetTextButtonColour(*wxBLACK);


	CheckUSB();

}

pnlUSB::~pnlUSB()
{
	//(*Destroy(pnlUSB)
	//*)
}

void pnlUSB::OnbtnUploadClick(wxCommandEvent& event)
{
    UsbChecker::UnmountDevice();
    EndModal(wxID_OK);
}


void pnlUSB::StartCheck()
{
    if(IsShownOnScreen())
    {
        CheckUSB();
    }
}

void pnlUSB::CheckUSB()
{
    m_pbtnCheck->Enable(false);
    m_pbtnUpload->Enable(false);
    m_plstFiles->Clear();
    m_plstLog->Clear();

    m_plstLog->AddButton("Searching for USB drives...");
    m_plstLog->Refresh();
    m_checker.RunCheck(m_sFilename);

}


void pnlUSB::OnUsbFound(const wxCommandEvent& event)
{

    m_plstLog->AddButton(wxString::Format("Found USB drive '%s'. Searching for files...", event.GetString().c_str()));
    m_plstLog->Refresh();
}


void pnlUSB::OnUsbFileFound(const wxCommandEvent& event)
{
    wxLogDebug(event.GetString());
    Json::Value jsValue(ConvertToJson(event.GetString().ToStdString()));
    if(jsValue["device"].isString() && jsValue["file"].isString())
    {
        m_mFiles.insert(std::make_pair(wxString::FromUTF8(jsValue["file"].asString().c_str()), wxString::FromUTF8(jsValue["device"].asString().c_str())));
        m_plstFiles->AddButton(wxString::FromUTF8(jsValue["file"].asString().c_str()));
        m_plstFiles->Refresh();
    }
    else
    {
        wxLogDebug("bad json");
    }
}


void pnlUSB::OnUsbFinished(const wxCommandEvent& event)
{
    m_plstLog->AddButton("Finished searching.");
    m_plstLog->Refresh();
    m_pbtnCheck->Enable(true);
}


void pnlUSB::OnUsbError(const wxCommandEvent& event)
{
    m_plstLog->AddButton(event.GetString());
    m_plstLog->Refresh();
}

void pnlUSB::OnFileSelected(const wxCommandEvent& event)
{
    m_sSelectedFile = event.GetString();
    auto itDev = m_mFiles.find(m_sSelectedFile);
    if(itDev != m_mFiles.end())
    {
        m_sSelectedDevice = itDev->second;
        m_pbtnUpload->Enable(true);
    }
}

void pnlUSB::OnbtnCheckClick(wxCommandEvent& event)
{
    CheckUSB();
}
