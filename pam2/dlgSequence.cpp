#include "dlgSequence.h"
#include "settings.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include "dlgEditName.h"
#include <map>

//(*InternalHeaders(dlgSequence)
#include <wx/intl.h>
#include <wx/string.h>
//*)


using namespace std;


const wxString dlgSequence::STR_CHANNELS[4] = {wxT("-"), wxT("L"), wxT("R"), wxT("L+R")};
const wxString dlgSequence::STR_SHAPE[4] = {wxT("Sin"), wxT("Sqr"), wxT("Saw"), wxT("Tri")};

//(*IdInit(dlgSequence)
const long dlgSequence::ID_STATICBOX1 = wxNewId();
const long dlgSequence::ID_M_PLST1 = wxNewId();
const long dlgSequence::ID_M_PBTN1 = wxNewId();
const long dlgSequence::ID_M_PBTN13 = wxNewId();
const long dlgSequence::ID_M_PBTN2 = wxNewId();
const long dlgSequence::ID_M_PBTN3 = wxNewId();
const long dlgSequence::ID_PANEL1 = wxNewId();
const long dlgSequence::ID_STATICBOX2 = wxNewId();
const long dlgSequence::ID_M_PLST2 = wxNewId();
const long dlgSequence::ID_M_PBTN4 = wxNewId();
const long dlgSequence::ID_M_PBTN14 = wxNewId();
const long dlgSequence::ID_M_PBTN5 = wxNewId();
const long dlgSequence::ID_M_PBTN6 = wxNewId();
const long dlgSequence::ID_PANEL2 = wxNewId();
const long dlgSequence::ID_STATICBOX3 = wxNewId();
const long dlgSequence::ID_M_PLST3 = wxNewId();
const long dlgSequence::ID_M_PBTN8 = wxNewId();
const long dlgSequence::ID_M_PBTN10 = wxNewId();
const long dlgSequence::ID_M_PBTN11 = wxNewId();
const long dlgSequence::ID_M_PLBL1 = wxNewId();
const long dlgSequence::ID_M_PLBL2 = wxNewId();
const long dlgSequence::ID_M_PLBL3 = wxNewId();
const long dlgSequence::ID_M_PLBL4 = wxNewId();
const long dlgSequence::ID_M_PEDT1 = wxNewId();
const long dlgSequence::ID_M_PEDT2 = wxNewId();
const long dlgSequence::ID_M_PEDT3 = wxNewId();
const long dlgSequence::ID_M_PLST4 = wxNewId();
const long dlgSequence::ID_M_PBTN7 = wxNewId();
const long dlgSequence::ID_M_PBTN9 = wxNewId();
const long dlgSequence::ID_M_PKBD1 = wxNewId();
const long dlgSequence::ID_M_PBTN12 = wxNewId();
const long dlgSequence::ID_PANEL3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgSequence,wxDialog)
	//(*EventTable(dlgSequence)
	//*)
END_EVENT_TABLE()

