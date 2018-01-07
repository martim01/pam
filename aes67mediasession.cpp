#include "aes67mediasession.h"
#include "UsageEnvironment.hh"
#include "aes67source.h"
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <cmath>
#include "timedbuffer.h"
#include <wx/log.h>

using namespace std;

Aes67MediaSession* Aes67MediaSession::createNew(UsageEnvironment& env,
        char const* sdpDescription)
{
    Aes67MediaSession* newSession = new Aes67MediaSession(env);
    if (newSession != NULL)
    {

        newSession->initializeSMPTE_SDP(sdpDescription);
        if (!newSession->initializeWithSDP(sdpDescription))
        {
            delete newSession;
            return NULL;
        }

    }

    return newSession;
}

void Aes67MediaSession::initializeSMPTE_SDP(char const* sdpDescription)
{
    if (sdpDescription == NULL)
    {
        return;
    }

    m_sRawSDP = wxString::FromAscii(sdpDescription);

    // Begin by processing all SDP lines until we see the first "m="
    char const* sdpLine = sdpDescription;
    char const* nextSDPLine;
    while (1)
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


Boolean Aes67MediaSession::parseSDPAttribute_RefClk(char const* sdpLine)
{
    wxString sSdp(wxString::FromAscii(sdpLine));

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
        else if(m_refclk.sType.CmpNoCase(wxT("ntp")) == 0)
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
}


Boolean Aes67MediaSession::parseSDPAttribute_ClockDomain(char const* sdpLine)
{
    wxString sSdp(wxString::FromAscii(sdpLine));

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

Boolean Aes67MediaSession::parseSDPAttribute_Group(char const* sdpLine)
{
    wxString sSdp(wxString::FromAscii(sdpLine));

    wxString sFind(wxT("a=group:DUP "));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        m_sGroups = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
        return True;
    }
    return False;
}

Boolean Aes67MediaSession::parseSDPAttribute_PTime(char const* sdpLine)
{
    wxString sSdp(wxString::FromAscii(sdpLine));

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
Boolean Aes67MediaSession::parseSDPAttribute_MaxPTime(char const* sdpLine)
{
    wxString sSdp(wxString::FromAscii(sdpLine));

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


Aes67MediaSession::Aes67MediaSession(UsageEnvironment& env) : MediaSession(env)
{

}




MediaSubsession* Aes67MediaSession::createNewMediaSubsession()
{
    return new Aes67MediaSubsession(*this);
}


Aes67MediaSubsession::Aes67MediaSubsession(MediaSession& parent) : MediaSubsession(parent)
{

}

wxString Aes67MediaSubsession::GetEndpoint()
{
    return m_sEndpoint;
}

Boolean Aes67MediaSubsession::createSourceObjects(int useSpecialRTPoffset)
{
    parseSDPAttribute_Sync();           //Sync time AES67 and Ravenna and SMPTE2110
    parseSDPAttribute_Deviation();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_RefClk();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_PTime();      //Clock deviation sample rate Ravenna
    parseSDPAttribute_MaxPTime();      //Clock deviation sample rate Ravenna
    void parseSDPAttribute_ExtMap();    //Header extension mapping

    if (strcmp(fCodecName, "L16") == 0 || strcmp(fCodecName, "L24") == 0) // 16 or 24-bit linear audio (RFC 3190)
    {
        m_sEndpoint = wxString::FromAscii(fConnectionEndpointName);

        char* mimeType = new char[strlen(mediumName()) + strlen(codecName()) + 2] ;
        sprintf(mimeType, "%s/%s", mediumName(), codecName());
        fReadSource = fRTPSource = Aes67Source::createNew(env(), fRTPSocket, fRTPPayloadFormat, fRTPTimestampFrequency, mimeType, 0,FALSE, m_nSyncTime);
        delete[] mimeType;

        return TRUE;
    }
    else
    {
        env().setResultMsg("RTP payload format unknown or not supported");
    }

    return False; // an error occurred
}


void Aes67MediaSubsession::parseSDPAttribute_Sync()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

    wxString sFind(wxT("a=sync-time:"));
    size_t nFront = sSdp.find(sFind);

    if(nFront == wxNOT_FOUND)   //not found try the mediaclk:direct
    {
        sFind = wxT("a=mediaclk:direct=");
        nFront = sSdp.find(sFind);
    }

    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sTime = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));

        //might possibly have clock rate after sync time. Ignore for now
        sTime.BeforeFirst(wxT(' ')).ToULong(&m_nSyncTime);
    }
    else
    {
        m_nSyncTime = 0;
    }
}

void Aes67MediaSubsession::parseSDPAttribute_Deviation()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

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

const pairTime_t& Aes67MediaSubsession::GetLastEpoch()
{
    Aes67Source* pSource = dynamic_cast<Aes67Source*>(rtpSource());
    return pSource->GetLastEpoch();
}



void Aes67MediaSubsession::parseSDPAttribute_RefClk()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

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
        else if(m_refclk.sType.CmpNoCase(wxT("ntp")) == 0)
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

void Aes67MediaSubsession::parseSDPAttribute_ClockDomain()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

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
        Aes67MediaSession* pParent = dynamic_cast<Aes67MediaSession*>(&fParent);
        if(pParent)
        {
            m_refclk = pParent->GetRefClock();
        }
    }
}

void Aes67MediaSubsession::parseSDPAttribute_PTime()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

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
        Aes67MediaSession* pParent = dynamic_cast<Aes67MediaSession*>(&fParent);
        if(pParent)
        {
            m_dPackageMs = pParent->GetPackageTime();
        }
    }
}

void Aes67MediaSubsession::parseSDPAttribute_MaxPTime()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

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
        Aes67MediaSession* pParent = dynamic_cast<Aes67MediaSession*>(&fParent);
        if(pParent)
        {
            m_dMaxPackageMs = pParent->GetMaxPackageTime();
        }
    }
}


void Aes67MediaSubsession::parseSDPAttribute_ExtMap()
{
    wxString sSdp(wxString::FromAscii(fSavedSDPLines));

    wxString sFind(wxT("a=extmap:"));
    size_t nFront = sSdp.find(sFind);
    if(nFront != wxNOT_FOUND)
    {
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        wxString sExt = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));

        unsigned long nId;
        sExt.BeforeFirst(wxT(' ')).ToULong(&nId);
        m_mExtHeader.insert(make_pair(nId, sExt.AfterFirst(wxT(' '))));

    }
}


map<unsigned long, wxString>::const_iterator Aes67MediaSubsession::GetExtHeaderBegin() const
{
    return m_mExtHeader.begin();
}

map<unsigned long, wxString>::const_iterator Aes67MediaSubsession::GetExtHeaderEnd() const
{
    return m_mExtHeader.end();
}

map<unsigned long, wxString>::const_iterator Aes67MediaSubsession::GetExtHeader(unsigned long nId) const
{
    return m_mExtHeader.find(nId);
}
