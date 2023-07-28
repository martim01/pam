#include "smpte2110mediasession.h"
#include "UsageEnvironment.hh"
#include "rawvideosource.h"
#include "aes67source.h"
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <cmath>
#include "timedbuffer.h"
#include <wx/log.h>
#include <iostream>
#include "wxptp.h"
#include "log.h"
#include "rtsputil.h"
#include "threadpool.h"

Smpte2110MediaSession* Smpte2110MediaSession::createNew(UsageEnvironment& env,
        char const* sdpDescription)
{
    auto newSession = new Smpte2110MediaSession(env);
    if (newSession != nullptr)
    {
        newSession->initializeSMPTE_SDP(sdpDescription);
        if (!newSession->initializeWithSDP(sdpDescription))
        {
            delete newSession;
            return nullptr;
        }

    }
    return newSession;
}

void Smpte2110MediaSession::initializeSMPTE_SDP(char const* sdpDescription)
{
    if (sdpDescription == nullptr)
    {
        return;
    }

    m_sRawSDP = wxString::FromUTF8(sdpDescription);

    // Begin by processing all SDP lines until we see the first "m="
    char const* sdpLine = sdpDescription;
    char const* nextSDPLine;
    while (true)
    {
        if (!parseSDPLine(sdpLine, nextSDPLine))
        {
            return;
        }
        if (sdpLine[0] == 'm')
        {
            break;
        }
        sdpLine = nextSDPLine;
        if (sdpLine == NULL)
        {
            break; // there are no m= lines at all
        }

        // Check for various special SMPTE 2110 and AES56 and Ravenna SDP lines that we understand:
        if (parseSDPAttribute_RefClk(sdpLine)) continue;
        if (parseSDPAttribute_ClockDomain(sdpLine)) continue;
        if (parseSDPAttribute_PTime(sdpLine)) continue;
        if (parseSDPAttribute_MaxPTime(sdpLine)) continue;
        if (parseSDPAttribute_Group(sdpLine)) continue;
    }

}


Boolean Smpte2110MediaSession::parseSDPAttribute_RefClk(char const* sdpLine)
{
    wxString sSdp(wxString::FromUTF8(sdpLine));

    wxString sFind(wxT("a=ts-refclk:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        if(nFront+sFind.length() < (nEnd-sFind.length()))
        {
            wxString sLine = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
            m_refclk.sType = sLine.BeforeFirst(wxT('='));


            if(m_refclk.sType.CmpNoCase(wxT("ntp")) == 0)
            {
                m_refclk.sId = sLine.AfterFirst(wxT('='));
            }
            else if(m_refclk.sType.CmpNoCase(wxT("localmac")) == 0)
            {
                m_refclk.sId = sLine.AfterFirst(wxT('='));
            }
            else if(m_refclk.sType.CmpNoCase(wxT("ptp")) == 0)
            {
                wxString sDetails = sLine.AfterFirst(wxT('='));
                if(sDetails.CmpNoCase(wxT("traceable")) == 0)
                {
                    m_refclk.sId = sDetails;
                }
                else
                {
                    wxArrayString asDetails(wxStringTokenize(sDetails, wxT(":")));
                    if(asDetails.Count() >= 3)
                    {
                        asDetails[2].ToULong(&m_refclk.nDomain);
                    }
                    if(asDetails.Count() >= 2)
                    {
                        m_refclk.sId = asDetails[1];
                    }
                    if(asDetails.Count() >= 1)
                    {
                        m_refclk.sVersion = asDetails[0];
                    }
                }
            }
        }
        return True;
    }
    return False;
}


Boolean Smpte2110MediaSession::parseSDPAttribute_ClockDomain(char const* sdpLine)
{
    wxString sSdp(wxString::FromUTF8(sdpLine));

    wxString sFind(wxT("a=clock-domain:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sLine = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));


        if(sLine.BeforeFirst(wxT(' ')).CmpNoCase(wxT("PTPV2")) == 0)
        {
            m_refclk.sType = wxT("PTP");
            m_refclk.sVersion = wxT("IEEE1588-2008");

            wxString sDomain(sLine.AfterFirst(wxT(' ')));
            if(sDomain != wxEmptyString)
            {
                sDomain.ToULong(&m_refclk.nDomain);
            }
        }
        return True;
    }
    return False;
}

Boolean Smpte2110MediaSession::parseSDPAttribute_Group(char const* sdpLine)
{
    wxString sSdp(wxString::FromUTF8(sdpLine));

    wxString sFind(wxT("a=group:DUP "));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        m_setGroups.clear();
        
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        auto asGroups = wxStringTokenize(sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length())), " ");
        for(const auto& sGroup : asGroups)
        {
            m_setGroups.insert(sGroup);
        }
        return True;
    }

    m_setGroups = {"0"};

    return False;
}

