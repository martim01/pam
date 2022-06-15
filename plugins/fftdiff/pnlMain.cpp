#include "pnlMain.h"
#include "fftdiffbuilder.h"
//(*InternalHeaders(pnlMain)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlMain)
const long pnlMain::ID_METER = wxNewId();
const long pnlMain::ID_M_PBTN1 = wxNewId();
const long pnlMain::ID_M_PBTN3 = wxNewId();
const long pnlMain::ID_M_PBTN4 = wxNewId();
const long pnlMain::ID_M_PBTN5 = wxNewId();
const long pnlMain::ID_M_PBTN7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMain,wxPanel)
	//(*EventTable(pnlMain)
	//*)
END_EVENT_TABLE()

pnlMain::pnlMain(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pBuilder(pBuilder)
{
	//(*Initialize(pnlMain)
	Create(parent, id, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pMeter = new fftdiffMeter(this,m_pBuilder, ID_METER,wxPoint(0,0),wxSize(800,440));
	m_pbtnDelay = new wmButton(this, ID_M_PBTN1, _("Delay"), wxPoint(5,440), wxSize(80,30), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnDelay->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnWindow = new wmButton(this, ID_M_PBTN3, _("Window"), wxPoint(175,440), wxSize(80,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnWindow->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnOverlap = new wmButton(this, ID_M_PBTN4, _("Overlap"), wxPoint(260,440), wxSize(80,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnOverlap->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnBins = new wmButton(this, ID_M_PBTN5, _("Bins"), wxPoint(345,440), wxSize(80,30), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnBins->SetBackgroundColour(wxColour(84,3,218));
	m_pbtnReset = new wmButton(this, ID_M_PBTN7, _("Reset"), wxPoint(515,440), wxSize(80,30), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnReset->SetBackgroundColour(wxColour(84,3,218));

	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlMain::OnbtnResetClick);
	//*)

	m_pbtnWindow->SetPopup({"None","Hann","Hamming","Blackman","Kaiser","KaiserBessel"}, {"0","1","2","3","4","5"});
	m_pbtnWindow->ConnectToSetting(m_pBuilder->GetSection(), "Window", "4");


	m_pbtnBins->SetPopup({"46 Hz","23 Hz","16 Hz","12 Hz"}, {"512","1024","1536","2048"});
	m_pbtnBins->ConnectToSetting(m_pBuilder->GetSection(), "Bins", "1024");

    m_pbtnOverlap->SetPopup({"0%","25%","50%","75%"}, {"0","25","50","75"});
	m_pbtnOverlap->ConnectToSetting(m_pBuilder->GetSection(), "Overlap", "75");

	m_pbtnDelay->SetPopup({"Calculate", "Reset"});

    Bind(wxEVT_LEFT_UP, [this](wxMouseEvent&){m_pBuilder->Maximize((GetSize().x <= 600));});
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event){
         if(event.GetString() == "Calculate")
         {
             CalculateDelay();
         }
         else
         {
             m_pMeter->ResetDelay();
             m_pbtnDelay->SetLabel("?ms");
        }}, m_pbtnDelay->GetId());

    Bind(wxEVT_SIZE, [this](wxSizeEvent&){m_pMeter->SetSize(GetSize().x, GetSize().y-40);});
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
        m_pbtnDelay->SetLabel(wxString::Format("%.1fms", static_cast<double>(m_nOffset*1000)/static_cast<double>(m_pMeter->GetSampleRate())));
    }
}
