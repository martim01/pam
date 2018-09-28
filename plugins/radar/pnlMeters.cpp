#include "pnlMeters.h"
#include "radarbuilder.h"
#include "ppmtypes.h"
using namespace std;

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
	m_plstMode = new wmList(this, ID_M_PLST19, wxPoint(0,0), wxSize(190,240), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstMode->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST19,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlMeters::OnlstMeterTypeSelected);
	//*)
	SetBackgroundColour(*wxBLACK);

	for(map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().GetTypeBegin(); itType != PPMTypeManager::Get().GetTypeEnd(); ++itType)
    {
        m_plstMode->AddButton(itType->first);
    }

	m_plstMode->SelectButton(m_pBuilder->ReadSetting(wxT("MeterMode"),wxT("BBC")), true);

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
    m_pBuilder->WriteSetting(wxT("MeterMode"), event.GetString());
}
