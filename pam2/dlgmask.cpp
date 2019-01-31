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
	unsigned long nHeight = asButtons.GetCount()*41;


	Create(parent, id, wxEmptyString, pos, wxSize(size.x, std::min((unsigned long)300, nHeight+4)), wxNO_BORDER, _T("id"));
    m_plstSubnet = new wmList(this, ID_M_PLST1, wxPoint(2,2), wxSize(size.x-4,GetSize().y-4), wmList::STYLE_SELECT, 1, wxSize(-1,40), 1, wxSize(2,2));
	m_plstSubnet->SetBackgroundColour(wxColour(0,0,0));
	m_plstSubnet->SetGradient(128);
	m_plstSubnet->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstSubnet->SetPressedButtonColour(wxColour(wxT("#8080FF")));
	m_plstSubnet->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plstSubnet->SetTextButtonColour(wxColour(wxT("#000000")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&dlgMask::OnlstSubnetSelected);
	Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&dlgMask::OnLeftDown);
	//*)
    Connect(wxEVT_ACTIVATE, (wxObjectEventFunction)&dlgMask::OnActivate);


    for(size_t i = 0; i < asButtons.GetCount(); i++)
    {
        m_plstSubnet->AddButton(asButtons[i]);
    }
    m_sSelected = sSelected;

    m_plstSubnet->SelectButton(m_sSelected, false);
    //m_plstSubnet->ShowButton(m_plstSubnet->FindButton(m_sSelected), wmList::MIDDLE, false);
    //m_plstSubnet->Refresh();

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

void dlgMask::OnActivate(wxActivateEvent& event)
{
    CaptureMouse();


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
