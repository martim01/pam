#include "dlgWav.h"
#include "dlgEditName.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include "usbchecker.h"
#include "settings.h"
#include <list>
#include "log.h"

//(*InternalHeaders(dlgWav)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgWav)
const long dlgWav::ID_M_PLST1 = wxNewId();
const long dlgWav::ID_M_PBTN5 = wxNewId();
const long dlgWav::ID_M_PBTN6 = wxNewId();
const long dlgWav::ID_M_PBTN7 = wxNewId();
const long dlgWav::ID_M_PBTN8 = wxNewId();
const long dlgWav::ID_M_PBTN1 = wxNewId();
const long dlgWav::ID_M_PBTN2 = wxNewId();
const long dlgWav::ID_M_PBTN3 = wxNewId();
const long dlgWav::ID_M_PBTN4 = wxNewId();
const long dlgWav::ID_M_PBTN9 = wxNewId();
const long dlgWav::ID_PANEL2 = wxNewId();
const long dlgWav::ID_PANEL5 = wxNewId();
const long dlgWav::ID_M_PLBL10 = wxNewId();
const long dlgWav::ID_M_PBTN13 = wxNewId();
const long dlgWav::ID_PANEL1 = wxNewId();
const long dlgWav::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgWav,wxDialog)
	//(*EventTable(dlgWav)
	//*)
END_EVENT_TABLE()

