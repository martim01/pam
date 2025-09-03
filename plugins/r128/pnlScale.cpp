#include "pnlScale.h"
#include "r128builder.h"
#include "r128types.h"
//(*InternalHeaders(pnlScale)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlScale)
const long pnlScale::ID_M_PBTN1 = wxNewId();
const long pnlScale::ID_M_PBTN2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlScale,wxPanel)
	//(*EventTable(pnlScale)
	//*)
END_EVENT_TABLE()

pnlScale::pnlScale(wxWindow* parent,R128Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
     m_pBuilder(pBuilder)
{

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnZero = new wmButton(this, ID_M_PBTN2, _("Zero Point"), wxPoint(0,190), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnZero->SetBackgroundColour(wxColour(0,0,160));

	m_plstMode = new wmList(this, wxNewId(), wxPoint(0,00), wxSize(190,190), wmList::STYLE_SELECT, 0, wxSize(-1,30), 2, wxSize(2,2));
	m_plstMode->SetBackgroundColour(wxColour(0,0,0));

	SetBackgroundColour(*wxBLACK);

	for(const auto& [sName, type] :R128TypeManager::Get().GetTypeMap())
    {
        m_plstMode->AddButton(sName);
    }

    m_plstMode->ConnectToSetting(m_pBuilder->GetSection(), "R128Mode", "R128 +18");


	m_pbtnZero->SetToggle(true, wxT("Rel"), wxT("Abs"), 50.0);
	m_pbtnZero->ConnectToSetting(m_pBuilder->GetSection(), "Zero", false);
}

pnlScale::~pnlScale()
{
	//(*Destroy(pnlScale)
	//*)
}


