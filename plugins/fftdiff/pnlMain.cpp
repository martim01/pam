#include "pnlMain.h"
#include "fftdiffbuilder.h"
#include "log.h"
#include "settingevent.h"
//(*InternalHeaders(pnlMain)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlMain)
const long pnlMain::ID_METER = wxNewId();
const long pnlMain::ID_M_PBTN1 = wxNewId();
const long pnlMain::ID_M_PLBL3 = wxNewId();
const long pnlMain::ID_M_PBTN2 = wxNewId();
const long pnlMain::ID_M_PBTN6 = wxNewId();
const long pnlMain::ID_M_PBTN3 = wxNewId();
const long pnlMain::ID_M_PBTN4 = wxNewId();
const long pnlMain::ID_M_PLBL1 = wxNewId();
const long pnlMain::ID_M_PBTN5 = wxNewId();
const long pnlMain::ID_M_PBTN11 = wxNewId();
const long pnlMain::ID_M_PBTN10 = wxNewId();
const long pnlMain::ID_M_PLBL4 = wxNewId();
const long pnlMain::ID_M_PLBL5 = wxNewId();
const long pnlMain::ID_M_PLBL2 = wxNewId();
const long pnlMain::ID_M_PBTN9 = wxNewId();
const long pnlMain::ID_M_PBTN8 = wxNewId();
const long pnlMain::ID_M_PBTN7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMain,wxPanel)
	//(*EventTable(pnlMain)
	//*)
END_EVENT_TABLE()

