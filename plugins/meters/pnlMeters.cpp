#include <wx/log.h>
#include "pnlMeters.h"
#include "session.h"
#include "timedbuffer.h"
#include "levelmeter.h"
#include "metersbuilder.h"
#include "wmbutton.h"
#include "levelcalculator.h"
#include <array>
#include "ppmtypes.h"
//(*InternalHeaders(pnlMeters)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;





//(*IdInit(pnlMeters)
const long pnlMeters::ID_M_PLBL59 = wxNewId();
const long pnlMeters::ID_M_PLBL38 = wxNewId();
const long pnlMeters::ID_M_PLBL40 = wxNewId();
const long pnlMeters::ID_M_PLBL39 = wxNewId();
const long pnlMeters::ID_M_PLBL60 = wxNewId();
const long pnlMeters::ID_M_PLBL41 = wxNewId();
const long pnlMeters::ID_M_PLBL43 = wxNewId();
const long pnlMeters::ID_M_PLBL42 = wxNewId();
const long pnlMeters::ID_M_PLBL44 = wxNewId();
const long pnlMeters::ID_M_PLBL47 = wxNewId();
const long pnlMeters::ID_M_PLBL45 = wxNewId();
const long pnlMeters::ID_M_PLBL48 = wxNewId();
const long pnlMeters::ID_M_PLBL46 = wxNewId();
const long pnlMeters::ID_M_PLBL49 = wxNewId();
const long pnlMeters::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMeters,wxPanel)
	//(*EventTable(pnlMeters)
	//*)
END_EVENT_TABLE()

