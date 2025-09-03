#include "pophubpanel.h"
#include "pophubbuilder.h"
#include "timedbuffer.h"
#include "session.h"

#include <wx/intl.h>
#include <wx/string.h>

#include "anglemeter.h"
#include "correlationbar.h"
#include "levelcalculator.h"
#include "log.h"
#include "ppmtypes.h"
#include "settingevent.h"
#include "settings.h"
#include "wmbutton.h"
#include "wmlabel.h"

#include "pnlr128.h"

wxIMPLEMENT_DYNAMIC_CLASS(pophubPanel, pmPanel)

BEGIN_EVENT_TABLE(pophubPanel,pmPanel)
END_EVENT_TABLE()


pophubPanel::pophubPanel(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : 
	pmPanel(),
    m_pBuilder(pBuilder),
	m_pCalculator(std::make_shared<LevelCalculator>(-70.0))
{
	//(*Initialize(pophubPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)
	
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pophubPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);

	ViewChanged(Settings::Get().Read(m_pBuilder->GetSection(), "view", pophubBuilder::enumView::kRadio));


	Settings::Get().AddHandler(this, m_pBuilder->GetSection());
	Bind(wxEVT_SETTING_CHANGED, [this](SettingEvent& event){
		pml::log::log(pml::log::Level::kDebug,"pophup") << event.GetKey() << "=" << event.GetValue();
		if(event.GetKey() == "view")
		{
			ViewChanged(event.GetValue(0L));
		}
		else
		{
			SetMeterSettings();
		}
	});
}

pophubPanel::~pophubPanel()=default;

void pophubPanel::SetAudioData(const timedbuffer* pBuffer)
{
	m_pCalculator->CalculateLevel(pBuffer);

	for(size_t nSide = 0 ; nSide < m_meters.size(); ++nSide)
	{
		if(m_meters[nSide])
		{
			if(m_meterDetails[nSide].bMS == false)		
			{
				m_meters[nSide]->ShowValue({m_pCalculator->GetLevel(m_meterDetails[nSide].nChannel), m_pCalculator->GetLevel(m_meterDetails[nSide].nChannel+1)});
			}
			else
			{
				m_meters[nSide]->ShowValue({m_pCalculator->GetMSLevel(false, m_meterDetails[nSide].nChannel*2), 
						    				m_pCalculator->GetMSLevel(true,m_meterDetails[nSide].nChannel*2)});
			}
		}
	}

	for(auto pBar : m_phasebars)
	{
		if(pBar)
		{
			pBar->SetAudioData(pBuffer);
		}
	}
    
	if(m_ppnlR128)
	{
		m_ppnlR128->SetAudioData(pBuffer);
	}
}

void pophubPanel::InputSession(const session& aSession)
{
	m_session = aSession;

    m_pCalculator->InputSession(aSession);

	if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nInputChannels = std::min((unsigned int)8, aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nInputChannels = 2;
    }

	for(auto pBar : m_phasebars)
	{
		if(pBar)
		{
			pBar->SetNumberOfInputChannels(m_nInputChannels);
		}
	}

	if(m_ppnlR128)
	{
		m_ppnlR128->SetSession(aSession);
	}
}

void pophubPanel::OutputChannels(const std::vector<char>& vChannels)
{
	
}


void pophubPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}

void pophubPanel::DestroyControls()
{
	for(size_t i = 0; i < m_meters.size(); ++i)
	{
		if(m_meters[i])
		{
			m_meters[i]->Destroy();
			m_meters[i] = nullptr;
		}
		if(m_phasebars[i])
		{
			m_phasebars[i]->Destroy();
			m_phasebars[i] = nullptr;
		}
		if(m_meterSelect[i])
		{
			m_meterSelect[i]->Destroy();
			m_meterSelect[i] = nullptr;
		}
	}
	if(m_ppnlR128)
	{
		m_ppnlR128->Destroy();
		m_ppnlR128 = nullptr;
	}
}

void pophubPanel::ViewChanged(unsigned long nView)
{
	DestroyControls();

	switch(nView)
	{
		case pophubBuilder::enumView::kRadio:
			ShowRadio();
			break;
		case pophubBuilder::enumView::kRadioLoudness:
			ShowRadioLoudness();
			break;
		case pophubBuilder::enumView::kNews:
			ShowNews();
			break;
		case pophubBuilder::enumView::kNewsLoudness:
			ShowNewsLoudness();
			break;
		case pophubBuilder::enumView::kNewsGain:
			ShowNewsGain();
			break;
		case pophubBuilder::enumView::kWorkshop:
			ShowWorkshop();
			break;
		case pophubBuilder::enumView::kTV:
			ShowTV();
			break;
	}
}

