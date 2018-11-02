#include "LTCGeneratorbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "LTCGeneratorpanel.h"
#include <wx/datetime.h>
#include <wx/log.h>
#include <cmath>


using namespace std;

LTCGeneratorBuilder::LTCGeneratorBuilder() : GeneratorPluginBuilder(),
m_pEncoder(0)
{

    RegisterForSettingsUpdates(wxT("FPS"), this);
	RegisterForSettingsUpdates(wxT("UserData"), this);
	RegisterForSettingsUpdates(wxT("DateFormat"), this);
	RegisterForSettingsUpdates(wxT("Amplitude"), this);
	RegisterForSettingsUpdates(wxT("OffsetYear"), this);
	RegisterForSettingsUpdates(wxT("OffsetMonth"), this);
	RegisterForSettingsUpdates(wxT("OffsetDay"), this);
	RegisterForSettingsUpdates(wxT("OffsetHour"), this);
	RegisterForSettingsUpdates(wxT("OffsetMinute"), this);
	RegisterForSettingsUpdates(wxT("OffsetSecond"), this);
	RegisterForSettingsUpdates(wxT("Time"), this);


    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&LTCGeneratorBuilder::OnSettingChanged);

}

void LTCGeneratorBuilder::GetAudioData(timedbuffer* pBuffer)
{
    while(m_queueSamples.size() < pBuffer->GetBufferSize()/2)
    {
        CreateFrame();
    }
    int nSample = 0;
    for(int i = 0; i < pBuffer->GetBufferSize(); i+=2)
    {
        if(m_queueSamples.empty() == false)
        {
            pBuffer->GetWritableBuffer()[i] = m_queueSamples.front();
            pBuffer->GetWritableBuffer()[i+1] = m_queueSamples.front();
            m_queueSamples.pop();
        }
        else
        {
            break;
        }
    }
}


void LTCGeneratorBuilder::CreateFrame()
{
    if(m_pEncoder)
    {
        int len;
        ltcsnd_sample_t* pBuffer;

        ltc_encoder_encode_frame(m_pEncoder);

        pBuffer = ltc_encoder_get_bufptr(m_pEncoder, &len, 1);


        //copy the buffer in to queue
        for(int i = 0; i < len; i++)
        {
            float dSample = pBuffer[i];
            dSample -= 128.0;
            dSample /= 128.0;
            m_queueSamples.push(dSample);
        }

        ltc_encoder_inc_timecode(m_pEncoder);
    }
    else
    {
        for(int i = 0; i < 1024;i++)
        {
            m_queueSamples.push(0.0);
        }
    }
}

wxWindow* LTCGeneratorBuilder::CreateGeneratorPanel(wxWindow* pParent)
{
	m_pMeter = new LTCGeneratorPanel(pParent, this);
	return m_pMeter;

}


void LTCGeneratorBuilder::LoadSettings()
{
	//@todo load any settings
}



void LTCGeneratorBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
    Init();
}


void LTCGeneratorBuilder::Init()
{
    if(m_pEncoder)
    {
        Stop();

    }

    wxDateTime dtStart;

    switch(ReadSetting(wxT("Time"), 0))
    {
        case LIVE:
            dtStart = wxDateTime::UNow();
            break;
        case OFFSET:
            {
                dtStart = wxDateTime::UNow();
                wxDateSpan dsOffset(ReadSetting(wxT("OffsetYear"), 0),ReadSetting(wxT("OffsetMonth"), 0),0,ReadSetting(wxT("OffsetDay"), 0));
                wxTimeSpan  tsOffset(ReadSetting(wxT("OffsetHour"), 0),ReadSetting(wxT("OffsetMinute"), 0),ReadSetting(wxT("OffsetSecond"),0));
                dtStart += dsOffset;
                dtStart += tsOffset;
            }
            break;
        case ABS:
            dtStart = wxDateTime(min(31, max(1,ReadSetting(wxT("AbsDay"), 1))), wxDateTime::Month(max(0, ReadSetting(wxT("AbsMonth"), 1)-1)), ReadSetting(wxT("AbsYear"), 2018), ReadSetting(wxT("AbsHour"), 0),ReadSetting(wxT("AbsMinute"), 0),ReadSetting(wxT("AbsSecond"),0));
            break;

    }


    double dFPS = ReadSetting(wxT("FPS"), 25.0);
    double dSampleRate = ReadSetting(wxT("SampleRate"), 48000.0);
    double dAmplitude = ReadSetting(wxT("Amplitude"), -18.0);

	int total = 0;


	SMPTETimecode stime;

	const char timezone[6] = "+000";//ReadSetting(wxT("TimeZone"), wxT("+000")).c_str();

	strcpy(stime.timezone, timezone);




	stime.years = dtStart.GetYear();
	if(stime.years > 2000 && stime.years < 2067)
    {
        stime.years-=2000;
    }
    else
    {
        stime.years-=1900;
    }
	stime.months = dtStart.GetMonth()+1;
	stime.days = dtStart.GetDay();
    stime.hours = dtStart.GetHour();
    stime.mins = dtStart.GetMinute();
    stime.secs = dtStart.GetSecond();
    stime.frame = round((dFPS/1000.0)* static_cast<double>(dtStart.GetMillisecond()));

    //@todo type of date


	m_pEncoder = ltc_encoder_create(1, 1,  dFPS==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);
	ltc_encoder_set_bufsize(m_pEncoder, dSampleRate, dFPS);

	ltc_encoder_reinit(m_pEncoder, dSampleRate, dFPS, dFPS==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);

	ltc_encoder_set_filter(m_pEncoder, 0);
	ltc_encoder_set_volume(m_pEncoder, dAmplitude);

	ltc_encoder_set_timecode(m_pEncoder, &stime);




}

void LTCGeneratorBuilder::Stop()
{
    if(m_pEncoder)
    {
        ltc_encoder_free(m_pEncoder);
    }
    m_pEncoder = 0;
}
