#include "dlgNoInput.h"

//(*InternalHeaders(dlgNoInput)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgNoInput)
const long dlgNoInput::ID_M_PLBL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgNoInput,wxDialog)
	//(*EventTable(dlgNoInput)
	//*)
END_EVENT_TABLE()

dlgNoInput::dlgNoInput(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgNoInput)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(520,50));
	Move(wxPoint(40,400));
	SetForegroundColour(wxColour(255,0,0));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(24,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("No Valid AES Input"), wxPoint(0,0), wxSize(560,50), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	wxFont m_pLbl1Font(28,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	//*)
}

dlgNoInput::~dlgNoInput()
{
	//(*Destroy(dlgNoInput)
	//*)
}

