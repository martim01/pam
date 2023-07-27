#include "pnlAoIPInfo.h"
#include "session.h"
#include "timedbuffer.h"
#include "settings.h"
#include "sdpelement.h"
#include <wx/log.h>
#ifdef PTPMONKEY
#include "wxptp.h"
#endif
#include "aoipinfobuilder.h"
#include "settings.h"
#include <wx/dcclient.h>
#include <iostream>
#include <memory>
//(*InternalHeaders(pnlAoIPInfo)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlAoIPInfo)
const long pnlAoIPInfo::ID_M_PLBL59 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL38 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL40 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL60 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL41 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL43 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL3 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL63 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL34 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL33 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL36 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL58 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL37 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL64 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL65 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL66 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL75 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL77 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL78 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL80 = wxNewId();
const long pnlAoIPInfo::pnlSessionInfo = wxNewId();
const long pnlAoIPInfo::ID_TEXTCTRL1 = wxNewId();
const long pnlAoIPInfo::ID_PANEL3 = wxNewId();
const long pnlAoIPInfo::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlAoIPInfo,wxPanel)
	//(*EventTable(pnlAoIPInfo)
	//*)
END_EVENT_TABLE()

pnlAoIPInfo::pnlAoIPInfo(wxWindow* parent,AoIPInfoBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder),
    m_pSession(0),
    m_nInitialLatencyCounter(0)
{
    //(*Initialize(pnlAoIPInfo)
    Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
    SetBackgroundColour(wxColour(0,0,0));
    m_pswpInfo = new wmSwitcherPanel(this, ID_M_PSWP1, wxDefaultPosition, wxSize(600,480), wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pswpInfo->SetPageNameStyle(2);
    m_pswpInfo->SetBackgroundColour(wxColour(0,0,0));
    pnlSession = new wxPanel(m_pswpInfo, pnlSessionInfo, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("pnlSessionInfo"));
    pnlSession->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl33 = new wmLabel(pnlSession, ID_M_PLBL59, _("Input"), wxPoint(5,10), wxSize(590,20), 0, _T("ID_M_PLBL59"));
    m_pLbl33->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl33->GetUiRect().SetGradient(0);
    m_pLbl33->SetForegroundColour(wxColour(255,255,255));
    m_pLbl33->SetBackgroundColour(wxColour(0,0,255));
    m_pLbl20 = new wmLabel(pnlSession, ID_M_PLBL38, _("Name"), wxPoint(5,70), wxSize(394,20), 0, _T("ID_M_PLBL38"));
    m_pLbl20->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl20->GetUiRect().SetGradient(0);
    m_pLbl20->SetForegroundColour(wxColour(255,255,255));
    m_pLbl20->SetBackgroundColour(wxColour(0,0,255));
    m_pLbl24 = new wmLabel(pnlSession, ID_M_PLBL40, _("Type"), wxPoint(400,70), wxSize(195,20), 0, _T("ID_M_PLBL40"));
    m_pLbl24->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl24->GetUiRect().SetGradient(0);
    m_pLbl24->SetForegroundColour(wxColour(255,255,255));
    m_pLbl24->SetBackgroundColour(wxColour(0,0,255));
    m_plblInput = new wmLabel(pnlSession, ID_M_PLBL60, wxEmptyString, wxPoint(5,31), wxSize(590,25), 0, _T("ID_M_PLBL60"));
    m_plblInput->SetBorderState(uiRect::BORDER_NONE);
    m_plblInput->GetUiRect().SetGradient(0);
    m_plblInput->SetForegroundColour(wxColour(0,128,0));
    m_plblInput->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblInputFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblInput->SetFont(m_plblInputFont);
    m_plblSessionName = new wmLabel(pnlSession, ID_M_PLBL41, wxEmptyString, wxPoint(5,91), wxSize(394,25), 0, _T("ID_M_PLBL41"));
    m_plblSessionName->SetBorderState(uiRect::BORDER_NONE);
    m_plblSessionName->GetUiRect().SetGradient(0);
    m_plblSessionName->SetForegroundColour(wxColour(0,128,0));
    m_plblSessionName->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSessionNameFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSessionName->SetFont(m_plblSessionNameFont);
    m_plblSessionType = new wmLabel(pnlSession, ID_M_PLBL43, wxEmptyString, wxPoint(400,91), wxSize(195,25), 0, _T("ID_M_PLBL43"));
    m_plblSessionType->SetBorderState(uiRect::BORDER_NONE);
    m_plblSessionType->GetUiRect().SetGradient(0);
    m_plblSessionType->SetForegroundColour(wxColour(0,128,0));
    m_plblSessionType->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSessionTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSessionType->SetFont(m_plblSessionTypeFont);
    m_pLbl3 = new wmLabel(pnlSession, ID_M_PLBL3, _("Description"), wxPoint(5,130), wxSize(590,20), 0, _T("ID_M_PLBL3"));
    m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl3->GetUiRect().SetGradient(0);
    m_pLbl3->SetForegroundColour(wxColour(255,255,255));
    m_pLbl3->SetBackgroundColour(wxColour(0,0,255));
    m_plblDescription = new wmLabel(pnlSession, ID_M_PLBL63, wxEmptyString, wxPoint(5,151), wxSize(590,25), 0, _T("ID_M_PLBL63"));
    m_plblDescription->SetBorderState(uiRect::BORDER_NONE);
    m_plblDescription->GetUiRect().SetGradient(0);
    m_plblDescription->SetForegroundColour(wxColour(0,128,0));
    m_plblDescription->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblDescriptionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblDescription->SetFont(m_plblDescriptionFont);
    m_pLbl19 = new wmLabel(pnlSession, ID_M_PLBL34, _("Domain"), wxPoint(540,200), wxSize(55,20), 0, _T("ID_M_PLBL34"));
    m_pLbl19->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl19->GetUiRect().SetGradient(0);
    m_pLbl19->SetForegroundColour(wxColour(255,255,255));
    m_pLbl19->SetBackgroundColour(wxColour(0,0,255));
    m_pLbl18 = new wmLabel(pnlSession, ID_M_PLBL33, _("Version"), wxPoint(150,200), wxSize(189,20), 0, _T("ID_M_PLBL33"));
    m_pLbl18->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl18->GetUiRect().SetGradient(0);
    m_pLbl18->SetForegroundColour(wxColour(255,255,255));
    m_pLbl18->SetBackgroundColour(wxColour(0,0,255));
    m_pLbl21 = new wmLabel(pnlSession, ID_M_PLBL36, _("Sync ID"), wxPoint(340,200), wxSize(199,20), 0, _T("ID_M_PLBL36"));
    m_pLbl21->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl21->GetUiRect().SetGradient(0);
    m_pLbl21->SetForegroundColour(wxColour(255,255,255));
    m_pLbl21->SetBackgroundColour(wxColour(0,0,255));
    m_pLbl32 = new wmLabel(pnlSession, ID_M_PLBL58, _("Sync Type"), wxPoint(5,200), wxSize(144,20), 0, _T("ID_M_PLBL58"));
    m_pLbl32->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl32->GetUiRect().SetGradient(0);
    m_pLbl32->SetForegroundColour(wxColour(255,255,255));
    m_pLbl32->SetBackgroundColour(wxColour(0,0,255));
    m_plblSyncType = new wmLabel(pnlSession, ID_M_PLBL37, wxEmptyString, wxPoint(5,221), wxSize(144,25), 0, _T("ID_M_PLBL37"));
    m_plblSyncType->SetBorderState(uiRect::BORDER_NONE);
    m_plblSyncType->GetUiRect().SetGradient(0);
    m_plblSyncType->SetForegroundColour(wxColour(0,128,0));
    m_plblSyncType->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSyncTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSyncType->SetFont(m_plblSyncTypeFont);
    m_plblSyncVersion = new wmLabel(pnlSession, ID_M_PLBL64, wxEmptyString, wxPoint(150,221), wxSize(189,25), 0, _T("ID_M_PLBL64"));
    m_plblSyncVersion->SetBorderState(uiRect::BORDER_NONE);
    m_plblSyncVersion->GetUiRect().SetGradient(0);
    m_plblSyncVersion->SetForegroundColour(wxColour(0,128,0));
    m_plblSyncVersion->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSyncVersionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSyncVersion->SetFont(m_plblSyncVersionFont);
    m_plblSyncId = new wmLabel(pnlSession, ID_M_PLBL65, wxEmptyString, wxPoint(340,221), wxSize(199,25), 0, _T("ID_M_PLBL65"));
    m_plblSyncId->SetBorderState(uiRect::BORDER_NONE);
    m_plblSyncId->GetUiRect().SetGradient(0);
    m_plblSyncId->SetForegroundColour(wxColour(0,128,0));
    m_plblSyncId->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSyncIdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSyncId->SetFont(m_plblSyncIdFont);
    m_plblSyncDomain = new wmLabel(pnlSession, ID_M_PLBL66, wxEmptyString, wxPoint(540,221), wxSize(55,25), 0, _T("ID_M_PLBL66"));
    m_plblSyncDomain->SetBorderState(uiRect::BORDER_NONE);
    m_plblSyncDomain->GetUiRect().SetGradient(0);
    m_plblSyncDomain->SetForegroundColour(wxColour(0,128,0));
    m_plblSyncDomain->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSyncDomainFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSyncDomain->SetFont(m_plblSyncDomainFont);
    m_pLbl40 = new wmLabel(pnlSession, ID_M_PLBL75, _("Audio Subsessions"), wxPoint(5,300), wxSize(144,20), 0, _T("ID_M_PLBL75"));
    m_pLbl40->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl40->GetUiRect().SetGradient(0);
    m_pLbl40->SetForegroundColour(wxColour(255,255,255));
    m_pLbl40->SetBackgroundColour(wxColour(0,0,255));
    m_pLbl42 = new wmLabel(pnlSession, ID_M_PLBL77, _("Duplicate Streams"), wxPoint(295,300), wxSize(144,20), 0, _T("ID_M_PLBL77"));
    m_pLbl42->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl42->GetUiRect().SetGradient(0);
    m_pLbl42->SetForegroundColour(wxColour(255,255,255));
    m_pLbl42->SetBackgroundColour(wxColour(0,0,255));
    m_plblSubsessionsAudio = new wmLabel(pnlSession, ID_M_PLBL78, wxEmptyString, wxPoint(5,321), wxSize(144,25), 0, _T("ID_M_PLBL78"));
    m_plblSubsessionsAudio->SetBorderState(uiRect::BORDER_NONE);
    m_plblSubsessionsAudio->GetUiRect().SetGradient(0);
    m_plblSubsessionsAudio->SetForegroundColour(wxColour(0,128,0));
    m_plblSubsessionsAudio->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblSubsessionsAudioFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblSubsessionsAudio->SetFont(m_plblSubsessionsAudioFont);
    m_plblGroups = new wmLabel(pnlSession, ID_M_PLBL80, wxEmptyString, wxPoint(295,321), wxSize(144,25), 0, _T("ID_M_PLBL80"));
    m_plblGroups->SetBorderState(uiRect::BORDER_NONE);
    m_plblGroups->GetUiRect().SetGradient(0);
    m_plblGroups->SetForegroundColour(wxColour(0,128,0));
    m_plblGroups->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblGroupsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    m_plblGroups->SetFont(m_plblGroupsFont);
    pnlSDP = new wxPanel(m_pswpInfo, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pnlSDP->SetBackgroundColour(wxColour(0,0,0));
    m_ptxtSDP = new wxTextCtrl(pnlSDP, ID_TEXTCTRL1, wxEmptyString, wxPoint(5,5), wxSize(590,435), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    m_pswpInfo->AddPage(pnlSession, _("Session"), false);x
    m_pswpInfo->AddPage(pnlSDP, _("Raw SDP"), false);
    //*)


	#ifdef PTPMONKEY
	wxPtp::Get().AddHandler(this);

	Connect(wxID_ANY, wxEVT_CLOCK_MASTER, (wxObjectEventFunction)&pnlAoIPInfo::OnPtpEvent);
	Connect(wxID_ANY, wxEVT_CLOCK_SLAVE, (wxObjectEventFunction)&pnlAoIPInfo::OnPtpEvent);
    Connect(wxID_ANY, wxEVT_CLOCK_UPDATED, (wxObjectEventFunction)&pnlAoIPInfo::OnPtpEvent);
    #endif // PTPMONKEY


	//ConnectLeftUp();

}

void pnlAoIPInfo::ConnectLeftUp()
{
    wxWindowList lst = GetChildren();
	for(auto pWnd : lst)
    {
        wxPanel* pPanel = dynamic_cast<wxPanel*>(pWnd);
        if(pPanel)
        {
            pPanel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlAoIPInfo::OnInfoLeftUp,0,this);
        }
        else
        {
            wmLabel* pLabel = dynamic_cast<wmLabel*>(pWnd);
            if(pLabel)
            {
                pLabel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlAoIPInfo::OnInfoLeftUp,0,this);
            }
        }
    }
}

void pnlAoIPInfo::OnInfoLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}