dlgSequence::dlgSequence(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pSequenceNode(0),
m_pFrequencyNode(0),
m_nFileButton(-1),
m_nSequenceButton(-1),
m_nFrequencyButton(-1),
m_nWaveShape(0)
{
	//(*Initialize(dlgSequence)
	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(800,480));
	Move(wxPoint(0,0));
	SetBackgroundColour(wxColour(0,0,0));
	pnlFile = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(190,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlFile->SetBackgroundColour(wxColour(0,0,0));
	StaticBox1 = new wxStaticBox(pnlFile, ID_STATICBOX1, _("Files"), wxPoint(0,0), wxSize(190,480), 0, _T("ID_STATICBOX1"));
	StaticBox1->SetForegroundColour(wxColour(255,255,255));
	m_plstFiles = new wmList(pnlFile, ID_M_PLST1, wxPoint(10,20), wxSize(170,360), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(-1,-1));
	m_plstFiles->SetForegroundColour(wxColour(255,255,255));
	m_plstFiles->SetBackgroundColour(wxColour(255,255,255));
	m_plstFiles->SetButtonColour(wxColour(wxT("#C2CEF5")));
	m_plstFiles->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstFiles->SetSelectedButtonColour(wxColour(wxT("#FF8040")));
	m_plstFiles->SetTextButtonColour(wxColour(wxT("#000000")));
	m_pbtnFileRename = new wmButton(pnlFile, ID_M_PBTN1, _("Rename"), wxPoint(10,385), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnFileRename->Disable();
	m_pbtnFileRename->SetBackgroundColour(wxColour(0,0,160));
	m_pbtnFileRename->SetColourSelected(wxColour(wxT("#0000FF")));
	m_pbtnFileRename->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnFileCopy = new wmButton(pnlFile, ID_M_PBTN13, _("Copy"), wxPoint(65,385), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN13"));
	m_pbtnFileCopy->Disable();
	m_pbtnFileCopy->SetBackgroundColour(wxColour(0,0,160));
	m_pbtnFileCopy->SetColourSelected(wxColour(wxT("#0000FF")));
	m_pbtnFileCopy->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnFileDelete = new wmButton(pnlFile, ID_M_PBTN2, _("Delete"), wxPoint(120,385), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnFileDelete->Disable();
	m_pbtnFileDelete->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnFileDelete->SetColourSelected(wxColour(wxT("#B00000")));
	m_pbtnFileDelete->SetColourDisabled(wxColour(wxT("#909090")));
	m_ptbnFileCreate = new wmButton(pnlFile, ID_M_PBTN3, _("Create"), wxPoint(65,430), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_ptbnFileCreate->SetBackgroundColour(wxColour(0,128,0));
	m_ptbnFileCreate->SetColourSelected(wxColour(wxT("#00C600")));
	pnlSequences = new wxPanel(this, ID_PANEL2, wxPoint(200,0), wxSize(190,480), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	pnlSequences->SetForegroundColour(wxColour(255,255,255));
	pnlSequences->SetBackgroundColour(wxColour(0,0,0));
	StaticBox2 = new wxStaticBox(pnlSequences, ID_STATICBOX2, _("Sequences"), wxPoint(0,0), wxSize(220,480), 0, _T("ID_STATICBOX2"));
	m_plstSequences = new wmList(pnlSequences, ID_M_PLST2, wxPoint(10,20), wxSize(170,360), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(-1,-1));
	m_plstSequences->SetForegroundColour(wxColour(255,255,255));
	m_plstSequences->SetBackgroundColour(wxColour(255,255,255));
	m_plstSequences->SetButtonColour(wxColour(wxT("#ECF099")));
	m_plstSequences->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstSequences->SetSelectedButtonColour(wxColour(wxT("#FF8040")));
	m_plstSequences->SetTextButtonColour(wxColour(wxT("#000000")));
	m_pbtnSequenceRename = new wmButton(pnlSequences, ID_M_PBTN4, _("Edit"), wxPoint(10,385), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnSequenceRename->Disable();
	m_pbtnSequenceRename->SetBackgroundColour(wxColour(0,0,160));
	m_pbtnSequenceRename->SetColourSelected(wxColour(wxT("#0000FF")));
	m_pbtnSequenceRename->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnSequenceCopy = new wmButton(pnlSequences, ID_M_PBTN14, _("Copy"), wxPoint(65,385), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN14"));
	m_pbtnSequenceCopy->Disable();
	m_pbtnSequenceCopy->SetBackgroundColour(wxColour(0,0,160));
	m_pbtnSequenceCopy->SetColourSelected(wxColour(wxT("#0000FF")));
	m_pbtnSequenceCopy->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnSequenceDelete = new wmButton(pnlSequences, ID_M_PBTN5, _("Delete"), wxPoint(120,385), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnSequenceDelete->Disable();
	m_pbtnSequenceDelete->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnSequenceDelete->SetColourSelected(wxColour(wxT("#B00000")));
	m_pbtnSequenceDelete->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnSequenceCreate = new wmButton(pnlSequences, ID_M_PBTN6, _("Create"), wxPoint(65,430), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnSequenceCreate->Disable();
	m_pbtnSequenceCreate->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnSequenceCreate->SetColourSelected(wxColour(wxT("#00C600")));
	pnlFrequencies = new wxPanel(this, ID_PANEL3, wxPoint(400,0), wxSize(400,480), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	pnlFrequencies->SetForegroundColour(wxColour(255,255,255));
	pnlFrequencies->SetBackgroundColour(wxColour(0,0,0));
	StaticBox3 = new wxStaticBox(pnlFrequencies, ID_STATICBOX3, _("Frequencies"), wxPoint(0,0), wxSize(400,480), 0, _T("ID_STATICBOX3"));
	StaticBox3->SetBackgroundColour(wxColour(0,0,0));
	m_plstFrequency = new wmList(pnlFrequencies, ID_M_PLST3, wxPoint(10,20), wxSize(170,405), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(-1,-1));
	m_plstFrequency->SetForegroundColour(wxColour(255,255,255));
	m_plstFrequency->SetBackgroundColour(wxColour(255,255,255));
	m_plstFrequency->SetButtonColour(wxColour(wxT("#B9B5CC")));
	m_plstFrequency->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstFrequency->SetSelectedButtonColour(wxColour(wxT("#FF8040")));
	m_plstFrequency->SetTextButtonColour(wxColour(wxT("#000000")));
	m_pbtnFrequencyDelete = new wmButton(pnlFrequencies, ID_M_PBTN8, _("Delete"), wxPoint(10,430), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnFrequencyDelete->Disable();
	m_pbtnFrequencyDelete->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnFrequencyDelete->SetColourSelected(wxColour(wxT("#B00000")));
	m_pbtnFrequencyDelete->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnFrequencyUp = new wmButton(pnlFrequencies, ID_M_PBTN10, _("Move\nUp"), wxPoint(65,430), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
	m_pbtnFrequencyUp->Disable();
	m_pbtnFrequencyUp->SetBackgroundColour(wxColour(128,128,64));
	m_pbtnFrequencyUp->SetColourSelected(wxColour(wxT("#808000")));
	m_pbtnFrequencyUp->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnFrequencyDown = new wmButton(pnlFrequencies, ID_M_PBTN11, _("Move Down"), wxPoint(120,430), wxSize(50,40), 0, wxDefaultValidator, _T("ID_M_PBTN11"));
	m_pbtnFrequencyDown->Disable();
	m_pbtnFrequencyDown->SetBackgroundColour(wxColour(128,128,64));
	m_pbtnFrequencyDown->SetColourSelected(wxColour(wxT("#808000")));
	m_pbtnFrequencyDown->SetColourDisabled(wxColour(wxT("#909090")));
	m_pLbl1 = new wmLabel(pnlFrequencies, ID_M_PLBL1, _("Frequency"), wxPoint(200,20), wxDefaultSize, 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2 = new wmLabel(pnlFrequencies, ID_M_PLBL2, _("Amplitude"), wxPoint(200,50), wxDefaultSize, 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3 = new wmLabel(pnlFrequencies, ID_M_PLBL3, _("Cycles"), wxPoint(200,80), wxDefaultSize, 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4 = new wmLabel(pnlFrequencies, ID_M_PLBL4, _("Shape"), wxPoint(250,110), wxDefaultSize, 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pedtFrequency = new wmEdit(pnlFrequencies, ID_M_PEDT1, wxEmptyString, wxPoint(260,20), wxSize(80,-1), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtFrequency->SetValidation(4);
	m_pedtFrequency->SetFocusedBackground(wxColour(wxT("#FFFF80")));
	m_pedtFrequency->SetBorderStyle(1,1);
	m_pedtAmplitude = new wmEdit(pnlFrequencies, ID_M_PEDT2, wxEmptyString, wxPoint(260,50), wxSize(80,-1), 0, wxDefaultValidator, _T("ID_M_PEDT2"));
	m_pedtAmplitude->SetValidation(5);
	m_pedtAmplitude->SetFocusedBackground(wxColour(wxT("#FFFF80")));
	m_pedtAmplitude->SetBorderStyle(1,1);
	m_pedtCycles = new wmEdit(pnlFrequencies, ID_M_PEDT3, wxEmptyString, wxPoint(260,80), wxSize(80,-1), 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pedtCycles->SetValidation(4);
	m_pedtCycles->SetFocusedBackground(wxColour(wxT("#FFFF80")));
	m_pedtCycles->SetBorderStyle(1,1);
	m_plstFrequencyShape = new wmList(pnlFrequencies, ID_M_PLST4, wxPoint(200,135), wxSize(180,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(2,-1));
	m_plstFrequencyShape->Disable();
	m_plstFrequencyShape->SetBackgroundColour(wxColour(0,0,0));
	m_plstFrequencyShape->SetSelectedButtonColour(wxColour(wxT("#FF8040")));
	m_plstFrequencyShape->SetDisabledColour(wxColour(wxT("#909090")));
	m_pbtnFrequencyAppend = new wmButton(pnlFrequencies, ID_M_PBTN7, _("Append"), wxPoint(200,190), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnFrequencyAppend->Disable();
	m_pbtnFrequencyAppend->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnFrequencyAppend->SetColourSelected(wxColour(wxT("#00C600")));
	m_pbtnFrequencyUpdate = new wmButton(pnlFrequencies, ID_M_PBTN9, _("Update"), wxPoint(285,190), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnFrequencyUpdate->Disable();
	m_pbtnFrequencyUpdate->SetBackgroundColour(wxColour(0,0,128));
	m_pbtnFrequencyUpdate->SetColourSelected(wxColour(wxT("#0000FF")));
	m_pbtnFrequencyUpdate->SetColourDisabled(wxColour(wxT("#909090")));
	m_pkbd = new wmKeyboard(pnlFrequencies, ID_M_PKBD1, wxPoint(190,235), wxSize(200,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	m_pbtnFinished = new wmButton(pnlFrequencies, ID_M_PBTN12, _("Close Sequence Creator"), wxPoint(190,435), wxSize(200,35), 0, wxDefaultValidator, _T("ID_M_PBTN12"));
	m_pbtnFinished->SetBackgroundColour(wxColour(0,128,255));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgSequence::OnlstFilesSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFileRenameClick);
	Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFileCopyClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OntbnFileDeleteClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OntbnFileCreateClick);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgSequence::OnlstSequencesSelected);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnSequenceRenameClick);
	Connect(ID_M_PBTN14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnSequencyCopyClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnSequenceDeleteClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnSequenceCreateClick);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgSequence::OnlstFrequencySelected);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFrequencyDeleteClick);
	Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFrequencyUpClick);
	Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFrequencyDownClick);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgSequence::OnCheckFrequency);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgSequence::OnedtFrequencyTextEnter);
	Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgSequence::OnCheckFrequency);
	Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgSequence::OnedtAmplitudeTextEnter);
	Connect(ID_M_PEDT3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgSequence::OnCheckFrequency);
	Connect(ID_M_PEDT3,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgSequence::OnedtCyclesTextEnter);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgSequence::OnlstFrequencyShapeSelected);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFrequencyAppendClick);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFrequencyUpdateClick);
	Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgSequence::OnbtnFinishedClick);
	//*)

	if(wxDir::Exists(wxString::Format(wxT("%s/generator"), Settings::Get().GetDocumentDirectory().c_str())) == false)
    {
        ::wxMkdir(wxString::Format(wxT("%s/generator"), Settings::Get().GetDocumentDirectory().c_str()));
    }
    m_pDoc = 0;
    wxArrayString asFiles;
    wxDir::GetAllFiles(wxString::Format(wxT("%s/generator"), Settings::Get().GetDocumentDirectory().c_str()), &asFiles, wxT("*.xml"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        m_plstFiles->AddButton(fn.GetName(), wxNullBitmap, (void*)i);
    }

    for(int i = 0; i < 4; i++)
    {
        m_plstFrequencyShape->AddButton(STR_SHAPE[i]);
    }
    m_plstFrequencyShape->SelectButton(0,true);
}

dlgSequence::~dlgSequence()
{
	//(*Destroy(dlgSequence)
	//*)
	delete m_pDoc;
}


void dlgSequence::OnlstFilesSelected(wxCommandEvent& event)
{
    m_nFileButton = event.GetInt();
    m_pbtnFileRename->Enable();
    m_pbtnFileCopy->Enable();
    m_pbtnFileDelete->Enable();

    m_sFilename = event.GetString();
    LoadFile();
}

void dlgSequence::OnbtnFileRenameClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this, m_sFilename);
    if(aDlg.ShowModal() == wxID_OK)
    {
        if(wxFileExists(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str())) == false)
        {
            wxRenameFile(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), m_sFilename.c_str()), wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str()));

            m_sFilename =  aDlg.m_pedtName->GetValue();
            m_plstFiles->SetButtonText(m_nFileButton, m_sFilename);
            LoadFile();
        }
    }
}
void dlgSequence::OnbtnFileCopyClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this, m_sFilename);
    if(aDlg.ShowModal() == wxID_OK)
    {
        if(wxFileExists(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str())) == false)
        {
            wxCopyFile(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), m_sFilename.c_str()), wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str()));

            m_sFilename = aDlg.m_pedtName->GetValue();
            m_plstFiles->SelectButton(m_plstFiles->AddButton(m_sFilename), true);
        }
    }
}