pnlMeters::pnlMeters(wxWindow* parent,MetersBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlMeters)
	Create(parent, id, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	pnlInfo = new wxPanel(this, ID_PANEL1, wxPoint(600,0), wxSize(200,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlInfo->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl33 = new wmLabel(pnlInfo, ID_M_PLBL59, _("Input"), wxPoint(0,0), wxSize(181,20), 0, _T("ID_M_PLBL59"));
	m_pLbl33->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl33->SetForegroundColour(wxColour(255,255,255));
	m_pLbl33->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl20 = new wmLabel(pnlInfo, ID_M_PLBL38, _("Session Name"), wxPoint(0,50), wxSize(181,20), 0, _T("ID_M_PLBL38"));
	m_pLbl20->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl20->SetForegroundColour(wxColour(255,255,255));
	m_pLbl20->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl24 = new wmLabel(pnlInfo, ID_M_PLBL40, _("Session Type"), wxPoint(0,100), wxSize(181,20), 0, _T("ID_M_PLBL40"));
	m_pLbl24->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl24->SetForegroundColour(wxColour(255,255,255));
	m_pLbl24->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl23 = new wmLabel(pnlInfo, ID_M_PLBL39, _("Source"), wxPoint(0,150), wxSize(181,20), 0, _T("ID_M_PLBL39"));
	m_pLbl23->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl23->SetForegroundColour(wxColour(255,255,255));
	m_pLbl23->SetBackgroundColour(wxColour(0,0,255));
	m_plblInput = new wmLabel(pnlInfo, ID_M_PLBL60, wxEmptyString, wxPoint(0,21), wxSize(181,25), 0, _T("ID_M_PLBL60"));
	m_plblInput->SetBorderState(uiRect::BORDER_NONE);
	m_plblInput->SetForegroundColour(wxColour(0,128,0));
	m_plblInput->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblInputFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblInput->SetFont(m_plblInputFont);
	m_plblSessionName = new wmLabel(pnlInfo, ID_M_PLBL41, wxEmptyString, wxPoint(0,71), wxSize(181,25), 0, _T("ID_M_PLBL41"));
	m_plblSessionName->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionName->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionName->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionNameFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionName->SetFont(m_plblSessionNameFont);
	m_plblSessionType = new wmLabel(pnlInfo, ID_M_PLBL43, wxEmptyString, wxPoint(0,121), wxSize(181,25), 0, _T("ID_M_PLBL43"));
	m_plblSessionType->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionType->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionType->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionType->SetFont(m_plblSessionTypeFont);
	m_plblSessionSource = new wmLabel(pnlInfo, ID_M_PLBL42, wxEmptyString, wxPoint(0,171), wxSize(181,25), 0, _T("ID_M_PLBL42"));
	m_plblSessionSource->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionSource->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionSource->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionSourceFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionSource->SetFont(m_plblSessionSourceFont);
	m_pLbl25 = new wmLabel(pnlInfo, ID_M_PLBL44, _("Bits"), wxPoint(0,210), wxSize(79,20), 0, _T("ID_M_PLBL44"));
	m_pLbl25->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl25->SetForegroundColour(wxColour(255,255,255));
	m_pLbl25->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionBits = new wmLabel(pnlInfo, ID_M_PLBL47, wxEmptyString, wxPoint(80,210), wxSize(100,20), 0, _T("ID_M_PLBL47"));
	m_plblSessionBits->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionBits->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionBits->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionBitsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionBits->SetFont(m_plblSessionBitsFont);
	m_pLbl26 = new wmLabel(pnlInfo, ID_M_PLBL45, _("Freq"), wxPoint(0,231), wxSize(79,20), 0, _T("ID_M_PLBL45"));
	m_pLbl26->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl26->SetForegroundColour(wxColour(255,255,255));
	m_pLbl26->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionFrequency = new wmLabel(pnlInfo, ID_M_PLBL48, wxEmptyString, wxPoint(80,231), wxSize(100,20), 0, _T("ID_M_PLBL48"));
	m_plblSessionFrequency->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionFrequency->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionFrequency->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionFrequencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionFrequency->SetFont(m_plblSessionFrequencyFont);
	m_pLbl27 = new wmLabel(pnlInfo, ID_M_PLBL46, _("Channels"), wxPoint(0,252), wxSize(79,20), 0, _T("ID_M_PLBL46"));
	m_pLbl27->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl27->SetForegroundColour(wxColour(255,255,255));
	m_pLbl27->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionChannels = new wmLabel(pnlInfo, ID_M_PLBL49, wxEmptyString, wxPoint(80,252), wxSize(100,20), 0, _T("ID_M_PLBL49"));
	m_plblSessionChannels->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionChannels->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionChannels->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionChannelsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionChannels->SetFont(m_plblSessionChannelsFont);

	pnlInfo->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnLeftUp);
	//*)
	m_pLevels = 0;
    m_dOvermod = -10.0;

	m_pLbl33->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_pLbl20->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_pLbl24->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_pLbl23->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_pLbl25->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_pLbl26->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_pLbl27->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblInput->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblSessionName->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblSessionType->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblSessionSource->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblSessionBits->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblSessionFrequency->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
	m_plblSessionChannels->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);


    Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&pnlMeters::OnMonitorClicked);

    m_vOutputChannels.resize(2);
    m_pCalculator = new LevelCalculator(-70.0);
}

pnlMeters::~pnlMeters()
{
	//(*Destroy(pnlMeters)
	//*)
}


