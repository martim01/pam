#include "dlgmask.h"

//(*InternalHeaders(dlgMask)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgMask)
const long dlgMask::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgMask,wxDialog)
	//(*EventTable(dlgMask)
	//*)
	EVT_MOUSE_CAPTURE_LOST(dlgMask::OnCaptureLost)
END_EVENT_TABLE()

dlgMask::dlgMask(wxWindow* parent, const wxArrayString& asButtons, const wxString& sSelected, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgMask)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	m_plstSubnet = new wmList(this, ID_M_PLST1, wxPoint(0,0), wxSize(100,300), wmList::STYLE_SELECT, 1, wxSize(-1,40), 1, wxSize(2,2));
	m_plstSubnet->SetBackgroundColour(wxColour(0,0,0));
	m_plstSubnet->SetGradient(128);
	m_plstSubnet->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstSubnet->SetPressedButtonColour(wxColour(wxT("#8080FF")));
	m_plstSubnet->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plstSubnet->SetTextButtonColour(wxColour(wxT("#000000")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgMask::OnlstSubnetSelected);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&dlgMask::OnLeftDown);
	//*)


	SetPosition(pos);
	SetSize(200,300);

	for(size_t i = 0; i < asButtons.GetCount(); i++)
    {
        m_plstSubnet->AddButton(asButtons[i]);
    }

    m_plstSubnet->SelectButton(sSelected, false);
    m_plstSubnet->ShowButton(m_plstSubnet->FindButton(sSelected), wmList::TOP, false);
    CaptureMouse();
}

dlgMask::~dlgMask()
{
	//(*Destroy(dlgMask)
	//*)
	if(HasCapture())
    {
        ReleaseMouse();
    }
}


void dlgMask::OnlstSubnetSelected(wxCommandEvent& event)
{
    m_sSelected = event.GetString();
    EndModal(wxID_OK);
}

void dlgMask::OnLeftDown(wxMouseEvent& event)
{
    if(GetClientRect().Contains(event.GetPosition()) == false)
    {
        if(HasCapture())
        {
            ReleaseMouse();
        }
        EndModal(wxID_CANCEL);
    }
    else
    {
        wxPostEvent(m_plstSubnet, event);
    }
    event.Skip();
}


void dlgMask::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    wxPostEvent(m_plstSubnet, event);
    EndModal(wxID_CANCEL);
}