void dlgSequence::OntbnFileDeleteClick(wxCommandEvent& event)
{
    wxRemoveFile(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), m_sFilename.c_str()));
    if(m_pDoc)
    {
        delete m_pDoc;
        m_pDoc = 0;
    }
    m_plstFiles->DeleteButton(m_nFileButton);
    m_nFileButton = -1;

    ClearSequencePanel();

}

void dlgSequence::OntbnFileCreateClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this, wxEmptyString);
    if(aDlg.ShowModal() == wxID_OK)
    {
        if(wxFileExists(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str())) == false)
        {
            if(m_pDoc)
            {
                delete m_pDoc;
            }
            m_pDoc = new wxXmlDocument();
            m_pDoc->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, wxT("generator")));
            m_pDoc->Save(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), aDlg.m_pedtName->GetValue().c_str()));
            m_sFilename = aDlg.m_pedtName->GetValue();
            m_plstFiles->SelectButton(m_plstFiles->AddButton(m_sFilename), true);
        }
    }
}

void dlgSequence::OnbtnSequenceRenameClick(wxCommandEvent& event)
{
    unsigned long nChannels;
    m_pSequenceNode->GetAttribute(wxT("channels"), wxT("3")).ToULong(&nChannels);

    dlgEditName aDlg(this, m_pSequenceNode->GetAttribute(wxT("name"), wxEmptyString), nChannels);
    if(aDlg.ShowModal() == wxID_OK)
    {
        m_pSequenceNode->DeleteAttribute(wxT("name"));
        m_pSequenceNode->AddAttribute(wxT("name"), aDlg.m_pedtName->GetValue());

        m_pSequenceNode->DeleteAttribute(wxT("channels"));
        m_pSequenceNode->AddAttribute(wxT("channels"), wxString::Format(wxT("%d"), aDlg.m_nChannels));

        m_plstSequences->SetButtonText(m_nSequenceButton, wxString::Format(wxT("%s [%s]"), aDlg.m_pedtName->GetValue().c_str(), STR_CHANNELS[aDlg.m_nChannels].c_str()));
        SaveDoc();
    }
}

