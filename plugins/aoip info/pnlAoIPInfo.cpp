#include "pnlAoIPInfo.h"
#include "session.h"
#include "timedbuffer.h"
#include "settings.h"
#include <wx/log.h>
#ifdef PTPMONKEY
#include "wxptp.h"
#endif
#include "aoipinfobuilder.h"

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
const long pnlAoIPInfo::ID_M_PLBL76 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL77 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL78 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL79 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL80 = wxNewId();
const long pnlAoIPInfo::pnlSessionInfo = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL39 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL42 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL44 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL47 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL45 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL48 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL46 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL62 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL49 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL84 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL67 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL68 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL69 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL70 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL71 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL72 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL74 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL73 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL52 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL54 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL50 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL55 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL57 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL51 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL1 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL53 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL4 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL61 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL81 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL87 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL82 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL83 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL88 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL6 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL2 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL56 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL5 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL7 = wxNewId();
const long pnlAoIPInfo::ID_PANEL1 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL8 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL9 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL10 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL11 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL13 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL12 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL14 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL15 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL16 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL17 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL18 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL19 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL20 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL21 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL22 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL23 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL24 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL25 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL26 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL27 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL28 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL29 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL30 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL31 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL32 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL85 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL35 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL89 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL90 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL86 = wxNewId();
const long pnlAoIPInfo::ID_CUSTOM12 = wxNewId();
const long pnlAoIPInfo::ID_PANEL4 = wxNewId();
const long pnlAoIPInfo::ID_PANEL2 = wxNewId();
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
    m_pSession(0)
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
	m_pLbl41 = new wmLabel(pnlSession, ID_M_PLBL76, _("Video Subsessions"), wxPoint(150,300), wxSize(144,20), 0, _T("ID_M_PLBL76"));
	m_pLbl41->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl41->GetUiRect().SetGradient(0);
	m_pLbl41->SetForegroundColour(wxColour(255,255,255));
	m_pLbl41->SetBackgroundColour(wxColour(0,0,255));
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
	m_plblSubsessionVideo = new wmLabel(pnlSession, ID_M_PLBL79, wxEmptyString, wxPoint(150,321), wxSize(144,25), 0, _T("ID_M_PLBL79"));
	m_plblSubsessionVideo->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubsessionVideo->GetUiRect().SetGradient(0);
	m_plblSubsessionVideo->SetForegroundColour(wxColour(0,128,0));
	m_plblSubsessionVideo->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubsessionVideoFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubsessionVideo->SetFont(m_plblSubsessionVideoFont);
	m_plblGroups = new wmLabel(pnlSession, ID_M_PLBL80, wxEmptyString, wxPoint(295,321), wxSize(144,25), 0, _T("ID_M_PLBL80"));
	m_plblGroups->SetBorderState(uiRect::BORDER_NONE);
	m_plblGroups->GetUiRect().SetGradient(0);
	m_plblGroups->SetForegroundColour(wxColour(0,128,0));
	m_plblGroups->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblGroupsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblGroups->SetFont(m_plblGroupsFont);
	pnlSubsession = new wxPanel(m_pswpInfo, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlSubsession->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl23 = new wmLabel(pnlSubsession, ID_M_PLBL39, _("Source"), wxPoint(5,10), wxSize(199,20), 0, _T("ID_M_PLBL39"));
	m_pLbl23->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl23->GetUiRect().SetGradient(0);
	m_pLbl23->SetForegroundColour(wxColour(255,255,255));
	m_pLbl23->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionSource = new wmLabel(pnlSubsession, ID_M_PLBL42, wxEmptyString, wxPoint(5,31), wxSize(199,25), 0, _T("ID_M_PLBL42"));
	m_plblSessionSource->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionSource->GetUiRect().SetGradient(0);
	m_plblSessionSource->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionSource->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionSourceFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionSource->SetFont(m_plblSessionSourceFont);
	m_pLbl25 = new wmLabel(pnlSubsession, ID_M_PLBL44, _("Codec"), wxPoint(205,10), wxSize(74,20), 0, _T("ID_M_PLBL44"));
	m_pLbl25->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl25->GetUiRect().SetGradient(0);
	m_pLbl25->SetForegroundColour(wxColour(255,255,255));
	m_pLbl25->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionBits = new wmLabel(pnlSubsession, ID_M_PLBL47, wxEmptyString, wxPoint(205,31), wxSize(74,25), 0, _T("ID_M_PLBL47"));
	m_plblSessionBits->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionBits->GetUiRect().SetGradient(0);
	m_plblSessionBits->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionBits->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionBitsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionBits->SetFont(m_plblSessionBitsFont);
	m_pLbl26 = new wmLabel(pnlSubsession, ID_M_PLBL45, _("Freq"), wxPoint(280,10), wxSize(100,20), 0, _T("ID_M_PLBL45"));
	m_pLbl26->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl26->GetUiRect().SetGradient(0);
	m_pLbl26->SetForegroundColour(wxColour(255,255,255));
	m_pLbl26->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionFrequency = new wmLabel(pnlSubsession, ID_M_PLBL48, wxEmptyString, wxPoint(280,31), wxSize(100,25), 0, _T("ID_M_PLBL48"));
	m_plblSessionFrequency->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionFrequency->GetUiRect().SetGradient(0);
	m_plblSessionFrequency->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionFrequency->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionFrequencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionFrequency->SetFont(m_plblSessionFrequencyFont);
	m_pLbl27 = new wmLabel(pnlSubsession, ID_M_PLBL46, _("Channels"), wxPoint(381,10), wxSize(59,20), 0, _T("ID_M_PLBL46"));
	m_pLbl27->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl27->GetUiRect().SetGradient(0);
	m_pLbl27->SetForegroundColour(wxColour(255,255,255));
	m_pLbl27->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl38 = new wmLabel(pnlSubsession, ID_M_PLBL62, _("Id"), wxPoint(441,10), wxSize(158,20), 0, _T("ID_M_PLBL62"));
	m_pLbl38->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl38->GetUiRect().SetGradient(0);
	m_pLbl38->SetForegroundColour(wxColour(255,255,255));
	m_pLbl38->SetBackgroundColour(wxColour(0,0,255));
	m_plblSessionChannels = new wmLabel(pnlSubsession, ID_M_PLBL49, wxEmptyString, wxPoint(381,31), wxSize(59,25), 0, _T("ID_M_PLBL49"));
	m_plblSessionChannels->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionChannels->GetUiRect().SetGradient(0);
	m_plblSessionChannels->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionChannels->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionChannelsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionChannels->SetFont(m_plblSessionChannelsFont);
	m_plblSubsessionId = new wmLabel(pnlSubsession, ID_M_PLBL84, wxEmptyString, wxPoint(441,31), wxSize(158,25), 0, _T("ID_M_PLBL84"));
	m_plblSubsessionId->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubsessionId->GetUiRect().SetGradient(0);
	m_plblSubsessionId->SetForegroundColour(wxColour(0,128,0));
	m_plblSubsessionId->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubsessionIdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubsessionId->SetFont(m_plblSubsessionIdFont);
	m_pLbl22 = new wmLabel(pnlSubsession, ID_M_PLBL67, _("Sync Type"), wxPoint(5,70), wxSize(144,20), 0, _T("ID_M_PLBL67"));
	m_pLbl22->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl22->GetUiRect().SetGradient(0);
	m_pLbl22->SetForegroundColour(wxColour(255,255,255));
	m_pLbl22->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl35 = new wmLabel(pnlSubsession, ID_M_PLBL68, _("Version"), wxPoint(150,70), wxSize(189,20), 0, _T("ID_M_PLBL68"));
	m_pLbl35->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl35->GetUiRect().SetGradient(0);
	m_pLbl35->SetForegroundColour(wxColour(255,255,255));
	m_pLbl35->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl36 = new wmLabel(pnlSubsession, ID_M_PLBL69, _("Sync ID"), wxPoint(340,70), wxSize(199,20), 0, _T("ID_M_PLBL69"));
	m_pLbl36->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl36->GetUiRect().SetGradient(0);
	m_pLbl36->SetForegroundColour(wxColour(255,255,255));
	m_pLbl36->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl37 = new wmLabel(pnlSubsession, ID_M_PLBL70, _("Domain"), wxPoint(540,70), wxSize(55,20), 0, _T("ID_M_PLBL70"));
	m_pLbl37->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl37->GetUiRect().SetGradient(0);
	m_pLbl37->SetForegroundColour(wxColour(255,255,255));
	m_pLbl37->SetBackgroundColour(wxColour(0,0,255));
	m_plblSubSyncType = new wmLabel(pnlSubsession, ID_M_PLBL71, wxEmptyString, wxPoint(5,91), wxSize(144,25), 0, _T("ID_M_PLBL71"));
	m_plblSubSyncType->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncType->GetUiRect().SetGradient(0);
	m_plblSubSyncType->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncType->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncType->SetFont(m_plblSubSyncTypeFont);
	m_plblSubSyncVersion = new wmLabel(pnlSubsession, ID_M_PLBL72, wxEmptyString, wxPoint(150,91), wxSize(189,25), 0, _T("ID_M_PLBL72"));
	m_plblSubSyncVersion->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncVersion->GetUiRect().SetGradient(0);
	m_plblSubSyncVersion->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncVersion->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncVersionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncVersion->SetFont(m_plblSubSyncVersionFont);
	m_plblSubSyncDomain = new wmLabel(pnlSubsession, ID_M_PLBL74, wxEmptyString, wxPoint(540,91), wxSize(55,25), 0, _T("ID_M_PLBL74"));
	m_plblSubSyncDomain->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncDomain->GetUiRect().SetGradient(0);
	m_plblSubSyncDomain->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncDomain->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncDomainFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncDomain->SetFont(m_plblSubSyncDomainFont);
	m_plblSubSyncId = new wmLabel(pnlSubsession, ID_M_PLBL73, wxEmptyString, wxPoint(340,91), wxSize(199,25), 0, _T("ID_M_PLBL73"));
	m_plblSubSyncId->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubSyncId->GetUiRect().SetGradient(0);
	m_plblSubSyncId->SetForegroundColour(wxColour(0,128,0));
	m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSubSyncIdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSubSyncId->SetFont(m_plblSubSyncIdFont);
	m_pLbl29 = new wmLabel(pnlSubsession, ID_M_PLBL52, _("Sync Timestamp"), wxPoint(5,130), wxSize(149,20), 0, _T("ID_M_PLBL52"));
	m_pLbl29->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl29->GetUiRect().SetGradient(0);
	m_pLbl29->SetForegroundColour(wxColour(255,255,255));
	m_pLbl29->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl31 = new wmLabel(pnlSubsession, ID_M_PLBL54, _("Current Timestamp"), wxPoint(155,130), wxSize(149,20), 0, _T("ID_M_PLBL54"));
	m_pLbl31->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl31->GetUiRect().SetGradient(0);
	m_pLbl31->SetForegroundColour(wxColour(255,255,255));
	m_pLbl31->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl28 = new wmLabel(pnlSubsession, ID_M_PLBL50, _("Transmission Time"), wxPoint(5,185), wxSize(165,20), 0, _T("ID_M_PLBL50"));
	m_pLbl28->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl28->GetUiRect().SetGradient(0);
	m_pLbl28->SetForegroundColour(wxColour(255,255,255));
	m_pLbl28->SetBackgroundColour(wxColour(0,0,255));
	m_plblSyncTimestamp = new wmLabel(pnlSubsession, ID_M_PLBL55, wxEmptyString, wxPoint(5,151), wxSize(149,25), 0, _T("ID_M_PLBL55"));
	m_plblSyncTimestamp->SetBorderState(uiRect::BORDER_NONE);
	m_plblSyncTimestamp->GetUiRect().SetGradient(0);
	m_plblSyncTimestamp->SetForegroundColour(wxColour(0,128,0));
	m_plblSyncTimestamp->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSyncTimestampFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSyncTimestamp->SetFont(m_plblSyncTimestampFont);
	m_plblCurrentTimestamp = new wmLabel(pnlSubsession, ID_M_PLBL57, wxEmptyString, wxPoint(155,151), wxSize(149,25), 0, _T("ID_M_PLBL57"));
	m_plblCurrentTimestamp->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrentTimestamp->GetUiRect().SetGradient(0);
	m_plblCurrentTimestamp->SetForegroundColour(wxColour(0,128,0));
	m_plblCurrentTimestamp->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblCurrentTimestampFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblCurrentTimestamp->SetFont(m_plblCurrentTimestampFont);
	m_plblTransmissionTime = new wmLabel(pnlSubsession, ID_M_PLBL51, wxEmptyString, wxPoint(5,206), wxSize(165,25), 0, _T("ID_M_PLBL51"));
	m_plblTransmissionTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblTransmissionTime->GetUiRect().SetGradient(0);
	m_plblTransmissionTime->SetForegroundColour(wxColour(0,128,0));
	m_plblTransmissionTime->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTransmissionTimeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTransmissionTime->SetFont(m_plblTransmissionTimeFont);
	m_pLbl1 = new wmLabel(pnlSubsession, ID_M_PLBL1, _("Presentation Time"), wxPoint(171,185), wxSize(165,20), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl30 = new wmLabel(pnlSubsession, ID_M_PLBL53, _("Network Latency"), wxPoint(337,185), wxSize(100,20), 0, _T("ID_M_PLBL53"));
	m_pLbl30->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl30->GetUiRect().SetGradient(0);
	m_pLbl30->SetForegroundColour(wxColour(255,255,255));
	m_pLbl30->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl2 = new wmLabel(pnlSubsession, ID_M_PLBL4, _("Display/Playback Time"), wxPoint(5,240), wxSize(166,20), 0, _T("ID_M_PLBL4"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl34 = new wmLabel(pnlSubsession, ID_M_PLBL61, _("Last Frame Size"), wxPoint(5,300), wxSize(144,20), 0, _T("ID_M_PLBL61"));
	m_pLbl34->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl34->GetUiRect().SetGradient(0);
	m_pLbl34->SetForegroundColour(wxColour(255,255,255));
	m_pLbl34->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl43 = new wmLabel(pnlSubsession, ID_M_PLBL81, _("Last Frame Duration"), wxPoint(150,300), wxSize(144,20), 0, _T("ID_M_PLBL81"));
	m_pLbl43->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl43->GetUiRect().SetGradient(0);
	m_pLbl43->SetForegroundColour(wxColour(255,255,255));
	m_pLbl43->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl44 = new wmLabel(pnlSubsession, ID_M_PLBL87, _("Playback Queue"), wxPoint(450,300), wxSize(144,20), 0, _T("ID_M_PLBL87"));
	m_pLbl44->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl44->GetUiRect().SetGradient(0);
	m_pLbl44->SetForegroundColour(wxColour(255,255,255));
	m_pLbl44->SetBackgroundColour(wxColour(0,0,255));
	m_plblFrameSize = new wmLabel(pnlSubsession, ID_M_PLBL82, wxEmptyString, wxPoint(5,321), wxSize(144,25), 0, _T("ID_M_PLBL82"));
	m_plblFrameSize->SetBorderState(uiRect::BORDER_NONE);
	m_plblFrameSize->GetUiRect().SetGradient(0);
	m_plblFrameSize->SetForegroundColour(wxColour(0,128,0));
	m_plblFrameSize->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblFrameSizeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblFrameSize->SetFont(m_plblFrameSizeFont);
	m_plblFrameDuration = new wmLabel(pnlSubsession, ID_M_PLBL83, wxEmptyString, wxPoint(150,321), wxSize(144,25), 0, _T("ID_M_PLBL83"));
	m_plblFrameDuration->SetBorderState(uiRect::BORDER_NONE);
	m_plblFrameDuration->GetUiRect().SetGradient(0);
	m_plblFrameDuration->SetForegroundColour(wxColour(0,128,0));
	m_plblFrameDuration->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblFrameDurationFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblFrameDuration->SetFont(m_plblFrameDurationFont);
	m_plblPlaybackQueue = new wmLabel(pnlSubsession, ID_M_PLBL88, wxEmptyString, wxPoint(450,321), wxSize(144,25), 0, _T("ID_M_PLBL88"));
	m_plblPlaybackQueue->SetBorderState(uiRect::BORDER_NONE);
	m_plblPlaybackQueue->GetUiRect().SetGradient(0);
	m_plblPlaybackQueue->SetForegroundColour(wxColour(0,128,0));
	m_plblPlaybackQueue->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPlaybackQueueFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPlaybackQueue->SetFont(m_plblPlaybackQueueFont);
	m_pLbl4 = new wmLabel(pnlSubsession, ID_M_PLBL6, _("Playback Latency"), wxPoint(238,240), wxSize(100,20), 0, _T("ID_M_PLBL6"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,255));
	m_plblTimestampIn = new wmLabel(pnlSubsession, ID_M_PLBL2, wxEmptyString, wxPoint(171,206), wxSize(165,25), 0, _T("ID_M_PLBL2"));
	m_plblTimestampIn->SetBorderState(uiRect::BORDER_NONE);
	m_plblTimestampIn->GetUiRect().SetGradient(0);
	m_plblTimestampIn->SetForegroundColour(wxColour(0,128,0));
	m_plblTimestampIn->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTimestampInFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTimestampIn->SetFont(m_plblTimestampInFont);
	m_plblLatencyNetwork = new wmLabel(pnlSubsession, ID_M_PLBL56, wxEmptyString, wxPoint(337,206), wxSize(100,25), 0, _T("ID_M_PLBL56"));
	m_plblLatencyNetwork->SetBorderState(uiRect::BORDER_NONE);
	m_plblLatencyNetwork->GetUiRect().SetGradient(0);
	m_plblLatencyNetwork->SetForegroundColour(wxColour(155,0,0));
	m_plblLatencyNetwork->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblLatencyNetworkFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblLatencyNetwork->SetFont(m_plblLatencyNetworkFont);
	m_plblTimestampOut = new wmLabel(pnlSubsession, ID_M_PLBL5, wxEmptyString, wxPoint(5,261), wxSize(166,25), 0, _T("ID_M_PLBL5"));
	m_plblTimestampOut->SetBorderState(uiRect::BORDER_NONE);
	m_plblTimestampOut->GetUiRect().SetGradient(0);
	m_plblTimestampOut->SetForegroundColour(wxColour(0,128,0));
	m_plblTimestampOut->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTimestampOutFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTimestampOut->SetFont(m_plblTimestampOutFont);
	m_plblLatency = new wmLabel(pnlSubsession, ID_M_PLBL7, wxEmptyString, wxPoint(238,261), wxSize(100,25), 0, _T("ID_M_PLBL7"));
	m_plblLatency->SetBorderState(uiRect::BORDER_NONE);
	m_plblLatency->GetUiRect().SetGradient(0);
	m_plblLatency->SetForegroundColour(wxColour(155,0,0));
	m_plblLatency->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblLatencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblLatency->SetFont(m_plblLatencyFont);
	pnlQoS = new wxPanel(m_pswpInfo, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	pnlQoS->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl5 = new wmLabel(pnlQoS, ID_M_PLBL8, _("QoS Measurement Time"), wxPoint(5,5), wxSize(194,20), 0, _T("ID_M_PLBL8"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl6 = new wmLabel(pnlQoS, ID_M_PLBL9, _("Total Packets Received"), wxPoint(200,5), wxSize(199,20), 0, _T("ID_M_PLBL9"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl7 = new wmLabel(pnlQoS, ID_M_PLBL10, _("Total Packets Lost"), wxPoint(400,5), wxSize(195,20), 0, _T("ID_M_PLBL10"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSTime = new wmLabel(pnlQoS, ID_M_PLBL11, wxEmptyString, wxPoint(5,26), wxSize(194,25), 0, _T("ID_M_PLBL11"));
	m_plblQoSTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSTime->GetUiRect().SetGradient(0);
	m_plblQoSTime->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSTime->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSTimeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSTime->SetFont(m_plblQoSTimeFont);
	m_plblQoSReceived = new wmLabel(pnlQoS, ID_M_PLBL13, wxEmptyString, wxPoint(200,26), wxSize(199,25), 0, _T("ID_M_PLBL13"));
	m_plblQoSReceived->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSReceived->GetUiRect().SetGradient(0);
	m_plblQoSReceived->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSReceived->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSReceivedFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSReceived->SetFont(m_plblQoSReceivedFont);
	m_plblQoSLost = new wmLabel(pnlQoS, ID_M_PLBL12, wxEmptyString, wxPoint(400,26), wxSize(195,25), 0, _T("ID_M_PLBL12"));
	m_plblQoSLost->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSLost->GetUiRect().SetGradient(0);
	m_plblQoSLost->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSLost->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSLostFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSLost->SetFont(m_plblQoSLostFont);
	m_pLbl8 = new wmLabel(pnlQoS, ID_M_PLBL14, _("kBits/s Min"), wxPoint(5,60), wxSize(194,20), 0, _T("ID_M_PLBL14"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl9 = new wmLabel(pnlQoS, ID_M_PLBL15, _("kBits/s Average"), wxPoint(200,60), wxSize(199,20), 0, _T("ID_M_PLBL15"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl10 = new wmLabel(pnlQoS, ID_M_PLBL16, _("kBits/s Max"), wxPoint(400,60), wxSize(195,20), 0, _T("ID_M_PLBL16"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(0);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSKbMin = new wmLabel(pnlQoS, ID_M_PLBL17, wxEmptyString, wxPoint(5,81), wxSize(194,25), 0, _T("ID_M_PLBL17"));
	m_plblQoSKbMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbMin->GetUiRect().SetGradient(0);
	m_plblQoSKbMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbMin->SetFont(m_plblQoSKbMinFont);
	m_plblQoSKbAv = new wmLabel(pnlQoS, ID_M_PLBL18, wxEmptyString, wxPoint(200,81), wxSize(199,25), 0, _T("ID_M_PLBL18"));
	m_plblQoSKbAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbAv->GetUiRect().SetGradient(0);
	m_plblQoSKbAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbAv->SetFont(m_plblQoSKbAvFont);
	m_plblQoSKbMax = new wmLabel(pnlQoS, ID_M_PLBL19, wxEmptyString, wxPoint(400,81), wxSize(195,25), 0, _T("ID_M_PLBL19"));
	m_plblQoSKbMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbMax->GetUiRect().SetGradient(0);
	m_plblQoSKbMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbMax->SetFont(m_plblQoSKbMaxFont);
	m_pLbl11 = new wmLabel(pnlQoS, ID_M_PLBL20, _("Packets Lost/s Min"), wxPoint(5,115), wxSize(194,20), 0, _T("ID_M_PLBL20"));
	m_pLbl11->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl11->GetUiRect().SetGradient(0);
	m_pLbl11->SetForegroundColour(wxColour(255,255,255));
	m_pLbl11->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl12 = new wmLabel(pnlQoS, ID_M_PLBL21, _("Packets Lost/s Average"), wxPoint(200,115), wxSize(199,20), 0, _T("ID_M_PLBL21"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl13 = new wmLabel(pnlQoS, ID_M_PLBL22, _("Packets Lost/s Max"), wxPoint(400,115), wxSize(195,20), 0, _T("ID_M_PLBL22"));
	m_pLbl13->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl13->GetUiRect().SetGradient(0);
	m_pLbl13->SetForegroundColour(wxColour(255,255,255));
	m_pLbl13->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSPacketsMin = new wmLabel(pnlQoS, ID_M_PLBL23, wxEmptyString, wxPoint(5,136), wxSize(194,25), 0, _T("ID_M_PLBL23"));
	m_plblQoSPacketsMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsMin->GetUiRect().SetGradient(0);
	m_plblQoSPacketsMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsMin->SetFont(m_plblQoSPacketsMinFont);
	m_plblQoSPacketsAv = new wmLabel(pnlQoS, ID_M_PLBL24, wxEmptyString, wxPoint(200,136), wxSize(199,25), 0, _T("ID_M_PLBL24"));
	m_plblQoSPacketsAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsAv->GetUiRect().SetGradient(0);
	m_plblQoSPacketsAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsAv->SetFont(m_plblQoSPacketsAvFont);
	m_plblQoSPacketsMax = new wmLabel(pnlQoS, ID_M_PLBL25, wxEmptyString, wxPoint(400,136), wxSize(195,25), 0, _T("ID_M_PLBL25"));
	m_plblQoSPacketsMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsMax->GetUiRect().SetGradient(0);
	m_plblQoSPacketsMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsMax->SetFont(m_plblQoSPacketsMaxFont);
	m_pLbl14 = new wmLabel(pnlQoS, ID_M_PLBL26, _("Inter Packet Gap Min"), wxPoint(5,170), wxSize(194,20), 0, _T("ID_M_PLBL26"));
	m_pLbl14->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl14->GetUiRect().SetGradient(0);
	m_pLbl14->SetForegroundColour(wxColour(255,255,255));
	m_pLbl14->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl15 = new wmLabel(pnlQoS, ID_M_PLBL27, _("Inter Packet Gap Average"), wxPoint(200,170), wxSize(199,20), 0, _T("ID_M_PLBL27"));
	m_pLbl15->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl15->GetUiRect().SetGradient(0);
	m_pLbl15->SetForegroundColour(wxColour(255,255,255));
	m_pLbl15->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl16 = new wmLabel(pnlQoS, ID_M_PLBL28, _("Inter Packet Gap Max"), wxPoint(400,170), wxSize(195,20), 0, _T("ID_M_PLBL28"));
	m_pLbl16->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl16->GetUiRect().SetGradient(0);
	m_pLbl16->SetForegroundColour(wxColour(255,255,255));
	m_pLbl16->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSInterMin = new wmLabel(pnlQoS, ID_M_PLBL29, wxEmptyString, wxPoint(5,191), wxSize(194,25), 0, _T("ID_M_PLBL29"));
	m_plblQoSInterMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterMin->GetUiRect().SetGradient(0);
	m_plblQoSInterMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterMin->SetFont(m_plblQoSInterMinFont);
	m_plblQoSInterAv = new wmLabel(pnlQoS, ID_M_PLBL30, wxEmptyString, wxPoint(200,191), wxSize(199,25), 0, _T("ID_M_PLBL30"));
	m_plblQoSInterAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterAv->GetUiRect().SetGradient(0);
	m_plblQoSInterAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterAv->SetFont(m_plblQoSInterAvFont);
	m_plblQoSInterMax = new wmLabel(pnlQoS, ID_M_PLBL31, wxEmptyString, wxPoint(400,191), wxSize(195,25), 0, _T("ID_M_PLBL31"));
	m_plblQoSInterMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterMax->GetUiRect().SetGradient(0);
	m_plblQoSInterMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterMax->SetFont(m_plblQoSInterMaxFont);
	m_pLbl17 = new wmLabel(pnlQoS, ID_M_PLBL32, _("Jitter"), wxPoint(5,225), wxSize(194,20), 0, _T("ID_M_PLBL32"));
	m_pLbl17->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl17->GetUiRect().SetGradient(0);
	m_pLbl17->SetForegroundColour(wxColour(255,255,255));
	m_pLbl17->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl39 = new wmLabel(pnlQoS, ID_M_PLBL85, _("Current Graph"), wxPoint(400,225), wxSize(194,20), 0, _T("ID_M_PLBL85"));
	m_pLbl39->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl39->GetUiRect().SetGradient(0);
	m_pLbl39->SetForegroundColour(wxColour(255,255,255));
	m_pLbl39->SetBackgroundColour(wxColour(0,128,64));
	m_plblQoSJitter = new wmLabel(pnlQoS, ID_M_PLBL35, wxEmptyString, wxPoint(5,246), wxSize(194,25), 0, _T("ID_M_PLBL35"));
	m_plblQoSJitter->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSJitter->GetUiRect().SetGradient(0);
	m_plblQoSJitter->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSJitter->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSJitterFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSJitter->SetFont(m_plblQoSJitterFont);
	m_pLbl45 = new wmLabel(pnlQoS, ID_M_PLBL89, _("TS-DF"), wxPoint(200,225), wxSize(194,20), 0, _T("ID_M_PLBL89"));
	m_pLbl45->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl45->GetUiRect().SetGradient(0);
	m_pLbl45->SetForegroundColour(wxColour(255,255,255));
	m_pLbl45->SetBackgroundColour(wxColour(0,0,255));
	m_plblTSDF = new wmLabel(pnlQoS, ID_M_PLBL90, wxEmptyString, wxPoint(200,246), wxSize(194,25), 0, _T("ID_M_PLBL90"));
	m_plblTSDF->SetBorderState(uiRect::BORDER_NONE);
	m_plblTSDF->GetUiRect().SetGradient(0);
	m_plblTSDF->SetForegroundColour(wxColour(0,128,0));
	m_plblTSDF->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTSDFFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTSDF->SetFont(m_plblTSDFFont);
	m_plblGraph = new wmLabel(pnlQoS, ID_M_PLBL86, wxEmptyString, wxPoint(400,246), wxSize(194,25), 0, _T("ID_M_PLBL86"));
	m_plblGraph->SetBorderState(uiRect::BORDER_NONE);
	m_plblGraph->GetUiRect().SetGradient(0);
	m_plblGraph->SetForegroundColour(wxColour(0,128,0));
	m_plblGraph->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblGraphFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblGraph->SetFont(m_plblGraphFont);
	Panel1 = new wxPanel(pnlQoS, ID_PANEL4, wxPoint(0,280), wxSize(600,160), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_pGraph = new LevelGraph(Panel1,ID_CUSTOM12, wxPoint(0,0),wxSize(600,160),1,10,0);
	pnlSDP = new wxPanel(m_pswpInfo, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	pnlSDP->SetBackgroundColour(wxColour(0,0,0));
	m_ptxtSDP = new wxTextCtrl(pnlSDP, ID_TEXTCTRL1, wxEmptyString, wxPoint(5,5), wxSize(590,435), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	m_pswpInfo->AddPage(pnlSession, _("Session"), false);
	m_pswpInfo->AddPage(pnlSubsession, _("Active Subsession"), false);
	m_pswpInfo->AddPage(pnlQoS, _("QoS"), false);
	m_pswpInfo->AddPage(pnlSDP, _("Raw SDP"), false);
	//*)

	#ifdef PTPMONKEY
	wxPtp::Get().AddHandler(this);

	Connect(wxID_ANY, wxEVT_CLOCK_MASTER, (wxObjectEventFunction)&pnlAoIPInfo::OnPtpEvent);
	Connect(wxID_ANY, wxEVT_CLOCK_SLAVE, (wxObjectEventFunction)&pnlAoIPInfo::OnPtpEvent);
    Connect(wxID_ANY, wxEVT_CLOCK_UPDATED, (wxObjectEventFunction)&pnlAoIPInfo::OnPtpEvent);
    #endif // PTPMONKEY

	m_plblEpoch = new wmLabel(pnlSubsession, wxNewId(), wxEmptyString, wxPoint(305,151), wxSize(249,25), 0, _T("ID_M_PLBL57"));
	m_plblEpoch->SetBorderState(uiRect::BORDER_NONE);
	m_plblEpoch->GetUiRect().SetGradient(0);
	m_plblEpoch->SetForegroundColour(wxColour(0,128,0));
	m_plblEpoch->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblEpochFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblEpoch->SetFont(m_plblEpochFont);

    m_pswpInfo->SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_pGraph->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));

    ClearGraphs();

	m_pGraph->AddGraph(wxT("kBit/s"), wxColour(0,255,0));
	m_pGraph->ShowGraph(wxT("kBit/s"), false);
	m_pGraph->ShowRange(wxT("kBit/s"), true);
	m_pGraph->SetLimit(wxT("kBit/s"), 2310.0, 2300.0);

	m_pGraph->AddGraph(wxT("TS-DF"), wxColour(255,255,255));
	m_pGraph->ShowGraph(wxT("TS-DF"), false);
	m_pGraph->ShowRange(wxT("TS-DF"), true);
	m_pGraph->SetLimit(wxT("TS-DF"), 0, 1);

	m_pGraph->AddGraph(wxT("Packet Gap"), wxColour(0,0,255));
	m_pGraph->ShowGraph(wxT("Packet Gap"), false);
	m_pGraph->ShowRange(wxT("Packet Gap"), true);
	m_pGraph->SetLimit(wxT("Packet Gap"), 1, 0.1);


	m_pGraph->AddGraph(wxT("Packet Loss"), wxColour(0,255,0));
	m_pGraph->ShowGraph(wxT("Packet Loss"), false);
	m_pGraph->ShowRange(wxT("Packet Loss"), true);
	m_pGraph->SetLimit(wxT("Packet Loss"), 2310.0, 2300.0);

	m_pGraph->AddGraph(wxT("Jitter"), wxColour(0,0,255));
	m_pGraph->ShowGraph(wxT("Jitter"), false);
	m_pGraph->ShowRange(wxT("Jitter"), true);
	m_pGraph->SetLimit(wxT("Jitter"), 1, 0.1);

	m_pGraph->AddGraph(wxT("Timestamp"), wxColour(0,0,255));
	m_pGraph->ShowGraph(wxT("Timestamp"), false);
	m_pGraph->ShowRange(wxT("Timestamp"), false);
	m_pGraph->SetLimit(wxT("Timestamp"), 1,0);

    m_pGraph->AddGraph(wxT("Timestamp Errors"), wxColour(255,0,0));
	m_pGraph->ShowGraph(wxT("Timestamp Errors"), false);
	m_pGraph->ShowRange(wxT("Timestamp Errors"), true);
	m_pGraph->SetLimit(wxT("Timestamp Errors"), 1, 0.1);

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
    m_plblQoSTime->SetLabel(pData->tsTime.Format(wxT("%H:%M:%S:%l")));
    m_plblQoSKbAv->SetLabel(wxString::Format(wxT("%f"), pData->dkbits_per_second_Av));
    m_plblQoSKbMax->SetLabel(wxString::Format(wxT("%f"), pData->dkbits_per_second_max));
    m_plblQoSKbMin->SetLabel(wxString::Format(wxT("%f"), pData->dkbits_per_second_min));
    m_plblQoSLost->SetLabel(wxString::Format(wxT("%d"), pData->nTotNumPacketsLost));
    m_plblQoSPacketsAv->SetLabel(wxString::Format(wxT("%.3f"), pData->dPacket_loss_fraction_av));
    m_plblQoSPacketsMax->SetLabel(wxString::Format(wxT("%.3f"), pData->dPacket_loss_fraction_max));
    m_plblQoSPacketsMin->SetLabel(wxString::Format(wxT("%.3f"), pData->dPacket_loss_fraction_min));
    m_plblQoSReceived->SetLabel(wxString::Format(wxT("%d"), pData->nTotNumPacketsReceived));

    m_plblQoSInterMin->SetLabel(wxString::Format(wxT("%f ms"), pData->dInter_packet_gap_ms_min));
    m_plblQoSInterAv->SetLabel(wxString::Format(wxT("%f ms"), pData->dInter_packet_gap_ms_av));
    m_plblQoSInterMax->SetLabel(wxString::Format(wxT("%f ms"), pData->dInter_packet_gap_ms_max));

    m_plblQoSJitter->SetLabel(wxString::Format(wxT("%f ms"),pData->dJitter));
    m_plblTSDF->SetLabel(wxString::Format(wxT("%f"), pData->dTSDF));


    m_dKbps[GRAPH_MIN] = std::min(pData->dkbits_per_second_Now, m_dKbps[GRAPH_MIN]);
    m_dKbps[GRAPH_MAX] = std::max(pData->dkbits_per_second_Now, m_dKbps[GRAPH_MAX]);

    m_dJitter[GRAPH_MIN] = std::min(pData->dJitter, m_dJitter[GRAPH_MIN]);
    m_dJitter[GRAPH_MAX] = std::max(pData->dJitter, m_dJitter[GRAPH_MAX]);

    m_dGap[GRAPH_MIN] = std::min(pData->dInter_packet_gap_ms_Now, m_dGap[GRAPH_MIN]);
    m_dGap[GRAPH_MAX] = std::max(pData->dInter_packet_gap_ms_Now, m_dGap[GRAPH_MAX]);

    m_dLoss[GRAPH_MIN] = std::min(pData->dPacket_loss_fraction_av, m_dLoss[GRAPH_MIN]);
    m_dLoss[GRAPH_MAX] = std::max(pData->dPacket_loss_fraction_av, m_dLoss[GRAPH_MAX]);

    m_dTSDF[GRAPH_MIN] = std::min(pData->dTSDF, m_dTSDF[GRAPH_MIN]);
    m_dTSDF[GRAPH_MAX] = std::max(pData->dTSDF, m_dTSDF[GRAPH_MAX]);

    m_nTimestampErrors[GRAPH_MIN] = std::min(pData->nTimestampErrors, m_nTimestampErrors[GRAPH_MIN]);
    m_nTimestampErrors[GRAPH_MAX] = std::max(pData->nTimestampErrors, m_nTimestampErrors[GRAPH_MAX]);

    m_pGraph->SetLimit(wxT("kBit/s"), m_dKbps[GRAPH_MAX], m_dKbps[GRAPH_MIN]);
    m_pGraph->AddPeak(wxT("kBit/s"), pData->dkbits_per_second_Now);

    m_pGraph->SetLimit(wxT("Packet Gap"),m_dGap[GRAPH_MAX], m_dGap[GRAPH_MIN]);
    m_pGraph->AddPeak(wxT("Packet Gap"), pData->dInter_packet_gap_ms_Now);

    m_pGraph->SetLimit(wxT("Packet Loss"), m_dLoss[GRAPH_MAX], m_dLoss[GRAPH_MIN]);
    m_pGraph->AddPeak(wxT("Packet Loss"), pData->dPacket_loss_fraction_av);


    m_pGraph->SetLimit(wxT("Jitter"), m_dJitter[GRAPH_MAX], m_dJitter[GRAPH_MIN]);
    m_pGraph->AddPeak(wxT("Jitter"), pData->dJitter);

    m_pGraph->SetLimit(wxT("TS-DF"), m_dTSDF[GRAPH_MAX], m_dTSDF[GRAPH_MIN]);
    m_pGraph->AddPeak(wxT("TS-DF"), pData->dTSDF);

    m_pGraph->SetLimit(wxT("Timestamp Errors"), 10,0);
    m_pGraph->AddPeak(wxT("Timestamp Errors"), pData->nTimestampErrors);

}


void pnlAoIPInfo::SetAudioData(const timedbuffer* pTimedBuffer)
{
    SetTimestamp(pTimedBuffer->GetTransmissionTime(), m_plblTransmissionTime, false);
    SetTimestamp(pTimedBuffer->GetTimeVal(), m_plblTimestampIn);
    SetTimestamp(pTimedBuffer->GetPlaybackTime(), m_plblTimestampOut);
    m_plblCurrentTimestamp->SetLabel(wxString::Format(wxT("%u"), pTimedBuffer->GetTimestamp()));
    ShowLatency(pTimedBuffer);

    m_plblFrameSize->SetLabel(wxString::Format(wxT("%d bytes"), pTimedBuffer->GetDuration()));

    m_dFrameDuration = static_cast<double>(pTimedBuffer->GetDuration())/static_cast<double>(m_nSampleRate*m_nFrameSize)*1e6;

    m_plblFrameDuration->SetLabel(wxString::Format(wxT("%.2f us"), m_dFrameDuration));

    m_plblPlaybackQueue->SetLabel(wxString::Format(wxT("%d"), pTimedBuffer->GetBufferDepth()));

    m_pGraph->SetLimit("Timestamp", 1, 0);
    double dTimestamp(static_cast<double>(pTimedBuffer->GetTimestamp())/4294967296.0);
    m_pGraph->AddPeak("Timestamp",dTimestamp);//static_cast<double>(pTimedBuffer->GetTimestamp())/2e32);

    #ifdef PTPMONKEY
    m_plblTransmissionTime->SetBackgroundColour(wxPtp::Get().IsSyncedToMaster(0) ? *wxWHITE : wxColour(255,100,100));
    m_plblTimestampIn->SetBackgroundColour(wxPtp::Get().IsSyncedToMaster(0) ? *wxWHITE : wxColour(255,100,100));
    m_plblLatencyNetwork->SetBackgroundColour(wxPtp::Get().IsSyncedToMaster(0) ? *wxWHITE : wxColour(255,100,100));
    #endif // PTPMONKEY
}



void pnlAoIPInfo::SetTimestamp(const pairTime_t& tv, wmLabel* pLabel, bool bDate)
{
    wxDateTime dt(time_t(tv.first));
    if(!bDate)
    {
        pLabel->SetLabel(wxString::Format(wxT("%s:%03d"), dt.Format(wxT("%H:%M:%S")).c_str(), tv.second/1000));
    }
    else
    {
        pLabel->SetLabel(wxString::Format(wxT("%s:%03d"), dt.Format(wxT("%Y-%m-%d %H:%M:%S")).c_str(), tv.second/1000));
    }
}


void pnlAoIPInfo::ShowLatency(const timedbuffer* pTimedBuffer)
{
    double dPlayback = pTimedBuffer->GetPlaybackLatency();
    double dTransmission = static_cast<double>(pTimedBuffer->GetTransmissionTime().second) + (static_cast<double>(pTimedBuffer->GetTransmissionTime().first)*1000000.0);
    double dPresentation = static_cast<double>(pTimedBuffer->GetTimeVal().second) + (static_cast<double>(pTimedBuffer->GetTimeVal().first)*1000000.0);

    dTransmission += m_dFrameDuration;   //we add the duration on because the transmission time is first sample not last sample of frane
    m_plblLatency->SetLabel(wxString::Format(wxT("%.0f us"), dPlayback));//+(dPresentation-dTransmission)));
    m_plblLatencyNetwork->SetLabel(wxString::Format(wxT("%.0f us"), (dPresentation-dTransmission)));
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
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxEmptyString) == wxT("AoIP"))
    {
        m_plblInput->SetLabel(Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString));
    }
    else
    {
        m_plblInput->SetLabel(wxEmptyString);
    }
    m_plblSyncType->SetLabel(aSession.refClock.sType);
    m_plblSyncVersion->SetLabel(aSession.refClock.sVersion);
    m_plblSyncId->SetLabel(aSession.refClock.sId);


    m_plblSyncDomain->SetLabel(wxString::Format(wxT("%u"), aSession.refClock.nDomain));
    m_plblSessionType->SetLabel(aSession.sType);
    m_ptxtSDP->SetValue(aSession.sRawSDP);

    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_plblSubsessionId->SetLabel(aSession.GetCurrentSubsession()->sId);
        m_plblSessionSource->SetLabel(aSession.GetCurrentSubsession()->sSourceAddress);
        m_plblSessionBits->SetLabel(aSession.GetCurrentSubsession()->sCodec);
        m_plblSessionFrequency->SetLabel(wxString::Format(wxT("%.1fkHz"), aSession.GetCurrentSubsession()->nSampleRate/1000.0));
        m_plblSessionChannels->SetLabel(wxString::Format(wxT("%d"), aSession.GetCurrentSubsession()->nChannels));

        m_plblSyncTimestamp->SetLabel(wxString::Format(wxT("%u"), aSession.GetCurrentSubsession()->nSyncTimestamp));

        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
        m_nFrameSize = min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
        if(aSession.GetCurrentSubsession()->sCodec == wxT("L24"))
        {
            m_nFrameSize*=3;
        }
        else if(aSession.GetCurrentSubsession()->sCodec == wxT("L16"))
        {
            m_nFrameSize*=2;
        }
        else if(aSession.GetCurrentSubsession()->sCodec == wxT("F32"))
        {
            m_nFrameSize*=4;
        }

        m_plblSubSyncType->SetLabel(aSession.GetCurrentSubsession()->refClock.sType);
        m_plblSubSyncVersion->SetLabel(aSession.GetCurrentSubsession()->refClock.sVersion);
        m_plblSubSyncId->SetLabel(aSession.GetCurrentSubsession()->refClock.sId);
        #ifdef PTPMONKEY
        if(m_plblSubSyncId->GetLabel().MakeLower() == wxPtp::Get().GetMasterClockId(0))
        {
            m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,255));
        }
        else
        {
            m_plblSubSyncId->SetBackgroundColour(wxColour(255,100,100));
        }
        #else
	    m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,100));
	#endif
        m_plblSubSyncDomain->SetLabel(wxString::Format(wxT("%u"), aSession.GetCurrentSubsession()->refClock.nDomain));



    }
    else
    {
        m_plblSessionSource->SetLabel(wxEmptyString);
        m_plblSessionBits->SetLabel(wxEmptyString);
        m_plblSessionFrequency->SetLabel(wxEmptyString);
        m_plblSessionChannels->SetLabel(wxEmptyString);
        m_plblSessionType->SetLabel(wxEmptyString);
        m_plblSyncTimestamp->SetLabel(wxEmptyString);
    }

    unsigned int nAudio(0), nVideo(0);
    for(list<subsession>::const_iterator itSub = aSession.lstSubsession.begin(); itSub != aSession.lstSubsession.end(); ++itSub)
    {
        if(itSub->sMedium.CmpNoCase(wxT("audio")) == 0)
        {
            nAudio++;
        }
        else if(itSub->sMedium.CmpNoCase(wxT("video")) == 0)
        {
            nVideo++;
        }
    }
    m_plblSubsessionsAudio->SetLabel(wxString::Format(wxT("%u"), nAudio));
    m_plblSubsessionVideo->SetLabel(wxString::Format(wxT("%u"), nVideo));
    m_plblGroups->SetLabel(aSession.sGroups);


}


void pnlAoIPInfo::ShowGraph(const wxString& sGraph)
{
    m_pGraph->HideAllGraphs();
    m_pGraph->ShowGraph(sGraph);
    m_plblGraph->SetLabel(sGraph);
}

void pnlAoIPInfo::ClearGraphs()
{
    m_dKbps[GRAPH_MIN] = 0xFFFFFF;
    m_dKbps[GRAPH_MAX] = -1;

    m_dJitter[GRAPH_MIN] = 0xFFFFFF;;
    m_dJitter[GRAPH_MAX] = -1;

    m_dGap[GRAPH_MIN] = 0xFFFFFF;
    m_dGap[GRAPH_MAX] = -1;

    m_dLoss[GRAPH_MIN] = 0xFFFFFF;;
    m_dLoss[GRAPH_MAX] = -1;

    m_dTSDF[GRAPH_MIN] = 0xFFFFFF;;
    m_dTSDF[GRAPH_MAX] = -1;

    m_nTimestampErrors[GRAPH_MIN] = 0xFFFFFFFF;
    m_nTimestampErrors[GRAPH_MAX] = -1;

    m_pGraph->ClearGraphs();
}

void pnlAoIPInfo::OnPtpEvent(wxCommandEvent& event)
{
    #ifdef PTPMONKEY
    if(m_plblSubSyncId->GetLabel().MakeLower() == wxPtp::Get().GetMasterClockId(0))
    {
        m_plblSubSyncId->SetBackgroundColour(wxColour(255,255,255));
    }
    else
    {
        m_plblSubSyncId->SetBackgroundColour(wxColour(255,100,100));
    }
    #endif
}
