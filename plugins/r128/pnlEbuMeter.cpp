#include <wx/log.h>
#include "pnlEbuMeter.h"
#include "session.h"
#include "timedbuffer.h"
#include "r128meter.h"
#include "r128builder.h"
#include "wmbutton.h"
#include "levelmeter.h"
#include "r128calculator.h"
#include "truepeakcalculator.h"
#include <array>
#include <wx/log.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
#include "settings.h"
#include "correlationbar.h"

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
const long pnlEbuMeter::ID_M_PBTN1 = wxNewId();
const long pnlEbuMeter::ID_M_PBTN2 = wxNewId();
//*)
const wxColour pnlEbuMeter::CLR_LUFS = wxColour(110,207,169);
const wxColour pnlEbuMeter::CLR_TIME = wxColour(100,200,159);
const wxColour pnlEbuMeter::CLR_SHORT = wxColour(110,169,207);
const wxColour pnlEbuMeter::CLR_PEAK = wxColour(110,169,169);
const wxColour pnlEbuMeter::CLR_PEAK_ALARM = wxColour(207,110,110);

BEGIN_EVENT_TABLE(pnlEbuMeter,wxPanel)
END_EVENT_TABLE()

pnlEbuMeter::pnlEbuMeter(wxWindow* parent,R128Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnLeftUp);
	//*)
	m_pLevels = 0;

	m_dOffset = 0.0;
	m_dPeak[0] = -80;
    m_dPeak[1] = -80;

    CreateMeters();

    m_pbtnCalculate = new wmButton(this, ID_M_PBTN1, _("R128"), wxPoint(370,380), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnReset = new wmButton(this, ID_M_PBTN2, _("Reset"), wxPoint(420,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlEbuMeter::OnbtnCalculateClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlEbuMeter::OnbtnResetClick);

    m_pbtnCalculate->SetBackgroundColour(CLR_SHORT);
	m_pbtnCalculate->SetToggleLook(true, wxT("Pause"), wxT("Run"), 50.0);
	m_pbtnCalculate->ToggleSelection(m_pBuilder->ReadSetting(wxT("Calculate"),1) == 1, true);



    m_pR128 = new R128Calculator();
    m_pTrue = new TruePeakCalculator();

    m_dtStart = wxDateTime::Now();
    LoadSettings();
}

pnlEbuMeter::~pnlEbuMeter()
{
	//(*Destroy(pnlEbuMeter)
	//*)
	delete m_pR128;
	delete m_pTrue;
}


void pnlEbuMeter::SetSession(const session& aSession)
{
    m_pR128->InputSession(aSession);
    m_pTrue->InputSession(aSession);

    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nChannels = aSession.GetCurrentSubsession()->nChannels;
    }
    else
    {
        m_nChannels = 0;
    }
}

