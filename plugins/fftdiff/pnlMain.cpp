#include "pnlMain.h"
#include "fftdiffbuilder.h"
#include "log.h"
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
	//(*Initialize(pnlMain)
	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pMeter = new fftdiffMeter(this,m_pBuilder, ID_METER,wxPoint(0,35),wxSize(800,400));
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
	m_pbtnWindow = new wmButton(this, ID_M_PBTN3, _("Window"), wxPoint(5,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnWindow->SetBackgroundColour(wxColour(12,163,16));
	m_pbtnOverlap = new wmButton(this, ID_M_PBTN4, _("Overlap"), wxPoint(66,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnOverlap->SetBackgroundColour(wxColour(12,163,16));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Range:"), wxPoint(200,2), wxSize(60,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnRange = new wmButton(this, ID_M_PBTN5, _("40dB"), wxPoint(260,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnRange->SetBackgroundColour(wxColour(11,119,138));
	m_pbtnCursor = new wmButton(this, ID_M_PBTN11, _("Cursor"), wxPoint(400,2), wxSize(120,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN11"));
	m_pbtnCursor->SetBackgroundColour(wxColour(11,119,138));
	m_pbtnCursor->SetToggle(true, wxT("Off"), wxT("On"), 50);
	m_pbtnBins = new wmButton(this, ID_M_PBTN10, _("Bins"), wxPoint(127,2), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
	m_pbtnBins->SetBackgroundColour(wxColour(12,163,19));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Routing:"), wxPoint(390,445), wxSize(60,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnChannelA = new wmButton(this, ID_M_PBTN9, _("Left"), wxPoint(450,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnChannelA->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnChannelB = new wmButton(this, ID_M_PBTN8, _("Right"), wxPoint(511,445), wxSize(60,30), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
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
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event){ m_pMeter->ResetDelay(); m_plblDelay->SetLabel("0ms"); }, m_pbtnDelayReset->GetId());

    Bind(wxEVT_SIZE, [this](wxSizeEvent&){m_pMeter->SetSize(GetSize().x, GetSize().y-80);});

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
}

void pnlMain::SetSampleRate(unsigned long nSampleRate)
{
    m_pMeter->SetSampleRate(nSampleRate);
}

void pnlMain::SetChannels(const std::vector<subsession::channelGrouping>& vChannels)
{
    m_pMeter->SetChannels(vChannels);

    if(vChannels.size() == 2)
    {
        m_pbtnChannelA->SetPopup({"Left","Right"},{"0","1"});
        m_pbtnChannelB->SetPopup({"Left","Right"},{"0","1"});

    }
    else
    {
        std::vector<wxString> vLabels(vChannels.size());
        std::vector<wxString> vValues(vChannels.size());

        for(unsigned int i = 0; i < vChannels.size(); i++)
        {
            vLabels[i] = GetChannelLabelLong(vChannels[i]);
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
    m_pMeter->SetDelayMode(1);
}


void pnlMain::SetAudioData(const timedbuffer* pBuffer)
{
    m_pMeter->SetAudioData(pBuffer);
    if(m_nOffset != m_pMeter->GetOffset())
    {
        m_nOffset = m_pMeter->GetOffset();
        m_plblDelay->SetLabel(wxString::Format("%.1fms", static_cast<double>(m_nOffset*1000)/static_cast<double>(m_pMeter->GetSampleRate())));
    }
}
