#include "pnlAbout.h"
#include "aboutelement.h"
//(*InternalHeaders(pnlAbout)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlAbout)
//*)

BEGIN_EVENT_TABLE(pnlAbout,wxPanel)
	//(*EventTable(pnlAbout)
	//*)
END_EVENT_TABLE()

pnlAbout::pnlAbout(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlAbout)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)

	m_plstAbout = new wmListAdv(this, wxNewId(), wxPoint(0,0), wxSize(600,440), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
	m_plstAbout->SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));
	m_plstAbout->SetBackgroundColour(*wxWHITE);


	m_plstAbout->AddElement(std::make_shared<AboutElement>(dc, GetClientSize().x, "pam", pml::pam2::VERSION_STRING, pml::pam2::GIT_DATE, pml::pam2::GIT_TAG, pml::pam2::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(dc, GetClientSize().x, "pambase", pml::pambase::VERSION_STRING, pml::pambase::GIT_DATE, pml::pambase::GIT_TAG, pml::pambase::GIT_BRANCH));
}

pnlAbout::~pnlAbout()
{
	//(*Destroy(pnlAbout)
	//*)
}

