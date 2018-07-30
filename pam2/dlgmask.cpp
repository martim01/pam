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

dlgMask::dlgMask(wxWindow* parent,const wxString& sSelected, wxWindowID id,const wxPoint& pos,const wxSize& size)
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

	m_plstSubnet->AddButton(wxT("/32 (255.255.255.255)"));
    m_plstSubnet->AddButton(wxT("/31 (255.255.255.254)"));
    m_plstSubnet->AddButton(wxT("/30 (255.255.255.252)"));
    m_plstSubnet->AddButton(wxT("/29 (255.255.255.248)"));
    m_plstSubnet->AddButton(wxT("/28 (255.255.255.240)"));
    m_plstSubnet->AddButton(wxT("/27 (255.255.255.224)"));
    m_plstSubnet->AddButton(wxT("/26 (255.255.255.192)"));
    m_plstSubnet->AddButton(wxT("/25 (255.255.255.128)"));
    m_plstSubnet->AddButton(wxT("/24 (255.255.255.0)"));
    m_plstSubnet->AddButton(wxT("/23 (255.255.254.0)"));
    m_plstSubnet->AddButton(wxT("/22 (255.255.252.0)"));
    m_plstSubnet->AddButton(wxT("/21 (255.255.248.0)"));
    m_plstSubnet->AddButton(wxT("/20 (255.255.240.0)"));
    m_plstSubnet->AddButton(wxT("/19 (255.255.224.0)"));
    m_plstSubnet->AddButton(wxT("/18 (255.255.192.0)"));
    m_plstSubnet->AddButton(wxT("/17 (255.255.128.0)"));
    m_plstSubnet->AddButton(wxT("/16 (255.255.0.0)"));
    m_plstSubnet->AddButton(wxT("/15 (255.254.0.0)"));
    m_plstSubnet->AddButton(wxT("/14 (255.252.0.0)"));
    m_plstSubnet->AddButton(wxT("/13 (255.248.0.0)"));
    m_plstSubnet->AddButton(wxT("/12 (255.240.0.0)"));
    m_plstSubnet->AddButton(wxT("/11 (255.224.0.0)"));
    m_plstSubnet->AddButton(wxT("/10 (255.192.0.0)"));
    m_plstSubnet->AddButton(wxT("/9 (255.128.0.0)"));
    m_plstSubnet->AddButton(wxT("/8 (255.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/7 (254.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/6 (252.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/5 (248.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/4 (240.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/3 (224.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/2 (192.0.0.0)"));
    m_plstSubnet->AddButton(wxT("/1 (128.0.0.0)"));

    m_plstSubnet->SelectButton(sSelected, false);
    m_plstSubnet->ShowButton(m_plstSubnet->FindButton(sSelected), wmList::TOP, false);
    //CaptureMouse();
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
