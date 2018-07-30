#include "pnlSettingsInput.h"

//(*InternalHeaders(pnlSettingsInput)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsInput)
//*)

BEGIN_EVENT_TABLE(pnlSettingsInput,wxPanel)
	//(*EventTable(pnlSettingsInput)
	//*)
END_EVENT_TABLE()

pnlSettingsInput::pnlSettingsInput(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlSettingsInput)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)
}

pnlSettingsInput::~pnlSettingsInput()
{
	//(*Destroy(pnlSettingsInput)
	//*)
}