void dlgSequence::OnbtnSequencyCopyClick(wxCommandEvent& event)
{

    unsigned long nChannels;
    m_pSequenceNode->GetAttribute(wxT("channels"), wxT("3")).ToULong(&nChannels);

    dlgEditName aDlg(this, m_pSequenceNode->GetAttribute(wxT("name"), wxEmptyString), nChannels);
    if(aDlg.ShowModal() == wxID_OK)
    {
        wxXmlNode* pNode = new wxXmlNode(*m_pSequenceNode);
        pNode->DeleteAttribute(wxT("name"));
        pNode->AddAttribute(wxT("name"), aDlg.m_pedtName->GetValue());

        pNode->DeleteAttribute(wxT("channels"));
        pNode->AddAttribute(wxT("channels"), wxString::Format(wxT("%d"), aDlg.m_nChannels));

        m_pDoc->GetRoot()->AddChild(pNode);
        SaveDoc();

        m_plstSequences->SelectButton(m_plstSequences->AddButton(wxString::Format(wxT("%s [%s]"), pNode->GetAttribute(wxT("name"), wxEmptyString).c_str(), STR_CHANNELS[aDlg.m_nChannels].c_str())));
    }
}


void dlgSequence::OnbtnSequenceDeleteClick(wxCommandEvent& event)
{
    if(m_pSequenceNode)
    {
        m_pSequenceNode->GetParent()->RemoveChild(m_pSequenceNode);
        delete m_pSequenceNode;
        m_pbtnSequenceDelete->Enable(false);
        m_pbtnSequenceRename->Enable(false);
        m_pbtnSequenceCopy->Enable(false);
        ClearFrequencyPanel();

        SaveDoc();

        LoadFile();
    }
}

void dlgSequence::OnbtnSequenceCreateClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this,wxEmptyString, 2);
    if(aDlg.ShowModal() == wxID_OK)
    {
        wxXmlNode* pNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("sequence"));
        pNode->AddAttribute(wxT("name"), aDlg.m_pedtName->GetValue());
        pNode->AddAttribute(wxT("channels"), wxString::Format(wxT("%d"), aDlg.m_nChannels));
        m_pDoc->GetRoot()->AddChild(pNode);
        SaveDoc();

        m_plstSequences->SelectButton(m_plstSequences->AddButton(wxString::Format(wxT("%s [%s]"), pNode->GetAttribute(wxT("name"), wxEmptyString).c_str(), STR_CHANNELS[aDlg.m_nChannels].c_str())));
    }
}

void dlgSequence::OnlstSequencesSelected(wxCommandEvent& event)
{
    m_nSequenceButton = event.GetInt();
    m_pbtnSequenceDelete->Enable();
    m_pbtnSequenceRename->Enable();
    m_pbtnSequenceCopy->Enable();

    wxString sSequence(event.GetString().BeforeFirst(wxT('[')));
    sSequence.Trim();
    for(wxXmlNode* pNode = m_pDoc->GetRoot()->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase(wxT("sequence")) == 0 && pNode->GetAttribute(wxT("name"), wxEmptyString) == sSequence)
        {
            m_pSequenceNode = pNode;
            LoadSequence();
            break;
        }
    }
}