pnlAoIPInfo::~pnlAoIPInfo()
{
	//(*Destroy(pnlAoIPInfo)
	//*)
}



void pnlAoIPInfo::QoSUpdated(qosData* pData)
{
    if(pData)
    {
        auto itPanel = m_mQos.find(pData->nStream);
        if(itPanel != m_mQos.end())
        {
            itPanel->second->QoSUpdated(pData);
        }
    }
}


void pnlAoIPInfo::SetAudioData(const timedbuffer* pTimedBuffer)
{
    SetTimestamp(pTimedBuffer->GetTransmissionTime(), m_plblTransmissionTime, false);
    SetTimestamp(pTimedBuffer->GetTimeVal(), m_plblTimestampIn);
    SetTimestamp(pTimedBuffer->GetPlaybackTime(), m_plblTimestampOut);
    m_plblCurrentTimestamp->SetLabel(wxString::Format("%u", pTimedBuffer->GetTimestamp()));
    ShowLatency(pTimedBuffer);

    m_plblFrameSize->SetLabel(wxString::Format("%u bytes", pTimedBuffer->GetDuration()));

    m_dFrameDuration = static_cast<double>(pTimedBuffer->GetDuration())/static_cast<double>(m_nSampleRate*m_nFrameSize)*1e6;

    m_plblFrameDuration->SetLabel(wxString::Format("%.2f us", m_dFrameDuration));

    m_plblPlaybackQueue->SetLabel(wxString::Format("%u", pTimedBuffer->GetBufferDepth()));

    double dTimestamp(static_cast<double>(pTimedBuffer->GetTimestamp())/4294967296.0);
    m_pGraph->AddPeak("Timestamp",dTimestamp);//static_cast<double>(pTimedBuffer->GetTimestamp())/2e32);
    m_pHistogram->AddPeak("Timestamp",dTimestamp);//static_cast<double>(pTimedBuffer->GetTimestamp())/2e32);

    #ifdef PTPMONKEY
    m_plblTransmissionTime->SetBackgroundColour(wxPtp::Get().IsSyncedToMaster(0) ? *wxWHITE : wxColour(255,180,180));
    m_plblTimestampIn->SetBackgroundColour(wxPtp::Get().IsSyncedToMaster(0) ? *wxWHITE : wxColour(255,180,180));
    m_plblLatencyNetwork->SetBackgroundColour(wxPtp::Get().IsSyncedToMaster(0) ? *wxWHITE : wxColour(255,180,180));
    #endif // PTPMONKEY
}