void pophubPanel::ShowOutputMonitorMeters(unsigned int nTop)
{
	
	m_meterSelect[enumSide::kLeft] = new wmButton(this, wxID_ANY, "", wxPoint(10, nTop), wxSize(380,25));
	m_meters[enumSide::kLeft] = new AngleMeter(this, wxID_ANY, "", -70.0, AngleMeter::LEFT_RIGHT, 0, wxPoint(10,nTop+25), wxSize(380,250));
	m_meters[enumSide::kLeft]->SetInputChannels(2);
	m_meters[enumSide::kLeft]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pophubPanel::OnLeftUp,0,this);
	
	m_meterSelect[enumSide::kRight] = new wmButton(this, wxID_ANY, "", wxPoint(400, nTop), wxSize(380,25));
	m_meters[enumSide::kRight] = new AngleMeter(this, wxID_ANY, "", -70.0, AngleMeter::LEFT_RIGHT, 0, wxPoint(400,nTop+25), wxSize(380,250));
	m_meters[enumSide::kRight]->SetInputChannels(2);
	m_meters[enumSide::kRight]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pophubPanel::OnLeftUp,0,this);
	
	m_phasebars[enumSide::kLeft] = new CorrelationBar(this, wxID_ANY, wxPoint(10, m_meters[enumSide::kLeft]->GetRect().GetBottom()+10), wxSize(380,40));
	m_phasebars[enumSide::kRight] = new CorrelationBar(this, wxID_ANY, wxPoint(400, m_meters[enumSide::kRight]->GetRect().GetBottom()+10), wxSize(380,40));

	m_phasebars[enumSide::kLeft]->SetNumberOfInputChannels(m_nInputChannels);
	m_phasebars[enumSide::kLeft]->SetAxisX(0);
	m_phasebars[enumSide::kLeft]->SetAxisY(1);


	m_phasebars[enumSide::kRight]->SetNumberOfInputChannels(m_nInputChannels);
	m_phasebars[enumSide::kRight]->SetAxisX(m_nInputChannels > 3 ? 2 : 0);
	m_phasebars[enumSide::kRight]->SetAxisY(m_nInputChannels > 3 ? 3 : 0);

	m_meterSelect[enumSide::kLeft]->SetPopup({"Output", "Output M/S", "Monitor", "Monitor M/S"});
	m_meterSelect[enumSide::kRight]->SetPopup({"Output", "Output M/S", "Monitor", "Monitor M/S"});

	m_meterSelect[enumSide::kLeft]->ConnectToSetting(m_pBuilder->GetSection(), "meter_left", "Output");
	m_meterSelect[enumSide::kRight]->ConnectToSetting(m_pBuilder->GetSection(), "meter_right", "Monitor");

	SetMeterSettings();
}

void pophubPanel::ShowNewsMeters(unsigned int nTop)
{
	m_meterSelect[enumSide::kLeft] = new wmButton(this, wxID_ANY, "", wxPoint(10, nTop), wxSize(380,25));

	m_meters[enumSide::kLeft] = new AngleMeter(this, wxID_ANY, "", -70.0, AngleMeter::LEFT_RIGHT, 0, wxPoint(10,nTop+25), wxSize(380,250));
	m_meters[enumSide::kLeft]->SetInputChannels(2);
	m_meters[enumSide::kLeft]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pophubPanel::OnLeftUp,0,this);
	

	m_meterSelect[enumSide::kRight] = new wmButton(this, wxID_ANY, "", wxPoint(400, nTop), wxSize(380,25));

	m_meters[enumSide::kRight] = new AngleMeter(this, wxID_ANY, "", -70.0, AngleMeter::LEFT_RIGHT, 0, wxPoint(400,nTop+25), wxSize(380,250));
	m_meters[enumSide::kRight]->SetInputChannels(2);
	m_meters[enumSide::kRight]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pophubPanel::OnLeftUp,0,this);

	
	m_phasebars[enumSide::kLeft] = new CorrelationBar(this, wxID_ANY, wxPoint(10, m_meters[enumSide::kLeft]->GetRect().GetBottom()+10), wxSize(770,40));
	m_phasebars[enumSide::kRight] = nullptr;

	m_phasebars[enumSide::kLeft]->SetNumberOfInputChannels(m_nInputChannels);
	m_phasebars[enumSide::kLeft]->SetAxisX(0);
	m_phasebars[enumSide::kLeft]->SetAxisY(1);

	SetMeterSettings();
}

void pophubPanel::ShowRadio()
{
	ShowOutputMonitorMeters(60);
}

