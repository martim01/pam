#include "pnlChannelDelay.h"
#include "offsetcalculator.h"
#include "offsetgraph.h"
#include "session.h"
#include "channeldelaybuilder.h"
#include <wx/log.h>
#include "log.h"
#include "settingevent.h"


//(*InternalHeaders(pnlChannelDelay)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlChannelDelay)
const long pnlChannelDelay::ID_CUSTOM9 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL37 = wxNewId();
const long pnlChannelDelay::ID_M_PKBD1 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL1 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL2 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL3 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL4 = wxNewId();
const long pnlChannelDelay::ID_M_PEDT1 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL5 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL6 = wxNewId();
const long pnlChannelDelay::ID_M_PLST2 = wxNewId();
const long pnlChannelDelay::ID_M_PLBL7 = wxNewId();
const long pnlChannelDelay::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlChannelDelay,wxPanel)
	//(*EventTable(pnlChannelDelay)
	//*)
END_EVENT_TABLE()

pnlChannelDelay::pnlChannelDelay(wxWindow* parent,ChannelDelayBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pBuilder(pBuilder)
{
	//(*Initialize(pnlChannelDelay)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pGraph = new OffsetGraph(this,ID_CUSTOM9,wxPoint(20,50),wxSize(560,150));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Channel Delay Measurements"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_pKbd1 = new wmKeyboard(this, ID_M_PKBD1, wxPoint(10,250), wxSize(240,200), 5, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pKbd1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pKbd1->SetFont(m_pKbd1Font);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Set maximum delay check in milliseconds"), wxPoint(10,220), wxSize(415,25), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(128,64,0));
	wxFont m_pLbl1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Current"), wxPoint(260,250), wxSize(60,0), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_plblCurrent = new wmLabel(this, ID_M_PLBL3, _("Label"), wxPoint(325,250), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_plblCurrent->SetBorderState(uiRect::BORDER_DOWN);
	m_plblCurrent->GetUiRect().SetGradient(0);
	m_plblCurrent->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL4, _("Set To"), wxPoint(260,280), wxSize(60,0), 0, _T("ID_M_PLBL4"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(64,0,128));
	m_pedtMaxDelay = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(325,280), wxSize(100,25), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtMaxDelay->SetValidation(0);
	m_pedtMaxDelay->SetFocus();
	m_pedtMaxDelay->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
	m_pedtMaxDelay->SetFocusedForeground(wxColour(wxT("#000000")));
	m_pedtMaxDelay->SetBorderStyle(2,1);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL5, _("Choose Channels"), wxPoint(260,320), wxSize(340,25), 0, _T("ID_M_PLBL5"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(128,64,0));
	wxFont m_pLbl4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_pLbl5 = new wmLabel(this, ID_M_PLBL6, _("Channel 1"), wxPoint(260,350), wxSize(340,20), 0, _T("ID_M_PLBL6"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(168,0,0));
	m_plstChannel1 = new wmList(this, ID_M_PLST2, wxPoint(260,370), wxSize(340,32), wmList::STYLE_SELECT, 0, wxSize(-1,30), 8, wxSize(-1,-1));
	m_plstChannel1->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl6 = new wmLabel(this, ID_M_PLBL7, _("Channel 2"), wxPoint(260,405), wxSize(340,20), 0, _T("ID_M_PLBL7"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,168,0));
	m_plstChannel2 = new wmList(this, ID_M_PLST1, wxPoint(260,425), wxSize(340,32), wmList::STYLE_SELECT, 0, wxSize(-1,30), 8, wxSize(-1,-1));
	m_plstChannel2->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlChannelDelay::OnedtMaxDelayTextEnter);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlChannelDelay::OnlstChannel1Selected);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlChannelDelay::OnlstChannel2Selected);
	//*)

	m_pedtMaxDelay->ConnectToSetting(m_pBuilder->GetSection(), "MaxDelay", "100", false);
	m_plstChannel1->ConnectToSetting(m_pBuilder->GetSection(), "Channel_1", size_t(0));
	m_plstChannel2->ConnectToSetting(m_pBuilder->GetSection(), "Channel_2", size_t(1));


	m_pCalc = 0;
	m_nChannel[0] = 0;
	m_nChannel[1] = 1;
	m_nTotalSamples = 65536;
	m_nTotalChannels = 0;

	m_pBuilder->RegisterForSettingsUpdates(this);
	Bind(wxEVT_SETTING_CHANGED, &pnlChannelDelay::OnSettingChanged, this);

    Connect(wxID_ANY, wxEVT_OFFSET_DONE, (wxObjectEventFunction)&pnlChannelDelay::OnOffsetDone);
}

