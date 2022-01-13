#include "pnlAbout.h"
#include "aboutelement.h"
//(*InternalHeaders(pnlAbout)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "pam2_version.h"
#include "pambase_version.h"
#include "log_version.h"
#include "dnssd_version.h"
#include "sapserver_version.h"
#include "ptpmonkey_version.h"
#ifdef __NMOS__
#include "restgoose_version.h"
#include "nmos_version.h"
#endif

//(*IdInit(pnlAbout)
//*)

BEGIN_EVENT_TABLE(pnlAbout,wxPanel)
	//(*EventTable(pnlAbout)
	//*)
END_EVENT_TABLE()

pnlAbout::pnlAbout(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	Create(parent, id, pos, size, wxTAB_TRAVERSAL, _T("id"));

	m_plstAbout = new wmListAdv(this, wxNewId(), wxPoint(0,0), wxSize(600,440), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
	m_plstAbout->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));
	m_plstAbout->SetBackgroundColour(*wxWHITE);


	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pam", pml::pam2::VERSION_STRING, pml::pam2::GIT_DATE, pml::pam2::GIT_TAG, pml::pam2::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pambase", pml::pambase::VERSION_STRING, pml::pambase::GIT_DATE, pml::pambase::GIT_TAG, pml::pambase::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pml::log", pml::log::VERSION_STRING, pml::log::GIT_DATE, pml::log::GIT_TAG, pml::log::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pml::dnssd", pml::dnssd::VERSION_STRING, pml::dnssd::GIT_DATE, pml::dnssd::GIT_TAG, pml::dnssd::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pml::sapserver", pml::sapserver::VERSION_STRING, pml::sapserver::GIT_DATE, pml::sapserver::GIT_TAG, pml::sapserver::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pml::ptpmonkey", pml::ptpmonkey::VERSION_STRING, pml::ptpmonkey::GIT_DATE, pml::ptpmonkey::GIT_TAG, pml::ptpmonkey::GIT_BRANCH));
	#ifdef __NMOS__
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pml::restgoose", pml::restgoose::VERSION_STRING, pml::restgoose::GIT_DATE, pml::restgoose::GIT_TAG, pml::restgoose::GIT_BRANCH));
	m_plstAbout->AddElement(std::make_shared<AboutElement>(590, "pml::nmos", pml::nmos::VERSION_STRING, pml::nmos::GIT_DATE, pml::nmos::GIT_TAG, pml::nmos::GIT_BRANCH));
	#endif
}

pnlAbout::~pnlAbout()
{
	//(*Destroy(pnlAbout)
	//*)
}