void pnlEbuMeter::CreateMeters()
{

    m_pLevels = new R128Meter(this, wxID_ANY, wxEmptyString, -59, 0, true, wxPoint(5,0), wxSize(50,481));
    m_aMeters[0] = new R128Meter(this,wxID_ANY, wxT("Momentary"), -59,0,  false, wxPoint(55, 0), wxSize(70, 480));
    m_aMeters[1] = new R128Meter(this,wxID_ANY, wxT("Short"), -59, 0, false, wxPoint(135, 0), wxSize(70, 480));
    m_aMeters[2] = new R128Meter(this,wxID_ANY, wxT("Integrated"), -59,0,  false, wxPoint(235, 0), wxSize(100, 480));

    m_pPeakLevels = new R128Meter(this, wxID_ANY, wxEmptyString, -40,0,  true, wxPoint(630,0), wxSize(50,481));
    m_pPeakLeft = new R128Meter(this, wxID_ANY, wxT("Left"), -40, 0, false, wxPoint(685,0), wxSize(50,481));
    m_pPeakRight = new R128Meter(this, wxID_ANY, wxT("Right"), -40, 0, false, wxPoint(740,0), wxSize(50,481));



    m_pPeakLeft->SetShading(false);
    m_pPeakRight->SetShading(false);
    m_pPeakLeft->SetLightColours(-8,wxColour(0,220,0), 0,wxColour(0,240,0),  wxColour(255,100,100));
    m_pPeakRight->SetLightColours(-8,wxColour(0,220,0), 0,wxColour(0,240,0),  wxColour(255,100,100));
    m_pPeakLevels->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    m_pPeakLeft->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    m_pPeakRight->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);




    m_pPeakLevels->SetMinMax(-40.0, 5.0);
    m_pPeakLeft->SetMinMax(-40.0, 5.0);
    m_pPeakRight->SetMinMax(-40.0, 5.0);
    m_pPeakLeft->SetFall(0.3);
    m_pPeakRight->SetFall(0.3);

    m_pPeakLeft->SetTargetLevel(0, wxPen(wxColour(255,255,255)));
    m_pPeakRight->SetTargetLevel(0, wxPen(wxColour(255,255,255)));

    double dLevels[14] = {5.0, 3.0, 0.0,-3.0, -6.0, -9.0, -12.0, -15.0, -18.0, -21.0, -24.0, -30.0, -36.0, -40.0};
    m_pPeakLevels->SetLevels(dLevels, 14,0.0);
    m_pPeakLeft->SetLevels(dLevels, 14,0.0);
    m_pPeakRight->SetLevels(dLevels, 14,0.0);


    m_plblMomentaryTitle = new wmLabel(this, wxID_ANY, wxT("Momentary"), wxPoint(350,0), wxSize(100,40));
    InitLabel(m_plblMomentaryTitle, CLR_SHORT);
    m_plblShortTitle = new wmLabel(this, wxID_ANY, wxT("Short"), wxPoint(350,50), wxSize(100,40));
    InitLabel(m_plblShortTitle, CLR_SHORT);

    m_plblLufsTitle = new wmLabel(this, wxID_ANY, wxT("Integrated"), wxPoint(350,100), wxSize(100,40));
    InitLabel(m_plblLufsTitle, CLR_LUFS);
    m_plblRangeTitle = new wmLabel(this, wxID_ANY, wxT("Range"), wxPoint(350,145), wxSize(100,40));
    InitLabel(m_plblRangeTitle, CLR_LUFS);
    m_plblTimeTitle = new wmLabel(this, wxID_ANY, wxT("Time"), wxPoint(350,190), wxSize(100,30));
    InitLabel(m_plblTimeTitle, CLR_TIME);



    m_plblPeakLeftTitle = new wmLabel(this, wxID_ANY, wxT("TP Left"), wxPoint(350,230), wxSize(100,40));
    InitLabel(m_plblPeakLeftTitle, CLR_PEAK);
    m_plblPeakRightTitle = new wmLabel(this, wxID_ANY, wxT("TP Right"), wxPoint(350,280), wxSize(100,40));
    InitLabel(m_plblPeakRightTitle, CLR_PEAK);

    m_plblMomentary = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,0), wxSize(100,40));
    InitLabel(m_plblMomentary, CLR_SHORT);
    m_plblShort = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,50), wxSize(100,40));
    InitLabel(m_plblShort, CLR_SHORT);
    m_plblLufs = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,100), wxSize(145,40));
    InitLabel(m_plblLufs, CLR_LUFS);
    m_plblRange = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,145), wxSize(145,40));
    InitLabel(m_plblRange, CLR_LUFS);
    m_plblTime = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,190), wxSize(145,30));
    InitLabel(m_plblTime, CLR_TIME);
    m_plblPeakLeft = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,230), wxSize(100,40));
    InitLabel(m_plblPeakLeft, CLR_PEAK);
    m_plblPeakRight = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,280), wxSize(100,40));
    InitLabel(m_plblPeakRight, CLR_PEAK);


    m_plblMomentaryMax = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(552, 0), wxSize(45,40));
    InitLabel(m_plblMomentaryMax, CLR_SHORT,8);
    m_plblShortMax = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(552, 50), wxSize(45,40));
    InitLabel(m_plblShortMax, CLR_SHORT,8);

    m_plblPeakLeftMax = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(552, 230), wxSize(45,40));
    InitLabel(m_plblPeakLeftMax, CLR_PEAK,8);
    m_plblPeakRightMax = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(552, 280), wxSize(45,40));
    InitLabel(m_plblPeakRightMax, CLR_PEAK,8);

    m_pBar = new CorrelationBar(this, wxID_ANY, wxPoint(350,330), wxSize(250,40));

    for(size_t i = 0; i < 3; i++)
    {

        m_aMeters[i]->SetShading(false);//m_pBuilder->ReadSetting(wxT("Shading"),0)==1);
        m_aMeters[i]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    }


    m_aMeters[0]->SetLightColours(-26,wxColour(0,0,100), -20, wxColour(50,255,50), wxColour(0,0,100));
    m_aMeters[1]->SetLightColours(-26,wxColour(0,0,100), -20, wxColour(50,255,50), wxColour(0,0,100));
    m_aMeters[2]->SetLightColours(-24,wxColour(0,0,100), -22, wxColour(50,255,50), wxColour(0,0,100));

    ChangeScale();
    m_pLevels->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);

}

