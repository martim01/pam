#include "pnlr128.h"


#include <wx/font.h>
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/string.h>

#include "log.h"

#include "pophubbuilder.h"
#include "r128calculator.h"
#include "r128meter.h"
#include "r128types.h"
#include "settings.h"
#include "timedbuffer.h"
#include "wmbutton.h"
#include "wmlabel.h"

const wxColour pnlR128::kColourLfs       = wxColour(110,207,169);
const wxColour pnlR128::kColourTime      = wxColour(100,200,159);
const wxColour pnlR128::kColourShort     = wxColour(110,169,207);
const wxColour pnlR128::kColourPeak      = wxColour(110,169,169);
const wxColour pnlR128::kColourPeakAlarm = wxColour(207,110,110);

BEGIN_EVENT_TABLE(pnlR128,wxPanel)
END_EVENT_TABLE()

pnlR128::pnlR128(wxWindow *parent, pophubBuilder *pBuilder, const wxPoint &pos, const wxSize &size, long nStyle) :
    m_pBuilder(pBuilder)
{
	Create(parent, wxID_ANY, pos, size, wxTAB_TRAVERSAL | nStyle, _T("id"));
	   

    wxBoxSizer* pBoxSizer;
    wxFlexGridSizer* pFlexGridSizer;
    pFlexGridSizer = new wxFlexGridSizer(4, 3, 0, 0);
    pFlexGridSizer->AddGrowableCol(2);

    m_plblMomentaryTitle = new wmLabel(this, wxID_ANY, "Momentary", wxPoint(-1,-1), wxSize(80,-1));
    pFlexGridSizer->Add(m_plblMomentaryTitle, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_plblMomentary = new wmLabel(this, wxID_ANY, "");
    pFlexGridSizer->Add(m_plblMomentary, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_aMeters[enumType::kMomentary] = new R128MeterH(this,wxID_ANY, "Momentary", -59,0,  false);
    pFlexGridSizer->Add(m_aMeters[enumType::kMomentary], 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());

    m_plblShortTitle = new wmLabel(this, wxID_ANY, "Short");
    

    pFlexGridSizer->Add(m_plblShortTitle, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_plblShort = new wmLabel(this, wxID_ANY, "");
    pFlexGridSizer->Add(m_plblShort, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_aMeters[enumType::kShort] = new R128MeterH(this,wxID_ANY, "Short", -59,0,  false);
    pFlexGridSizer->Add(m_aMeters[enumType::kShort], 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());

    m_plblLufsTitle = new wmLabel(this, wxID_ANY, "Integrated");
    pFlexGridSizer->Add(m_plblLufsTitle, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_plblLufs = new wmLabel(this, wxID_ANY, "");
    pFlexGridSizer->Add(m_plblLufs, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_aMeters[enumType::kIntegrated] = new R128MeterH(this,wxID_ANY, "Integrated", -59,0,  false);
    pFlexGridSizer->Add(m_aMeters[enumType::kIntegrated], 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());


    m_plblRangeTitle = new wmLabel(this, wxID_ANY, "Range");
    pFlexGridSizer->Add(m_plblRangeTitle, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    m_plblRange = new wmLabel(this, wxID_ANY, "");
    pFlexGridSizer->Add(m_plblRange, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    
    pBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_plblTimeTitle = new wmLabel(this, wxID_ANY, "Time");
    pBoxSizer->Add(m_plblTimeTitle, 0, wxALL|wxEXPAND);
    m_plblTime = new wmLabel(this, wxID_ANY, "");
    pBoxSizer->Add(m_plblTime, 0, wxALL|wxEXPAND);
    pBoxSizer->AddStretchSpacer(1);

    m_pbtnCalculate = new wmButton(this, wxID_ANY, _("R128"), wxPoint(370,380), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnCalculate->SetBackgroundColour(kColourShort);
	m_pbtnCalculate->SetToggle(true, "Pause", "Run", 50.0);
	m_pbtnCalculate->ConnectToSetting(m_pBuilder->GetSection(), "r128_run", true);
    pBoxSizer->Add(m_pbtnCalculate, 0, wxALL);

    m_pbtnReset = new wmButton(this, wxID_ANY, _("Reset"), wxPoint(470,430), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnReset->SetBackgroundColour(wxColour(150,0,0));
    
    pBoxSizer->Add(m_pbtnReset, 0, wxALL);

    pFlexGridSizer->Add(pBoxSizer, 0, wxALL|wxEXPAND,  wxDLG_UNIT(this,wxSize(2,0)).GetWidth());
    SetSizer(pFlexGridSizer);
    pFlexGridSizer->SetSizeHints(this);
    
    CreateMeters();

    

    SetSize(796, 140);

    for(auto pMeter : m_aMeters)
    {
        pMeter->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlR128::OnLeftUp,0,this);
    }

    Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlR128::OnLeftUp);
    
    Bind(wxEVT_BUTTON, [this](wxCommandEvent& e){ClearMeters(); }, m_pbtnReset->GetId());

    Settings::Get().AddHandler(this, m_pBuilder->GetSection());
	Bind(wxEVT_SETTING_CHANGED, [this](SettingEvent& event){
		pml::log::log(pml::log::Level::kDebug,"pophup") << event.GetKey() << "=" << event.GetValue();
		if(event.GetKey() == "r128_run")
        {
            m_bRun = event.GetValue(true);
        }
    });
}

pnlR128::~pnlR128()
{
    Settings::Get().RemoveHandler(this);
}


void pnlR128::SetSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_subsession = (*aSession.GetCurrentSubsession());
        m_nChannels = aSession.GetCurrentSubsession()->nChannels;
    }
    else
    {
        m_subsession = subsession();
        m_nChannels = 0;
    }
    
    m_calc.InputSession(aSession);    
    m_calc.SetChannelGroup(-1);
    
}

void pnlR128::CreateMeters()
{
    InitLabel(m_plblMomentaryTitle);
    InitLabel(m_plblShortTitle);
    InitLabel(m_plblLufsTitle);
    InitLabel(m_plblRangeTitle);
    InitLabel(m_plblTimeTitle);
    InitLabel(m_plblMomentary);
    InitLabel(m_plblShort);
    InitLabel(m_plblLufs);
    InitLabel(m_plblRange);
    InitLabel(m_plblTime);
    

    for(auto pMeter : m_aMeters)
    {    
        pMeter->SetShading(false);//m_pBuilder->ReadSetting(wxT("Shading"),0)==1);
    }


    m_aMeters[0]->SetLightColours(-26,wxColour(0,0,100), -20, wxColour(50,255,50), wxColour(0,0,100));
    m_aMeters[1]->SetLightColours(-26,wxColour(0,0,100), -20, wxColour(50,255,50), wxColour(0,0,100));
    m_aMeters[2]->SetLightColours(-24,wxColour(0,0,100), -22, wxColour(50,255,50), wxColour(0,0,100));


    ChangeR128(m_pBuilder->ReadSetting("r128_type", "R128 +18"));

}

void pnlR128::InitLabel(wmLabel* pLabel)
{
    pLabel->SetForegroundColour(*wxWHITE);
    pLabel->SetBackgroundColour(*wxBLACK);
    pLabel->SetBorderState(uiRect::BORDER_NONE);
    pLabel->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    pLabel->GetUiRect().Pad(5,0);
    pLabel->SetFont(wxFont(9,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));
    pLabel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlR128::OnLeftUp,0,this);
}

void pnlR128::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_nChannels > 0 && m_bRun)
    {
        m_calc.CalculateLevel(pBuffer);
        UpdateMeters();
    }
}

void pnlR128::UpdateMeters()
{
    m_aMeters[0]->ShowValue(m_calc.GetMomentaryLevel());
    m_aMeters[1]->ShowValue(m_calc.GetShortLevel());
    m_aMeters[2]->ShowValue(m_calc.GetLiveLevel());

    m_plblRange->SetLabel(wxString::Format("%.1f LU", m_calc.GetLURange()));
    if(m_dOffset == 0.0)
    {
        m_plblLufs->SetLabel(wxString::Format("%.1f LUFS", m_calc.GetLiveLevel()));
        m_plblMomentary->SetLabel(wxString::Format("%.1f LUFS", m_calc.GetMomentaryLevel()));
        m_plblShort->SetLabel(wxString::Format("%.1f LUFS", m_calc.GetShortLevel()));
    }
    else
    {
        m_plblLufs->SetLabel(wxString::Format("%.1f LU", m_calc.GetLiveLevel()-m_dOffset));
        m_plblMomentary->SetLabel(wxString::Format("%.1f LU", m_calc.GetMomentaryLevel()-m_dOffset));
        m_plblShort->SetLabel(wxString::Format("%.1f LU", m_calc.GetShortLevel()-m_dOffset));
    }
    //m_plblMomentaryMax->SetLabel(wxString::Format("%.1f", m_calc.GetMomentaryMax()));
    //m_plblShortMax->SetLabel(wxString::Format("%.1f", m_calc.GetShortMax()));

    m_plblTime->SetLabel((wxTimeSpan(0,0,m_calc.GetIntegrationTime()/10,0)).Format("%H:%M:%S"));
}

void pnlR128::Freeze(bool bFreeze)
{
    for(auto pMeter : m_aMeters)
    {
        pMeter->FreezeMeter(bFreeze);
    }
}

void pnlR128::ShowPeaks(unsigned int nMode)
{
    for(auto pMeter : m_aMeters)
    {
        pMeter->SetPeakMode(nMode);
    }
}

void pnlR128::ClearMeters()
{
    for(auto pMeter : m_aMeters)
    {
        pMeter->ResetMeter();
    }
    m_calc.ResetMeter();

    UpdateMeters();
}



void pnlR128::SetShading(bool bShaded)
{
    for(auto pMeter : m_aMeters)
    {
        pMeter->SetShading(bShaded);
    }
}

void pnlR128::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetParent()->GetSize().x <= 600));
}


