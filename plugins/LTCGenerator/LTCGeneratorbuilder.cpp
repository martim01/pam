#include "LTCGeneratorbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "LTCGeneratorpanel.h"
#include <wx/datetime.h>
#include <wx/log.h>
#include <cmath>
#include <bitset>
#include "ltc.h"

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
        m_nFrame++;
        if(m_nFrame == round(m_dFPS))
        {
            m_nFrame = 0;
            m_dtFrame+=wxTimeSpan(0,0,1,0);
        }
        SetFrameTime();
//        ltc_encoder_inc_timecode(m_pEncoder);
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


    switch(ReadSetting(wxT("Time"), 0))
    {
        case LIVE:
            m_dtFrame = wxDateTime::UNow();
            break;
        case OFFSET:
            {
                m_dtFrame = wxDateTime::UNow();
                wxDateSpan dsOffset(ReadSetting(wxT("OffsetYear"), 0),ReadSetting(wxT("OffsetMonth"), 0),0,ReadSetting(wxT("OffsetDay"), 0));
                wxTimeSpan  tsOffset(ReadSetting(wxT("OffsetHour"), 0),ReadSetting(wxT("OffsetMinute"), 0),ReadSetting(wxT("OffsetSecond"),0));
                m_dtFrame += dsOffset;
                m_dtFrame += tsOffset;
            }
            break;
        case ABS:
            m_dtFrame = wxDateTime(min(31, max(1,ReadSetting(wxT("AbsDay"), 1))), wxDateTime::Month(max(0, ReadSetting(wxT("AbsMonth"), 1)-1)), ReadSetting(wxT("AbsYear"), 2018), ReadSetting(wxT("AbsHour"), 0),ReadSetting(wxT("AbsMinute"), 0),ReadSetting(wxT("AbsSecond"),0));
            break;

    }

    m_nDateType = ReadSetting(wxT("DateFormat"),0);

    m_dFPS = ReadSetting(wxT("FPS"), 25.0);
    double dSampleRate = ReadSetting(wxT("SampleRate"), 48000.0);
    double dAmplitude = ReadSetting(wxT("Amplitude"), -18.0);


	int total = 0;


	m_pEncoder = ltc_encoder_create(1, 1,  static_cast<int>(round(m_dFPS))==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);
	ltc_encoder_set_bufsize(m_pEncoder, dSampleRate, m_dFPS);
	ltc_encoder_reinit(m_pEncoder, dSampleRate, m_dFPS, static_cast<int>(round(m_dFPS))==25?LTC_TV_625_50:LTC_TV_525_60, LTC_USE_DATE);
	ltc_encoder_set_filter(m_pEncoder, 0);
	ltc_encoder_set_volume(m_pEncoder, dAmplitude);

    m_nFrame = round((m_dFPS/1000.0)* static_cast<double>(m_dtFrame.GetMillisecond()));
	SetFrameTime();



}

void LTCGeneratorBuilder::Stop()
{
    if(m_pEncoder)
    {
        ltc_encoder_free(m_pEncoder);
    }
    m_pEncoder = 0;
}


void LTCGeneratorBuilder::SetFrameTime()
{
    SMPTETimecode stime;

	const char timezone[6] = "+000";//ReadSetting(wxT("TimeZone"), wxT("+000")).c_str();
	strcpy(stime.timezone, timezone);

	stime.years = m_dtFrame.GetYear();
	if(stime.years > 2000 && stime.years < 2067)
    {
        stime.years-=2000;
    }
    else
    {
        stime.years-=1900;
    }
	stime.months = m_dtFrame.GetMonth()+1;
	stime.days = m_dtFrame.GetDay();
    stime.hours = m_dtFrame.GetHour();
    stime.mins = m_dtFrame.GetMinute();
    stime.secs = m_dtFrame.GetSecond();
    stime.frame = m_nFrame;

    ltc_encoder_set_timecode(m_pEncoder, &stime);

    //@todo type of date
    switch(m_nDateType)
    {
        case BBC:
            CreateBBCTime();
            break;
        case TVE:
            CreateTVETime();
            break;
        case MTD:
            CreateMTDTime();
            break;
    }
}


void LTCGeneratorBuilder::CreateBBCTime()
{
    int nYears = m_dtFrame.GetYear();
	if(nYears > 2000 && nYears < 2067)
    {
        nYears-=2000;
    }
    else
    {
        nYears-=1900;
    }

    unsigned int nBits(0);
    unsigned int nYear10 = nYears/10;
    unsigned int nYear = nYears%10;
    unsigned int nMonth10 = (m_dtFrame.GetMonth()+1)/10;
    unsigned int nDay10 = (m_dtFrame.GetDay())/10;
    unsigned int nMonth = (m_dtFrame.GetMonth()+1)%10;
    unsigned int nDay = (m_dtFrame.GetDay())%10;

    nBits = nYear10 << 28;
    nBits+= nYear << 20;
    nBits+= nMonth10 << 14;
    nBits+= nDay10 << 12;
    nBits+= nMonth << 8;
    nBits+= nDay << 4;

    ltc_encoder_set_user_bits(m_pEncoder, nBits);
}

void LTCGeneratorBuilder::CreateTVETime()
{
    int nYears = m_dtFrame.GetYear();
	if(nYears > 2000 && nYears < 2067)
    {
        nYears-=2000;
    }
    else
    {
        nYears-=1900;
    }

    unsigned int nYear10 = nYears/10;
    unsigned int nYear = nYears%10;
    unsigned int nMonth10 = (m_dtFrame.GetMonth()+1)/10;
    unsigned int nDay10 = (m_dtFrame.GetDay())/10;
    unsigned int nMonth = (m_dtFrame.GetMonth()+1)%10;
    unsigned int nDay = (m_dtFrame.GetDay())%10;


    unsigned int nBits(nDay << 4);
    nBits += nDay10 << 8;
    nBits += nMonth << 12;
    nBits += nMonth10 << 16;
    nBits += nYear << 20;
    nBits += nYear10 << 24;

    ltc_encoder_set_user_bits(m_pEncoder, nBits);
}

void LTCGeneratorBuilder::CreateMTDTime()
{
    int nYears = m_dtFrame.GetYear();
	if(nYears > 2000 && nYears < 2067)
    {
        nYears-=2000;
    }
    else
    {
        nYears-=1900;
    }

    unsigned int nYear10 = nYears/10;
    unsigned int nYear = nYears%10;
    unsigned int nMonth10 = (m_dtFrame.GetMonth()+1)/10;
    unsigned int nDay10 = (m_dtFrame.GetDay())/10;
    unsigned int nMonth = (m_dtFrame.GetMonth()+1)%10;
    unsigned int nDay = (m_dtFrame.GetDay())%10;



    unsigned int nBits(nDay << 4);
    nBits += nDay10 << 8;
    nBits += nMonth << 12;
    nBits += nMonth10 << 16;
    nBits += nYear << 20;
    nBits += nYear10 << 24;

    ltc_encoder_set_user_bits(m_pEncoder, nBits);
}