void dlgSequence::OnlstFrequencySelected(wxCommandEvent& event)
{
    m_nFrequencyButton = event.GetInt();
    m_pbtnFrequencyDelete->Enable();
    m_pbtnFrequencyDown->Enable();
    m_pbtnFrequencyUp->Enable();

    m_pFrequencyNode = reinterpret_cast<wxXmlNode*>(event.GetClientData());
    LoadFrequency();
}

void dlgSequence::OnbtnFrequencyDeleteClick(wxCommandEvent& event)
{
    if(m_pFrequencyNode)
    {
        m_pFrequencyNode->GetParent()->RemoveChild(m_pFrequencyNode);
        delete m_pFrequencyNode;
        m_plstFrequency->DeleteButton(m_nFrequencyButton);
        m_nFrequencyButton = -1;
        ClearFrequencyPanel();
        SaveDoc();
    }
}

void dlgSequence::OnlstFrequencyShapeSelected(wxCommandEvent& event)
{
    m_nWaveShape = event.GetInt();
    //NOISE Genera  tor
}

void dlgSequence::OnbtnFrequencyUpClick(wxCommandEvent& event)
{
    if(m_nFrequencyButton != 0)
    {
        map<int, wxXmlNode*> mNode;
        int i = 0;
        for(wxXmlNode* pNode = m_pSequenceNode->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            mNode.insert(make_pair(i, pNode));
            ++i;
        }
        //now remove all the childrem
        while(m_pSequenceNode->GetChildren())
        {
            m_pSequenceNode->RemoveChild(m_pSequenceNode->GetChildren());
        }

        //now rearrange the map...
        map<int, wxXmlNode*>::iterator itPrevNode = mNode.find(m_nFrequencyButton-1);
        if(itPrevNode != mNode.end())
        {
            wxXmlNode* pPrev(itPrevNode->second);
            mNode[m_nFrequencyButton-1] = m_pFrequencyNode;
            mNode[m_nFrequencyButton] = pPrev;

            for(map<int, wxXmlNode*>::iterator itNode = mNode.begin(); itNode != mNode.end(); ++itNode)
            {
                m_pSequenceNode->AddChild(itNode->second);
            }

            SaveDoc();
            LoadSequence();
        }
    }
}

void dlgSequence::OnbtnFrequencyDownClick(wxCommandEvent& event)
{
    if(m_nFrequencyButton != m_plstFrequency->GetItemCount()-1)
    {

        map<int, wxXmlNode*> mNode;
        int i = 0;
        for(wxXmlNode* pNode = m_pSequenceNode->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            mNode.insert(make_pair(i, pNode));
            ++i;
        }
        //now remove all the childrem
        while(m_pSequenceNode->GetChildren())
        {
            m_pSequenceNode->RemoveChild(m_pSequenceNode->GetChildren());
        }

        //now rearrange the map...
        map<int, wxXmlNode*>::iterator itNextNode = mNode.find(m_nFrequencyButton+1);
        if(itNextNode != mNode.end())
        {
            wxXmlNode* pNext(itNextNode->second);
            mNode[m_nFrequencyButton+1] = m_pFrequencyNode;
            mNode[m_nFrequencyButton] = pNext;

            for(map<int, wxXmlNode*>::iterator itNode = mNode.begin(); itNode != mNode.end(); ++itNode)
            {
                m_pSequenceNode->AddChild(itNode->second);
            }

            SaveDoc();
            LoadSequence();
        }
    }
}

