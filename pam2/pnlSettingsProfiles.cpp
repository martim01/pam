#include "pnlSettingsProfiles.h"
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/app.h>
#include "settings.h"
#include <wx/msgdlg.h>
#include "dlgEditName.h"


//(*InternalHeaders(pnlSettingsProfiles)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsProfiles)
const long pnlSettingsProfiles::ID_M_PLST1 = wxNewId();
const long pnlSettingsProfiles::ID_M_PBTN1 = wxNewId();
const long pnlSettingsProfiles::ID_M_PBTN3 = wxNewId();
const long pnlSettingsProfiles::ID_M_PBTN4 = wxNewId();
const long pnlSettingsProfiles::ID_M_PBTN2 = wxNewId();
//*)



BEGIN_EVENT_TABLE(pnlSettingsProfiles,wxPanel)
	//(*EventTable(pnlSettingsProfiles)
	//*)
END_EVENT_TABLE()

pnlSettingsProfiles::pnlSettingsProfiles(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId) :
    m_fnPam(wxString::Format(wxT("%s/pam2.ini"), Settings::Get().GetConfigDirectory().c_str()))
{
	//(*Initialize(pnlSettingsProfiles)
	Create(parent, id, wxDefaultPosition, wxSize(600,480), wxTAB_TRAVERSAL, _T("id"));
	wxFont thisFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_plstProfiles = new wmList(this, ID_M_PLST1, wxPoint(10,10), wxSize(200,420), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(1,3));
	m_pbtnLoad = new wmButton(this, ID_M_PBTN1, _("Hold To Load Selected"), wxPoint(230,10), wxSize(120,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnLoad->Disable();
	m_pbtnLoad->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnLoad->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnRename = new wmButton(this, ID_M_PBTN3, _("Rename Selected"), wxPoint(230,70), wxSize(120,50), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnRename->Disable();
	m_pbtnRename->SetBackgroundColour(wxColour(0,0,160));
	m_pbtnRename->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnDelete = new wmButton(this, ID_M_PBTN4, _("Delete Selected"), wxPoint(230,130), wxSize(120,50), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnDelete->Disable();
	m_pbtnDelete->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnDelete->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnSave = new wmButton(this, ID_M_PBTN2, _("Save Active Settings"), wxPoint(230,380), wxSize(120,50), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnSave->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnSave->SetColourDisabled(wxColour(wxT("#808080")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsProfiles::OnlstProfilesSelected);
	Connect(ID_M_PBTN1,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsProfiles::OnbtnLoadClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsProfiles::OnbtnRenameClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsProfiles::OnbtnDeleteClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsProfiles::OnbtnSaveClick);
	//*)
    SetBackgroundColour(*wxBLACK);
    PopulateList();
}

pnlSettingsProfiles::~pnlSettingsProfiles()
{
	//(*Destroy(pnlSettingsProfiles)
	//*)

}



void pnlSettingsProfiles::OnlstProfilesSelected(wxCommandEvent& event)
{
    m_fnSelected.Assign(wxString::Format(wxT("%s/%s.ini"), Settings::Get().GetDocumentDirectory().c_str(), event.GetString().c_str()));
    m_pbtnLoad->Enable();
    m_pbtnRename->Enable();
    m_pbtnDelete->Enable();
}

void pnlSettingsProfiles::OnbtnLoadClick(wxCommandEvent& event)
{
    wxCopyFile(m_fnSelected.GetFullPath(), m_fnPam.GetFullPath());

    //and restart the app
    wxTheApp->GetTopWindow()->Close();
}

bool pnlSettingsProfiles::SaveFile(wxFileName fnFrom, wxFileName fnTo)
{
    if(wxCopyFile(fnFrom.GetFullPath(), fnTo.GetFullPath(), false) == false)
    {
        if(wxMessageBox(wxString::Format(wxT("Profile '%s' already exists.\nOverwrite?"), fnTo.GetName().c_str()), wxT("PAM"), wxYES_NO) == wxID_YES)
        {
           return wxCopyFile(fnFrom.GetFullPath(), fnTo.GetFullPath());
        }
        return false;
    }
    return true;
}

void pnlSettingsProfiles::OnbtnSaveClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this, m_fnSelected.GetName());
    aDlg.m_plstChannels->Hide();
    if(aDlg.ShowModal() == wxID_OK)
    {
        wxFileName fnTo(wxString::Format(wxT("%s/%s.ini"),Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str()));
        if(SaveFile(m_fnPam, fnTo))
        {
            PopulateList();
        }
    }
}

void pnlSettingsProfiles::OnbtnRenameClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this, m_fnSelected.GetName());
    aDlg.m_plstChannels->Hide();
    if(aDlg.ShowModal() == wxID_OK)
    {
        wxFileName fnTo(wxString::Format(wxT("%s/%s.ini"),Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str()));

        if(SaveFile(m_fnSelected, fnTo))
        {
            wxRemoveFile(m_fnSelected.GetFullPath());
            PopulateList();
        }
    }
}

void pnlSettingsProfiles::OnbtnDeleteClick(wxCommandEvent& event)
{
    wxRemoveFile(m_fnSelected.GetFullPath());
    PopulateList();
}

void pnlSettingsProfiles::PopulateList()
{
    m_pbtnLoad->Enable(false);
    m_pbtnRename->Enable(false);
    m_pbtnDelete->Enable(false);

    m_plstProfiles->Freeze();
    m_plstProfiles->Clear();

    wxArrayString asFiles;
    wxDir::GetAllFiles(Settings::Get().GetDocumentDirectory(), &asFiles, wxT("*.ini"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        if(fn.GetName() != m_fnPam.GetName())
        {
            m_plstProfiles->AddButton(fn.GetName());
        }
    }

    m_plstProfiles->Thaw();
}