void pnlEbuMeter::InitLabel(wmLabel* pLabel, const wxColour clrBack, int nFontSize)
{
    pLabel->SetForegroundColour(*wxWHITE);
    pLabel->SetBackgroundColour(clrBack);
    pLabel->SetBorderState(uiRect::BORDER_NONE);
    pLabel->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    pLabel->GetUiRect().Pad(5,0);
    pLabel->SetFont(wxFont(nFontSize,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));
    pLabel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
}

void pnlEbuMeter::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_nChannels > 0)
    {



        m_pR128->CalculateLevel(pBuffer);
        if(m_bTrue)
        {
            m_pTrue->CalculateLevel(pBuffer);
        }
        if(m_bBar)
        {
            m_pBar->SetAudioData(pBuffer);
        }
    }
    UpdateMeters();
}

void pnlEbuMeter::UpdateMeters()
{
    m_aMeters[0]->ShowValue(m_pR128->GetMomentaryLevel());
    m_aMeters[1]->ShowValue(m_pR128->GetShortLevel());
    m_aMeters[2]->ShowValue(m_pR128->GetLiveLevel());

    m_pPeakLeft->ShowValue(m_pTrue->GetLevel(0));
    m_pPeakRight->ShowValue(m_pTrue->GetLevel(1));

    m_dPeak[0] = max(m_pTrue->GetLevel(0), m_dPeak[0]);
    m_dPeak[1] = max(m_pTrue->GetLevel(1), m_dPeak[1]);

    m_plblRange->SetLabel(wxString::Format(wxT("%.1f LU"), m_pR128->GetLURange()));
    if(m_dOffset == 0.0)
    {
        m_plblLufs->SetLabel(wxString::Format(wxT("%.1f LUFS"), m_pR128->GetLiveLevel()));
        m_plblMomentary->SetLabel(wxString::Format(wxT("%.1f LUFS"), m_pR128->GetMomentaryLevel()));
        m_plblShort->SetLabel(wxString::Format(wxT("%.1f LUFS"), m_pR128->GetShortLevel()));
    }
    else
    {
        m_plblLufs->SetLabel(wxString::Format(wxT("%.1f LU"), m_pR128->GetLiveLevel()-m_dOffset));
        m_plblMomentary->SetLabel(wxString::Format(wxT("%.1f LU"), m_pR128->GetMomentaryLevel()-m_dOffset));
        m_plblShort->SetLabel(wxString::Format(wxT("%.1f LU"), m_pR128->GetShortLevel()-m_dOffset));
    }
    m_plblMomentaryMax->SetLabel(wxString::Format(wxT("%.1f"), m_pR128->GetMomentaryMax()));
    m_plblShortMax->SetLabel(wxString::Format(wxT("%.1f"), m_pR128->GetShortMax()));

    SetPeakColour(m_plblPeakLeft, m_pPeakLeft->GetValue());
    SetPeakColour(m_plblPeakRight, m_pPeakRight->GetValue());
    SetPeakColour(m_plblPeakLeftMax, m_dPeak[0]);
    SetPeakColour(m_plblPeakRightMax, m_dPeak[1]);

    m_plblTime->SetLabel((wxTimeSpan(0,0,m_pR128->GetIntegrationTime()/10,0)).Format(wxT("%H:%M:%S")));
}

void pnlEbuMeter::SetPeakColour(wmLabel* pLabel, double dValue)
{
    pLabel->SetLabel(wxString::Format(wxT("%.1fdB"), dValue));
    if(dValue > 0.0)
    {
        pLabel->SetBackgroundColour(CLR_PEAK_ALARM);
    }
    else
    {
        pLabel->SetBackgroundColour(CLR_PEAK);
    }
}

void pnlEbuMeter::Freeze(bool bFreeze)
{
    for(size_t i = 0; i < m_aMeters.size(); i++)
    {
        m_aMeters[i]->FreezeMeter(bFreeze);
    }
}

void pnlEbuMeter::ShowPeaks(unsigned int nMode)
{
    for(size_t i = 0; i < m_aMeters.size(); i++)
    {
        m_aMeters[i]->SetPeakMode(nMode);
    }
}