void pnlAoIPInfo::SetTimestamp(const timeval& tv, wmLabel* pLabel, bool bDate)
{
    wxDateTime dt(time_t(tv.tv_sec));
    if(!bDate)
    {
        pLabel->SetLabel(wxString::Format("%s:%03ld", dt.Format("%H:%M:%S").c_str(), tv.tv_usec/1000));
    }
    else
    {
        pLabel->SetLabel(wxString::Format("%s:%03ld", dt.Format("%Y-%m-%d %H:%M:%S").c_str(), tv.tv_usec/1000));
    }
}


void pnlAoIPInfo::ShowLatency(const timedbuffer* pTimedBuffer)
{
    double dPlayback = pTimedBuffer->GetPlaybackLatency();

    timeval tvLatency;
    timersub(&pTimedBuffer->GetTimeVal(), &pTimedBuffer->GetTransmissionTime(), &tvLatency);

    m_dLatency = static_cast<double>(tvLatency.tv_sec)*1000000.0 + static_cast<double>(tvLatency.tv_usec);
    m_dLatency -= m_dFrameDuration;   //we add the duration on because the transmission time is first sample not last sample of frane

    m_plblLatency->SetLabel(wxString::Format("%.0f us", dPlayback));
    m_plblLatencyNetwork->SetLabel(wxString::Format("%.0f us", m_dLatency));  //@todo confirm that our latency calculation is back to front - hence the minus sign

    if(m_nInitialLatencyCounter < 3)
    {
        m_dInitialLatency = m_dLatency;
        m_nInitialLatencyCounter++;
    }
    m_dSlip = m_dLatency-m_dInitialLatency;


    #ifdef PTPMONKEY
    timeval tv(wxPtp::Get().GetLastPtpOffset(0));
    timeval tvSet(wxPtp::Get().GetPtpOffset(0));
    long long int nLast = static_cast<long long int>(tv.tv_sec)*1e6 + static_cast<long long int>(tv.tv_usec);
    long long int nSet = static_cast<long long int>(tvSet.tv_sec)*1e6 + static_cast<long long int>(tvSet.tv_usec);
    m_plblEpoch->SetLabel(wxString::Format("%lld us", nLast-nSet));
    #endif
}