void pnlR128::ChangeR128(const wxString& sType)
{
    auto itType = R128TypeManager::Get().GetTypeMap().find(sType);
    if(itType != R128TypeManager::Get().GetTypeMap().end())
    {
        if(m_pBuilder->ReadSetting("Zero",1) == 0)
        {
            m_dOffset = itType->second.dTarget;
        }
        else
        {
            m_dOffset = 0.0;
        }

        for(auto pMeter : m_aMeters)
        {
            pMeter->SetMinMax(itType->second.dMin, itType->second.dMax);
            pMeter->SetLevels(itType->second.vLevels, m_dOffset);
            //pMeter->SetLightColours(itType->second.dOk, itType->second.clrLow,itType->second.dHigh, itType->second.clrOk, itType->second.clrHigh);
            pMeter->Refresh();
            pMeter->SetTargetLevel(itType->second.dTarget, wxPen(wxColour(255,255,255)));
        }

     //   m_pLevels->SetMinMax(itType->second.dMin, itType->second.dMax);
     //   m_pLevels->SetLevels(itType->second.vLevels, m_dOffset);
     //   m_pLevels->Refresh();
    }

}

void pnlR128::OnbtnCalculateClick(wxCommandEvent& event)
{
}

void pnlR128::OnbtnResetClick(wxCommandEvent& event)
{
    ClearMeters();
}




