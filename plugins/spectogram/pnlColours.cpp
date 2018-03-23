#include "pnlColours.h"
#include "spectogrammeter.h"
//(*InternalHeaders(pnlColours)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "spectogrambuilder.h"

//(*IdInit(pnlColours)
const long pnlColours::ID_M_PLST3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlColours,wxPanel)
	//(*EventTable(pnlColours)
	//*)
END_EVENT_TABLE()

pnlColours::pnlColours(wxWindow* parent,SpectogramBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlColours)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstColours = new wmList(this, ID_M_PLST3, wxPoint(0,0), wxSize(190,200), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstColours->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlColours::OnlstColoursSelected);
	//*)

	m_plstColours->AddButton(wxT("Mono"), wxNullBitmap, reinterpret_cast<void*>(SpectogramMeter::MAP_MONO));
	m_plstColours->AddButton(wxT("Blue-Red"), wxNullBitmap, reinterpret_cast<void*>(SpectogramMeter::MAP_HOTCOLD));
	m_plstColours->AddButton(wxT("BCGYR"), wxNullBitmap, reinterpret_cast<void*>(SpectogramMeter::MAP_FIVE));
	m_plstColours->AddButton(wxT("PPM"), wxNullBitmap, reinterpret_cast<void*>(SpectogramMeter::MAP_PPM));
	m_plstColours->AddButton(wxT("BlBCGYRWh"), wxNullBitmap, reinterpret_cast<void*>(SpectogramMeter::MAP_SEVEN));

	m_plstColours->SelectButton(m_plstColours->FindButton(reinterpret_cast<void*>(m_pBuilder->ReadSetting(wxT("HeatMap"), SpectogramMeter::MAP_FIVE))), true);
}
pnlColours::~pnlColours()
{
	//(*Destroy(pnlColours)
	//*)
}


void pnlColours::OnlstColoursSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("HeatMap"), reinterpret_cast<int>(event.GetClientData()));
}
