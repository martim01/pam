#include "pnlSettingsRemote.h"
#include "settings.h"
#include "settingevent.h"
#include "networkcontrol.h"

//(*InternalHeaders(pnlSettingsRemote)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsRemote)
const long pnlSettingsRemote::ID_M_PBTN22 = wxNewId();
const long pnlSettingsRemote::ID_M_PKBD2 = wxNewId();
const long pnlSettingsRemote::ID_M_PEDT3 = wxNewId();
const long pnlSettingsRemote::ID_M_PBTN6 = wxNewId();
const long pnlSettingsRemote::ID_M_PLBL1 = wxNewId();
const long pnlSettingsRemote::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsRemote,wxPanel)
	//(*EventTable(pnlSettingsRemote)
	//*)
END_EVENT_TABLE()

pnlSettingsRemote::pnlSettingsRemote(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsRemote)
	Create(parent, id, wxDefaultPosition, wxSize(600,440), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnRemote = new wmButton(this, ID_M_PBTN22, _("Remote API"), wxPoint(10,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnRemote->SetBackgroundColour(wxColour(0,128,64));
	m_pbtnRemote->SetToggle(true, wxT("Off"), wxT("On"), 60);
	m_pkbd = new wmKeyboard(this, ID_M_PKBD2, wxPoint(8,160), wxSize(240,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pkbdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pkbd->SetFont(m_pkbdFont);
	m_pedtPort = new wmEdit(this, ID_M_PEDT3, wxEmptyString, wxPoint(260,115), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pedtPort->SetValidation(4);
	m_pedtPort->SetBackgroundColour(wxColour(255,255,255));
	m_pedtPort->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
	m_pedtPort->SetBorderStyle(1,1);
	m_pbtnInterface = new wmButton(this, ID_M_PBTN6, _("Any"), wxPoint(10,115), wxSize(200,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnInterface->SetForegroundColour(wxColour(0,0,0));
	m_pbtnInterface->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnInterface->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("PORT"), wxPoint(210,115), wxSize(50,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnWebsocket = new wmButton(this, ID_M_PBTN1, _("Websockets"), wxPoint(10,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnWebsocket->SetBackgroundColour(wxColour(0,128,64));
	m_pbtnWebsocket->SetToggle(true, wxT("Off"), wxT("On"), 60);
	//*)
	SetSize(size);
	SetPosition(pos);

	m_pbtnRemote->ConnectToSetting("RemoteApi", "Enable", false);
	m_pbtnWebsocket->ConnectToSetting("RemoteApi", "Websockets", false);
	auto setInt = NetworkControl::Get().GetInterfaceNames();
	std::vector<wxString> vEntries(setInt.begin(), setInt.end());
	vEntries.push_back("Any");
    m_pbtnInterface->SetPopup(vEntries);
    m_pbtnInterface->ConnectToSetting("RemoteApi", "_Interface", "Any");
    m_pedtPort->ConnectToSetting("RemoteApi", "Port", "8090", true);

    Settings::Get().AddHandler(this, "RemoteApi");
    Bind(wxEVT_SETTING_CHANGED, &pnlSettingsRemote::OnSettingChange, this);

    m_pbtnWebsocket->SetColourDisabled(wxColour(170,170,170));
    m_pbtnInterface->Enable(!m_pbtnRemote->IsChecked());
    m_pedtPort->Enable(!m_pbtnRemote->IsChecked());
    m_pbtnWebsocket->Enable(m_pbtnRemote->IsChecked());
}

pnlSettingsRemote::~pnlSettingsRemote()
{
	//(*Destroy(pnlSettingsRemote)
	//*)
	Settings::Get().RemoveHandler(this);
}


void pnlSettingsRemote::OnSettingChange(SettingEvent& event)
{
    if(event.GetKey() == "Enable")
    {
        m_pbtnInterface->Enable(!event.GetValue(false));
        m_pedtPort->Enable(!event.GetValue(false));
        m_pbtnWebsocket->Enable(event.GetValue(false));
    }
}