Boolean Smpte2110MediaSession::parseSDPAttribute_PTime(char const* sdpLine)
{
    wxString sSdp(wxString::FromUTF8(sdpLine));

    wxString sFind(wxT("a=ptime:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length())).ToDouble(&m_dPackageMs);
        return True;
    }
    return False;
}
Boolean Smpte2110MediaSession::parseSDPAttribute_MaxPTime(char const* sdpLine)
{
    wxString sSdp(wxString::FromUTF8(sdpLine));

    wxString sFind(wxT("a=maxptime:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length())).ToDouble(&m_dMaxPackageMs);
        return True;
    }
    return False;
}


Smpte2110MediaSession::Smpte2110MediaSession(UsageEnvironment& env) : MediaSession(env)
{
    m_setGroups = {"0"};
}




MediaSubsession* Smpte2110MediaSession::createNewMediaSubsession()
{
    return new Smpte2110MediaSubsession(*this);
}


Smpte2110MediaSubsession::Smpte2110MediaSubsession(MediaSession& parent) : MediaSubsession(parent), 
m_sGroup("0"),
m_qos(this)
{

}

wxString Smpte2110MediaSubsession::GetEndpoint()
{
    return m_sEndpoint;
}

Boolean Smpte2110MediaSubsession::createSourceObjects(int useSpecialRTPoffset)
{

    parseSDPAttribute_Sync();           //Sync time Smpte2110 and Ravenna and SMPTE2110
    parseSDPAttribute_Deviation();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_RefClk();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_PTime();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_MaxPTime();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_ExtMap();    //Header extension mapping
    parseSDPAttribute_Mid();        //Group if any


    if (strcmp(fCodecName, "L16") == 0 || strcmp(fCodecName, "L24") == 0 || strcmp(fCodecName, "AM824") == 0) // 16 or 24-bit linear audio (RFC 3190) or 2110-31
    {
        m_sEndpoint = wxString::FromUTF8(fConnectionEndpointName);

        auto mimeType = new char[strlen(mediumName()) + strlen(codecName()) + 2] ;
        sprintf(mimeType, "%s/%s", mediumName(), codecName());
        fReadSource = fRTPSource = Aes67Source::createNew(env(), fRTPSocket, fRTPPayloadFormat, fRTPTimestampFrequency, mimeType, 0,FALSE, m_nSyncTime);
        delete[] mimeType;
        
        pmlLog() << "-------------- " << fNumChannels << "----------------------";
        m_channels.resize(fNumChannels);
        parseSDPAttribute_Channels();    //Channel mapping if any

        return TRUE;
    }
    else
    {
        env().setResultMsg("RTP payload format unknown or not supported");
    }

    return False; // an error occurred
}


void Smpte2110MediaSubsession::parseSDPAttribute_Sync()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=sync-time:"));
    size_t nFront = sSdp.find(sFind);
    size_t nDante = sSdp.Find(wxT("Dante"));

    m_nSyncTime = 0;

    if(nFront == wxNOT_FOUND)   //not found try the mediaclk:direct
    {
        sFind = "a=mediaclk:direct=";
        nFront = sSdp.find(sFind);
    }


    if(nFront != wxNOT_FOUND && nDante == wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sTime = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
        if(sTime.Find(" ") != wxNOT_FOUND)
        {
            //might possibly have clock rate after sync time. Ignore for now
            sTime = sTime.BeforeFirst(' ');
        }
        sTime.ToULong(&m_nSyncTime);

    }

}

void Smpte2110MediaSubsession::parseSDPAttribute_Deviation()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=clock-deviation:"));
    size_t nFront = sSdp.find(sFind);

    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sDeviation = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
        double dNumerator, dDenominator;
        sDeviation.BeforeFirst(wxT('/')).ToDouble(&dNumerator);
        sDeviation.AfterFirst(wxT('/')).ToDouble(&dDenominator);

        m_dClockDeviation = dNumerator/dDenominator;

    }
    else
    {
        m_dClockDeviation = 1.0;
    }
}

