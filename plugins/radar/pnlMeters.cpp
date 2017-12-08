#include "pnlMeters.h"
#include "radarbuilder.h"

//(*InternalHeaders(pnlMeters)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlMeters)
const long pnlMeters::ID_M_PLST19 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMeters,wxPanel)
	//(*EventTable(pnlMeters)
	//*)
END_EVENT_TABLE()

pnlMeters::pnlMeters(wxWindow* parent, RadarBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlMeters)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_plstMode = new wmList(this, ID_M_PLST19, wxPoint(0,0), wxSize(190,90), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstMode->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST19,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlMeters::OnlstMeterTypeSelected);
	//*)
	SetBackgroundColour(*wxBLACK);

	m_plstMode->AddButton(wxT("PPM"));
	m_plstMode->AddButton(wxT("Peak"));
	m_plstMode->AddButton(wxT("Energy"));
	m_plstMode->AddButton(wxT("Loud"));

	m_plstMode->SelectButton(pBuilder->ReadSetting(wxT("MeterMode"), 0), true);

}

pnlMeters::~pnlMeters()
{
	//(*Destroy(pnlMeters)
	//*)
}


void pnlMeters::OnlstMetersSelected(wxCommandEvent& event)
{
}

void pnlMeters::OnlstMeterTypeSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("MeterMode"), event.GetInt());
}
