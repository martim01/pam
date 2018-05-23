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
#include "maxmingraph.h"
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
const wxColour pnlEbuMeter::CLR_LUFS = wxColour(110,207,169);
const wxColour pnlEbuMeter::CLR_SHORT = wxColour(110,169,207);

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
    CreateMeters();


    m_pR128 = new R128Calculator();
    m_pTrue = new TruePeakCalculator();
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
}

void pnlEbuMeter::CreateMeters()
{

    m_pLevels = new R128Meter(this, wxID_ANY, wxEmptyString, -59, true, wxPoint(5,0), wxSize(50,481));
    m_aMeters[0] = new R128Meter(this,wxID_ANY, wxT("Momentary"), -59, false, wxPoint(55, 0), wxSize(80, 480));
    m_aMeters[1] = new R128Meter(this,wxID_ANY, wxT("Short"), -59, false, wxPoint(145, 0), wxSize(80, 480));
    m_aMeters[2] = new R128Meter(this,wxID_ANY, wxT("Integrated"), -59, false, wxPoint(235, 0), wxSize(100, 480));

    m_pPeakLevels = new R128Meter(this, wxID_ANY, wxEmptyString, -40, true, wxPoint(630,0), wxSize(50,481));
    m_pPeakLeft = new R128Meter(this, wxID_ANY, wxT("Left"), -40, false, wxPoint(685,0), wxSize(50,481));
    m_pPeakRight = new R128Meter(this, wxID_ANY, wxT("Right"), -40, false, wxPoint(740,0), wxSize(50,481));
    m_pPeakLeft->SetLightColours(-8,wxColour(220,0,0), -8,wxColour(240,0,0),  wxColour(255,100,100));
    m_pPeakRight->SetLightColours(-8,wxColour(0,220,0), -8,wxColour(0,240,0),  wxColour(255,100,100));
    m_pPeakLevels->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    m_pPeakLeft->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    m_pPeakRight->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);

    m_pPeakLevels->SetMinMax(-40.0, 5.0);
    m_pPeakLeft->SetMinMax(-40.0, 5.0);
    m_pPeakRight->SetMinMax(-40.0, 5.0);

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
    m_plblRangeTitle = new wmLabel(this, wxID_ANY, wxT("Range"), wxPoint(350,150), wxSize(100,40));
    InitLabel(m_plblRangeTitle, CLR_LUFS);

    m_plblMomentary = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,0), wxSize(100,40));
    InitLabel(m_plblMomentary, CLR_SHORT);
    m_plblShort = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,50), wxSize(100,40));
    InitLabel(m_plblShort, CLR_SHORT);
    m_plblLufs = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,100), wxSize(145,40));
    InitLabel(m_plblLufs, CLR_LUFS);
    m_plblRange = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(452,150), wxSize(145,40));
    InitLabel(m_plblRange, CLR_LUFS);

    m_plblMomentaryMax = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(552, 0), wxSize(45,40));
    InitLabel(m_plblMomentaryMax, CLR_SHORT,8);
    m_plblShortMax = new wmLabel(this, wxID_ANY, wxT(""), wxPoint(552, 50), wxSize(45,40));
    InitLabel(m_plblShortMax, CLR_SHORT,8);


    m_aMeters[0]->SetLightColours(-8,wxColour(0,0,200), -23, wxColour(255,0,0), wxColour(255,100,100));
    m_aMeters[1]->SetLightColours(-8,wxColour(0,0,200), -23, wxColour(50,50,255), wxColour(255,100,100));
    m_aMeters[2]->SetLightColours(-8,wxColour(0,200,0), -23, wxColour(230,230,0), wxColour(255,100,100));

    for(size_t i = 0; i < 3; i++)
    {

        m_aMeters[i]->SetShading(m_pBuilder->ReadSetting(wxT("Shading"),0)==1);
        m_aMeters[i]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlEbuMeter::OnInfoLeftUp,0,this);
    }

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
    m_pR128->CalculateLevel(pBuffer);
    m_pTrue->CalculateLevel(pBuffer);
    UpdateMeters();
}

void pnlEbuMeter::UpdateMeters()
{
    m_aMeters[0]->ShowValue(m_pR128->GetMomentaryLevel());
    m_aMeters[1]->ShowValue(m_pR128->GetShortLevel());
    m_aMeters[2]->ShowValue(m_pR128->GetLiveLevel());

    m_pPeakLeft->ShowValue(m_pTrue->GetLevel(0));
    m_pPeakRight->ShowValue(m_pTrue->GetLevel(1));


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
    m_plblMomentaryMax->SetLabel(wxString::Format(wxT("[%.1f]"), m_pR128->GetMomentaryMax()));
    m_plblShortMax->SetLabel(wxString::Format(wxT("[%.1f]"), m_pR128->GetShortMax()));
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
    for(size_t i = 0; i < m_aMeters.size(); i++)
    {
        m_aMeters[i]->ResetMeter();
    }
    m_pR128->ResetMeter();
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