void pnlMeters::SetSession(const session& aSession)
{
    m_plblSessionType->SetLabel(aSession.sType);
    m_plblSessionName->SetLabel(aSession.sName);

    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_plblInput->SetLabel(aSession.GetCurrentSubsession()->sProtocol);
        m_plblSessionSource->SetLabel(aSession.GetCurrentSubsession()->sSourceAddress);
        m_plblSessionChannels->SetLabel(wxString::Format(wxT("%u"), aSession.GetCurrentSubsession()->nChannels));
        m_plblSessionFrequency->SetLabel(wxString::Format(wxT("%u"), aSession.GetCurrentSubsession()->nSampleRate));
        m_plblSessionBits->SetLabel(aSession.GetCurrentSubsession()->sCodec);
        m_nInputChannels = min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_plblInput->SetLabel(wxEmptyString);
        m_plblSessionSource->SetLabel(wxEmptyString);
        m_plblSessionChannels->SetLabel(wxT("0"));
        m_plblSessionFrequency->SetLabel(wxT("----"));
        m_plblSessionBits->SetLabel(wxEmptyString);
        m_nInputChannels = 0;
    }

    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->Destroy();
    }

    for(size_t i = 0; i < m_vMonitor.size(); i++)
    {
        m_vMonitor[i]->Destroy();
    }
    m_vMonitor.resize(0);
    if(m_pLevels)
    {
        m_pLevels->Destroy();
    }

    m_pCalculator->InputSession(aSession);

    int x = 55;
    if(m_nInputChannels != 2) //not stereo
    {
        m_vMeters.resize(m_nInputChannels);
        if(m_nInputChannels%2 == 0)
        {
            m_vMonitor.resize(m_nInputChannels/2);
        }
        else
        {
            m_vMonitor.resize(m_nInputChannels/2+1);
        }

        for(unsigned long i = 0; i < m_vMeters.size(); i++)
        {
            m_vMeters[i] = new LevelMeter(this,wxID_ANY, wxString::Format(wxT("%lu"), i+1), -70, false, wxPoint(x, 0), wxSize(50, 440));



            if(i%2 == 0)
            {
                m_vMonitor[i/2] = new wmButton(this, wxNewId(), wxT("Monitor"), wxPoint(x, 442), wxSize(101, 35));
                m_vMonitor[i/2]->SetBackgroundColour(wxColour(80,70,180));
                m_vMonitor[i/2]->SetIntData(i);
            }
            x+= 51;
        }
        m_pLevels = new LevelMeter(this, wxID_ANY, wxEmptyString, -70, true, wxPoint(5,0), wxSize(50,440));
    }
    else
    {
        m_vMeters.resize(4);
        m_vMonitor.resize(0);
        m_vMeters[0] = new LevelMeter(this,wxID_ANY, wxT("L"), -70, false, wxPoint(55, 0), wxSize(50, 480));
        m_vMeters[1] = new LevelMeter(this,wxID_ANY, wxT("R"), -70, false, wxPoint(110, 0), wxSize(50, 480));
        m_vMeters[2] = new LevelMeter(this,wxID_ANY, wxT("M"), -70, false, wxPoint(200, 0), wxSize(50, 480));
        m_vMeters[3] = new LevelMeter(this,wxID_ANY, wxT("S"), -70, false, wxPoint(260, 0), wxSize(50, 480));




        m_pLevels = new LevelMeter(this, wxID_ANY, wxEmptyString, -70, true, wxPoint(5,0), wxSize(50,481));
    }




    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetNumberOfChannels(m_nInputChannels);
        m_vMeters[i]->SetShading(m_pBuilder->ReadSetting(wxT("Shading"),0)==1);
        m_vMeters[i]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlMeters::OnInfoLeftUp,0,this);
    }

    SetMode(m_pBuilder->ReadSetting(wxT("Mode"),wxT("BBC")));
    SetSpeed(m_pBuilder->ReadSetting(wxT("Speed"),1));
    SetM3M6(m_pBuilder->ReadSetting(wxT("M3M6"),1));
}


void pnlMeters::SetAudioData(const timedbuffer* pBuffer)
{
    m_pCalculator->CalculateLevel(pBuffer);
    if(m_nInputChannels != 2)
    {
        for(size_t i = 0; i < m_vMeters.size(); i++)
        {
            m_vMeters[i]->ShowValue(m_pCalculator->GetLevel(i));
        }
    }
    else
    {
        m_vMeters[0]->ShowValue(m_pCalculator->GetLevel(0));
        m_vMeters[1]->ShowValue(m_pCalculator->GetLevel(1));
        m_vMeters[2]->ShowValue(m_pCalculator->GetMSLevel(false));
        m_vMeters[3]->ShowValue(m_pCalculator->GetMSLevel(true));
    }
    if(m_pLevels)
    {
        m_pLevels->Refresh();
    }
}


