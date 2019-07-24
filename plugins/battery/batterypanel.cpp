#include "batterypanel.h"
#include "batterybuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "myprocess.h"


//(*InternalHeaders(batteryPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(batteryPanel)
const long batteryPanel::ID_M_PMETER1 = wxNewId();
const long batteryPanel::ID_M_PLBL1 = wxNewId();
const long batteryPanel::ID_M_PLBL4 = wxNewId();
const long batteryPanel::ID_M_PLBL2 = wxNewId();
const long batteryPanel::ID_M_PLBL5 = wxNewId();
const long batteryPanel::ID_M_PLBL3 = wxNewId();
const long batteryPanel::ID_M_PLBL6 = wxNewId();
const long batteryPanel::ID_TIMER_CHECK = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(batteryPanel, wxPanel);

batteryPanel::batteryPanel(wxWindow* parent,batteryBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : wxPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(batteryPanel)
	Create(parent, id, wxDefaultPosition, wxSize(105,361), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pMeter1 = new LevelMeter(this, ID_M_PMETER1, _("Charge"), -100, 0, wxPoint(350,30), wxSize(60,300));
	m_pMeter1->SetLightColours(wxColour(wxT("#00FF00")), -70, wxColour(wxT("#00FF00")));
	std::vector<double> vLevels;
	vLevels.push_back(100);
	vLevels.push_back(90);
	vLevels.push_back(80);
	vLevels.push_back(70);
	vLevels.push_back(60);
	vLevels.push_back(50);
	vLevels.push_back(40);
	vLevels.push_back(30);
	vLevels.push_back(20);
	vLevels.push_back(10);
	vLevels.push_back(0);
	m_pMeter1->SetLevels(vLevels, 0, _("%"), _("battery"), _("-15"), 1);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Battery Status"), wxPoint(20,30), wxSize(100,25), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(44,169,84));
	m_plblStatus = new wmLabel(this, ID_M_PLBL4, _("Charging"), wxPoint(121,30), wxSize(200,25), 0, _T("ID_M_PLBL4"));
	m_plblStatus->SetBorderState(uiRect::BORDER_FLAT);
	m_plblStatus->GetUiRect().SetGradient(0);
	m_plblStatus->SetForegroundColour(wxColour(0,0,0));
	m_plblStatus->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Power Input"), wxPoint(20,60), wxSize(100,25), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(44,169,84));
	m_plblInput = new wmLabel(this, ID_M_PLBL5, _("WEAK"), wxPoint(121,60), wxSize(200,25), 0, _T("ID_M_PLBL5"));
	m_plblInput->SetBorderState(uiRect::BORDER_FLAT);
	m_plblInput->GetUiRect().SetGradient(0);
	m_plblInput->SetForegroundColour(wxColour(0,0,0));
	m_plblInput->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Error"), wxPoint(20,90), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(44,169,84));
	m_plblError = new wmLabel(this, ID_M_PLBL6, _("No Error"), wxPoint(121,90), wxSize(200,25), 0, _T("ID_M_PLBL6"));
	m_plblError->SetBorderState(uiRect::BORDER_FLAT);
	m_plblError->GetUiRect().SetGradient(0);
	m_plblError->SetForegroundColour(wxColour(0,0,0));
	m_plblError->SetBackgroundColour(wxColour(255,255,255));
	m_timerCheck.SetOwner(this, ID_TIMER_CHECK);
	m_timerCheck.Start(10000, false);

	Connect(ID_TIMER_CHECK,wxEVT_TIMER,(wxObjectEventFunction)&batteryPanel::OntimerCheckTrigger);
	//*)
	m_pLbl4 = new wmLabel(this, wxNewId(), _("Charge"), wxPoint(20,90), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_pLbl4->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(44,169,84));
	m_plblCharge = new wmLabel(this, wxNewId(), _("0"), wxPoint(121,90), wxSize(200,25), 0, _T("ID_M_PLBL6"));
	m_plblCharge->SetBorderState(uiRect::BORDER_FLAT);
	m_plblCharge->GetUiRect().SetGradient(0);
	m_plblCharge->SetForegroundColour(wxColour(0,0,0));
	m_plblCharge->SetBackgroundColour(wxColour(255,255,255));

	m_plblRaw = new wmLabel(this, wxNewId(), _(""), wxPoint(20,150), wxSize(301,200), 0, _T("ID_M_PLBL6"));
	m_plblRaw->SetBorderState(uiRect::BORDER_FLAT);
	m_plblRaw->GetUiRect().SetGradient(0);
	m_plblRaw->SetForegroundColour(wxColour(0,0,0));
	m_plblRaw->SetBackgroundColour(wxColour(255,255,255));
    m_plblRaw->Hide();


	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&batteryPanel::OnLeftUp);

	Connect(wxID_ANY, wxEVT_PROCESS_FINISHED, (wxObjectEventFunction)&batteryPanel::OnProcess);
    CheckStatus();
	SetSize(wxSize(800,480));
	SetPosition(pos);
}

batteryPanel::~batteryPanel()
{
	//(*Destroy(batteryPanel)
	//*)
}

void batteryPanel::SetAudioData(const timedbuffer* pBuffer)
{

}

void batteryPanel::InputSession(const session& aSession)
{

}

void batteryPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void batteryPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}



void batteryPanel::OntimerCheckTrigger(wxTimerEvent& event)
{
    if(IsShown())
    {
        CheckStatus();
    }
}

void batteryPanel::CheckStatus()
{
    m_pProcess = new MyProcess(this);
    if(!wxExecute("sudo status.py", wxEXEC_ASYNC, m_pProcess))
    {
        delete m_pProcess;
        m_pProcess = 0;
        m_plblError->SetLabel(wxT("Process Failed!"));
    }
}


void batteryPanel::OnProcess(wxCommandEvent& event)
{
    if(m_pProcess->IsProcessOk())
    {
        m_plblRaw->SetLabel(m_pProcess->GetRaw());
        m_plblError->SetLabel(m_pProcess->GetError());
        m_plblStatus->SetLabel(m_pProcess->GetStatus());
        m_plblInput->SetLabel(m_pProcess->GetPower());
        m_pMeter1->ShowValue(100-m_pProcess->GetCharge());
        m_plblCharge->SetLabel(wxString::Format(wxT("%.0f%%"), m_pProcess->GetCharge()));
    }
    else
    {
        m_plblError->SetLabel(m_pProcess->GetError());
        m_plblStatus->SetLabel(wxEmptyString);
        m_plblInput->SetLabel(wxEmptyString);
        m_pMeter1->ShowValue(-100);
    }
    delete m_pProcess;
        m_pProcess = 0;
}
