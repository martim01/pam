#include "pnlAngleMeters.h"
#include "anglemeter.h"
#include "anglemetersbuilder.h"
#include "wmbutton.h"
#include "session.h"
#include "timedbuffer.h"
#include "levelcalculator.h"
#include "ppmtypes.h"

//(*InternalHeaders(pnlAngleMeters)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;


//(*IdInit(pnlAngleMeters)
//*)

BEGIN_EVENT_TABLE(pnlAngleMeters,wxPanel)
	//(*EventTable(pnlAngleMeters)
	//*)
END_EVENT_TABLE()

pnlAngleMeters::pnlAngleMeters(wxWindow* parent,AngleMetersBuilder* pBuilder,wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder),
    m_nInputChannels(2),
    m_bStereo(true),
    m_pCalculator(0)
{
	//(*Initialize(pnlAngleMeters)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlAngleMeters::OnLeftUp);
	//*)

    SetBackgroundColour(*wxBLACK);
    Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&pnlAngleMeters::OnMonitorClicked);
    m_pCalculator = new LevelCalculator(-70.0);
    CreateMeters();

}

pnlAngleMeters::~pnlAngleMeters()
{
	//(*Destroy(pnlAngleMeters)
	//*)
	delete m_pCalculator;

}

void pnlAngleMeters::SetSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nInputChannels = min((unsigned int)256, aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nInputChannels = 2;
    }

    m_pCalculator->InputSession(aSession);
    CreateMeters();
}

void pnlAngleMeters::UpdateMeterStereo()
{
    CreateMeters();
}
void pnlAngleMeters::CreateMeters()
{
    m_bStereo = (m_pBuilder->ReadSetting(wxT("Stereo"),0)==1);
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->Destroy();
    }

    for(size_t i = 0; i < m_vMonitor.size(); i++)
    {
        m_vMonitor[i]->Destroy();
    }

    int x = 10;
    int y = 10;

    if(!m_bStereo)
    {
        if(m_nInputChannels != 2) //not stereo
        {
            m_vMeters.resize(m_nInputChannels);
            m_vMonitor.resize(m_nInputChannels);

            for(unsigned long i = 0; i < m_vMeters.size(); i++)
            {
                m_vMeters[i] = new AngleMeter(this, wxID_ANY, wxString::Format(wxT("Channel %lu"),i), -70.0, AngleMeter::MONO, i, wxPoint(x,y), wxSize(180,150));
                m_vMeters[i]->SetInputChannels(m_nInputChannels);

                m_vMonitor[i] = new wmButton(this, wxNewId(), wxT("Monitor"), wxPoint(x, y+155), wxSize(180, 35));
                m_vMonitor[i]->SetBackgroundColour(wxColour(80,70,180));
                m_vMonitor[i]->SetIntData(i);

                x+= 190;
                if(i == 3)
                {
                    x = 10;
                    y = 250;
                }
            }
        }
        else
        {
            m_vMeters.resize(3);
            m_vMonitor.resize(0);
            m_vMeters[0] = new AngleMeter(this, wxID_ANY, wxT("Left"), -70.0, AngleMeter::MONO, 0, wxPoint(10,10), wxSize(280,200));
            m_vMeters[1] = new AngleMeter(this, wxID_ANY, wxT("Right"), -70.0, AngleMeter::MONO, 1, wxPoint(300,10), wxSize(280,200));
            m_vMeters[2] = new AngleMeter(this, wxID_ANY, wxT("Mono/Stereo"), -70.0, AngleMeter::MONO_STEREO, 0, wxPoint(200,250), wxSize(280,200));
            m_vMeters[0]->SetInputChannels(2);
            m_vMeters[1]->SetInputChannels(2);
            m_vMeters[2]->SetInputChannels(2);


        }
    }
    else
    {
        if(m_nInputChannels != 2)
        {
            m_vMeters.resize(m_nInputChannels/2);
            m_vMonitor.resize(m_nInputChannels/2);

            for(unsigned long i = 0; i < m_vMeters.size(); i++)
            {
                m_vMeters[i] = new AngleMeter(this, wxID_ANY, wxString::Format(wxT("Channel %lu"),i), -70.0, AngleMeter::LEFT_RIGHT, i*2, wxPoint(x,y), wxSize(360,180));
                m_vMeters[i]->SetInputChannels(m_nInputChannels);
                m_vMonitor[i] = new wmButton(this, wxNewId(), wxT("Monitor"), wxPoint(x, y+185), wxSize(360, 35));
                m_vMonitor[i]->SetBackgroundColour(wxColour(80,70,180));
                m_vMonitor[i]->SetIntData(i);

                x+= 380;
                if(i == 1)
                {
                    x = 10;
                    y = 240;
                }
            }
        }
        else
        {
            m_vMeters.resize(2);
            m_vMonitor.resize(0);
            m_vMeters[0] = new AngleMeter(this, wxID_ANY, wxT("Left/Right"), -70.0, AngleMeter::LEFT_RIGHT, 0, wxPoint(10,60), wxSize(380,250));
            m_vMeters[1] = new AngleMeter(this, wxID_ANY, wxT("Mono/Stereo"), -70.0, AngleMeter::MONO_STEREO, 0, wxPoint(400,60), wxSize(380,250));
            m_vMeters[0]->SetInputChannels(2);
            m_vMeters[1]->SetInputChannels(2);
        }
    }
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetPeakMode(m_pBuilder->ReadSetting(wxT("Peaks"),0));
        m_vMeters[i]->SetMeterMSMode(m_pBuilder->ReadSetting(wxT("M3M6"),2));
        m_vMeters[i]->DisplayCurrentLevelAsText(m_pBuilder->ReadSetting(wxT("DisplayText_Current"),1));
        m_vMeters[i]->DisplayPeakLevelAsText(m_pBuilder->ReadSetting(wxT("DisplayText_Peak"),1));
        m_vMeters[i]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlAngleMeters::OnLeftUp,0,this);
    }
    SetMode(m_pBuilder->ReadSetting(wxT("Mode"), wxT("BBC")));
    ColourMonitorButtons();
}