timeval Smpte2110MediaSubsession::GetLastEpoch()
{
    if(dynamic_cast<RawVideoSource*>(rtpSource()))
    {
        return dynamic_cast<RawVideoSource*>(rtpSource())->GetLastEpoch();
    }
    else if(dynamic_cast<Aes67Source*>(rtpSource()))
    {
        return dynamic_cast<Aes67Source*>(rtpSource())->GetLastEpoch();
    }
    return {0,0};
}



void Smpte2110MediaSubsession::parseSDPAttribute_RefClk()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=ts-refclk:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sLine = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
        m_refclk.sType = sLine.BeforeFirst(wxT('='));


        if(m_refclk.sType.CmpNoCase(wxT("ntp")) == 0)
        {
            m_refclk.sId = sLine.AfterFirst(wxT('='));
        }
        else if(m_refclk.sType.CmpNoCase(wxT("localmac")) == 0)
        {
            m_refclk.sId = sLine.AfterFirst(wxT('='));
        }
        else if(m_refclk.sType.CmpNoCase(wxT("ptp")) == 0)
        {
            wxString sDetails = sLine.AfterFirst(wxT('='));
            if(sDetails.CmpNoCase(wxT("traceable")) == 0)
            {
                m_refclk.sId = sDetails;
            }
            else
            {
                wxArrayString asDetails(wxStringTokenize(sDetails, wxT(":")));
                if(asDetails.Count() >= 3)
                {
                    asDetails[2].ToULong(&m_refclk.nDomain);
                }
                if(asDetails.Count() >= 2)
                {
                    m_refclk.sId = asDetails[1];
                }
                if(asDetails.Count() >= 1)
                {
                    m_refclk.sVersion = asDetails[0];
                }
            }
        }
    }
    else    //use the parent session
    {
        parseSDPAttribute_ClockDomain();
    }
}

void Smpte2110MediaSubsession::parseSDPAttribute_ClockDomain()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=clock-domain:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sLine = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));


        if(sLine.BeforeFirst(wxT(' ')).CmpNoCase(wxT("PTPV2")) == 0)
        {
            m_refclk.sType = wxT("PTP");
            m_refclk.sVersion = wxT("IEEE1588-2008");

            wxString sDomain(sLine.AfterFirst(wxT(' ')));
            if(sDomain != wxEmptyString)
            {
                sDomain.ToULong(&m_refclk.nDomain);
            }
        }
    }
    else    //use the parent session
    {
        Smpte2110MediaSession* pParent = dynamic_cast<Smpte2110MediaSession*>(&fParent);
        if(pParent)
        {
            m_refclk = pParent->GetRefClock();
        }
    }
}

void Smpte2110MediaSubsession::parseSDPAttribute_PTime()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=ptime:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length())).ToDouble(&m_dPackageMs);
    }
    else
    {
        Smpte2110MediaSession* pParent = dynamic_cast<Smpte2110MediaSession*>(&fParent);
        if(pParent)
        {
            m_dPackageMs = pParent->GetPackageTime();
        }
    }
}

void Smpte2110MediaSubsession::parseSDPAttribute_MaxPTime()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=maxptime:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length())).ToDouble(&m_dMaxPackageMs);

    }
    else
    {
        Smpte2110MediaSession* pParent = dynamic_cast<Smpte2110MediaSession*>(&fParent);
        if(pParent)
        {
            m_dMaxPackageMs = pParent->GetMaxPackageTime();
        }
    }
}


void Smpte2110MediaSubsession::parseSDPAttribute_ExtMap()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=extmap:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sExt = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));

        unsigned long nId;
        sExt.BeforeFirst(wxT(' ')).ToULong(&nId);
        m_mExtHeader.insert({nId, sExt.AfterFirst(' ')});

    }
}

void Smpte2110MediaSubsession::parseSDPAttribute_Mid()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind(wxT("a=mid:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        m_sGroup = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
    }
}