void dlgSequence::OnbtnFrequencyUpdateClick(wxCommandEvent& event)
{
    if(m_pFrequencyNode)
    {
        m_pFrequencyNode->DeleteAttribute(wxT("frequency"));
        m_pFrequencyNode->DeleteAttribute(wxT("dBFS"));
        m_pFrequencyNode->DeleteAttribute(wxT("cycles"));
        m_pFrequencyNode->DeleteAttribute(wxT("type"));

        FillInFreqGen(m_pFrequencyNode);

        unsigned long nShape;
        m_pFrequencyNode->GetAttribute(wxT("type"), wxT("0")).ToULong(&nShape);

        m_plstFrequency->SetButtonText(m_nFrequencyButton, wxString::Format(wxT("%sHz %s @ %sdBFS for %s cycles"), m_pFrequencyNode->GetAttribute(wxT("frequency"), wxEmptyString).c_str(), STR_SHAPE[nShape].c_str(), m_pFrequencyNode->GetAttribute(wxT("dBFS"), wxEmptyString).c_str(), m_pFrequencyNode->GetAttribute(wxT("cycles"), wxEmptyString).c_str()));

        SaveDoc();
    }
}


void dlgSequence::OnbtnFrequencyAppendClick(wxCommandEvent& event)
{
    wxXmlNode* pNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("genfreq"));
    FillInFreqGen(pNode);
    m_pSequenceNode->AddChild(pNode);

    unsigned long nShape;
    pNode->GetAttribute(wxT("type"), wxT("0")).ToULong(&nShape);

    size_t nButton = m_plstFrequency->AddButton(wxString::Format(wxT("%sHz %s @ %sdBFS for %s cycles"), pNode->GetAttribute(wxT("frequency"), wxEmptyString).c_str(), STR_SHAPE[nShape].c_str(), pNode->GetAttribute(wxT("dBFS"), wxEmptyString).c_str(), pNode->GetAttribute(wxT("cycles"), wxEmptyString).c_str()), wxNullBitmap, reinterpret_cast<void*>(pNode));
    m_plstFrequency->SelectButton(nButton, true);

    SaveDoc();
}

void dlgSequence::FillInFreqGen(wxXmlNode* pNode)
{
    double dAmpl(-18.0);
    m_pedtAmplitude->GetValue().ToDouble(&dAmpl);
    if(dAmpl > 0)
    {
        dAmpl = -dAmpl;
    }

    pNode->AddAttribute(wxT("frequency"), m_pedtFrequency->GetValue());
    pNode->AddAttribute(wxT("dBFS"), wxString::Format(wxT("%.2f"), dAmpl));
    pNode->AddAttribute(wxT("cycles"), m_pedtCycles->GetValue());
    pNode->AddAttribute(wxT("type"), wxString::Format(wxT("%d"), m_nWaveShape));
}

void dlgSequence::LoadFile()
{

    ClearSequencePanel();
    //ClearFrequencyPanel();

    if(m_pDoc)
    {
        delete m_pDoc;
    }
    m_pDoc = new wxXmlDocument();

    if(m_pDoc->Load(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), m_sFilename.c_str())))
    {
        for(wxXmlNode* pNode = m_pDoc->GetRoot()->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            if(pNode->GetName().CmpNoCase(wxT("sequence")) == 0)
            {
                unsigned long nChannels(3);
                pNode->GetAttribute(wxT("channels"), wxT("3")).ToULong(&nChannels);

                m_plstSequences->AddButton(wxString::Format(wxT("%s [%s]"), pNode->GetAttribute(wxT("name"), wxEmptyString).c_str(), STR_CHANNELS[nChannels].c_str()));
            }
        }
        m_pbtnSequenceCreate->Enable();
    }


}