void pnlAngleMeters::SetAudioData(const timedbuffer* pBuffer)
{
    m_pCalculator->CalculateLevel(pBuffer);
    if(m_nInputChannels != 2)
    {
        if(!m_bStereo)
        {
            for(size_t i = 0; i < m_vMeters.size(); i++)
            {
                double dValue[2] = {m_pCalculator->GetLevel(i), m_pCalculator->GetLevel(i)};
                m_vMeters[i]->ShowValue(dValue);
            }
        }
        else
        {
             for(size_t i = 0; i < m_vMeters.size(); i++)
            {
                double dValue[2] = {m_pCalculator->GetLevel(i*2), m_pCalculator->GetLevel(i*2+1)};
                m_vMeters[i]->ShowValue(dValue);
            }
        }
    }
    else
    {
        if(!m_bStereo)
        {
            double dLeft[2] = {m_pCalculator->GetLevel(0), m_pCalculator->GetLevel(0)};
            double dRight[2] = {m_pCalculator->GetLevel(1), m_pCalculator->GetLevel(1)};
            double dMS[2] = {m_pCalculator->GetMSLevel(false), m_pCalculator->GetMSLevel(true)};
            m_vMeters[0]->ShowValue(dLeft);
            m_vMeters[1]->ShowValue(dRight);
            m_vMeters[2]->ShowValue(dMS);
        }
        else
        {
            double dLeftRight[2] = {m_pCalculator->GetLevel(0), m_pCalculator->GetLevel(1)};
            double dMS[2] = {m_pCalculator->GetMSLevel(false), m_pCalculator->GetMSLevel(true)};
            m_vMeters[0]->ShowValue(dLeftRight);
            m_vMeters[1]->ShowValue(dMS);
        }
    }
}


void pnlAngleMeters::SetMode(const wxString& sMode)
{
    map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().FindType(sMode);
    if(itType != PPMTypeManager::Get().GetTypeEnd())
    {
        m_pCalculator->SetMode(itType->second.nType);
        m_pCalculator->SetDynamicResponse(itType->second.dRiseTime, itType->second.dRisedB, itType->second.dFallTime, itType->second.dFalldB);
        for(size_t i = 0; i < m_vMeters.size(); i++)
        {
            m_vMeters[i]->SetLevels(itType->second.vLevels,itType->second.dOffset, itType->second.dScaling, itType->first, itType->second.sUnit, itType->second.dOverMod, itType->second.sReference);
        }
    }
}


void pnlAngleMeters::Freeze(bool bFreeze)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->FreezeMeter(bFreeze);
    }
}

void pnlAngleMeters::ShowPeaks(unsigned int nMode)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetPeakMode(nMode);
    }
}

void pnlAngleMeters::ClearMeters()
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->ResetMeter();
    }
}


void pnlAngleMeters::SetSpeed(unsigned long nSpeed)
{
    m_pCalculator->SetSpeed(nSpeed);
}

void pnlAngleMeters::SetM3M6(unsigned long nMode)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetMeterMSMode(nMode);
    }
    m_pCalculator->SetMSMode(nMode);
}




void pnlAngleMeters::OnMonitorClicked(wxCommandEvent& event)
{
    vector<char> vChannels;
    vChannels.resize(2);
    vChannels[0] = event.GetExtraLong();
    if((m_pBuilder->ReadSetting(wxT("Stereo"),0)==1))   //stereo
    {
        vChannels[1] = event.GetExtraLong()+1;
    }
    else
    {
        vChannels[1] = event.GetExtraLong();
    }

    m_pBuilder->AskToMonitor(vChannels);
}


void pnlAngleMeters::OutputChannels(const std::vector<char>& vChannels)
{
    m_vOutputChannels = vChannels;
    ColourMonitorButtons();
}

void pnlAngleMeters::ColourMonitorButtons()
{
     for(size_t i = 0; i < m_vMonitor.size(); i++)
    {
        m_vMonitor[i]->SetBackgroundColour(wxColour(80,70,180));
    }

    m_bStereo = (m_pBuilder->ReadSetting(wxT("Stereo"),0)==1);
    if(!m_bStereo)
    {
        for(size_t i = 0; i < m_vOutputChannels.size(); i++)
        {
            if(m_vOutputChannels[i] < m_vMonitor.size())
            {
                m_vMonitor[m_vOutputChannels[i]]->SetBackgroundColour(wxColour(255,200,0));
            }
        }
    }
    else
    {
        if(m_vOutputChannels.size() ==2)
        {

            if(m_vOutputChannels[0] % 2 == 0 && m_vOutputChannels[1] == m_vOutputChannels[0] + 1 && m_vOutputChannels[0]/2 < m_vMonitor.size())   //monitoring subsequent channels
            {
                m_vMonitor[m_vOutputChannels[0]/2]->SetBackgroundColour(wxColour(255,200,0));
            }
        }
    }
}


void pnlAngleMeters::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetClientSize().x <=600));
}


void pnlAngleMeters::DisplayCurrentLevelAsText(bool bShow)
{
    for(size_t i= 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->DisplayCurrentLevelAsText(bShow);
    }
}
void pnlAngleMeters::DisplayPeakLevelAsText(bool bShow)
{
    for(size_t i= 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->DisplayPeakLevelAsText(bShow);
    }
}

void pnlAngleMeters::ShowSurround(bool bShow)
{
    for(size_t i= 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->ShowSurround(bShow);
    }
}