std::map<unsigned long, wxString>::const_iterator Smpte2110MediaSubsession::GetExtHeaderBegin() const
{
    return m_mExtHeader.begin();
}

std::map<unsigned long, wxString>::const_iterator Smpte2110MediaSubsession::GetExtHeaderEnd() const
{
    return m_mExtHeader.end();
}

std::map<unsigned long, wxString>::const_iterator Smpte2110MediaSubsession::GetExtHeader(unsigned long nId) const
{
    return m_mExtHeader.find(nId);
}


 bool Smpte2110MediaSubsession::SetChannelGrouping(size_t& nChannel, const subsession::channelGrouping& grouping)
 {
    if(nChannel < m_channels.size())
    {
        m_channels[nChannel] = grouping;
        ++nChannel;
    }
    return nChannel < m_channels.size();
 }

void Smpte2110MediaSubsession::parseSDPAttribute_Channels()
{
    wxString sSdp(wxString::FromUTF8(fSavedSDPLines));

    wxString sFind("channel-order=SMPTE2110.");
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        wxString sChannels = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length())).AfterFirst('(').BeforeFirst(')');


        wxArrayString asChannels = wxStringTokenize(sChannels, ",");

        size_t nChannel(0);
        for(size_t i = 0; i < asChannels.GetCount(); ++i)
        {
            if(asChannels[i].CmpNoCase("M") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::M, subsession::enumChannel::MONO));

            }
            else if(asChannels[i].CmpNoCase("DM") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::DM, subsession::enumChannel::MONO_1));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::DM, subsession::enumChannel::MONO_2));
            }
            else if(asChannels[i].CmpNoCase("St") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::ST, subsession::enumChannel::LEFT));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::ST, subsession::enumChannel::RIGHT));
            }
            else if(asChannels[i].CmpNoCase("LtRt") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::LtRt, subsession::enumChannel::LEFT_TOTAL));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::LtRt, subsession::enumChannel::RIGHT_TOTAL));
            }
            else if(asChannels[i].CmpNoCase("51") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LEFT));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::RIGHT));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::CENTER));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LFE));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LEFT_SIDE));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::RIGHT_SIDE));
            }
            else if(asChannels[i].CmpNoCase("71") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::CENTER));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LFE));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT_SIDE));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT_SIDE));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT_REAR_SIDE));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT_REAR_SIDE));
            }
            else if(asChannels[i].CmpNoCase("SGRP") == 0)
            {
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_1));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_2));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_3));
                SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_4));
            }
            else if(asChannels[i].GetChar(0) == 'U' || asChannels[i].GetChar(0) == 'u')
            {
                unsigned long nUndefined;
                asChannels[i].Mid(1).ToULong(&nUndefined);

                for(size_t j = nChannel; j < nUndefined; j++)
                {
                    SetChannelGrouping(nChannel, subsession::channelGrouping(i, subsession::enumChannelGrouping::U01+(nUndefined-1), subsession::enumChannel::UNDEFINED_1+j));
                }
            }
        }
    }
}


static void periodicQOSMeasurement(void* clientData)
{
    auto pSubsession = reinterpret_cast<Smpte2110MediaSubsession*>(clientData);
    pSubsession->PeriodicQOSMeasurement();
}


void Smpte2110MediaSubsession::PeriodicQOSMeasurement()
{
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);

    m_qos.periodicQOSMeasurement(timeNow);

    // Do this again later:
    ScheduleNextQOSMeasurement();    
}

void Smpte2110MediaSubsession::ScheduleNextQOSMeasurement()
{
    nextQOSMeasurementUSecs += m_qos.GetQosMeasurementIntervalMS()*1000;

    int usecsToDelay = m_qos.GetQosMeasurementIntervalMS()*1000;
    m_qosMeasurementTimerTask = env().taskScheduler().scheduleDelayedTask(usecsToDelay, (TaskFunc*)periodicQOSMeasurement, (void*)this);
    if(m_qosMeasurementTimerTask == nullptr)
    {
        pmlLog(pml::LOG_ERROR) << "Smpte2110MediaSubsession::ScheduleNextQOSMeasurement - FAILED";
    }    
    
}
