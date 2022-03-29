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


using namespace std;

const wxString Smpte2110MediaSubsession::STR_SAMPLING[13] = { wxT("YCbCr-4:4:4"), wxT("YCbCr-4:2:2"), wxT("YCbCr-4:2:0"), wxT("CLYCbCr-4:4:4"), wxT("CLYCbCr-4:2:2"), wxT("CLYCbCr-4:2:0"), wxT("ICtCp-4:4:4"), wxT("ICtCp-4:2:2"), wxT("ICtCp-4:2:0"), wxT("RGB"), wxT("XYZ"), wxT("KEY"), wxT("Unknown")};
const wxString Smpte2110MediaSubsession::STR_COLORIMETRY[8] = {wxT("BT601"), wxT("BT709"), wxT("BT2020"), wxT("BT2100"), wxT("ST2065-1"), wxT("ST2065-3"), wxT("XYZ"), wxT("Unspecified")};
const wxString Smpte2110MediaSubsession::STR_TCS[10] = {wxT("SDR"), wxT("PQ"), wxT("HLG"), wxT("LINEAR"), wxT("BT2100LINPQ"), wxT("BT2100LINHLG"), wxT("ST2065-1"), wxT("ST2428-1"), wxT("DENSITY"), wxT("Unspecified")};
const wxString Smpte2110MediaSubsession::STR_RANGE[3] = {wxT("Narrow"), wxT("FullProtect"), wxT("Full")};
const wxString Smpte2110MediaSubsession::STR_PACKING[2] = {wxT("General"), wxT("Block")};


