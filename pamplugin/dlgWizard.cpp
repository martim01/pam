#include "dlgWizard.h"

//(*InternalHeaders(dlgWizard)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/xml/xml.h>
#include <iostream>
#include <fstream>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>

#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/fs_zip.h>
    #include <wx/intl.h>
    #include <wx/menu.h>
    #include <wx/string.h>
    #include <wx/utils.h>
    #include <wx/xrc/xmlres.h>
    #include "cbproject.h"
    #include "configmanager.h"
    #include "globals.h"
    #include "logmanager.h"
    #include "manager.h"
    #include "projectmanager.h"
#endif

//(*IdInit(dlgWizard)
const long dlgWizard::ID_CHOICE1 = wxNewId();
const long dlgWizard::ID_CHOICE2 = wxNewId();
const long dlgWizard::ID_STATICTEXT1 = wxNewId();
const long dlgWizard::ID_TEXTCTRL1 = wxNewId();
const long dlgWizard::ID_STATICTEXT2 = wxNewId();
const long dlgWizard::ID_TEXTCTRL2 = wxNewId();
const long dlgWizard::ID_STATICTEXT3 = wxNewId();
const long dlgWizard::ID_TEXTCTRL3 = wxNewId();
const long dlgWizard::ID_STATICTEXT4 = wxNewId();
const long dlgWizard::ID_TEXTCTRL4 = wxNewId();
const long dlgWizard::ID_CHECKLISTBOX1 = wxNewId();
const long dlgWizard::ID_DIRPICKERCTRL1 = wxNewId();
const long dlgWizard::ID_BUTTON1 = wxNewId();
const long dlgWizard::ID_BUTTON2 = wxNewId();
//*)

const wxString dlgWizard::STR_TYPE[2] = { wxT("monitor"), wxT("test")};

BEGIN_EVENT_TABLE(dlgWizard,wxDialog)
	//(*EventTable(dlgWizard)
	//*)
END_EVENT_TABLE()