void pophubPanel::ShowRadioLoudness()
{
	ShowOutputMonitorMeters(10);

	m_ppnlR128 = new pnlR128(this, m_pBuilder, wxPoint(2,335), wxSize(800, 140));
	m_ppnlR128->SetSession(m_session);
}

void pophubPanel::ShowNews()
{
	ShowNewsMeters(60);

}

void pophubPanel::ShowNewsLoudness()
{
	ShowNewsMeters(10);
	m_ppnlR128 = new pnlR128(this, m_pBuilder, wxPoint(2,335), wxSize(800, 140));
	m_ppnlR128->SetSession(m_session);
}

void pophubPanel::ShowNewsGain()
{
	ShowNewsMeters(10);
}

void pophubPanel::ShowWorkshop()
{
	m_meterSelect[enumSide::kLeft] = new wmButton(this, wxID_ANY, "", wxPoint(10, 10), wxSize(780,25));
	
	m_meters[enumSide::kLeft] = new AngleMeter(this, wxID_ANY, "", -70.0, AngleMeter::LEFT_RIGHT, 0, wxPoint(10,35), wxSize(780,380));
	m_meters[enumSide::kLeft]->SetInputChannels(2);
	m_meters[enumSide::kLeft]->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pophubPanel::OnLeftUp,0,this);
	
	
	m_phasebars[enumSide::kLeft] = new CorrelationBar(this, wxID_ANY, wxPoint(10, m_meters[enumSide::kLeft]->GetRect().GetBottom()+10), wxSize(m_meters[enumSide::kLeft]->GetSize().x,40));
	m_phasebars[enumSide::kRight] = nullptr;

	m_meterSelect[enumSide::kRight] = nullptr;

	m_phasebars[enumSide::kLeft]->SetNumberOfInputChannels(m_nInputChannels);
	m_phasebars[enumSide::kLeft]->SetAxisX(0);
	m_phasebars[enumSide::kLeft]->SetAxisY(1);

	SetMeterSettings();
}

void pophubPanel::ShowTV()
{

}

void pophubPanel::SetMeterMSMode(int nSide, const wxString& sLabel)
{
	if(sLabel.Find("Monitor") != wxNOT_FOUND)
	{
		m_meterDetails[nSide].nChannel = 2;
	}
	else
	{
		m_meterDetails[nSide].nChannel = 0;
	}

	if(sLabel.Find("M/S") != wxNOT_FOUND)
	{
		m_meterDetails[nSide].bMS = true;
		if(m_meters[nSide])
		{
			m_meters[nSide]->SetRouting(AngleMeter::MONO_STEREO);
		}
	}
	else
	{
		m_meterDetails[nSide].bMS = false;
		if(m_meters[nSide])
		{
			m_meters[nSide]->SetRouting(AngleMeter::LEFT_RIGHT);
		}
	}

	if(m_meterSelect[nSide])
	{
		m_meterSelect[nSide]->SetLabel(sLabel);
	}
}

void pophubPanel::SetMeterSettings()
{
	
	
	SetMeterMSMode(enumSide::kLeft, Settings::Get().Read(m_pBuilder->GetSection(), "meter_left", "Output"));
	SetMeterMSMode(enumSide::kRight, Settings::Get().Read(m_pBuilder->GetSection(), "meter_right", "Output"));
	
	for(auto pMeter : m_meters)
	{
		if(pMeter)
		{
			pMeter->SetPeakMode(m_pBuilder->ReadSetting("peaks",0));
			pMeter->DisplayCurrentLevelAsText(m_pBuilder->ReadSetting("display_text_current",1));
			pMeter->DisplayPeakLevelAsText(m_pBuilder->ReadSetting("display_text_peak",1));
		}
	}

	for(auto pBar : m_phasebars)
	{
		if(pBar)
		{
			pBar->SetThreshold(m_pBuilder->ReadSetting("phase_threshold", -0.75));
			pBar->ShowBar(m_pBuilder->ReadSetting("phase_display",true));
		}
	}

	SetMode(m_pBuilder->ReadSetting("mode", "BBC"));
}

void pophubPanel::SetMode(const wxString& sMode)
{
    if(auto itType = PPMTypeManager::Get().FindType(sMode); itType != PPMTypeManager::Get().GetTypeEnd())
    {
        m_pCalculator->SetMode(itType->second.nType);
        m_pCalculator->SetDynamicResponse(itType->second.dRiseTime, itType->second.dRisedB, itType->second.dFallTime, itType->second.dFalldB);
        for(auto pMeter : m_meters)
        {
			if(pMeter)
            {
				pMeter->SetLevels(itType->second.vLevels,itType->second.dOffset, itType->second.dScaling, itType->first, itType->second.sUnit, itType->second.dOverMod, itType->second.sReference);
			}
        }
    }
}