#include "pnlUSB.h"
#include "settings.h"
#include "log.h"
#include <wx/sizer.h>
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

pnlUSB::pnlUSB(wxWindow* parent, const wxString& sFileType, const wxString& sSelectLabel, bool bMultiSelect, wxWindowID id,const wxPoint& pos,const wxSize& size, int nStyle, const wxString& sn) : m_checker(this), m_sFilename(sFileType)
{
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;

	Create(parent, id, pos, size, wxTAB_TRAVERSAL, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetBackgroundColour(*wxWHITE);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);

	m_plblUSB = new wmLabel(this, wxNewId(), _("Checking USB Drives For Files"));
	m_plblUSB->SetMinSize(wxSize(600,25));
	m_plblUSB->SetForegroundColour(*wxBLACK);
	m_plblUSB->SetBackgroundColour(*wxWHITE);
	m_plblUSB->SetTextAlign(wxALIGN_CENTER);


	BoxSizer1->Add(m_plblUSB, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);

    wxBoxSizer* BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    m_plstLog = new wmList(this, wxNewId(), wxDefaultPosition, wxSize(280,300), 0, wmList::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(-1,-1));
	BoxSizer3->Add(m_plstLog, 0, wxALL|wxEXPAND, 5);

	long nListStyle =wmList::STYLE_SELECT;
	if(bMultiSelect)
    {
        nListStyle |= wmList::STYLE_SELECT_MULTI;
    }
	m_plstFiles = new wmList(this, wxNewId(), wxDefaultPosition, wxSize(280,300), nListStyle, wmList::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(-1,-1));
	BoxSizer3->Add(m_plstFiles, 0, wxALL|wxEXPAND, 5);


	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);

	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_pbtnCheck = new wmButton(this, wxNewId(), _("Search Again"), wxDefaultPosition, wxSize(120,40), 0, wxDefaultValidator, _T("ID_BUTTON_UPLOAD"));
	BoxSizer2->Add(m_pbtnCheck, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_pbtnUpload = new wmButton(this, wxNewId(), sSelectLabel, wxDefaultPosition, wxSize(120,40), 0, wxDefaultValidator, _T("ID_BUTTON_UPLOAD"));
	BoxSizer2->Add(m_pbtnUpload, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

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

    Connect(m_plstFiles->GetId(), wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlUSB::OnFileSelected);

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
    m_mFiles.clear();
    Log("Searching for USB drives...");
    m_checker.RunCheck(m_sFilename);

}


void pnlUSB::OnUsbFound(const wxCommandEvent& event)
{
    Log(wxString::Format("Found USB drive '%s'. Searching for files...", event.GetString().c_str()));
}

void pnlUSB::Log(const wxString& sLog)
{
    m_plstLog->AddButton(sLog);
    m_plstLog->Refresh();
}

void pnlUSB::OnUsbFileFound(const wxCommandEvent& event)
{
    wxString sDevice = event.GetString().Before('=');
    wxArrayString asFiles = wxStringTokenize(event.GetString().After('='), ",");
    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        if(m_mFiles.insert(std::make_pair(asFiles[i], sDevice)).second)
        {
            m_plstFiles->AddButton(asFiles[i]);
            m_plstFiles->Refresh();
        }
    }
}


void pnlUSB::OnUsbFinished(const wxCommandEvent& event)
{
    Log("Finished searching.");
    m_pbtnCheck->Enable(true);
}


void pnlUSB::OnUsbError(const wxCommandEvent& event)
{
    Log(event.GetString());
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

wxString pnlUSB::GetDevice(const wxString& sFile) const
{
    auto itFile = m_mFiles.find(sFile);
    if(itFile != m_mFiles.end())
    {
        return itFile->second;
    }
    return wxEmptyString;
}
