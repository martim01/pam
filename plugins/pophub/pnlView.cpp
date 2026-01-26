#include "pnlView.h"
#include "pophubbuilder.h"

#include <wx/intl.h>
#include <wx/string.h>


BEGIN_EVENT_TABLE(pnlView,wxPanel)
END_EVENT_TABLE()

pnlView::pnlView(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	
	/*
		Radio 1/2
		Radio 1/2 Loudness
		News
		News+Loudness
		News+Gain reduction
		Workshop
		TV
	*/

	m_plstView = new wmList(this, wxID_ANY, wxPoint(2,0), wxSize(196,190), wmList::STYLE_SELECT, 0, wxSize(-1,40), 2, wxSize(5,5));
	m_plstView->SetBackgroundColour(wxColour(0,0,0));
	m_plstView->AddButton("Radio");
	m_plstView->AddButton("Radio + Loudness");
	m_plstView->AddButton("News");
	m_plstView->AddButton("News + Loudness");
	m_plstView->AddButton("News + Gain Reduction");
	m_plstView->AddButton("Workshop");
	m_plstView->AddButton("TV");

	m_plstView->ConnectToSetting(pBuilder->GetSection(), "view", size_t(0));
}

pnlView::~pnlView()
{
	//(*Destroy(pnlView)
	//*)
}


