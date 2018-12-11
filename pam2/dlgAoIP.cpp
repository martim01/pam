#include "dlgAoIP.h"

//(*InternalHeaders(dlgAoIP)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgAoIP)
const long dlgAoIP::ID_PANEL1 = wxNewId();
const long dlgAoIP::ID_M_PBTN7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgAoIP,wxDialog)
	//(*EventTable(dlgAoIP)
	//*)
END_EVENT_TABLE()

dlgAoIP::dlgAoIP(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgAoIP)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxPoint(0,0));
	SetBackgroundColour(wxColour(0,0,0));
	m_ppnlRTP = new pnlRTP(this, ID_PANEL1, wxPoint(0,0), wxSize(800,440), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_pbtnFinished = new wmButton(this, ID_M_PBTN7, _("Close Manager"), wxPoint(335,443), wxSize(120,35), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnFinished->SetForegroundColour(wxColour(0,0,0));
	m_pbtnFinished->SetBackgroundColour(wxColour(128,255,0));
	m_pbtnFinished->SetColourDisabled(wxColour(wxT("#909090")));

	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAoIP::OnbtnFinishedClick);
	//*)
}

dlgAoIP::~dlgAoIP()
{
	//(*Destroy(dlgAoIP)
	//*)
}


void dlgAoIP::OnbtnFinishedClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}
