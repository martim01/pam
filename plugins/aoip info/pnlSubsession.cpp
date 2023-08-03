#include "pnlSubsession.h"

//(*InternalHeaders(pnlSubsession)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "rtpframeevent.h"
#ifdef PTPMONKEY
#include "wxptp.h"
#endif

#include "settings.h"

//(*IdInit(pnlSubsession)
const long pnlSubsession::ID_M_PLBL39 = wxNewId();
const long pnlSubsession::ID_M_PLBL42 = wxNewId();
const long pnlSubsession::ID_M_PLBL44 = wxNewId();
const long pnlSubsession::ID_M_PLBL47 = wxNewId();
const long pnlSubsession::ID_M_PLBL45 = wxNewId();
const long pnlSubsession::ID_M_PLBL48 = wxNewId();
const long pnlSubsession::ID_M_PLBL46 = wxNewId();
const long pnlSubsession::ID_M_PLBL62 = wxNewId();
const long pnlSubsession::ID_M_PLBL49 = wxNewId();
const long pnlSubsession::ID_M_PLBL84 = wxNewId();
const long pnlSubsession::ID_M_PLBL67 = wxNewId();
const long pnlSubsession::ID_M_PLBL68 = wxNewId();
const long pnlSubsession::ID_M_PLBL69 = wxNewId();
const long pnlSubsession::ID_M_PLBL70 = wxNewId();
const long pnlSubsession::ID_M_PLBL71 = wxNewId();
const long pnlSubsession::ID_M_PLBL72 = wxNewId();
const long pnlSubsession::ID_M_PLBL74 = wxNewId();
const long pnlSubsession::ID_M_PLBL73 = wxNewId();
const long pnlSubsession::ID_M_PLBL52 = wxNewId();
const long pnlSubsession::ID_M_PLBL54 = wxNewId();
const long pnlSubsession::ID_M_PLBL3 = wxNewId();
const long pnlSubsession::ID_M_PLBL4 = wxNewId();
const long pnlSubsession::ID_M_PLBL50 = wxNewId();
const long pnlSubsession::ID_M_PLBL55 = wxNewId();
const long pnlSubsession::ID_M_PLBL57 = wxNewId();
const long pnlSubsession::ID_M_PLBL5 = wxNewId();
const long pnlSubsession::ID_M_PLBL6 = wxNewId();
const long pnlSubsession::ID_M_PLBL51 = wxNewId();
const long pnlSubsession::ID_M_PLBL1 = wxNewId();
const long pnlSubsession::ID_M_PLBL53 = wxNewId();
const long pnlSubsession::ID_M_PLBL61 = wxNewId();
const long pnlSubsession::ID_M_PLBL81 = wxNewId();
const long pnlSubsession::ID_M_PLBL82 = wxNewId();
const long pnlSubsession::ID_M_PLBL83 = wxNewId();
const long pnlSubsession::ID_M_PLBL2 = wxNewId();
const long pnlSubsession::ID_M_PLBL56 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSubsession,wxPanel)
	//(*EventTable(pnlSubsession)
	//*)
END_EVENT_TABLE()

