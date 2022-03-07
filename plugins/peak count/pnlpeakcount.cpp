#include "pnlpeakcount.h"
#include "pnlpeakcountchannel.h"
#include "peakcountbuilder.h"
#include "log.h"

//(*InternalHeaders(pnlPeakCount)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlPeakCount)
const long pnlPeakCount::ID_M_PLBL37 = wxNewId();
const long pnlPeakCount::ID_M_PLBL1 = wxNewId();
const long pnlPeakCount::ID_M_PLBL6 = wxNewId();
const long pnlPeakCount::ID_M_PLBL3 = wxNewId();
const long pnlPeakCount::ID_M_PLBL2 = wxNewId();
const long pnlPeakCount::ID_M_PLBL5 = wxNewId();
const long pnlPeakCount::ID_M_PLBL4 = wxNewId();
const long pnlPeakCount::ID_M_PKBD1 = wxNewId();
const long pnlPeakCount::ID_M_PBTN1 = wxNewId();
const long pnlPeakCount::ID_M_PLBL7 = wxNewId();
const long pnlPeakCount::ID_M_PEDT1 = wxNewId();
const long pnlPeakCount::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlPeakCount,wxPanel)
	//(*EventTable(pnlPeakCount)
	//*)
END_EVENT_TABLE()

pnlPeakCount::pnlPeakCount(wxWindow* parent,PeakCountBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder),
    m_dLimit(-8.0)
{
	//(*Initialize(pnlPeakCount)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Peak Sample Count"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	pnlLeft = new wxPanel(this, ID_PANEL1, wxPoint(0,50), wxSize(600,430), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlLeft->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(pnlLeft, ID_M_PLBL1, _("Smp"), wxPoint(5,60), wxSize(32,25), wxALIGN_LEFT, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl5 = new wmLabel(pnlLeft, ID_M_PLBL6, _("Pks"), wxPoint(5,90), wxSize(32,25), 0, _T("ID_M_PLBL6"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl3 = new wmLabel(pnlLeft, ID_M_PLBL3, _("%"), wxPoint(5,120), wxSize(32,25), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl2 = new wmLabel(pnlLeft, ID_M_PLBL2, _("Count Samples Whose Amplitudes is Greater Than:"), wxDefaultPosition, wxSize(440,25), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(128,64,0));
	m_plblLimit = new wmLabel(pnlLeft, ID_M_PLBL5, _("0"), wxPoint(446,0), wxSize(50,25), 0, _T("ID_M_PLBL5"));
	m_plblLimit->SetBorderState(uiRect::BORDER_NONE);
	m_plblLimit->SetForegroundColour(wxColour(255,255,255));
	m_plblLimit->SetBackgroundColour(wxColour(0,64,0));
	m_pLbl4 = new wmLabel(pnlLeft, ID_M_PLBL4, _("dBFS"), wxPoint(500,0), wxSize(50,25), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(128,64,0));
	m_pKbd1 = new wmKeyboard(pnlLeft, ID_M_PKBD1, wxPoint(5,220), wxSize(240,200), 5, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pKbd1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pKbd1->SetFont(m_pKbd1Font);
	m_pbtnReset = new wmButton(pnlLeft, ID_M_PBTN1, _("Reset"), wxPoint(5,160), wxSize(90,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pLbl6 = new wmLabel(pnlLeft, ID_M_PLBL7, _("Set To"), wxPoint(260,220), wxSize(60,0), 0, _T("ID_M_PLBL7"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(64,0,128));
	m_pedtLimit = new wmEdit(pnlLeft, ID_M_PEDT1, wxEmptyString, wxPoint(325,220), wxSize(100,25), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtLimit->SetValidation(0);
	m_pedtLimit->SetFocus();
	m_pedtLimit->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
	m_pedtLimit->SetFocusedForeground(wxColour(wxT("#000000")));
	m_pedtLimit->SetBorderStyle(2,1);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPeakCount::OnbtnResetClick);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlPeakCount::OnedtLimitTextEnter);
	//*)
    m_pedtLimit->SetFocus();
    m_dLimit = m_pBuilder->ReadSetting("limit", -8.0);
    m_plblLimit->SetLabel(wxString::Format(wxT("%.2f"), m_dLimit));

    m_timerLog.SetOwner(this, wxNewId());
    m_timerLog.Start(1000);
    Connect(m_timerLog.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&pnlPeakCount::OnTimerLog);
}

pnlPeakCount::~pnlPeakCount()
{
	//(*Destroy(pnlPeakCount)
	//*)
}

void pnlPeakCount::InputSession(const session& aSession)
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        pnlLeft->RemoveChild(m_vChannels[i]);
    }
    m_vChannels.clear();

    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_vChannels.resize(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
    }

    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i] = new pnlPeakCountChannel(pnlLeft, i, wxID_ANY, wxPoint(38+(71*i), 30));
    }
}