pnlChannelDelay::~pnlChannelDelay()
{
	//(*Destroy(pnlChannelDelay)
	//*)
}



void pnlChannelDelay::SetData(const timedbuffer* pBuffer)
{
    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=m_nTotalChannels)
    {
        if(m_vBufferL.size() == m_nTotalSamples)
        {
            CalculateOffset();
            break;
        }
        else
        {
            m_vBufferL.push_back(pBuffer->GetBuffer()[i+m_nChannel[0]]);
            m_vBufferR.push_back(pBuffer->GetBuffer()[i+m_nChannel[1]]);
        }
    }
    m_pGraph->ShowSamplesLeftToGet(m_nTotalSamples-m_vBufferL.size());
}


void pnlChannelDelay::SetTotalSamples(size_t nSamples)
{
    m_vBufferL.clear();
    m_vBufferR.clear();
    m_vBufferL.reserve(nSamples);
    m_vBufferR.reserve(nSamples);

    m_nTotalSamples = nSamples;
    m_pGraph->SetTimeFrame(m_nTotalSamples);

    m_plblCurrent->SetLabel(wxString::Format(wxT("%.0f ms"), static_cast<double>(m_nTotalSamples)/96.0));

}


void pnlChannelDelay::CalculateOffset()
{
    if(m_pCalc == 0)
    {
        m_pGraph->SetCalculating(true);
        m_pCalc = new OffsetCalculator(this, m_vBufferL, m_vBufferR);
        m_pCalc->Create();
        m_pCalc->Run();
    }
}


void pnlChannelDelay::OnOffsetDone(wxCommandEvent& event)
{
    m_pCalc = 0;

    m_pGraph->SetCalculating(false);
    SetTotalSamples(m_nTotalSamples);

    m_pGraph->AddOffset(event.GetInt());

    if(m_pBuilder->IsLogActive())
    {
        pmlLog(pml::LOG_INFO) << "Channel Delay\t" << event.GetInt() << "ms";
    }

    if(m_pBuilder->WebsocketsActive())
    {
        Json::Value jsResult;
        for(int i = 0; i < 2; i++)
        {
            Json::Value jsChannel;
            jsChannel["id"] = m_nChannel[i];
            jsChannel["name"] = m_sChannel[i].ToStdString();
            jsResult["channels"].append(jsChannel);
        }
        jsResult["offset"] = event.GetInt();
        m_pBuilder->SendWebsocketMessage(jsResult);
    }
}


void pnlChannelDelay::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "MaxDelay")
    {
        SetTotalSamples(event.GetValue(0l)*96);
        m_pedtMaxDelay->SetValue("");
    }
    else if(event.GetKey() == "Channel_1")
    {
        m_nChannel[0] = event.GetValue(0l);
        m_sChannel[0] = m_plstChannel1->GetButtonText(m_nChannel[0]);
        m_pGraph->SetChannel1Name(m_sChannel[0]);
    }
    else if(event.GetKey() == "Channel_2")
    {
        m_nChannel[1] = event.GetValue(1l);
        m_sChannel[1] = m_plstChannel2->GetButtonText(m_nChannel[1]);
        m_pGraph->SetChannel1Name(m_sChannel[1]);
    }
}

void pnlChannelDelay::OnedtMaxDelayTextEnter(wxCommandEvent& event)
{

}


void pnlChannelDelay::InputSession(const session& aSession)
{
    subsession sub;
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        sub = (*aSession.GetCurrentSubsession());
        m_pGraph->SetSampleRate(sub.nSampleRate);
        m_nTotalChannels = std::min((unsigned int)8 ,sub.nChannels);
    }
    else
    {
        m_pGraph->SetSampleRate(48000);
        m_nTotalChannels = 0;
    }

    m_plstChannel1->Freeze();
    m_plstChannel1->Clear();

    m_plstChannel2->Freeze();
    m_plstChannel2->Clear();

    for(size_t i = 0; i < sub.vChannels.size(); i++)
    {
        m_plstChannel1->AddButton(GetChannelLabel(sub.vChannels[i]));
        m_plstChannel2->AddButton(GetChannelLabel(sub.vChannels[i]));
    }

    m_plstChannel1->Thaw();
    m_plstChannel2->Thaw();

}

void pnlChannelDelay::OnlstChannel1Selected(wxCommandEvent& event)
{

}

void pnlChannelDelay::OnlstChannel2Selected(wxCommandEvent& event)
{
}