void pnlEbuMeter::ClearMeters()
{
    m_dtStart = wxDateTime::Now();
    for(size_t i = 0; i < m_aMeters.size(); i++)
    {
        m_aMeters[i]->ResetMeter();
    }
    m_dPeak[0] = -80;
    m_dPeak[1] = -80;
    m_pR128->ResetMeter();
    m_pBar->ClearMeter();
    UpdateMeters();
}



void pnlEbuMeter::SetShading(bool bShaded)
{
    for(size_t i = 0; i < m_aMeters.size(); i++)
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

void pnlEbuMeter::ChangeScale()
{
    if(m_pBuilder->ReadSetting(wxT("Zero"),1) == 0)
    {
        m_dOffset = -23.0;
    }
    else
    {
        m_dOffset = 0.0;
    }

    if(m_pBuilder->ReadSetting(wxT("Scale"),1) == 1)
    {
        double dLevels[14] = {0,-5, -8, -11, -14, -17, -20, -23, -30, -36, -42, -48, -54, -59};
        for(size_t i = 0; i < 3; i++)
        {
            m_aMeters[i]->SetMinMax(-59.0, -5.0);
            m_aMeters[i]->SetLevels(dLevels,14, m_dOffset);
            m_aMeters[i]->Refresh();
            m_aMeters[i]->SetTargetLevel(-23, wxPen(wxColour(255,255,255)));
        }
        m_pLevels->SetMinMax(-59.0,-5.0);
        m_pLevels->SetLevels(dLevels,14, m_dOffset);
        m_pLevels->Refresh();
    }
    else
    {
        double dLevels[10] = {-14, -17, -20, -23, -26, -29, -32, -35, -38, -41};
        for(size_t i = 0; i < 3; i++)
        {
            m_aMeters[i]->SetMinMax(-41.0,-14.0);
            m_aMeters[i]->SetLevels(dLevels,10, m_dOffset);
            m_aMeters[i]->SetTargetLevel(-23, wxPen(wxColour(255,255,255)));
            m_aMeters[i]->Refresh();
        }
        m_pLevels->SetMinMax(-41.0,-14.0);
        m_pLevels->SetLevels(dLevels,10, m_dOffset);
        m_pLevels->Refresh();
    }


}

void pnlEbuMeter::OnbtnCalculateClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Calculate"), event.IsChecked());
}

void pnlEbuMeter::OnbtnResetClick(wxCommandEvent& event)
{
    ClearMeters();
}


void pnlEbuMeter::LoadSettings()
{
    bool bShort((m_pBuilder->ReadSetting(wxT("Show_Short"), wxT("1")) == wxT("1")));
    bool bMomentary((m_pBuilder->ReadSetting(wxT("Show_Momentary"), wxT("1")) == wxT("1")));
    bool bIntegrated((m_pBuilder->ReadSetting(wxT("Show_Live"), wxT("1")) == wxT("1")));
    m_bTrue = (m_pBuilder->ReadSetting(wxT("Show_True"), wxT("1")) == wxT("1"));

    m_aMeters[0]->Show(bMomentary);
    m_aMeters[1]->Show(bShort);
    m_aMeters[2]->Show(bIntegrated);

    m_plblShortTitle->Show(bShort);
    m_plblShort->Show(bShort);
    m_plblShortMax->Show(bShort);

    m_plblMomentaryTitle->Show(bMomentary);
    m_plblMomentary->Show(bMomentary);
    m_plblMomentaryMax->Show(bMomentary);

    m_plblLufs->Show(bIntegrated);
    m_plblLufsTitle->Show(bIntegrated);
    m_plblRange->Show(bIntegrated);
    m_plblRangeTitle->Show(bIntegrated);
    m_plblTime->Show(bIntegrated);
    m_plblTimeTitle->Show(bIntegrated);

    m_pPeakLevels->Show(m_bTrue);
    m_pPeakLeft->Show(m_bTrue);
    m_pPeakRight->Show(m_bTrue);

    m_plblPeakLeft->Show(m_bTrue);
    m_plblPeakLeftMax->Show(m_bTrue);
    m_plblPeakLeftTitle->Show(m_bTrue);
    m_plblPeakRight->Show(m_bTrue);
    m_plblPeakRightMax->Show(m_bTrue);
    m_plblPeakRightTitle->Show(m_bTrue);

    m_bBar = m_pBuilder->ReadSetting(wxT("Show_Phase"), wxT("1")) == wxT("1");
    m_pBar->Show(m_bBar);
}