pnlSubsession::pnlSubsession(wxWindow* parent, const subsession& sub, wxWindowID id,const wxPoint& pos,const wxSize& size) :
m_sub(sub)
{
	//(*Initialize(pnlSubsession)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl23 = new wmLabel(this, ID_M_PLBL39, _("Source"), wxPoint(5,10), wxSize(199,20), 0, _T("ID_M_PLBL39"));
	m_pLbl23->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl23->GetUiRect().SetGradient(0);
	m_pLbl23->SetForegroundColour(wxColour(255,255,255));
	m_pLbl23->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionSource = new wmLabel(this, ID_M_PLBL42, wxEmptyString, wxPoint(5,31), wxSize(199,25), 0, _T("ID_M_PLBL42"));
	m_plblSessionSource->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionSource->GetUiRect().SetGradient(0);
	m_plblSessionSource->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionSource->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionSourceFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionSource->SetFont(m_plblSessionSourceFont);
	m_pLbl25 = new wmLabel(this, ID_M_PLBL44, _("Codec"), wxPoint(205,10), wxSize(74,20), 0, _T("ID_M_PLBL44"));
	m_pLbl25->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl25->GetUiRect().SetGradient(0);
	m_pLbl25->SetForegroundColour(wxColour(255,255,255));
	m_pLbl25->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionBits = new wmLabel(this, ID_M_PLBL47, wxEmptyString, wxPoint(205,31), wxSize(74,25), 0, _T("ID_M_PLBL47"));
	m_plblSessionBits->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionBits->GetUiRect().SetGradient(0);
	m_plblSessionBits->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionBits->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionBitsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionBits->SetFont(m_plblSessionBitsFont);
	m_pLbl26 = new wmLabel(this, ID_M_PLBL45, _("Freq"), wxPoint(280,10), wxSize(100,20), 0, _T("ID_M_PLBL45"));
	m_pLbl26->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl26->GetUiRect().SetGradient(0);
	m_pLbl26->SetForegroundColour(wxColour(255,255,255));
	m_pLbl26->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionFrequency = new wmLabel(this, ID_M_PLBL48, wxEmptyString, wxPoint(280,31), wxSize(100,25), 0, _T("ID_M_PLBL48"));
	m_plblSessionFrequency->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionFrequency->GetUiRect().SetGradient(0);
	m_plblSessionFrequency->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionFrequency->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionFrequencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionFrequency->SetFont(m_plblSessionFrequencyFont);
	m_pLbl27 = new wmLabel(this, ID_M_PLBL46, _("Channels"), wxPoint(381,10), wxSize(59,20), 0, _T("ID_M_PLBL46"));
	m_pLbl27->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl27->GetUiRect().SetGradient(0);
	m_pLbl27->SetForegroundColour(wxColour(255,255,255));
	m_pLbl27->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl38 = new wmLabel(this, ID_M_PLBL62, _("Id"), wxPoint(441,10), wxSize(158,20), 0, _T("ID_M_PLBL62"));
	m_pLbl38->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl38->GetUiRect().SetGradient(0);
	m_pLbl38->SetForegroundColour(wxColour(255,255,255));
	m_pLbl38->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionChannels = new wmLabel(this, ID_M_PLBL49, wxEmptyString, wxPoint(381,31), wxSize(59,25), 0, _T("ID_M_PLBL49"));
	m_plblSessionChannels->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionChannels->GetUiRect().SetGradient(0);
	m_plblSessionChannels->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionChannels->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionChannelsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionChannels->SetFont(m_plblSessionChannelsFont);
	m_plblSubsessionId = new wmLabel(this, ID_M_PLBL84, wxEmptyString, wxPoint(441,31), wxSize(158,25), 0, _T("ID_M_PLBL84"));
	m_plblSubsessionId->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubsessionId->GetUiRect().SetGradient(0);
	m_plblSubsessionId->SetForegroundColour(wxColour(0,128,0));
	m_plblSubsessionId->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubsessionIdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubsessionId->SetFont(m_plblSubsessionIdFont);
	m_pLbl22 = new wmLabel(this, ID_M_PLBL67, _("Sync Type"), wxPoint(5,70), wxSize(144,20), 0, _T("ID_M_PLBL67"));
	m_pLbl22->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl22->GetUiRect().SetGradient(0);
	m_pLbl22->SetForegroundColour(wxColour(255,255,255));
	m_pLbl22->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl35 = new wmLabel(this, ID_M_PLBL68, _("Version"), wxPoint(150,70), wxSize(189,20), 0, _T("ID_M_PLBL68"));
	m_pLbl35->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl35->GetUiRect().SetGradient(0);
	m_pLbl35->SetForegroundColour(wxColour(255,255,255));
	m_pLbl35->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl36 = new wmLabel(this, ID_M_PLBL69, _("Sync ID"), wxPoint(340,70), wxSize(199,20), 0, _T("ID_M_PLBL69"));
	m_pLbl36->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl36->GetUiRect().SetGradient(0);
	m_pLbl36->SetForegroundColour(wxColour(255,255,255));
	m_pLbl36->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl37 = new wmLabel(this, ID_M_PLBL70, _("Domain"), wxPoint(540,70), wxSize(55,20), 0, _T("ID_M_PLBL70"));
	m_pLbl37->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl37->GetUiRect().SetGradient(0);
	m_pLbl37->SetForegroundColour(wxColour(255,255,255));
	m_pLbl37->SetBackgroundColour(wxColour(0,0,255));
	m_plblSubSyncType = new wmLabel(this, ID_M_PLBL71, wxEmptyString, wxPoint(5,91), wxSize(144,25), 0, _T("ID_M_PLBL71"));
	m_plblSubSyncType->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncType->GetUiRect().SetGradient(0);
	m_plblSubSyncType->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncType->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncType->SetFont(m_plblSubSyncTypeFont);
	m_plblSubSyncVersion = new wmLabel(this, ID_M_PLBL72, wxEmptyString, wxPoint(150,91), wxSize(189,25), 0, _T("ID_M_PLBL72"));
	m_plblSubSyncVersion->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncVersion->GetUiRect().SetGradient(0);
	m_plblSubSyncVersion->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncVersion->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncVersionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncVersion->SetFont(m_plblSubSyncVersionFont);
	m_plblSubSyncDomain = new wmLabel(this, ID_M_PLBL74, wxEmptyString, wxPoint(540,91), wxSize(55,25), 0, _T("ID_M_PLBL74"));
	m_plblSubSyncDomain->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncDomain->GetUiRect().SetGradient(0);
	m_plblSubSyncDomain->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncDomain->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncDomainFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncDomain->SetFont(m_plblSubSyncDomainFont);
	m_plblSubSyncId = new wmLabel(this, ID_M_PLBL73, wxEmptyString, wxPoint(340,91), wxSize(199,25), 0, _T("ID_M_PLBL73"));
	m_plblSubSyncId->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncId->GetUiRect().SetGradient(0);
	m_plblSubSyncId->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncIdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncId->SetFont(m_plblSubSyncIdFont);
	m_pLbl29 = new wmLabel(this, ID_M_PLBL52, _("Sync Timestamp"), wxPoint(5,130), wxSize(160,20), 0, _T("ID_M_PLBL52"));
	m_pLbl29->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl29->GetUiRect().SetGradient(0);
	m_pLbl29->SetForegroundColour(wxColour(255,255,255));
	m_pLbl29->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl31 = new wmLabel(this, ID_M_PLBL54, _("Current Timestamp"), wxPoint(166,130), wxSize(160,20), 0, _T("ID_M_PLBL54"));
	m_pLbl31->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl31->GetUiRect().SetGradient(0);
	m_pLbl31->SetForegroundColour(wxColour(255,255,255));
	m_pLbl31->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL3, _("Buffer Size"), wxPoint(400,130), wxSize(80,20), 0, _T("ID_M_PLBL3"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL4, _("Frames Used"), wxPoint(485,130), wxSize(110,20), 0, _T("ID_M_PLBL4"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl28 = new wmLabel(this, ID_M_PLBL50, _("Transmission Time"), wxPoint(5,185), wxSize(160,20), 0, _T("ID_M_PLBL50"));
	m_pLbl28->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl28->GetUiRect().SetGradient(0);
	m_pLbl28->SetForegroundColour(wxColour(255,255,255));
	m_pLbl28->SetBackgroundColour(wxColour(0,0,255));
	m_plblSyncTimestamp = new wmLabel(this, ID_M_PLBL55, wxEmptyString, wxPoint(5,151), wxSize(160,25), 0, _T("ID_M_PLBL55"));
	m_plblSyncTimestamp->SetBorderState(uiRect::BORDER_NONE);
	m_plblSyncTimestamp->GetUiRect().SetGradient(0);
	m_plblSyncTimestamp->SetForegroundColour(wxColour(0,128,0));
	m_plblSyncTimestamp->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSyncTimestampFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSyncTimestamp->SetFont(m_plblSyncTimestampFont);
	m_plblCurrentTimestamp = new wmLabel(this, ID_M_PLBL57, wxEmptyString, wxPoint(166,151), wxSize(160,25), 0, _T("ID_M_PLBL57"));
	m_plblCurrentTimestamp->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrentTimestamp->GetUiRect().SetGradient(0);
	m_plblCurrentTimestamp->SetForegroundColour(wxColour(0,128,0));
	m_plblCurrentTimestamp->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblCurrentTimestampFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblCurrentTimestamp->SetFont(m_plblCurrentTimestampFont);
	m_plblBuffer = new wmLabel(this, ID_M_PLBL5, wxEmptyString, wxPoint(400,151), wxSize(80,25), 0, _T("ID_M_PLBL5"));
	m_plblBuffer->SetBorderState(uiRect::BORDER_NONE);
	m_plblBuffer->GetUiRect().SetGradient(0);
	m_plblBuffer->SetForegroundColour(wxColour(0,128,0));
	m_plblBuffer->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblBufferFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblBuffer->SetFont(m_plblBufferFont);
	m_plblContribution = new wmLabel(this, ID_M_PLBL6, wxEmptyString, wxPoint(485,151), wxSize(110,25), 0, _T("ID_M_PLBL6"));
	m_plblContribution->SetBorderState(uiRect::BORDER_NONE);
	m_plblContribution->GetUiRect().SetGradient(0);
	m_plblContribution->SetForegroundColour(wxColour(0,128,0));
	m_plblContribution->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblContributionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblContribution->SetFont(m_plblContributionFont);
	m_plblTransmissionTime = new wmLabel(this, ID_M_PLBL51, wxEmptyString, wxPoint(5,206), wxSize(160,25), 0, _T("ID_M_PLBL51"));
	m_plblTransmissionTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblTransmissionTime->GetUiRect().SetGradient(0);
	m_plblTransmissionTime->SetForegroundColour(wxColour(0,128,0));
	m_plblTransmissionTime->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTransmissionTimeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTransmissionTime->SetFont(m_plblTransmissionTimeFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Presentation Time"), wxPoint(166,185), wxSize(160,20), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl30 = new wmLabel(this, ID_M_PLBL53, _("Network Latency"), wxPoint(327,185), wxSize(100,20), 0, _T("ID_M_PLBL53"));
	m_pLbl30->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl30->GetUiRect().SetGradient(0);
	m_pLbl30->SetForegroundColour(wxColour(255,255,255));
	m_pLbl30->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl34 = new wmLabel(this, ID_M_PLBL61, _("Last Frame Size"), wxPoint(5,240), wxSize(144,20), 0, _T("ID_M_PLBL61"));
	m_pLbl34->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl34->GetUiRect().SetGradient(0);
	m_pLbl34->SetForegroundColour(wxColour(255,255,255));
	m_pLbl34->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl43 = new wmLabel(this, ID_M_PLBL81, _("Last Frame Duration"), wxPoint(150,240), wxSize(144,20), 0, _T("ID_M_PLBL81"));
	m_pLbl43->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl43->GetUiRect().SetGradient(0);
	m_pLbl43->SetForegroundColour(wxColour(255,255,255));
	m_pLbl43->SetBackgroundColour(wxColour(0,0,255));
	m_plblFrameSize = new wmLabel(this, ID_M_PLBL82, wxEmptyString, wxPoint(5,261), wxSize(144,25), 0, _T("ID_M_PLBL82"));
	m_plblFrameSize->SetBorderState(uiRect::BORDER_NONE);
	m_plblFrameSize->GetUiRect().SetGradient(0);
	m_plblFrameSize->SetForegroundColour(wxColour(0,128,0));
	m_plblFrameSize->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblFrameSizeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblFrameSize->SetFont(m_plblFrameSizeFont);
	m_plblFrameDuration = new wmLabel(this, ID_M_PLBL83, wxEmptyString, wxPoint(150,261), wxSize(144,25), 0, _T("ID_M_PLBL83"));
	m_plblFrameDuration->SetBorderState(uiRect::BORDER_NONE);
	m_plblFrameDuration->GetUiRect().SetGradient(0);
	m_plblFrameDuration->SetForegroundColour(wxColour(0,128,0));
	m_plblFrameDuration->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblFrameDurationFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblFrameDuration->SetFont(m_plblFrameDurationFont);
	m_plblTimestampIn = new wmLabel(this, ID_M_PLBL2, wxEmptyString, wxPoint(166,206), wxSize(160,25), 0, _T("ID_M_PLBL2"));
	m_plblTimestampIn->SetBorderState(uiRect::BORDER_NONE);
	m_plblTimestampIn->GetUiRect().SetGradient(0);
	m_plblTimestampIn->SetForegroundColour(wxColour(0,128,0));
	m_plblTimestampIn->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTimestampInFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTimestampIn->SetFont(m_plblTimestampInFont);
	m_plblLatencyNetwork = new wmLabel(this, ID_M_PLBL56, wxEmptyString, wxPoint(327,206), wxSize(100,25), 0, _T("ID_M_PLBL56"));
	m_plblLatencyNetwork->SetBorderState(uiRect::BORDER_NONE);
	m_plblLatencyNetwork->GetUiRect().SetGradient(0);
	m_plblLatencyNetwork->SetForegroundColour(wxColour(155,0,0));
	m_plblLatencyNetwork->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblLatencyNetworkFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblLatencyNetwork->SetFont(m_plblLatencyNetworkFont);
	//*)



	m_plblSubsessionId->SetLabel(sub.sId);
    m_plblSessionSource->SetLabel(sub.sSourceAddress);
    m_plblSessionBits->SetLabel(sub.sCodec);
    m_plblSessionFrequency->SetLabel(wxString::Format("%.1fkHz", static_cast<double>(sub.nSampleRate)/1000.0));
    m_plblSessionChannels->SetLabel(wxString::Format("%u", sub.nChannels));

    m_plblSyncTimestamp->SetLabel(wxString::Format("%u", sub.nSyncTimestamp));

    /*m_nSampleRate = sub.nSampleRate;
    
	m_nFrameSize = min((unsigned int)256 ,sub.nChannels);
    if(sub.sCodec == "L24")
    {
        m_nFrameSize*=3;
    }
    else if(sub.sCodec == "L16")
    {
        m_nFrameSize*=2;
    }
    else if(sub.sCodec == "F32")
    {
        m_nFrameSize*=4;
    }*/

    m_plblSubSyncType->SetLabel(sub.refClock.sType);
    m_plblSubSyncVersion->SetLabel(sub.refClock.sVersion);
    m_plblSubSyncId->SetLabel(sub.refClock.sId);
#ifdef PTPMONKEY
    if(m_plblSubSyncId->GetLabel().MakeLower() == wxPtp::Get().GetMasterClockId())
    {
        m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,255));
    }
    else
    {
        m_plblSubSyncId->SetBackgroundColour(wxColour(255,100,100));
    }
    wxPtp::Get().AddHandler(this);

	Connect(wxID_ANY, wxEVT_CLOCK_MASTER, (wxObjectEventFunction)&pnlSubsession::OnPtpEvent);
	Connect(wxID_ANY, wxEVT_CLOCK_SLAVE, (wxObjectEventFunction)&pnlSubsession::OnPtpEvent);
    Connect(wxID_ANY, wxEVT_CLOCK_UPDATED, (wxObjectEventFunction)&pnlSubsession::OnPtpEvent);

#else
    m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,100));