dlgWav::dlgWav(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_usb(this),
m_bTransfer(false)
{
	//(*Initialize(dlgWav)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxDefaultPosition);
	m_pSwp1 = new wmSwitcherPanel(this, ID_M_PSWP1, wxDefaultPosition, wxSize(800,600), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pSwp1->SetPageNameStyle(0);
	Panel2 = new wxPanel(m_pSwp1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_plstFiles = new wmList(Panel2, ID_M_PLST1, wxPoint(5,10), wxSize(670,410), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 2, wxSize(-1,0), 5, wxSize(5,5));
	m_plstFiles->SetBackgroundColour(wxColour(0,0,0));
	m_plstFiles->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstFiles->SetDisabledColour(wxColour(wxT("#808080")));
	m_pbtnHome = new wmButton(Panel2, ID_M_PBTN5, wxEmptyString, wxPoint(70,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnHome->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnPrevious = new wmButton(Panel2, ID_M_PBTN6, wxEmptyString, wxPoint(190,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnPrevious->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNext = new wmButton(Panel2, ID_M_PBTN7, wxEmptyString, wxPoint(310,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnNext->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnEnd = new wmButton(Panel2, ID_M_PBTN8, wxEmptyString, wxPoint(430,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnEnd->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnSave = new wmButton(Panel2, ID_M_PBTN1, _("Save To USB"), wxPoint(690,40), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSave->Disable();
	m_pbtnSave->SetBackgroundColour(wxColour(12,22,116));
	m_pbtnSave->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnRename = new wmButton(Panel2, ID_M_PBTN2, _("Rename"), wxPoint(690,120), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnRename->Disable();
	m_pbtnRename->SetBackgroundColour(wxColour(12,116,22));
	m_pbtnRename->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnDelete = new wmButton(Panel2, ID_M_PBTN3, _("Delete"), wxPoint(690,200), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnDelete->Disable();
	m_pbtnDelete->SetBackgroundColour(wxColour(116,22,12));
	m_pbtnDelete->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnImport = new wmButton(Panel2, ID_M_PBTN4, _("Import From USB"), wxPoint(690,280), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnImport->SetBackgroundColour(wxColour(12,22,116));
	m_pbtnImport->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnClose = new wmButton(Panel2, ID_M_PBTN9, _("Close Manager"), wxPoint(690,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnClose->SetBackgroundColour(wxColour(64,0,64));
	m_pbtnClose->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	Panel1 = new wxPanel(m_pSwp1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_pnlUSB = new pnlUSB(Panel1, "*.wav", "Import", true, ID_PANEL5, wxPoint(0,0), wxSize(600,400), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	m_pnlUSB->SetBackgroundColour(wxColour(0,0,0));
	m_plblImportProgress = new wmLabel(Panel1, ID_M_PLBL10, wxEmptyString, wxPoint(610,10), wxSize(180,80), 0, _T("ID_M_PLBL10"));
	m_plblImportProgress->SetBorderState(uiRect::BORDER_NONE);
	m_plblImportProgress->GetUiRect().SetGradient(0);
	m_plblImportProgress->SetForegroundColour(wxColour(255,255,255));
	m_plblImportProgress->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnImportBack = new wmButton(Panel1, ID_M_PBTN13, _("Back"), wxPoint(690,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN13"));
	m_pbtnImportBack->SetBackgroundColour(wxColour(146,50,252));
	m_pbtnImportBack->SetColourSelected(wxColour(wxT("#F07800")));
	m_pSwp1->AddPage(Panel2, _("Main"), true);
	m_pSwp1->AddPage(Panel1, _("Import"), false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgWav::OnlstFilesSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWav::OnbtnSaveClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWav::OnbtnRenameClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWav::OnbtnDeleteClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWav::OnbtnImportClick);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWav::OnbtnCloseClick);
	//*)

	Bind(wxEVT_USB_FINISHED, &dlgWav::OnUsbFinished, this);
	wxArrayString asFiles;
    wxDir::GetAllFiles(Settings::Get().GetWavDirectory(), &asFiles, wxT("*.wav"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        m_plstFiles->AddButton(fn.GetName());
    }
    m_pnlUSB->SetSection("ImportWav");
    Connect(m_pnlUSB->m_pbtnUpload->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWav::OnbtnImportImportClick);

}

dlgWav::~dlgWav()
{
	//(*Destroy(dlgWav)
	//*)
}


void dlgWav::OnbtnSaveClick(wxCommandEvent& event)
{
    std::list<wxFileName> lstFn;
    wxArrayInt ai = m_plstFiles->GetSelectedButtons();
    for(size_t i = 0; i < ai.GetCount(); i++)
    {
        wxFileName fn;
        fn.Assign(Settings::Get().GetWavDirectory(), m_plstFiles->GetButtonText(ai[i])+".wav");
        lstFn.push_back(fn);
    }
    m_bTransfer = true;
    EnableButtons();
    m_usb.SaveToUSB(lstFn, false);
}

void dlgWav::OnbtnRenameClick(wxCommandEvent& event)
{
    wxArrayInt ai = m_plstFiles->GetSelectedButtons();
    if(ai.GetCount() == 1)
    {
        wxString sName = m_plstFiles->GetButtonText(ai[0]);
        dlgEditName aDlg(this, sName);
        if(aDlg.ShowModal() == wxID_OK)
        {
            wxRename(Settings::Get().GetWavDirectory()+"/"+sName+".wav", Settings::Get().GetWavDirectory()+"/"+aDlg.m_pedtName->GetValue()+".wav");
            m_plstFiles->SetButtonText(ai[0], aDlg.m_pedtName->GetValue());
        }
    }
}

void dlgWav::OnbtnDeleteClick(wxCommandEvent& event)
{
    wxArrayInt ai = m_plstFiles->GetSelectedButtons();
    for(size_t i = 0; i < ai.GetCount(); i++)
    {
        wxFileName fn;
        fn.Assign(Settings::Get().GetWavDirectory(), m_plstFiles->GetButtonText(ai[i])+".wav");
        wxRemoveFile(fn.GetFullPath());
        m_plstFiles->DeleteButton(ai[i]);
    }
}

void dlgWav::OnbtnImportClick(wxCommandEvent& event)
{
    m_pSwp1->ChangeSelection(1);
    m_pnlUSB->StartCheck();
}

void dlgWav::OnlstFilesSelected(wxCommandEvent& event)
{
    EnableButtons();
}

void dlgWav::EnableButtons()
{
    m_plstFiles->Enable(!m_bTransfer);

    m_pbtnDelete->Enable(m_plstFiles->GetSelectionCount()>0 && !m_bTransfer);
    m_pbtnRename->Enable(m_plstFiles->GetSelectionCount() == 1 && !m_bTransfer);
    m_pbtnSave->Enable(m_plstFiles->GetSelectionCount()>0 && !m_bTransfer);
}
void dlgWav::OnUsbFinished(wxCommandEvent& event)
{
    m_bTransfer = false;
    EnableButtons();
}

void dlgWav::OnbtnImportImportClick(wxCommandEvent& event)
{
    m_pbtnImportBack->Enable(false);
    UsbChecker::UnmountDevice();

    m_pnlUSB->Log("Importing wav files.");

    wxString sDevice;
    wxArrayInt ai = m_pnlUSB->m_plstFiles->GetSelectedButtons();
    for(size_t i = 0; i < ai.GetCount(); i++)
    {
        wxString sNextDevice = m_pnlUSB->GetDevice(m_pnlUSB->m_plstFiles->GetButtonText(ai[i]));
        if(sNextDevice.empty() == false)
        {
            if(sNextDevice != sDevice)
            {
                UsbChecker::UnmountDevice();
                if(UsbChecker::MountDevice(sNextDevice) != 0)
                {
                    sDevice = "";
                    m_pnlUSB->Log("Failed to mount "+sNextDevice);
                    m_pnlUSB->Log(strerror(errno));
                }
                else
                {
                    sDevice = sNextDevice;
                }
            }

            if(sDevice.empty() == false)
            {
                ImportWavFile(wxFileName("/mnt/share/"+m_pnlUSB->m_plstFiles->GetButtonText(ai[i])));
            }
        }
    }
    m_pnlUSB->Log("All files imported.");
    m_pbtnImportBack->Enable(true);
}

void dlgWav::ImportWavFile(const wxFileName& fnWav)
{
    m_pnlUSB->Log(wxString::Format("Importing '%s'", fnWav.GetName().c_str()));

    if(wxCopyFile(fnWav.GetFullPath(), Settings::Get().GetWavDirectory()+"/"+fnWav.GetFullName()) == true)
    {
        pmlLog() << "Imported wav file " << fnWav.GetName();
        m_plstFiles->AddButton(fnWav.GetName());
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "Importing wav file: Reading '" << fnWav.GetName() << "' invalid file";
        m_pnlUSB->Log(wxString::Format("Reading '%s' invalid file", fnWav.GetName().c_str()));
    }
}

void dlgWav::OnbtnCloseClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}
