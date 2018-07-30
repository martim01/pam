#include "pnlAoIPInfo.h"

//(*InternalHeaders(pnlAoIPInfo)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlAoIPInfo)
const long pnlAoIPInfo::ID_M_PLBL3 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL59 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL38 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL39 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL44 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL45 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL46 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL40 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL60 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL41 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL42 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL47 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL48 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL49 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL52 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL54 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL61 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL55 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL57 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL62 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL43 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL50 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL1 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL51 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL2 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL53 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL4 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL56 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL5 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL6 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL7 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL58 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL8 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL9 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL10 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL11 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL12 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL13 = wxNewId();
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
const long pnlAoIPInfo::ID_M_PLBL33 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL34 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL35 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL36 = wxNewId();
const long pnlAoIPInfo::ID_M_PLBL37 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlAoIPInfo,wxPanel)
	//(*EventTable(pnlAoIPInfo)
	//*)
END_EVENT_TABLE()

pnlAoIPInfo::pnlAoIPInfo(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlAoIPInfo)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("AoIP Information"), wxPoint(0,0), wxSize(600,25), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(61,146,146));
	wxFont m_pLbl3Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_pLbl33 = new wmLabel(this, ID_M_PLBL59, _("Input"), wxPoint(0,25), wxSize(149,20), 0, _T("ID_M_PLBL59"));
	m_pLbl33->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl33->SetForegroundColour(wxColour(255,255,255));
	m_pLbl33->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl20 = new wmLabel(this, ID_M_PLBL38, _("Session Name"), wxPoint(150,25), wxSize(149,20), 0, _T("ID_M_PLBL38"));
	m_pLbl20->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl20->SetForegroundColour(wxColour(255,255,255));
	m_pLbl20->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl23 = new wmLabel(this, ID_M_PLBL39, _("Source"), wxPoint(450,25), wxSize(150,20), 0, _T("ID_M_PLBL39"));
	m_pLbl23->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl23->SetForegroundColour(wxColour(255,255,255));
	m_pLbl23->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl25 = new wmLabel(this, ID_M_PLBL44, _("Bits"), wxPoint(0,80), wxSize(49,20), 0, _T("ID_M_PLBL44"));
	m_pLbl25->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl25->SetForegroundColour(wxColour(255,255,255));
	m_pLbl25->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl26 = new wmLabel(this, ID_M_PLBL45, _("Freq"), wxPoint(105,80), wxSize(79,20), 0, _T("ID_M_PLBL45"));
	m_pLbl26->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl26->SetForegroundColour(wxColour(255,255,255));
	m_pLbl26->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl27 = new wmLabel(this, ID_M_PLBL46, _("Channels"), wxPoint(290,80), wxSize(59,20), 0, _T("ID_M_PLBL46"));
	m_pLbl27->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl27->SetForegroundColour(wxColour(255,255,255));
	m_pLbl27->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl24 = new wmLabel(this, ID_M_PLBL40, _("Session Type"), wxPoint(300,25), wxSize(149,20), 0, _T("ID_M_PLBL40"));
	m_pLbl24->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl24->SetForegroundColour(wxColour(255,255,255));
	m_pLbl24->SetBackgroundColour(wxColour(0,0,255));
	m_plblInput = new wmLabel(this, ID_M_PLBL60, wxEmptyString, wxPoint(0,46), wxSize(149,25), 0, _T("ID_M_PLBL60"));
	m_plblInput->SetBorderState(uiRect::BORDER_NONE);
	m_plblInput->SetForegroundColour(wxColour(0,128,0));
	m_plblInput->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblInputFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblInput->SetFont(m_plblInputFont);
	m_plblSessionName = new wmLabel(this, ID_M_PLBL41, wxEmptyString, wxPoint(150,46), wxSize(149,25), 0, _T("ID_M_PLBL41"));
	m_plblSessionName->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionName->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionName->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionNameFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionName->SetFont(m_plblSessionNameFont);
	m_plblSessionSource = new wmLabel(this, ID_M_PLBL42, wxEmptyString, wxPoint(450,46), wxSize(150,25), 0, _T("ID_M_PLBL42"));
	m_plblSessionSource->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionSource->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionSource->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionSourceFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionSource->SetFont(m_plblSessionSourceFont);
	m_plblSessionBits = new wmLabel(this, ID_M_PLBL47, wxEmptyString, wxPoint(50,80), wxSize(49,20), 0, _T("ID_M_PLBL47"));
	m_plblSessionBits->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionBits->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionBits->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionBitsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionBits->SetFont(m_plblSessionBitsFont);
	m_plblSessionFrequency = new wmLabel(this, ID_M_PLBL48, wxEmptyString, wxPoint(185,80), wxSize(100,20), 0, _T("ID_M_PLBL48"));
	m_plblSessionFrequency->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionFrequency->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionFrequency->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionFrequencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionFrequency->SetFont(m_plblSessionFrequencyFont);
	m_plblSessionChannels = new wmLabel(this, ID_M_PLBL49, wxEmptyString, wxPoint(350,80), wxSize(49,20), 0, _T("ID_M_PLBL49"));
	m_plblSessionChannels->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionChannels->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionChannels->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionChannelsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionChannels->SetFont(m_plblSessionChannelsFont);
	m_pLbl29 = new wmLabel(this, ID_M_PLBL52, _("Sync Timestamp"), wxPoint(0,110), wxSize(149,20), 0, _T("ID_M_PLBL52"));
	m_pLbl29->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl29->SetForegroundColour(wxColour(255,255,255));
	m_pLbl29->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl31 = new wmLabel(this, ID_M_PLBL54, _("Current Timestamp"), wxPoint(150,110), wxSize(149,20), 0, _T("ID_M_PLBL54"));
	m_pLbl31->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl31->SetForegroundColour(wxColour(255,255,255));
	m_pLbl31->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl34 = new wmLabel(this, ID_M_PLBL61, _("Buffer"), wxPoint(434,160), wxSize(65,20), 0, _T("ID_M_PLBL61"));
	m_pLbl34->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl34->SetForegroundColour(wxColour(255,255,255));
	m_pLbl34->SetBackgroundColour(wxColour(0,0,255));
	m_plblSyncTimestamp = new wmLabel(this, ID_M_PLBL55, wxEmptyString, wxPoint(0,131), wxSize(149,25), 0, _T("ID_M_PLBL55"));
	m_plblSyncTimestamp->SetBorderState(uiRect::BORDER_NONE);
	m_plblSyncTimestamp->SetForegroundColour(wxColour(0,128,0));
	m_plblSyncTimestamp->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSyncTimestampFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSyncTimestamp->SetFont(m_plblSyncTimestampFont);
	m_plblCurrentTimestamp = new wmLabel(this, ID_M_PLBL57, wxEmptyString, wxPoint(150,131), wxSize(149,25), 0, _T("ID_M_PLBL57"));
	m_plblCurrentTimestamp->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrentTimestamp->SetForegroundColour(wxColour(0,128,0));
	m_plblCurrentTimestamp->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblCurrentTimestampFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblCurrentTimestamp->SetFont(m_plblCurrentTimestampFont);
	m_plblDuration = new wmLabel(this, ID_M_PLBL62, wxEmptyString, wxPoint(434,181), wxSize(65,25), 0, _T("ID_M_PLBL62"));
	m_plblDuration->SetBorderState(uiRect::BORDER_NONE);
	m_plblDuration->SetForegroundColour(wxColour(0,128,0));
	m_plblDuration->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblDurationFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblDuration->SetFont(m_plblDurationFont);
	m_plblSessionType = new wmLabel(this, ID_M_PLBL43, wxEmptyString, wxPoint(300,46), wxSize(149,25), 0, _T("ID_M_PLBL43"));
	m_plblSessionType->SetBorderState(uiRect::BORDER_NONE);
	m_plblSessionType->SetForegroundColour(wxColour(0,128,0));
	m_plblSessionType->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSessionTypeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblSessionType->SetFont(m_plblSessionTypeFont);
	m_pLbl28 = new wmLabel(this, ID_M_PLBL50, _("Transmission Time"), wxPoint(300,110), wxSize(165,20), 0, _T("ID_M_PLBL50"));
	m_pLbl28->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl28->SetForegroundColour(wxColour(255,255,255));
	m_pLbl28->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Presentation Time"), wxPoint(0,160), wxSize(165,20), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,255));
	m_plblTransmissionTime = new wmLabel(this, ID_M_PLBL51, wxEmptyString, wxPoint(300,131), wxSize(165,25), 0, _T("ID_M_PLBL51"));
	m_plblTransmissionTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblTransmissionTime->SetForegroundColour(wxColour(0,128,0));
	m_plblTransmissionTime->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTransmissionTimeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTransmissionTime->SetFont(m_plblTransmissionTimeFont);
	m_plblTimestampIn = new wmLabel(this, ID_M_PLBL2, wxEmptyString, wxPoint(0,181), wxSize(165,25), 0, _T("ID_M_PLBL2"));
	m_plblTimestampIn->SetBorderState(uiRect::BORDER_NONE);
	m_plblTimestampIn->SetForegroundColour(wxColour(0,128,0));
	m_plblTimestampIn->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTimestampInFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTimestampIn->SetFont(m_plblTimestampInFont);
	m_pLbl30 = new wmLabel(this, ID_M_PLBL53, _("Network Latency"), wxPoint(166,160), wxSize(100,20), 0, _T("ID_M_PLBL53"));
	m_pLbl30->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl30->SetForegroundColour(wxColour(255,255,255));
	m_pLbl30->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL4, _("Display/Playback Time"), wxPoint(267,160), wxSize(166,20), 0, _T("ID_M_PLBL4"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,255));
	m_plblLatencyNetwork = new wmLabel(this, ID_M_PLBL56, wxEmptyString, wxPoint(166,181), wxSize(100,25), 0, _T("ID_M_PLBL56"));
	m_plblLatencyNetwork->SetBorderState(uiRect::BORDER_NONE);
	m_plblLatencyNetwork->SetForegroundColour(wxColour(155,0,0));
	m_plblLatencyNetwork->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblLatencyNetworkFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblLatencyNetwork->SetFont(m_plblLatencyNetworkFont);
	m_plblTimestampOut = new wmLabel(this, ID_M_PLBL5, wxEmptyString, wxPoint(267,181), wxSize(166,25), 0, _T("ID_M_PLBL5"));
	m_plblTimestampOut->SetBorderState(uiRect::BORDER_NONE);
	m_plblTimestampOut->SetForegroundColour(wxColour(0,128,0));
	m_plblTimestampOut->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTimestampOutFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTimestampOut->SetFont(m_plblTimestampOutFont);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL6, _("Playback Latency"), wxPoint(500,160), wxSize(100,20), 0, _T("ID_M_PLBL6"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,255));
	m_plblLatency = new wmLabel(this, ID_M_PLBL7, wxEmptyString, wxPoint(500,181), wxSize(100,25), 0, _T("ID_M_PLBL7"));
	m_plblLatency->SetBorderState(uiRect::BORDER_NONE);
	m_plblLatency->SetForegroundColour(wxColour(155,0,0));
	m_plblLatency->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblLatencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblLatency->SetFont(m_plblLatencyFont);
	m_pLbl32 = new wmLabel(this, ID_M_PLBL58, _("Quality of Service Statistics"), wxPoint(0,210), wxSize(600,25), 0, _T("ID_M_PLBL58"));
	m_pLbl32->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl32->SetForegroundColour(wxColour(255,255,255));
	m_pLbl32->SetBackgroundColour(wxColour(0,128,0));
	wxFont m_pLbl32Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl32->SetFont(m_pLbl32Font);
	m_pLbl5 = new wmLabel(this, ID_M_PLBL8, _("QoS Measurement Time"), wxPoint(0,235), wxSize(199,20), 0, _T("ID_M_PLBL8"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl6 = new wmLabel(this, ID_M_PLBL9, _("Total Packets Received"), wxPoint(200,235), wxSize(199,20), 0, _T("ID_M_PLBL9"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl7 = new wmLabel(this, ID_M_PLBL10, _("Total Packets Lost"), wxPoint(400,235), wxSize(200,20), 0, _T("ID_M_PLBL10"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSTime = new wmLabel(this, ID_M_PLBL11, wxEmptyString, wxPoint(0,256), wxSize(199,25), 0, _T("ID_M_PLBL11"));
	m_plblQoSTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSTime->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSTime->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSTimeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSTime->SetFont(m_plblQoSTimeFont);
	m_plblQoSLost = new wmLabel(this, ID_M_PLBL12, wxEmptyString, wxPoint(400,256), wxSize(200,25), 0, _T("ID_M_PLBL12"));
	m_plblQoSLost->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSLost->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSLost->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSLostFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSLost->SetFont(m_plblQoSLostFont);
	m_plblQoSReceived = new wmLabel(this, ID_M_PLBL13, wxEmptyString, wxPoint(200,256), wxSize(199,25), 0, _T("ID_M_PLBL13"));
	m_plblQoSReceived->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSReceived->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSReceived->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSReceivedFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSReceived->SetFont(m_plblQoSReceivedFont);
	m_pLbl8 = new wmLabel(this, ID_M_PLBL14, _("kBits/s Min"), wxPoint(0,285), wxSize(199,20), 0, _T("ID_M_PLBL14"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl9 = new wmLabel(this, ID_M_PLBL15, _("kBits/s Average"), wxPoint(200,285), wxSize(199,20), 0, _T("ID_M_PLBL15"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl10 = new wmLabel(this, ID_M_PLBL16, _("kBits/s Max"), wxPoint(400,285), wxSize(200,20), 0, _T("ID_M_PLBL16"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSKbMin = new wmLabel(this, ID_M_PLBL17, wxEmptyString, wxPoint(0,306), wxSize(199,25), 0, _T("ID_M_PLBL17"));
	m_plblQoSKbMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbMin->SetFont(m_plblQoSKbMinFont);
	m_plblQoSKbAv = new wmLabel(this, ID_M_PLBL18, wxEmptyString, wxPoint(200,306), wxSize(199,25), 0, _T("ID_M_PLBL18"));
	m_plblQoSKbAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbAv->SetFont(m_plblQoSKbAvFont);
	m_plblQoSKbMax = new wmLabel(this, ID_M_PLBL19, wxEmptyString, wxPoint(400,306), wxSize(200,25), 0, _T("ID_M_PLBL19"));
	m_plblQoSKbMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbMax->SetFont(m_plblQoSKbMaxFont);
	m_pLbl11 = new wmLabel(this, ID_M_PLBL20, _("Packets Lost/s Min"), wxPoint(0,335), wxSize(199,20), 0, _T("ID_M_PLBL20"));
	m_pLbl11->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl11->SetForegroundColour(wxColour(255,255,255));
	m_pLbl11->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL21, _("Packets Lost/s Average"), wxPoint(200,335), wxSize(199,20), 0, _T("ID_M_PLBL21"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl13 = new wmLabel(this, ID_M_PLBL22, _("Packets Lost/s Max"), wxPoint(400,335), wxSize(200,20), 0, _T("ID_M_PLBL22"));
	m_pLbl13->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl13->SetForegroundColour(wxColour(255,255,255));
	m_pLbl13->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSPacketsMin = new wmLabel(this, ID_M_PLBL23, wxEmptyString, wxPoint(0,356), wxSize(199,25), 0, _T("ID_M_PLBL23"));
	m_plblQoSPacketsMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsMin->SetFont(m_plblQoSPacketsMinFont);
	m_plblQoSPacketsAv = new wmLabel(this, ID_M_PLBL24, wxEmptyString, wxPoint(200,356), wxSize(199,25), 0, _T("ID_M_PLBL24"));
	m_plblQoSPacketsAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsAv->SetFont(m_plblQoSPacketsAvFont);
	m_plblQoSPacketsMax = new wmLabel(this, ID_M_PLBL25, wxEmptyString, wxPoint(400,356), wxSize(200,25), 0, _T("ID_M_PLBL25"));
	m_plblQoSPacketsMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsMax->SetFont(m_plblQoSPacketsMaxFont);
	m_pLbl14 = new wmLabel(this, ID_M_PLBL26, _("Inter Packet Gap Min"), wxPoint(0,385), wxSize(199,20), 0, _T("ID_M_PLBL26"));
	m_pLbl14->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl14->SetForegroundColour(wxColour(255,255,255));
	m_pLbl14->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl15 = new wmLabel(this, ID_M_PLBL27, _("Inter Packet Gap Average"), wxPoint(200,385), wxSize(199,20), 0, _T("ID_M_PLBL27"));
	m_pLbl15->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl15->SetForegroundColour(wxColour(255,255,255));
	m_pLbl15->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl16 = new wmLabel(this, ID_M_PLBL28, _("Inter Packet Gap Max"), wxPoint(400,385), wxSize(200,20), 0, _T("ID_M_PLBL28"));
	m_pLbl16->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl16->SetForegroundColour(wxColour(255,255,255));
	m_pLbl16->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSInterMin = new wmLabel(this, ID_M_PLBL29, wxEmptyString, wxPoint(0,406), wxSize(199,25), 0, _T("ID_M_PLBL29"));
	m_plblQoSInterMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterMin->SetFont(m_plblQoSInterMinFont);
	m_plblQoSInterAv = new wmLabel(this, ID_M_PLBL30, wxEmptyString, wxPoint(200,406), wxSize(199,25), 0, _T("ID_M_PLBL30"));
	m_plblQoSInterAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterAv->SetFont(m_plblQoSInterAvFont);
	m_plblQoSInterMax = new wmLabel(this, ID_M_PLBL31, wxEmptyString, wxPoint(400,406), wxSize(200,25), 0, _T("ID_M_PLBL31"));
	m_plblQoSInterMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterMax->SetFont(m_plblQoSInterMaxFont);
	m_pLbl17 = new wmLabel(this, ID_M_PLBL32, _("Jitter"), wxPoint(0,435), wxSize(199,20), 0, _T("ID_M_PLBL32"));
	m_pLbl17->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl17->SetForegroundColour(wxColour(255,255,255));
	m_pLbl17->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl18 = new wmLabel(this, ID_M_PLBL33, wxEmptyString, wxPoint(200,435), wxSize(199,20), 0, _T("ID_M_PLBL33"));
	m_pLbl18->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl18->SetForegroundColour(wxColour(255,255,255));
	m_pLbl18->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl19 = new wmLabel(this, ID_M_PLBL34, wxEmptyString, wxPoint(400,435), wxSize(200,20), 0, _T("ID_M_PLBL34"));
	m_pLbl19->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl19->SetForegroundColour(wxColour(255,255,255));
	m_pLbl19->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSJitter = new wmLabel(this, ID_M_PLBL35, wxEmptyString, wxPoint(0,456), wxSize(199,25), 0, _T("ID_M_PLBL35"));
	m_plblQoSJitter->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSJitter->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSJitter->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSJitterFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSJitter->SetFont(m_plblQoSJitterFont);
	m_pLbl21 = new wmLabel(this, ID_M_PLBL36, wxEmptyString, wxPoint(200,456), wxSize(199,25), 0, _T("ID_M_PLBL36"));
	m_pLbl21->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl21->SetForegroundColour(wxColour(0,128,0));
	m_pLbl21->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl21Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_pLbl21->SetFont(m_pLbl21Font);
	m_pLbl22 = new wmLabel(this, ID_M_PLBL37, wxEmptyString, wxPoint(400,456), wxSize(200,25), 0, _T("ID_M_PLBL37"));
	m_pLbl22->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl22->SetForegroundColour(wxColour(0,128,0));
	m_pLbl22->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl22Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_pLbl22->SetFont(m_pLbl22Font);
	//*)
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

    m_plblQoSJitter->SetLabel(wxString::Format(wxT("%d"),pData->nJitter));

}


void pnlAoIPInfo::AddData(const timedbuffer* pTimedBuffer, unsigned int nFrameSize)
{
    SetTimestamp(pTimedBuffer->GetTransmissionTime(), m_plblTransmissionTime);
    SetTimestamp(pTimedBuffer->GetTimeVal(), m_plblTimestampIn);
    SetTimestamp(pTimedBuffer->GetPlaybackTime(), m_plblTimestampOut);
    m_plblCurrentTimestamp->SetLabel(wxString::Format(wxT("%u"), pTimedBuffer->GetTimestamp()));
    ShowLatency(pTimedBuffer);

    m_plblDuration->SetLabel(wxString::Format(wxT("%u"), pTimedBuffer->GetBufferDepth()));
}



void pnlAoIPInfo::SetTimestamp(const pairTime_t& tv, wmLabel* pLabel)
{
    wxDateTime dt(time_t(tv.first));
    pLabel->SetLabel(wxString::Format(wxT("%s:%03d"), dt.Format(wxT("%H:%M:%S")).c_str(), tv.second/1000));
}


void pnlAoIPInfo::ShowLatency(const timedbuffer* pTimedBuffer)
{
    double dPlayback = static_cast<double>(pTimedBuffer->GetPlaybackTime().second)/1000000.0 + static_cast<double>(pTimedBuffer->GetPlaybackTime().first);
    double dTransmission = static_cast<double>(pTimedBuffer->GetTransmissionTime().second)/1000000.0 + static_cast<double>(pTimedBuffer->GetTransmissionTime().first);
    double dPresentation = static_cast<double>(pTimedBuffer->GetTimeVal().second)/1000000.0 + static_cast<double>(pTimedBuffer->GetTimeVal().first);

    m_plblLatency->SetLabel(wxString::Format(wxT("%.03f s"), (dPlayback-dPresentation)));//+(dPresentation-dTransmission)));
    m_plblLatencyNetwork->SetLabel(wxString::Format(wxT("%.03f s"), (dPresentation-dTransmission)));
}


void pnlAoIPInfo::SessionStarted(session* pSession)
{
    m_plblSessionName->SetLabel(pSession->sName);
    m_plblSessionSource->SetLabel(pSession->sEndpoint);
    m_plblInput->SetLabel(Settings::Get()->Read(wxT("/input/rtp"), wxEmptyString));

    m_plblSessionBits->SetLabel(pSession->sCodec);
    m_plblSessionFrequency->SetLabel(wxString::Format(wxT("%.1fkHz"), pSession->nSampleRate/1000.0));
    m_plblSessionChannels->SetLabel(wxString::Format(wxT("%d"), pSession->nChannels));
    m_plblSessionType->SetLabel(pSession->sType);

    m_plblSyncTimestamp->SetLabel(wxString::Format(wxT("%u"), pSession->nSyncTimestamp));
}