void pnlMeters::SetMode(const wxString& sMode)
{
    map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().FindType(sMode);
    if(itType != PPMTypeManager::Get().GetTypeEnd())
    {
        m_pCalculator->SetMode(itType->second.nType);
        m_pCalculator->SetDynamicResponse(itType->second.dRiseTime, itType->second.dRisedB, itType->second.dFallTime, itType->second.dFalldB);
        SetScale(itType->first, itType->second);
    }

}

void pnlMeters::Freeze(bool bFreeze)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->FreezeMeter(bFreeze);
    }
}

void pnlMeters::ShowPeaks(unsigned int nMode)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetPeakMode(nMode);
    }
}

void pnlMeters::ClearMeters()
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->ResetMeter();
    }
}


void pnlMeters::SetSpeed(unsigned long nSpeed)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetSpeed(nSpeed);
    }
    m_pCalculator->SetSpeed(nSpeed);
}

void pnlMeters::SetM3M6(unsigned long nMode)
{
    m_pCalculator->SetMSMode(nMode);

}


void pnlMeters::SetShading(bool bShaded)
{
    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetShading(bShaded);
    }
}

void pnlMeters::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}


void pnlMeters::OnMonitorClicked(wxCommandEvent& event)
{
    vector<char> vChannels;
    vChannels.resize(2);
    vChannels[0] = event.GetExtraLong();
    vChannels[1] = event.GetExtraLong()+1;

    m_pBuilder->AskToMonitor(vChannels);
}


void pnlMeters::OutputChannels(const std::vector<char>& vChannels)
{
    m_vOutputChannels = vChannels;

    for(size_t i = 0; i < m_vMonitor.size(); i++)
    {
        m_vMonitor[i]->SetBackgroundColour(wxColour(80,70,180));
    }

    if(vChannels.size() ==2)
    {
        if(vChannels[0] % 2 == 0 && vChannels[1] == vChannels[0] + 1 && vChannels[0]/2 < m_vMonitor.size())   //monitoring subsequent channels
        {
            m_vMonitor[vChannels[0]/2]->SetBackgroundColour(wxColour(255,200,0));
        }
    }

    SetLightColours();


}

void pnlMeters::OnInfoLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}


void pnlMeters::SetScale(const wxString& sTitle, const ppmtype& aType)
{
    m_dOvermod = aType.dOverMod;
    wxLogDebug(wxT("Overmod = %.f"), m_dOvermod);

    for(size_t i = 0; i < m_vMeters.size(); i++)
    {
        m_vMeters[i]->SetLevels(aType.vLevels, aType.dOffset, aType.sUnit, sTitle, aType.sReference, aType.dScaling);
    }
    if(m_pLevels)
    {
        m_pLevels->SetLevels(aType.vLevels, aType.dOffset, aType.sUnit, sTitle, aType.sReference, aType.dScaling);
    }
    SetLightColours();
}

void pnlMeters::SetLightColours()
{

    if(m_nInputChannels == 2)
    {
        m_vMeters[0]->SetLightColours(wxColour(220,0,0), m_dOvermod, wxColour(255,50,50));
        m_vMeters[1]->SetLightColours(wxColour(0,220,0), m_dOvermod, wxColour(255,50,50));
        m_vMeters[2]->SetLightColours(wxColour(255,255,255), m_dOvermod, wxColour(255,50,50));
        m_vMeters[3]->SetLightColours(wxColour(255,128,0), m_dOvermod, wxColour(255,50,50));
    }
    else
    {
        for(size_t i = 0; i < m_vMeters.size(); i++)
        {
            if(m_vOutputChannels[0] == i)
            {
                m_vMeters[i]->SetLightColours(wxColour(220,0,0), m_dOvermod,wxColour(255,50,50));
            }
            else if(m_vOutputChannels[1] == i)
            {
                m_vMeters[i]->SetLightColours(wxColour(0,220,0), m_dOvermod, wxColour(255,50,50));
            }
            else
            {
                m_vMeters[i]->SetLightColours(wxColour(245,255,245), m_dOvermod, wxColour(255,50,50));
            }
            m_vMeters[i]->ResetMeter();
        }
    }

}
