#include <wx/log.h>
#include "pnlEbuMeter.h"
#include "session.h"
#include "timedbuffer.h"
#include "r128meter.h"
#include "r128builder.h"
#include "wmbutton.h"
#include "r128calculator.h"
#include <array>
#include <wx/log.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>

using namespace std;



const long pnlEbuMeter::ID_M_PLBL59 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL38 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL40 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL39 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL60 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL41 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL43 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL42 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL44 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL47 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL45 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL48 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL46 = wxNewId();
const long pnlEbuMeter::ID_M_PLBL49 = wxNewId();
const long pnlEbuMeter::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlEbuMeter,wxPanel)
END_EVENT_TABLE()

pnlEbuMeter::pnlEbuMeter(wxWindow* parent,R128Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
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

	pnlInfo->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnLeftUp);
	//*)
	m_pLevels = 0;

	m_pLbl33->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_pLbl20->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_pLbl24->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_pLbl23->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_pLbl25->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_pLbl26->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_pLbl27->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblInput->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblSessionName->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblSessionType->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblSessionSource->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblSessionBits->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblSessionFrequency->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
	m_plblSessionChannels->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);

    CreateMeters();


    m_pR128 = new R128Calculator();
}

pnlEbuMeter::~pnlEbuMeter()
{
	//(*Destroy(pnlEbuMeter)
	//*)
	delete m_pR128;
}


void pnlEbuMeter::SetSession(const session& aSession)
{
    m_plblSessionType->SetLabel(aSession.sType);
    m_plblSessionName->SetLabel(aSession.sName);

    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_plblInput->SetLabel(aSession.itCurrentSubsession->sProtocol);
        m_plblSessionSource->SetLabel(aSession.itCurrentSubsession->sSourceAddress);
        m_plblSessionChannels->SetLabel(wxString::Format(wxT("%u"), aSession.itCurrentSubsession->nChannels));
        m_plblSessionFrequency->SetLabel(wxString::Format(wxT("%u"), aSession.itCurrentSubsession->nSampleRate));
        m_plblSessionBits->SetLabel(aSession.itCurrentSubsession->sCodec);
    }
    else
    {
        m_plblInput->SetLabel(wxEmptyString);
        m_plblSessionSource->SetLabel(wxEmptyString);
        m_plblSessionChannels->SetLabel(wxT("0"));
        m_plblSessionFrequency->SetLabel(wxT("----"));
        m_plblSessionBits->SetLabel(wxEmptyString);
    }

    m_pR128->InputSession(aSession);
}

void pnlEbuMeter::CreateMeters()
{
    int x = 55;
    m_aMeters[0] = new R128Meter(this,wxID_ANY, wxT("Momentary"), -70, false, wxPoint(55, 0), wxSize(80, 480));
    m_aMeters[1] = new R128Meter(this,wxID_ANY, wxT("Short"), -70, false, wxPoint(145, 0), wxSize(80, 480));
    m_aMeters[2] = new R128Meter(this,wxID_ANY, wxT("Integrated"), -70, false, wxPoint(235, 0), wxSize(100, 480));
    m_aMeters[3] = new R128Meter(this,wxID_ANY, wxT("Range"), -70, false, wxPoint(355, 0), wxSize(80, 480));
    m_pLevels = new R128Meter(this, wxID_ANY, wxEmptyString, -70, true, wxPoint(5,0), wxSize(50,481));

    double dLevels[15] = {0,-3, -6, -9, -12, -15, -18, -21, -24, -30, -36, -42, -48, -54, -60};

    m_aMeters[0]->SetLightColours(-38,wxColour(0,0,200), -8, wxColour(230,230,0), wxColour(255,100,100));
    m_aMeters[1]->SetLightColours(-38,wxColour(0,0,200), -8, wxColour(230,230,0), wxColour(255,100,100));
    m_aMeters[2]->SetLightColours(-38,wxColour(0,200,0), -8, wxColour(230,230,0), wxColour(255,100,100));

    for(size_t i = 0; i < 4; i++)
    {

        m_aMeters[i]->SetLevels(dLevels,15, 0.0);
        m_aMeters[i]->SetShading(m_pBuilder->ReadSetting(wxT("Shading"),0)==1);
        m_aMeters[i]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    }

    m_pLevels->SetLevels(dLevels,15,0.0);
    m_pLevels->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);

}


void pnlEbuMeter::SetAudioData(const timedbuffer* pBuffer)
{
    m_pR128->CalculateLevel(pBuffer);

    m_aMeters[0]->ShowValue(m_pR128->GetMomentaryLevel());
    m_aMeters[1]->ShowValue(m_pR128->GetShortLevel());
    m_aMeters[2]->ShowValue(m_pR128->GetLiveLevel());
    m_aMeters[3]->ShowValue(-70);//m_pR128->GetLURange());

//    if(m_pLevels)
//    {
//        m_pLevels->Refresh();
//    }
}


void pnlEbuMeter::Freeze(bool bFreeze)
{
    for(size_t i = 0; i < 4; i++)
    {
        m_aMeters[i]->FreezeMeter(bFreeze);
    }
}

void pnlEbuMeter::ShowPeaks(unsigned int nMode)
{
    for(size_t i = 0; i < 4; i++)
    {
        m_aMeters[i]->SetPeakMode(nMode);
    }
}

void pnlEbuMeter::ClearMeters()
{
    for(size_t i = 0; i < m_aMeters.size(); i++)
    {
        m_aMeters[i]->ResetMeter();
    }
    m_pR128->ResetMeter();
}



void pnlEbuMeter::SetShading(bool bShaded)
{
    for(size_t i = 0; i < 4; i++)
    {
        m_aMeters[i]->SetShading(bShaded);
    }
}

void pnlEbuMeter::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}



void pnlEbuMeter::OnInfoLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}



