#include "pnlHelp.h"
#include <wx/textfile.h>
#include <wx/dir.h>
#include "settings.h"

//(*InternalHeaders(pnlHelp)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlHelp)
const long pnlHelp::ID_M_PLST1 = wxNewId();
const long pnlHelp::ID_M_PBTN1 = wxNewId();
const long pnlHelp::ID_PANEL1 = wxNewId();
const long pnlHelp::ID_HTMLWINDOW1 = wxNewId();
//*)

DEFINE_EVENT_TYPE(wxEVT_HELP_CLOSE)

BEGIN_EVENT_TABLE(pnlHelp,wxPanel)
	//(*EventTable(pnlHelp)
	//*)
END_EVENT_TABLE()

pnlHelp::pnlHelp(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlHelp)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_plstHelp = new wmList(this, ID_M_PLST1, wxPoint(0,0), wxSize(100,436), wmList::STYLE_SELECT, 1, wxSize(-1,30), 1, wxSize(1,2));
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,436), wxSize(100,44), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnClose = new wmButton(Panel1, ID_M_PBTN1, _("Close"), wxPoint(2,2), wxSize(96,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClose->SetBackgroundColour(wxColour(0,128,0));
	m_pHtml = new wxTouchScreenHtml(this, ID_HTMLWINDOW1, wxPoint(101,0), wxSize(720,480), wxHW_SCROLLBAR_AUTO, _T("ID_HTMLWINDOW1"));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlHelp::OnlstHelpSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlHelp::OnbtnCloseClick);
	//*)
	m_plstHelp->AddButton(wxT("Pam"));

    wxArrayString asFiles;
    wxDir::GetAllFiles(wxString::Format(wxT("%s/help"), Settings::Get().GetDocumentDirectory().c_str()), &asFiles, wxT("*.html"));
    asFiles.Sort();

    for(int i = 0; i < asFiles.GetCount(); i++)
    {
        asFiles[i].Replace(wxT("\\"), wxT("/"));
        asFiles[i] = asFiles[i].AfterLast(wxT('/')).BeforeLast(wxT('.'));

        if(asFiles[i].CmpNoCase(wxT("pam"))!=0)
        {
            asFiles[i].Replace(wxT("\\"), wxT("/"));
            m_plstHelp->AddButton(asFiles[i]);
        }
    }
}

pnlHelp::~pnlHelp()
{
	//(*Destroy(pnlHelp)
	//*)
}

void pnlHelp::SelectHelp(const wxString& sHelp)
{
    m_sHelp = sHelp;
    m_plstHelp->SelectButton(sHelp, true);
}

void pnlHelp::OnlstHelpSelected(wxCommandEvent& event)
{
    m_pHtml->LoadFile(wxString::Format(wxT("%s/help/%s.html"), Settings::Get().GetDocumentDirectory().c_str(), event.GetString().c_str()));
}

void pnlHelp::OnbtnCloseClick(wxCommandEvent& event)
{
    wxCommandEvent eventClose(wxEVT_HELP_CLOSE);
    eventClose.SetString(m_sHelp);
    wxPostEvent(GetEventHandler(), eventClose);
}