dlgWizard::dlgWizard(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgWizard)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* StaticBoxSizer5;

	Create(parent, id, _("Create New PAM Plugin"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Type"));
	m_pchType = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	m_pchType->SetSelection( m_pchType->Append(_("Monitor Plugin")) );
	m_pchType->Append(_("Test Plugin"));
	StaticBoxSizer3->Add(m_pchType, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Window Type"));
	m_pchWindow = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	m_pchWindow->SetSelection( m_pchWindow->Append(_("meter")) );
	m_pchWindow->Append(_("panel"));
	StaticBoxSizer4->Add(m_pchWindow, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Details"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Plugin Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtName = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(m_ptxtName, 1, wxALL|wxEXPAND, 2);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Author"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtAuthor = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer1->Add(m_ptxtAuthor, 1, wxALL|wxEXPAND, 2);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Email"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtEmail = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer1->Add(m_ptxtEmail, 1, wxALL|wxEXPAND, 2);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Description"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtDescription = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxSize(300,40), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	FlexGridSizer1->Add(m_ptxtDescription, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Extra Core Libraries"));
	BoxSizer1->Add(StaticBoxSizer5, 0, wxALL|wxEXPAND, 2);
	m_pchlbxLibs = new wxCheckListBox(this, ID_CHECKLISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHECKLISTBOX1"));
	m_pchlbxLibs->Append(_("pamfft"));
	m_pchlbxLibs->Append(_("pamlevel"));
	BoxSizer1->Add(m_pchlbxLibs, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Path To PAM Dev"));
	m_pdpcPath = new wxDirPickerCtrl(this, ID_DIRPICKERCTRL1, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_DIRPICKERCTRL1"));
	StaticBoxSizer2->Add(m_pdpcPath, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 2);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_pbtnCreate = new wxButton(this, ID_BUTTON1, _("Create Project"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	m_pbtnCreate->Disable();
	BoxSizer2->Add(m_pbtnCreate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pbtnCancel = new wxButton(this, ID_BUTTON2, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer2->Add(m_pbtnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgWizard::OntxtNameText);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWizard::OnbtnCreateClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgWizard::OnbtnCancelClick);
	//*)


	m_pdpcPath->SetPath(Manager::Get()->GetConfigManager(_T("pam"))->Read(wxT("path"), wxT("C:/developer/matt/pam2")));
	m_ptxtAuthor->SetValue(Manager::Get()->GetConfigManager(_T("pam"))->Read(wxT("author"), wxEmptyString));
	m_ptxtEmail->SetValue(Manager::Get()->GetConfigManager(_T("pam"))->Read(wxT("email"), wxEmptyString));

}

dlgWizard::~dlgWizard()
{
	//(*Destroy(dlgWizard)
	//*)
}


void dlgWizard::OnbtnCreateClick(wxCommandEvent& event)
{
    //Check the path is correct

    if(wxFileName::FileExists(wxString::Format(wxT("%s/templates/%s/project.xml"), m_pdpcPath->GetPath().c_str(), STR_TYPE[m_pchType->GetSelection()].c_str())) == false)
    {
        wxMessageBox(wxT("Can't find the correct files in the given directory:\n"+wxString::Format(wxT("%s/templates/%s/project.xml"), m_pdpcPath->GetPath().c_str(), STR_TYPE[m_pchType->GetSelection()].c_str())));
    }
    else
    {
        Manager::Get()->GetConfigManager(_T("pam"))->Write(wxT("path"), m_pdpcPath->GetPath());
        Manager::Get()->GetConfigManager(_T("pam"))->Write(wxT("author"), m_ptxtAuthor->GetValue());
        Manager::Get()->GetConfigManager(_T("pam"))->Write(wxT("email"), m_ptxtEmail->GetValue());

        wxString sName(m_ptxtName->GetValue());
        sName << wxT(".cbp");
        wxFileName fnProject(wxString::Format(wxT("%s/plugins/%s"), m_pdpcPath->GetPath().c_str(), m_ptxtName->GetValue().c_str()), sName);
        m_sProjectPath = fnProject.GetFullPath();


        wxFileName::Mkdir(wxString::Format(wxT("%s/plugins/%s"), m_pdpcPath->GetPath().c_str(), m_ptxtName->GetValue().c_str()), wxPATH_MKDIR_FULL);




        wxArrayString asFiles;
        wxDir::GetAllFiles(wxString::Format(wxT("%s/templates/%s"), m_pdpcPath->GetPath().c_str(), STR_TYPE[m_pchType->GetSelection()].c_str()), &asFiles, wxEmptyString,  wxDIR_FILES);
        CreateFiles(asFiles);

        //now get the window files
        wxArrayString asFilesWindows;
        wxDir::GetAllFiles(wxString::Format(wxT("%s/templates/%s/%s"), m_pdpcPath->GetPath().c_str(), STR_TYPE[m_pchType->GetSelection()].c_str(), m_pchWindow->GetStringSelection().c_str()), &asFilesWindows, wxEmptyString,  wxDIR_FILES);
        CreateFiles(asFilesWindows);


        if(m_pchWindow->GetSelection() == 1)
        {
            wxFileName::Mkdir(wxString::Format(wxT("%s/plugins/%s/wxsmith"), m_pdpcPath->GetPath().c_str(), m_ptxtName->GetValue().c_str()), wxPATH_MKDIR_FULL);
            wxArrayString asFilesSmith;
            wxDir::GetAllFiles(wxString::Format(wxT("%s/templates/%s/panel/wxsmith"), m_pdpcPath->GetPath().c_str(), STR_TYPE[m_pchType->GetSelection()].c_str()), &asFilesSmith, wxEmptyString,  wxDIR_FILES);
            CreateFiles(asFilesSmith, wxT("wxsmith"));
        }

        EndModal(wxID_OK);
    }
}

void dlgWizard::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void dlgWizard::OntxtNameText(wxCommandEvent& event)
{
    m_pbtnCreate->Enable(m_ptxtName->GetValue().empty() == false);
}


void dlgWizard::CreateFiles(const wxArrayString& asFiles, const wxString& sSub)
{
    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fileFrom(asFiles[i]);


        wxString sPath;
        if(sSub.empty())
        {
            sPath = wxString::Format(wxT("%s/plugins/%s"), m_pdpcPath->GetPath().c_str(), m_ptxtName->GetValue().c_str());
        }
        else
        {
            sPath = wxString::Format(wxT("%s/plugins/%s/%s"), m_pdpcPath->GetPath().c_str(), m_ptxtName->GetValue().c_str(), sSub.c_str());
        }

        wxString sName;
        if(fileFrom.GetName() == wxT("project"))
        {
            sName << m_ptxtName->GetValue() << wxT(".cbp");
        }
        else
        {
            sName << m_ptxtName->GetValue() << fileFrom.GetFullName();


        }



        wxFileName fileTo(sPath, sName);


        std::ifstream ifile;
        std::ofstream ofile;

        ifile.open(fileFrom.GetFullPath().mb_str(), std::ios::in);
        ofile.open(fileTo.GetFullPath().mb_str());

        std::string stdsLine;
        wxString sLine;
        while(!ifile.eof())
        {
            getline(ifile,stdsLine,'\n');
            sLine = wxString::FromAscii(stdsLine.c_str());

            if(sLine.GetChar(0) == wxT('@'))
            {
                if(sLine.Mid(1,5) == m_pchWindow->GetStringSelection())
                {
                    sLine = sLine.Mid(6);
                }
                else
                {
                    continue;
                }
            }
            else if(sLine.GetChar(0) == wxT('!'))
            {
                if((sLine.Mid(1,6) == wxT("pamfft") && m_pchlbxLibs->IsChecked(0)))
                {
                    sLine = sLine.Mid(7);
                }
                else if(sLine.Mid(1,8) == wxT("pamlevel") && m_pchlbxLibs->IsChecked(1))
                {
                    sLine = sLine.Mid(9);
                }
                else
                {
                    continue;
                }
            }

            sLine.Replace(wxT("[CLASS_PREFIX]"), m_ptxtName->GetValue());
            sLine.Replace(wxT("[DESCRIPTION]"), m_ptxtDescription->GetValue());
            sLine.Replace(wxT("[AUTHOR]"), m_ptxtAuthor->GetValue());
            sLine.Replace(wxT("[EMAIL]"), m_ptxtEmail->GetValue());

            ofile << sLine.mb_str() << "\n";

        }
        ifile.close();
        ofile.close();
    }
}