pnlMain::pnlMain(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pBuilder(pBuilder)
{
    m_nSelectedChannels[0] = 0;
    m_nSelectedChannels[1] = 1;

	//(*Initialize(pnlMain)
	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pMeter = new fftdiffMeter(this,m_pBuilder, ID_METER,wxPoint(0,35),wxSize(800,430));
	m_pbtnDelay = new wmButton(this, ID_M_PBTN1, _("Latency"), wxPoint(5,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnDelay->SetBackgroundColour(wxColour(84,3,218));
	m_plblDelay = new wmLabel(this, ID_M_PLBL3, wxEmptyString, wxPoint(66,445), wxSize(60,30), 0, _T("ID_M_PLBL3"));
	m_plblDelay->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelay->GetUiRect().SetGradient(0);
	m_plblDelay->SetForegroundColour(wxColour(0,0,0));
	m_plblDelay->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnDelayCalculate = new wmButton(this, ID_M_PBTN2, _("Calculate"), wxPoint(128,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnDelayCalculate->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnDelayReset = new wmButton(this, ID_M_PBTN6, _("Reset"), wxPoint(190,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnDelayReset->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnWindow = new wmButton(this, ID_M_PBTN3, _("Window"), wxPoint(55,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnWindow->SetBackgroundColour(wxColour(12,163,16));
	m_pbtnOverlap = new wmButton(this, ID_M_PBTN4, _("Overlap"), wxPoint(116,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnOverlap->SetBackgroundColour(wxColour(12,163,16));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Range:"), wxPoint(300,2), wxSize(60,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnRange = new wmButton(this, ID_M_PBTN5, _("40dB"), wxPoint(360,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnRange->SetBackgroundColour(wxColour(11,119,138));
	m_pbtnCursor = new wmButton(this, ID_M_PBTN11, _("Cursor"), wxPoint(500,2), wxSize(120,30), 0, wxDefaultValidator, _T("ID_M_PBTN11"));
	m_pbtnCursor->SetBackgroundColour(wxColour(11,119,138));
	m_pbtnCursor->SetToggleLook(true, wxT("Off"), wxT("On"), 50);
	m_pbtnBins = new wmButton(this, ID_M_PBTN10, _("Bins"), wxPoint(177,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
	m_pbtnBins->SetBackgroundColour(wxColour(12,163,19));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL4, _("Level Diff:"), wxPoint(280,445), wxSize(60,30), 0, _T("ID_M_PLBL4"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,0));
	m_plblLevel = new wmLabel(this, ID_M_PLBL5, wxEmptyString, wxPoint(340,445), wxSize(60,30), 0, _T("ID_M_PLBL5"));
	m_plblLevel->SetBorderState(uiRect::BORDER_DOWN);
	m_plblLevel->GetUiRect().SetGradient(0);
	m_plblLevel->SetForegroundColour(wxColour(0,0,0));
	m_plblLevel->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Routing:"), wxPoint(440,445), wxSize(60,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnChannelA = new wmButton(this, ID_M_PBTN9, _("Left"), wxPoint(500,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnChannelA->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnChannelB = new wmButton(this, ID_M_PBTN8, _("Right"), wxPoint(561,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnChannelB->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnReset = new wmButton(this, ID_M_PBTN7, _("Reset"), wxPoint(700,445), wxSize(90,30), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnReset->SetBackgroundColour(wxColour(218,84,3));

	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlMain::OnbtnResetClick);
	//*)

	m_pbtnCursor->ConnectToSetting(m_pBuilder->GetSection(), "Cursor", false);

	m_pbtnWindow->SetPopup({"None","Hann","Hamming","Blackman","Kaiser","KaiserBessel"}, {"0","1","2","3","4","5"});
	m_pbtnWindow->ConnectToSetting(m_pBuilder->GetSection(), "Window", "4");


	m_pbtnBins->SetPopup({"46 Hz","23 Hz","16 Hz","12 Hz"}, {"512","1024","1536","2048"});
	m_pbtnBins->ConnectToSetting(m_pBuilder->GetSection(), "Bins", "1024");

    m_pbtnOverlap->SetPopup({"0%","25%","50%","75%"}, {"0","25","50","75"});
	m_pbtnOverlap->ConnectToSetting(m_pBuilder->GetSection(), "Overlap", "75");

	m_pbtnChannelA->SetPopup({"Left","Right"},{"0","1"});
	m_pbtnChannelA->ConnectToSetting(m_pBuilder->GetSection(), "ChannelA", "0");

	m_pbtnChannelB->SetPopup({"Left","Right"},{"0","1"});
	m_pbtnChannelB->ConnectToSetting(m_pBuilder->GetSection(), "ChannelB", "1");

	m_pbtnRange->SetPopup({"10 dB","20 dB","30 dB","40 dB","50 dB","60 dB","70 dB","80 dB"},
                        {"20","40","60","80","100","120","140","160"});
    m_pbtnRange->ConnectToSetting(m_pBuilder->GetSection(), "Range", "60");

    Bind(wxEVT_LEFT_UP, [this](wxMouseEvent&){m_pBuilder->Maximize((GetSize().x <= 600));});
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event){ CalculateDelay(); }, m_pbtnDelayCalculate->GetId());
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event){ m_delayLine.Reset(); m_plblDelay->SetLabel("0ms"); }, m_pbtnDelayReset->GetId());

    Bind(wxEVT_SIZE, [this](wxSizeEvent&){m_pMeter->SetSize(GetSize().x, GetSize().y-80);});

    m_pBuilder->RegisterForSettingsUpdates(this);
    Bind(wxEVT_SETTING_CHANGED, &pnlMain::OnSettingChange, this);

    m_vChannels.push_back(subsession::channelGrouping(0,subsession::enumChannelGrouping::ST, subsession::enumChannel::LEFT));
    m_vChannels.push_back(subsession::channelGrouping(0,subsession::enumChannelGrouping::ST, subsession::enumChannel::RIGHT));
}

pnlMain::~pnlMain()
{
	//(*Destroy(pnlMain)
	//*)
}


void pnlMain::OnbtnResetClick(wxCommandEvent& event)
{
    m_pMeter->ResetAverage();
    m_pMeter->ResetMax();
    m_pMeter->ResetMin();

    m_dAverageA = 0.0;
    m_dAverageB = 0.0;
    m_dTotalFrames = 0.0;
    m_plblLevel->SetLabel("0.00 db");
}

void pnlMain::SetSampleRate(unsigned long nSampleRate)
{
    m_nSampleRate = nSampleRate;
    m_pMeter->SetSampleRate(nSampleRate);
}

void pnlMain::SetChannels(const std::vector<subsession::channelGrouping>& vChannels)
{
    m_vChannels = vChannels;

    if(m_vChannels.size() == 2)
    {
        m_pbtnChannelA->SetPopup({"Left","Right"},{"0","1"});
        m_pbtnChannelB->SetPopup({"Left","Right"},{"0","1"});

    }
    else
    {
        std::vector<wxString> vLabels(m_vChannels.size());
        std::vector<wxString> vValues(m_vChannels.size());

        for(unsigned int i = 0; i < m_vChannels.size(); i++)
        {
            vLabels[i] = GetChannelLabelLong(m_vChannels[i]);
            vValues[i] = wxString::Format("%u",i);
        }
        m_pbtnChannelA->SetPopup(vLabels, vValues);;
        m_pbtnChannelB->SetPopup(vLabels, vValues);;
    }
}


void pnlMain::ResetMax()
{
    m_pMeter->ResetMax();
}

void pnlMain::ResetMin()
{
    m_pMeter->ResetMin();
}

void pnlMain::ResetAverage()
{
    m_pMeter->ResetAverage();
}

void pnlMain::CalculateDelay()
{
    m_delayLine.SetCalculationMode(true);
}

void pnlMain::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_vChannels.size() != 0)
    {
        nonInterlacedVector data;
        data.first.reserve((pBuffer->GetBufferSize()/m_vChannels.size()));
        data.second.reserve((pBuffer->GetBufferSize()/m_vChannels.size()));

        float dPeakA = 0.0;
        float dPeakB = 0.0;
        for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=m_vChannels.size())
        {
            data.first.push_back(pBuffer->GetBuffer()[i+m_nSelectedChannels[0]]);
            data.second.push_back(pBuffer->GetBuffer()[i+m_nSelectedChannels[1]]);

            dPeakA = std::max(dPeakA, abs(pBuffer->GetBuffer()[i+m_nSelectedChannels[0]]));
            dPeakB = std::max(dPeakB, abs(pBuffer->GetBuffer()[i+m_nSelectedChannels[1]]));
        }

        m_dAverageA = m_dAverageA + ((dPeakA-m_dAverageA)/(m_dTotalFrames+1));
        m_dAverageB = m_dAverageB + ((dPeakB-m_dAverageB)/(m_dTotalFrames+1));
        m_dTotalFrames++;

        auto dDiff = -(20*log10(m_dAverageA))+(20*log10(m_dAverageB));
        m_plblLevel->SetLabel(wxString::Format("%.2f dB", dDiff));

        m_nOffset = m_delayLine.ProcessAudio(data);

        m_plblDelay->SetLabel(wxString::Format("%.1fms", static_cast<double>(m_nOffset*1000)/static_cast<double>(m_pMeter->GetSampleRate())));

        m_pMeter->SetAudioData(data);
    }
}

void pnlMain::OnSettingChange(SettingEvent& event)
{
    if(event.GetKey() == "DelayWindow")
    {
        m_delayLine.SetWindowSize(event.GetValue(long(0))*m_nSampleRate/500);
    }
    else if(event.GetKey() == "DelayAccuracy")
    {
        m_delayLine.SetAccuracy(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "ChannelA")
    {
        m_nSelectedChannels[0] = std::min(m_vChannels.size(), static_cast<size_t>(event.GetValue(0l)));
    }
    else if(event.GetKey() == "ChannelB")
    {
        m_nSelectedChannels[1] = std::min(m_vChannels.size(), static_cast<size_t>(event.GetValue(1l)));
    }
}
