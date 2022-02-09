#include "pnlDistortion.h"
#include "pnlDistortionChannel.h"
//(*InternalHeaders(pnlDistortion)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "session.h"
#include "distortionbuilder.h"
#include "settingevent.h"
#include "log.h"

//(*IdInit(pnlDistortion)
const long pnlDistortion::ID_M_PLBL37 = wxNewId();
const long pnlDistortion::ID_M_PLBL1 = wxNewId();
const long pnlDistortion::ID_M_PLBL3 = wxNewId();
const long pnlDistortion::ID_M_PLBL4 = wxNewId();
const long pnlDistortion::ID_M_PLBL5 = wxNewId();
const long pnlDistortion::ID_M_PLBL38 = wxNewId();
const long pnlDistortion::ID_M_PLBL40 = wxNewId();
const long pnlDistortion::ID_M_PBTN1 = wxNewId();
const long pnlDistortion::ID_PANEL1 = wxNewId();
const long pnlDistortion::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDistortion,wxPanel)
	//(*EventTable(pnlDistortion)
	//*)
END_EVENT_TABLE()

pnlDistortion::pnlDistortion(wxWindow* parent, DistortionBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlDistortion)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Distortion Measurements"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	pnlLeft = new wxPanel(this, ID_PANEL1, wxPoint(0,50), wxSize(600,430), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlLeft->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(pnlLeft, ID_M_PLBL1, _("Frq"), wxPoint(5,30), wxSize(32,25), wxALIGN_LEFT, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl3 = new wmLabel(pnlLeft, ID_M_PLBL3, _("Amp"), wxPoint(5,60), wxSize(32,25), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl2 = new wmLabel(pnlLeft, ID_M_PLBL4, _("Peaks"), wxPoint(5,90), wxSize(32,25), 0, _T("ID_M_PLBL4"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl4 = new wmLabel(pnlLeft, ID_M_PLBL5, _("THD"), wxPoint(5,120), wxSize(32,25), 0, _T("ID_M_PLBL5"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl15 = new wmLabel(pnlLeft, ID_M_PLBL38, _("Max"), wxPoint(5,150), wxSize(32,25), 0, _T("ID_M_PLBL38"));
	m_pLbl15->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl15->GetUiRect().SetGradient(0);
	m_pLbl15->SetForegroundColour(wxColour(255,255,255));
	m_pLbl15->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl17 = new wmLabel(pnlLeft, ID_M_PLBL40, _("Max\nTime"), wxPoint(5,180), wxSize(32,40), 0, _T("ID_M_PLBL40"));
	m_pLbl17->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl17->GetUiRect().SetGradient(0);
	m_pLbl17->SetForegroundColour(wxColour(255,255,255));
	m_pLbl17->SetBackgroundColour(wxColour(47,94,94));
	m_pbtnReset = new wmButton(pnlLeft, ID_M_PBTN1, _("Reset"), wxPoint(5,370), wxSize(90,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_timerTest.SetOwner(this, ID_TIMER1);
	m_timerTest.Start(500, false);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlDistortion::OnbtnResetClick);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pnlDistortion::OntimerTestTrigger);
	//*)

	m_pBuilder->RegisterForSettingsUpdates("reset", this);
	Bind(wxEVT_SETTING_CHANGED, &pnlDistortion::OnSettingEvent, this);
}

pnlDistortion::~pnlDistortion()
{
	//(*Destroy(pnlDistortion)
	//*)
}


void pnlDistortion::InputSession(const session& aSession)
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        pnlLeft->RemoveChild(m_vChannels[i]);
    }
    m_vChannels.clear();

    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_vChannels.resize(std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
    }

    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i] = new pnlDistortionChannel(pnlLeft, i,aSession.GetCurrentSubsession()->nSampleRate, wxID_ANY, wxPoint(38+(71*i), 0));
    }
}

void pnlDistortion::SetAudioData(const timedbuffer* pBuffer)
{
    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+= m_vChannels.size())
    {
        for(size_t j = 0; j < m_vChannels.size(); j++)
        {
            m_vChannels[j]->AddAudioData(pBuffer->GetBuffer()[i+j]);
        }
    }
}

void pnlDistortion::RunTest()
{

}


void pnlDistortion::ResetTest()
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i]->Reset();
    }
}




void pnlDistortion::OntimerTestTrigger(wxTimerEvent& event)
{
    Json::Value jsMessage;

    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        auto res = m_vChannels[i]->RunTest();
        if(m_pBuilder->WebsocketsActive())
        {
            Json::Value jsChannel;
            jsChannel["distortion"]["current"] = res.dDistortion;
            jsChannel["fundamental"]["frequency"] = res.dFrequency;
            jsChannel["fundamental"]["amplitude"] = res.dAmplitude;
            jsChannel["peaks"] = res.nPeaks;
            jsChannel["distortion"]["max"]["percentage"] = res.dMaxDistortion;
            jsChannel["distortion"]["max"]["when"] = res.sMaxTime.ToStdString();

            jsMessage.append(jsChannel);
        }
        if(m_pBuilder->IsLogActive() && res.nPeaks > 1)
        {
            pmlLog(pml::LOG_INFO) << "Distortion\t" << "Channel: " << i << " Peaks=" << res.nPeaks << " Distortion=" << res.dDistortion;
        }
    }

    if(m_pBuilder->WebsocketsActive())
    {
        m_pBuilder->SendWebsocketMessage(jsMessage);
    }
}

void pnlDistortion::OnbtnResetClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting("reset", 1);
}

void pnlDistortion::OnSettingEvent(SettingEvent& event)
{
    if(event.GetKey() == "reset" && event.GetValue(0l) == 1)
    {
        m_pBuilder->WriteSetting("reset",0);
        ResetTest();
    }
}
