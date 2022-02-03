#include "pnlMode.h"
#include "anglemetersbuilder.h"
#include "ppmtypes.h"


//(*InternalHeaders(pnlMode)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlMode)
const long pnlMode::ID_M_PLST14 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMode,wxPanel)
	//(*EventTable(pnlMode)
	//*)
END_EVENT_TABLE()


pnlMode::pnlMode(wxWindow* parent, AngleMetersBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstMeters_Display = new wmList(this, ID_M_PLST14, wxPoint(0,0), wxSize(190,240), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstMeters_Display->SetBackgroundColour(wxColour(0,0,0));
	m_plstMeters_Display->SetTextButtonColour(wxColour(wxT("#FFFFFF")));


	for(map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().GetTypeBegin(); itType != PPMTypeManager::Get().GetTypeEnd(); ++itType)
    {
        m_plstMeters_Display->AddButton(itType->first);
    }
    m_plstMeters_Display->ConnectToSetting(m_pBuilder->GetName(), "Mode", "BBC");

}

pnlMode::~pnlMode()
{
	//(*Destroy(pnlMode)
	//*)
}