void dlgSequence::LoadSequence()
{
    ClearFrequencyPanel();

    if(m_pSequenceNode)
    {
        for(wxXmlNode* pNode = m_pSequenceNode->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            if(pNode->GetName().CmpNoCase(wxT("genfreq")) == 0)
            {
                unsigned long nShape;
                pNode->GetAttribute(wxT("type"), wxT("0")).ToULong(&nShape);

                m_plstFrequency->AddButton(wxString::Format(wxT("%sHz %s @ %sdBFS for %s cycles"), pNode->GetAttribute(wxT("frequency"), wxEmptyString).c_str(), STR_SHAPE[nShape].c_str(), pNode->GetAttribute(wxT("dBFS"), wxEmptyString).c_str(), pNode->GetAttribute(wxT("cycles"), wxEmptyString).c_str()), wxNullBitmap, reinterpret_cast<void*>(pNode));
            }
        }

        m_pedtAmplitude->Enable();
        m_pedtCycles->Enable();
        m_pedtFrequency->Enable();
        m_plstFrequencyShape->Enable();
    }



}

void dlgSequence::LoadFrequency()
{
    m_pedtAmplitude->SetValue(m_pFrequencyNode->GetAttribute(wxT("dBFS"), wxEmptyString));
    m_pedtFrequency->SetValue(m_pFrequencyNode->GetAttribute(wxT("frequency"), wxEmptyString));
    m_pedtCycles->SetValue(m_pFrequencyNode->GetAttribute(wxT("cycles"), wxEmptyString));
    unsigned long nShape;
    m_pFrequencyNode->GetAttribute(wxT("type"), wxT("0")).ToULong(&nShape);
    m_plstFrequencyShape->SelectButton(nShape);
    m_pbtnFrequencyUpdate->Enable();
    m_pedtFrequency->SetFocus();
}

void dlgSequence::OnedtFrequencyTextEnter(wxCommandEvent& event)
{
    m_pedtAmplitude->SetFocus();
}

void dlgSequence::OnedtAmplitudeTextEnter(wxCommandEvent& event)
{
    m_pedtCycles->SetFocus();
}

void dlgSequence::OnedtCyclesTextEnter(wxCommandEvent& event)
{
    m_pedtFrequency->SetFocus();
}

void dlgSequence::OnbtnFinishedClick(wxCommandEvent& event)
{
    EndModal(wxOK);
}

void dlgSequence::SaveDoc()
{
    if(m_pDoc)
    {
        m_pDoc->Save(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), m_sFilename.c_str()));
    }
}

void dlgSequence::ClearSequencePanel()
{
    m_plstSequences->Clear();
    m_pbtnSequenceCreate->Disable();
    m_pbtnSequenceDelete->Disable();
    m_pbtnSequenceRename->Disable();
    m_pbtnSequenceCopy->Disable();
    m_pSequenceNode = 0;

    ClearFrequencyPanel();
}

void dlgSequence::ClearFrequencyPanel()
{

    m_plstFrequency->Clear();

    m_pedtFrequency->Disable();
    m_pedtAmplitude->Disable();
    m_pedtCycles->Disable();
    m_pedtAmplitude->SetValue(wxEmptyString);
    m_pedtFrequency->SetValue(wxEmptyString);
    m_pedtCycles->SetValue(wxEmptyString);

    m_pbtnFrequencyDelete->Disable();
    m_pbtnFrequencyUp->Disable();
    m_pbtnFrequencyDown->Disable();
    m_pbtnFrequencyAppend->Disable();
    m_pbtnFrequencyUpdate->Disable();
    m_plstFrequencyShape->Disable();

    m_pFrequencyNode = 0;
}

void dlgSequence::OnCheckFrequency(wxCommandEvent& event)
{
    m_pbtnFrequencyAppend->Enable((m_pedtFrequency->GetValue() != wxEmptyString) && (m_pedtAmplitude->GetValue() != wxEmptyString) && (m_pedtCycles->GetValue() != wxEmptyString));
    m_pbtnFrequencyUpdate->Enable((m_pedtFrequency->GetValue() != wxEmptyString) && (m_pedtAmplitude->GetValue() != wxEmptyString) && (m_pedtCycles->GetValue() != wxEmptyString) && m_pFrequencyNode);
}