void pnlPeakCount::SetAudioData(const timedbuffer* pBuffer)
{
    vector<unsigned int> vPeaks(m_vChannels.size(), 0);
    vector<unsigned int> vSamples(m_vChannels.size(), 0);

    for(unsigned int i = 0; i < pBuffer->GetBufferSize(); i+=m_vChannels.size())
    {
        for(size_t j = 0; j < vPeaks.size(); j++)
        {
            double dAmplitude = 20*log10(pBuffer->GetBuffer()[i+j]);
            if(dAmplitude > m_dLimit)
            {
                vSamples[j]++;

                if(m_vChannels[j]->GetLastAmplitude() < m_dLimit)   //crossing in to peak
                {
                    vPeaks[j]++;
                }

            }
            m_vChannels[j]->SetLastAmplitude(dAmplitude);
        }
    }

    for(size_t i = 0; i < vPeaks.size(); i++)
    {
        m_vChannels[i]->AddPeaks(vSamples[i], vPeaks[i], pBuffer->GetBufferSize()/m_vChannels.size());
    }

    if(m_pBuilder->WebsocketsActive())
    {
        Json::Value jsArray(Json::arrayValue);
        for(size_t i = 0; i < vPeaks.size(); i++)
        {
            Json::Value jsValue;
            if(vSamples[i] != 0 || vPeaks[i] != 0)
            {
                jsValue["peaks"] = vPeaks[i];
                jsValue["samples"] = vSamples[i];
            }
            jsArray.append(jsValue);
        }
        m_pBuilder->SendWebsocketMessage(jsArray);
    }
}

void pnlPeakCount::ResetTest()
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i]->ResetTest();
    }
}


void pnlPeakCount::OnbtnResetClick(wxCommandEvent& event)
{
    ResetTest();
}

void pnlPeakCount::OnedtLimitTextEnter(wxCommandEvent& event)
{
    double dLimit;
    m_pedtLimit->GetValue().ToDouble(&dLimit);
    if(dLimit > 0)
    {
        dLimit = -dLimit;
    }
    m_pBuilder->WriteSetting("limit", dLimit);


}

void pnlPeakCount::SetLimit(double dLimit)
{
    m_dLimit = dLimit;
    m_plblLimit->SetLabel(wxString::Format(wxT("%.2f"), m_dLimit));
    m_pedtLimit->SetValue(wxEmptyString);
    ResetTest();
}


void pnlPeakCount::OnTimerLog(wxTimerEvent& event)
{
    if(m_pBuilder->IsLogActive())
    {
        for(size_t i = 0; i < m_vChannels.size(); i++)
        {
            int nTotal(m_vChannels[i]->SamplesSinceLastCheck());
            if(nTotal > 0)
            {
                pmlLog(pml::LOG_WARN) << "PeakCount\tChannel"<<i<< " " << nTotal << "samples over " << m_dLimit << "dB in last second";
            }
        }
    }
}