void pnlAoIPInfo::SessionStarted(const session& aSession)
{
    m_pSession = &aSession;
    m_plblSessionName->SetLabel(aSession.sName);
    if(Settings::Get().Read("Input", "Type", wxEmptyString) == "AoIP")
    {
        m_plblInput->SetLabel(Settings::Get().Read("Input", "AoIP", wxEmptyString));
    }
    else
    {
        m_plblInput->SetLabel(wxEmptyString);
    }
    m_plblSyncType->SetLabel(aSession.refClock.sType);
    m_plblSyncVersion->SetLabel(aSession.refClock.sVersion);
    m_plblSyncId->SetLabel(aSession.refClock.sId);

    m_plblSyncDomain->SetLabel(wxString::Format("%lu", aSession.refClock.nDomain));
    m_plblSessionType->SetLabel(aSession.sType);

    wxClientDC dc(this);
    dc.SetFont(m_pSdp->GetFont());

    m_pSdp->Clear();
    wxArrayString asLines(wxStringTokenize(aSession.sRawSDP, "\n"));
    for(size_t i = 0; i < asLines.GetCount(); i++)
    {
        m_pSdp->AddElement(std::make_shared<SdpElement>(dc, m_pSdp->GetClientRect().GetWidth(), asLines[i]));
    }

    //delete the old subsession pages
    m_mQos.clear();
    m_mSubsessions.clear();
    for(size_t i = 0 ; i < m_pswpInfo->GetPageCount();)
    {
        if(m_pswpInfo->GetPageText(i).Before(':') == "Subsession" || m_pswpInfo->GetPageText(i).Before(':') == "QoS")
        {
            m_pswpInfo->DeletePage(i);
        }
        else
        {
            ++i;
        }
    }

    unsigned int nAudio(0), nVideo(0);
    for(list<subsession>::const_iterator itSub = aSession.lstSubsession.begin(); itSub != aSession.lstSubsession.end(); ++itSub)
    {
        if(itSub->sMedium.CmpNoCase("audio") == 0)
        {
            nAudio++;
        }
    }
    m_plblSubsessionsAudio->SetLabel(wxString::Format("%u", nAudio));
    m_plblGroups->SetLabel(aSession.sGroups);



    for(const auto& sub : aSession.lstSubsession)
    {
        auto pnlSub = new pnlSubsession(m_pswpInfo, wxNewId(), wxDefaultPosition, wxDefaultSize);
        m_pswpInfo->AddPage(pnlSub, "Subsession: "+sub.sGroup, false);

        auto pnlQos = new pnlQoS(m_pswpInfo, wxNewId(), wxDefaultPosition, wxDefaultSize);
        m_pswpInfo->AddPage(pnlQos, "QoS: "+sub.sGroup, false);
    }

}


void pnlAoIPInfo::OnPtpEvent(wxCommandEvent& event)
{
    #ifdef PTPMONKEY
    if(m_plblSyncId->GetLabel().MakeLower() == wxPtp::Get().GetMasterClockId(0))
    {
        m_plblSyncId->SetBackgroundColour(wxColour(255,255,255));
    }
    else
    {
        m_plblSyncId->SetBackgroundColour(wxColour(255,100,100));
    }
    #endif
}



void pnlAoIPInfo::ChangeGranularity(int nWhich)
{
    m_pHistogram->ChangeGranularity(m_sGraph,nWhich);
}

void pnlAoIPInfo::ChangeResolution(int nWhich)
{
    m_pHistogram->ChangeResolution(m_sGraph,nWhich);
}

void pnlAoIPInfo::RecalculateRange()
{
    m_pGraph->RecalculateRange(m_sGraph);
}
