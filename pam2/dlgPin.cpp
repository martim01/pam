#include "dlgPin.h"
#include "settings.h"

//(*InternalHeaders(dlgPin)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgPin)
const long dlgPin::ID_M_PLBL1 = wxNewId();
const long dlgPin::ID_M_PEDT1 = wxNewId();
const long dlgPin::ID_M_PKBD1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgPin,wxDialog)
	//(*EventTable(dlgPin)
	//*)
END_EVENT_TABLE()

dlgPin::dlgPin(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgPin)
	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(800,480));
	Move(wxPoint(0,0));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Enter PIN to access Settings"), wxPoint(0,60), wxSize(780,50), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(0,255,0));
	wxFont m_pLbl1Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pedtPin = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(300,150), wxSize(200,50), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtPin->SetMaxLength(8);
	m_pedtPin->SetValidation(4);
	m_pedtPin->SetBorderStyle(1,1);
	m_pkbd = new wmKeyboard(this, ID_M_PKBD1, wxPoint(200,232), wxSize(440,240), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));

	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&dlgPin::OnedtPinTextEnter);
	//*)
	m_pedtPin->SetFocus();
}

dlgPin::~dlgPin()
{
	//(*Destroy(dlgPin)
	//*)
}


void dlgPin::OnedtPinTextEnter(wxCommandEvent& event)
{
    if(m_pedtPin->GetValue() == Settings::Get().Read(wxT("General"), wxT("Pin_Value"), wxEmptyString))
    {
        EndModal(wxID_OK);
    }
    else
    {
        EndModal(wxID_CANCEL);
    }
}