Smpte2110MediaSession* Smpte2110MediaSession::createNew(UsageEnvironment& env,
        char const* sdpDescription)
{
    Smpte2110MediaSession* newSession = new Smpte2110MediaSession(env);
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

void Smpte2110MediaSession::initializeSMPTE_SDP(char const* sdpDescription)
{
    if (sdpDescription == NULL)
    {
        return;
    }

    m_sRawSDP = wxString::FromUTF8(sdpDescription);

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
        size_t nEnd = sSdp.find(wxT("\n"), nFront);
        nEnd -= nFront;
        m_sGroups = sSdp.substr(nFront+sFind.length(), (nEnd-sFind.length()));
        return True;
    }
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


Smpte2110MediaSession::Smpte2110MediaSession(UsageEnvironment& env) : MediaSession(env),
 m_dPackageMs(0),
 m_dMaxPackageMs(0)
{

}




MediaSubsession* Smpte2110MediaSession::createNewMediaSubsession()
{
    return new Smpte2110MediaSubsession(*this);
}


Smpte2110MediaSubsession::Smpte2110MediaSubsession(MediaSession& parent) : MediaSubsession(parent),
 m_nSyncTime(0),
 m_nFirstTimestamp(0),
 m_dClockDeviation(0),
 m_dPackageMs(0),
 m_dMaxPackageMs(0),
 m_nSampling(0),
 m_nDepth(0),
 m_bFloating(false),
 m_nWidth(0),
 m_nHeight(0),
 m_nColorimetry(0),
 m_nPackingMode(0),
 m_bInterlaced(false),
 m_bSegmented(false),
 m_nTCS(0),
 m_nRange(0),
 m_bMaxUdp(false),
 m_channels(8)
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

    if (strcmp(fCodecName, "L16") == 0 || strcmp(fCodecName, "L24") == 0) // 16 or 24-bit linear audio (RFC 3190)
    {
        m_sEndpoint = wxString::FromUTF8(fConnectionEndpointName);

        char* mimeType = new char[strlen(mediumName()) + strlen(codecName()) + 2] ;
        sprintf(mimeType, "%s/%s", mediumName(), codecName());
        fReadSource = fRTPSource = Aes67Source::createNew(env(), fRTPSocket, fRTPPayloadFormat, fRTPTimestampFrequency, mimeType, 0,FALSE, m_nSyncTime);
        delete[] mimeType;

        parseSDPAttribute_Channels();    //Channel mapping if any

        return TRUE;
    }
    else if(strcmp(fCodecName,"RAW") == 0)
    {
        AnalyzeAttributes();

        m_sEndpoint = wxString::FromUTF8(fConnectionEndpointName);

        char* mimeType = new char[strlen(mediumName()) + strlen(codecName()) + 2] ;
        sprintf(mimeType, "%s/%s", mediumName(), codecName());
        fReadSource = fRTPSource = RawVideoSource::createNew(env(), fRTPSocket, fRTPPayloadFormat, fRTPTimestampFrequency, mimeType, 0,FALSE, m_nSyncTime);
        delete[] mimeType;

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
        m_mExtHeader.insert(make_pair(nId, sExt.AfterFirst(wxT(' '))));

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

map<unsigned long, wxString>::const_iterator Smpte2110MediaSubsession::GetExtHeaderBegin() const
{
    return m_mExtHeader.begin();
}

map<unsigned long, wxString>::const_iterator Smpte2110MediaSubsession::GetExtHeaderEnd() const
{
    return m_mExtHeader.end();
}

map<unsigned long, wxString>::const_iterator Smpte2110MediaSubsession::GetExtHeader(unsigned long nId) const
{
    return m_mExtHeader.find(nId);
}





int Smpte2110MediaSubsession::GetSampling()
{
    return m_nSampling;
}

unsigned char Smpte2110MediaSubsession::GetDepth()
{
    return m_nDepth;
}

bool Smpte2110MediaSubsession::AreSamplesInteger()
{
    return !m_bFloating;
}

unsigned int Smpte2110MediaSubsession::GetWidth()
{
    return m_nWidth;
}

unsigned int Smpte2110MediaSubsession::GetHeight()
{
    return m_nHeight;
}

pair<unsigned long, unsigned long> Smpte2110MediaSubsession::GetExactFrameRate()
{
    return m_pairFrameRate;
}

int Smpte2110MediaSubsession::GetColorimetry()
{
    return m_nColorimetry;
}

int Smpte2110MediaSubsession::GetPackingMode()
{
   return m_nPackingMode;
}

wxString Smpte2110MediaSubsession::GetSSN()
{
    return m_sSSN;
}

bool Smpte2110MediaSubsession::IsInterlaced()
{
    return m_bInterlaced;
}

bool Smpte2110MediaSubsession::IsSegmented()
{
    return m_bSegmented;
}

int Smpte2110MediaSubsession::GetTCS()
{
    return m_nTCS;
}

int Smpte2110MediaSubsession::GetRange()
{
    return m_nRange;
}

bool Smpte2110MediaSubsession::UseMaxUDP()
{
    return m_bMaxUdp;
}

pair<unsigned long, unsigned long> Smpte2110MediaSubsession::GetAspectRatio()
{
    return m_pairAspectRatio;
}


void Smpte2110MediaSubsession::AnalyzeAttributes()
{
    wxString str(wxString::FromUTF8(attrVal_str("sampling")));
    for(m_nSampling = 0; m_nSampling < 12; m_nSampling++)
    {
        if(str.CmpNoCase(STR_SAMPLING[m_nSampling]) == 0)
            break;
    }
    env() << "Sampling: " << STR_SAMPLING[m_nSampling] << "\n";

    str = wxString::FromUTF8(attrVal_str("depth"));
    if(str.CmpNoCase(wxT("16f")) == 0)
    {
        m_nDepth = 16;
        m_bFloating = true;
    }
    else
    {
        m_bFloating = false;
        unsigned long nChar;
        str.ToULong(&nChar);
        m_nDepth = nChar;
    }
    env() << "Depth: " << m_nDepth;
    if(m_bFloating)
    {
        env() << "f";
    }
    env() << "\n";

    m_nWidth = attrVal_int("width");
    m_nHeight = attrVal_int("height");

    env() << "Resolution: " << (int)m_nWidth << "x" << (int)m_nHeight << "\n";

    str = wxString::FromUTF8(attrVal_str("exactframerate"));
    str.BeforeFirst(wxT('/')).ToULong(&m_pairFrameRate.first);
    if(str.AfterFirst(wxT('/')) == wxEmptyString)
    {
        m_pairFrameRate.second = 1;
    }
    else
    {
        str.AfterFirst(wxT('/')).ToULong(&m_pairFrameRate.second);
    }
    env() << "Frame Rate: "   << (int)m_pairFrameRate.first << "/" << (int)m_pairFrameRate.second << "\n";

    str = wxString::FromUTF8(attrVal_str("colorimetry"));
    for(m_nColorimetry = 0; m_nColorimetry < 7; m_nColorimetry++)
    {
        if(str.CmpNoCase(STR_COLORIMETRY[m_nColorimetry]) == 0)
            break;
    }

    env() << "Colorimetry:" << STR_COLORIMETRY[m_nColorimetry] << "\n";

    str = wxString::FromUTF8(attrVal_str("pm"));
    if(str.CmpNoCase(wxT("2110GPM")) == 0)
    {
        m_nPackingMode = GPM;
    }
    else
    {
        m_nPackingMode = BPM;
    }
    env() << "Packing:" << STR_PACKING[m_nPackingMode] << "\n";

    m_sSSN = wxString::FromUTF8(attrVal_str("ssn"));
    env() << "SSN:" << m_sSSN << "\n";

    m_bInterlaced = (fAttributeTable->Lookup("interlace") != NULL);
    m_bSegmented = (fAttributeTable->Lookup("segmented") != NULL);
    m_bMaxUdp = (fAttributeTable->Lookup("maxudp") != NULL);


    str = wxString::FromUTF8(attrVal_str("tcs"));
    for(m_nTCS = 0; m_nTCS < 9; m_nTCS++)
    {
        if(str.CmpNoCase(STR_TCS[m_nTCS]) == 0)
            break;
    }
    env() << "TCS:" << STR_TCS[m_nTCS] << "(" << str << ")\n";

    str = wxString::FromUTF8(attrVal_str("range"));
    for(m_nRange = 0; m_nRange < 3; m_nRange++)
    {
        if(str.CmpNoCase(STR_RANGE[m_nRange]) == 0)
            break;
    }
    if(m_nRange == 3)
    {
        m_nRange = NARROW;
    }

    env() << "RANGE:" << STR_RANGE[m_nRange] << "\n";


    str = wxString::FromUTF8(attrVal_str("par"));
    if(str != wxEmptyString)
    {
        str.BeforeFirst(wxT(':')).ToULong(&m_pairAspectRatio.first);
        if(str.AfterFirst(wxT(':')) == wxEmptyString)
        {
            m_pairAspectRatio.second = 1;
        }
        else
        {
            str.AfterFirst(wxT(':')).ToULong(&m_pairAspectRatio.second);
        }
    }
    else
    {
        m_pairAspectRatio.first = 1;
        m_pairAspectRatio.second = 1;
    }
    env() << "Pixel Aspect Radio: "   << (int)m_pairAspectRatio.first << ":" << (int)m_pairAspectRatio.second << "\n";

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

        unsigned long nChannel(0);
        for(size_t i = 0; i < asChannels.GetCount(); ++i)
        {
            if(asChannels[i].CmpNoCase("M") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::M, subsession::enumChannel::MONO);
                ++nChannel;
            }
            else if(asChannels[i].CmpNoCase("DM") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::DM, subsession::enumChannel::MONO_1);
                m_channels[nChannel+1] = subsession::channelGrouping(i, subsession::enumChannelGrouping::DM, subsession::enumChannel::MONO_2);

                nChannel+=2;
            }
            else if(asChannels[i].CmpNoCase("St") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::ST, subsession::enumChannel::LEFT);
                m_channels[nChannel+1] = subsession::channelGrouping(i, subsession::enumChannelGrouping::ST, subsession::enumChannel::RIGHT);

                nChannel+=2;
            }
            else if(asChannels[i].CmpNoCase("LtRt") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::LtRt, subsession::enumChannel::LEFT_TOTAL);
                m_channels[nChannel+1] = subsession::channelGrouping(i, subsession::enumChannelGrouping::LtRt, subsession::enumChannel::RIGHT_TOTAL);

                nChannel+=2;
            }
            else if(asChannels[i].CmpNoCase("51") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LEFT);
                m_channels[nChannel+1] = subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::RIGHT);
                m_channels[nChannel+2] = subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::CENTER);
                m_channels[nChannel+3] = subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LFE);
                m_channels[nChannel+4] = subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LEFT_SIDE);
                m_channels[nChannel+5] = subsession::channelGrouping(i, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::RIGHT_SIDE);

                nChannel+=6;
            }
            else if(asChannels[i].CmpNoCase("71") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT);
                m_channels[nChannel+1] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT);
                m_channels[nChannel+2] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::CENTER);
                m_channels[nChannel+3] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LFE);
                m_channels[nChannel+4] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT_SIDE);
                m_channels[nChannel+5] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT_SIDE);
                m_channels[nChannel+6] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT_REAR_SIDE);
                m_channels[nChannel+7] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT_REAR_SIDE);

                nChannel+=8;
            }
            else if(asChannels[i].CmpNoCase("SGRP") == 0)
            {
                m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_1);
                m_channels[nChannel+1] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_2);
                m_channels[nChannel+2] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_3);
                m_channels[nChannel+3] = subsession::channelGrouping(i, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_4);

                nChannel+=4;
            }
            else if(asChannels[i].GetChar(0) == 'U' || asChannels[i].GetChar(0) == 'u')
            {
                unsigned long nUndefined;
                asChannels[i].Mid(1).ToULong(&nUndefined);

                for(size_t j = 0; j < nUndefined; j++)
                {
                    if(j+nChannel < m_channels.size())
                    {
                        m_channels[nChannel] = subsession::channelGrouping(i, subsession::enumChannelGrouping::U01+(nUndefined-1), subsession::enumChannel::UNDEFINED_1+j);

                    }
                    else
                    {
                        break;
                    }
                }
                nChannel+=nUndefined;
            }
        }
    }
}