#endif
    m_plblSubSyncDomain->SetLabel(wxString::Format("%lu", sub.refClock.nDomain));

	m_plblSyncTimestamp->SetLabel(wxString::Format("%u", sub.nSyncTimestamp));
}

pnlSubsession::~pnlSubsession()
{
	//(*Destroy(pnlSubsession)
	//*)
}

void pnlSubsession::OnPtpEvent(wxCommandEvent& event)
{
#ifdef PTPMONKEY
    if(m_plblSubSyncId->GetLabel().MakeLower() == wxPtp::Get().GetMasterClockId())
    {
        m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,255));
    }
    else
    {
        m_plblSubSyncId->SetBackgroundColour(wxColour(255,100,100));
    }
#endif
}

void pnlSubsession::SetAudioData(const timedbuffer* pTimedBuffer)
{
	
}

void pnlSubsession::RtpFrame(std::shared_ptr<const rtpFrame> pFrame)
{
	if(IsShownOnScreen())
	{
		m_plblCurrentTimestamp->SetLabel(wxString::Format("%u", pFrame->nTimestamp));

	m_dDuration = (1e6*static_cast<double>(pFrame->nFrameSize)) / (static_cast<double>(m_sub.nSampleRate* m_sub.nChannels*pFrame->nBytesPerSample));
	m_plblFrameDuration->SetLabel(wxString::Format(L"%.0f \u03bcs", m_dDuration));
	m_plblFrameSize->SetLabel(wxString::Format("%lu bytes", pFrame->nFrameSize));
	
	
	
	SetTimestamp(pFrame->timePresentation, m_plblTimestampIn, false);
	SetTimestamp(pFrame->timeTransmission, m_plblTransmissionTime, false);
	ShowLatency(pFrame);
	}
}

void pnlSubsession::SetTimestamp(const timeval& tv, wmLabel* pLabel, bool bDate)
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


void pnlSubsession::ShowLatency(std::shared_ptr<const rtpFrame> pFrame)
{

    auto dLatency = static_cast<double>(pFrame->timeLatency.tv_sec)*1000000.0 + static_cast<double>(pFrame->timeLatency.tv_usec);
    dLatency += m_dDuration;   //we add the duration on because the transmission time is first sample not last sample of frane

    m_plblLatencyNetwork->SetLabel(wxString::Format("%.0f us", dLatency));

}

void pnlSubsession::QoSUpdated(qosData* pData)
{
	if(IsShownOnScreen())
	{
		m_plblBuffer->SetLabel(wxString::Format("%lu", pData->nBufferSize));
		m_plblContribution->SetLabel(wxString::Format("%.1f%%", (static_cast<double>(pData->nFramesUsed)/static_cast<double>(pData->nTotalFrames))*100.0));
	}
}