#include "pnlRTCPTransmission.h"
#include "settings.h"
#include "iomanager.h"
#include "RTCPTransmissionEvent.h"
#include "log.h"
#include "ondemandstreamer.h"

//(*InternalHeaders(pnlRTCPTransmission)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlRTCPTransmission)
const long pnlRTCPTransmission::ID_M_PLBL8 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL9 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL10 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL1 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL11 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL13 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL12 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL2 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL3 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL4 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL5 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL6 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL7 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL26 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL27 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL28 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL14 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL15 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL16 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL17 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL18 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL19 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL20 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL21 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL22 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL23 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL24 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL25 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL32 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL85 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL35 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL89 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL90 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL86 = wxNewId();
const long pnlRTCPTransmission::ID_CUSTOM12 = wxNewId();
const long pnlRTCPTransmission::ID_PANEL4 = wxNewId();
const long pnlRTCPTransmission::ID_PANEL2 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLST1 = wxNewId();
const long pnlRTCPTransmission::ID_PANEL1 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLST2 = wxNewId();
const long pnlRTCPTransmission::ID_PANEL3 = wxNewId();
const long pnlRTCPTransmission::ID_M_PSWP2 = wxNewId();
const long pnlRTCPTransmission::ID_PANEL6 = wxNewId();
const long pnlRTCPTransmission::ID_M_PSWP1 = wxNewId();
const long pnlRTCPTransmission::ID_M_PBTN1 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL29 = wxNewId();
const long pnlRTCPTransmission::ID_M_PLBL37 = wxNewId();
//*)

const wxString pnlRTCPTransmission::KBPS        = "KBit/s";
const wxString pnlRTCPTransmission::PLPS        = "Packets lost/s";
const wxString pnlRTCPTransmission::JITTER      = "Jitter";
const wxString pnlRTCPTransmission::DELAY       = "Roundtrip Delay";
const wxString pnlRTCPTransmission::LOSS_RATIO  = "Packet loss Ratio";

BEGIN_EVENT_TABLE(pnlRTCPTransmission,wxPanel)
	//(*EventTable(pnlRTCPTransmission)
	//*)
END_EVENT_TABLE()

pnlRTCPTransmission::pnlRTCPTransmission(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, int , const wxString&)
{
	//(*Initialize(pnlRTCPTransmission)
	Create(parent, id, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	pnlQoS = new wxPanel(this, ID_PANEL2, wxPoint(0,40), wxSize(600,440), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	pnlQoS->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl5 = new wmLabel(pnlQoS, ID_M_PLBL8, _("First RR"), wxPoint(5,5), wxSize(167,20), 0, _T("ID_M_PLBL8"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl6 = new wmLabel(pnlQoS, ID_M_PLBL9, _("Latest RR"), wxPoint(173,5), wxSize(167,20), 0, _T("ID_M_PLBL9"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl7 = new wmLabel(pnlQoS, ID_M_PLBL10, _("RR Gap"), wxPoint(341,5), wxSize(127,20), 0, _T("ID_M_PLBL10"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl1 = new wmLabel(pnlQoS, ID_M_PLBL1, _("SR RR Diff"), wxPoint(469,5), wxSize(126,20), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,255));
	m_plblRRFirst = new wmLabel(pnlQoS, ID_M_PLBL11, wxEmptyString, wxPoint(5,26), wxSize(167,25), 0, _T("ID_M_PLBL11"));
	m_plblRRFirst->SetBorderState(uiRect::BORDER_NONE);
	m_plblRRFirst->GetUiRect().SetGradient(0);
	m_plblRRFirst->SetForegroundColour(wxColour(0,128,0));
	m_plblRRFirst->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRRFirstFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRRFirst->SetFont(m_plblRRFirstFont);
	m_plblRRLast = new wmLabel(pnlQoS, ID_M_PLBL13, wxEmptyString, wxPoint(173,26), wxSize(167,25), 0, _T("ID_M_PLBL13"));
	m_plblRRLast->SetBorderState(uiRect::BORDER_NONE);
	m_plblRRLast->GetUiRect().SetGradient(0);
	m_plblRRLast->SetForegroundColour(wxColour(0,128,0));
	m_plblRRLast->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRRLastFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRRLast->SetFont(m_plblRRLastFont);
	m_plblRRGap = new wmLabel(pnlQoS, ID_M_PLBL12, wxEmptyString, wxPoint(341,26), wxSize(127,25), 0, _T("ID_M_PLBL12"));
	m_plblRRGap->SetBorderState(uiRect::BORDER_NONE);
	m_plblRRGap->GetUiRect().SetGradient(0);
	m_plblRRGap->SetForegroundColour(wxColour(0,128,0));
	m_plblRRGap->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRRGapFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRRGap->SetFont(m_plblRRGapFont);
	m_plblRRSR = new wmLabel(pnlQoS, ID_M_PLBL2, wxEmptyString, wxPoint(469,26), wxSize(126,25), 0, _T("ID_M_PLBL2"));
	m_plblRRSR->SetBorderState(uiRect::BORDER_NONE);
	m_plblRRSR->GetUiRect().SetGradient(0);
	m_plblRRSR->SetForegroundColour(wxColour(0,128,0));
	m_plblRRSR->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRRSRFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRRSR->SetFont(m_plblRRSRFont);
	m_pLbl2 = new wmLabel(pnlQoS, ID_M_PLBL3, _("First Packet"), wxPoint(5,60), wxSize(147,20), 0, _T("ID_M_PLBL3"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl3 = new wmLabel(pnlQoS, ID_M_PLBL4, _("Latest Packet"), wxPoint(153,60), wxSize(147,20), 0, _T("ID_M_PLBL4"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl4 = new wmLabel(pnlQoS, ID_M_PLBL5, _("Total Packets"), wxPoint(301,60), wxSize(147,20), 0, _T("ID_M_PLBL5"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl14 = new wmLabel(pnlQoS, ID_M_PLBL6, _("Lost Packets"), wxPoint(449,60), wxSize(146,20), 0, _T("ID_M_PLBL6"));
	m_pLbl14->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl14->GetUiRect().SetGradient(0);
	m_pLbl14->SetForegroundColour(wxColour(255,255,255));
	m_pLbl14->SetBackgroundColour(wxColour(0,0,255));
	m_plblPacketFirst = new wmLabel(pnlQoS, ID_M_PLBL7, wxEmptyString, wxPoint(5,81), wxSize(147,25), 0, _T("ID_M_PLBL7"));
	m_plblPacketFirst->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketFirst->GetUiRect().SetGradient(0);
	m_plblPacketFirst->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketFirst->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketFirstFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketFirst->SetFont(m_plblPacketFirstFont);
	m_plblPacketLatest = new wmLabel(pnlQoS, ID_M_PLBL26, wxEmptyString, wxPoint(153,81), wxSize(147,25), 0, _T("ID_M_PLBL26"));
	m_plblPacketLatest->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketLatest->GetUiRect().SetGradient(0);
	m_plblPacketLatest->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketLatest->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketLatestFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketLatest->SetFont(m_plblPacketLatestFont);
	m_plblPacketsTotal = new wmLabel(pnlQoS, ID_M_PLBL27, wxEmptyString, wxPoint(301,81), wxSize(147,25), 0, _T("ID_M_PLBL27"));
	m_plblPacketsTotal->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketsTotal->GetUiRect().SetGradient(0);
	m_plblPacketsTotal->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketsTotal->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketsTotalFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketsTotal->SetFont(m_plblPacketsTotalFont);
	m_plblPacketsLost = new wmLabel(pnlQoS, ID_M_PLBL28, wxEmptyString, wxPoint(449,81), wxSize(146,25), 0, _T("ID_M_PLBL28"));
	m_plblPacketsLost->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketsLost->GetUiRect().SetGradient(0);
	m_plblPacketsLost->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketsLost->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketsLostFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketsLost->SetFont(m_plblPacketsLostFont);
	m_pLbl8 = new wmLabel(pnlQoS, ID_M_PLBL14, _("kBits/s Min"), wxPoint(5,115), wxSize(194,20), 0, _T("ID_M_PLBL14"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl9 = new wmLabel(pnlQoS, ID_M_PLBL15, _("kBits/s Average"), wxPoint(200,115), wxSize(199,20), 0, _T("ID_M_PLBL15"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl10 = new wmLabel(pnlQoS, ID_M_PLBL16, _("kBits/s Max"), wxPoint(400,115), wxSize(195,20), 0, _T("ID_M_PLBL16"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(0);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(0,0,255));
	m_plblKbMin = new wmLabel(pnlQoS, ID_M_PLBL17, wxEmptyString, wxPoint(5,136), wxSize(194,25), 0, _T("ID_M_PLBL17"));
	m_plblKbMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblKbMin->GetUiRect().SetGradient(0);
	m_plblKbMin->SetForegroundColour(wxColour(0,128,0));
	m_plblKbMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblKbMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblKbMin->SetFont(m_plblKbMinFont);
	m_plblKbAv = new wmLabel(pnlQoS, ID_M_PLBL18, wxEmptyString, wxPoint(200,136), wxSize(199,25), 0, _T("ID_M_PLBL18"));
	m_plblKbAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblKbAv->GetUiRect().SetGradient(0);
	m_plblKbAv->SetForegroundColour(wxColour(0,128,0));
	m_plblKbAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblKbAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblKbAv->SetFont(m_plblKbAvFont);
	m_plblKbMax = new wmLabel(pnlQoS, ID_M_PLBL19, wxEmptyString, wxPoint(400,136), wxSize(195,25), 0, _T("ID_M_PLBL19"));
	m_plblKbMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblKbMax->GetUiRect().SetGradient(0);
	m_plblKbMax->SetForegroundColour(wxColour(0,128,0));
	m_plblKbMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblKbMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblKbMax->SetFont(m_plblKbMaxFont);
	m_pLbl11 = new wmLabel(pnlQoS, ID_M_PLBL20, _("Packets Lost/s Min"), wxPoint(5,170), wxSize(194,20), 0, _T("ID_M_PLBL20"));
	m_pLbl11->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl11->GetUiRect().SetGradient(0);
	m_pLbl11->SetForegroundColour(wxColour(255,255,255));
	m_pLbl11->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl12 = new wmLabel(pnlQoS, ID_M_PLBL21, _("Packets Lost/s Average"), wxPoint(200,170), wxSize(199,20), 0, _T("ID_M_PLBL21"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl13 = new wmLabel(pnlQoS, ID_M_PLBL22, _("Packets Lost/s Max"), wxPoint(400,170), wxSize(195,20), 0, _T("ID_M_PLBL22"));
	m_pLbl13->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl13->GetUiRect().SetGradient(0);
	m_pLbl13->SetForegroundColour(wxColour(255,255,255));
	m_pLbl13->SetBackgroundColour(wxColour(0,0,255));
	m_plblPacketsMin = new wmLabel(pnlQoS, ID_M_PLBL23, wxEmptyString, wxPoint(5,191), wxSize(194,25), 0, _T("ID_M_PLBL23"));
	m_plblPacketsMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketsMin->GetUiRect().SetGradient(0);
	m_plblPacketsMin->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketsMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketsMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketsMin->SetFont(m_plblPacketsMinFont);
	m_plblPacketsAv = new wmLabel(pnlQoS, ID_M_PLBL24, wxEmptyString, wxPoint(200,191), wxSize(199,25), 0, _T("ID_M_PLBL24"));
	m_plblPacketsAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketsAv->GetUiRect().SetGradient(0);
	m_plblPacketsAv->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketsAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketsAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketsAv->SetFont(m_plblPacketsAvFont);
	m_plblPacketsMax = new wmLabel(pnlQoS, ID_M_PLBL25, wxEmptyString, wxPoint(400,191), wxSize(195,25), 0, _T("ID_M_PLBL25"));
	m_plblPacketsMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblPacketsMax->GetUiRect().SetGradient(0);
	m_plblPacketsMax->SetForegroundColour(wxColour(0,128,0));
	m_plblPacketsMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblPacketsMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblPacketsMax->SetFont(m_plblPacketsMaxFont);
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
	m_plblJitter = new wmLabel(pnlQoS, ID_M_PLBL35, wxEmptyString, wxPoint(5,246), wxSize(194,25), 0, _T("ID_M_PLBL35"));
	m_plblJitter->SetBorderState(uiRect::BORDER_NONE);
	m_plblJitter->GetUiRect().SetGradient(0);
	m_plblJitter->SetForegroundColour(wxColour(0,128,0));
	m_plblJitter->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblJitterFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblJitter->SetFont(m_plblJitterFont);
	m_pLbl45 = new wmLabel(pnlQoS, ID_M_PLBL89, _("Roundtrip Delay"), wxPoint(200,225), wxSize(194,20), 0, _T("ID_M_PLBL89"));
	m_pLbl45->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl45->GetUiRect().SetGradient(0);
	m_pLbl45->SetForegroundColour(wxColour(255,255,255));
	m_pLbl45->SetBackgroundColour(wxColour(0,0,255));
	m_plblRoundtrip = new wmLabel(pnlQoS, ID_M_PLBL90, wxEmptyString, wxPoint(200,246), wxSize(194,25), 0, _T("ID_M_PLBL90"));
	m_plblRoundtrip->SetBorderState(uiRect::BORDER_NONE);
	m_plblRoundtrip->GetUiRect().SetGradient(0);
	m_plblRoundtrip->SetForegroundColour(wxColour(0,128,0));
	m_plblRoundtrip->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRoundtripFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRoundtrip->SetFont(m_plblRoundtripFont);
	m_plblGraph = new wmLabel(pnlQoS, ID_M_PLBL86, wxEmptyString, wxPoint(400,246), wxSize(194,25), 0, _T("ID_M_PLBL86"));
	m_plblGraph->SetBorderState(uiRect::BORDER_NONE);
	m_plblGraph->GetUiRect().SetGradient(0);
	m_plblGraph->SetForegroundColour(wxColour(0,128,0));
	m_plblGraph->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblGraphFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblGraph->SetFont(m_plblGraphFont);
	Panel1 = new wxPanel(pnlQoS, ID_PANEL4, wxPoint(0,280), wxSize(600,160), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_pGraph = new HistoryGraph(Panel1,ID_CUSTOM12, wxPoint(0,0),wxSize(600,160));
	m_pHistogram = new Histogram(Panel1,ID_CUSTOM12, wxPoint(0,0),wxSize(600,160));
	m_pHistogram->Hide();
	m_pswpMain = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(600,0), wxSize(200,440), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpMain->SetPageNameStyle(3);
	Panel2 = new wxPanel(m_pswpMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel2->SetBackgroundColour(wxColour(0,0,0));
	m_plstSubscribers = new wmList(Panel2, ID_M_PLST1, wxPoint(0,0), wxSize(200,400), wmList::STYLE_SELECT, 0, wxSize(-1,30), 2, wxSize(2,2));
	m_plstSubscribers->SetBackgroundColour(wxColour(0,0,0));
	m_plstSubscribers->SetPressedButtonColour(wxColour(wxT("#8000FF")));
	m_plstSubscribers->SetSelectedButtonColour(wxColour(wxT("#8000FF")));
	Panel3 = new wxPanel(m_pswpMain, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	Panel3->SetBackgroundColour(wxColour(0,0,0));
	m_plstOptions = new wmList(Panel3, ID_M_PLST2, wxPoint(0,5), wxSize(200,200), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
	m_plstOptions->SetBackgroundColour(wxColour(0,0,0));
	m_plstOptions->SetButtonColour(wxColour(wxT("#000080")));
	m_plstOptions->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pswpOptions = new wmSwitcherPanel(Panel3, ID_M_PSWP2, wxPoint(0,210), wxSize(200,230), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pswpOptions->SetPageNameStyle(0);
	m_pswpOptions->SetBackgroundColour(wxColour(0,0,0));
	Panel4 = new wxPanel(m_pswpOptions, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	Panel4->SetBackgroundColour(wxColour(0,0,0));
	m_pswpOptions->AddPage(Panel4, _("Settings"), false);
	m_pswpMain->AddPage(Panel2, _("Subscribers"), false);
	m_pswpMain->AddPage(Panel3, _("Options"), false);
	m_pbtnClose = new wmButton(this, ID_M_PBTN1, _("Close"), wxPoint(610,440), wxSize(180,37), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClose->SetBackgroundColour(wxColour(128,128,64));
	m_pbtnClose->SetColourDisabled(wxColour(wxT("#909090")));
	m_plblSubscriber = new wmLabel(this, ID_M_PLBL29, _("127.0.0.1"), wxPoint(200,0), wxSize(400,40), 0, _T("ID_M_PLBL29"));
	m_plblSubscriber->SetBorderState(uiRect::BORDER_NONE);
	m_plblSubscriber->GetUiRect().SetGradient(0);
	m_plblSubscriber->SetForegroundColour(wxColour(255,255,255));
	m_plblSubscriber->SetBackgroundColour(wxColour(61,146,146));
	wxFont m_plblSubscriberFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblSubscriber->SetFont(m_plblSubscriberFont);
	m_plblHostname = new wmLabel(this, ID_M_PLBL37, _("RTCP Statistics"), wxPoint(0,0), wxSize(200,40), 0, _T("ID_M_PLBL37"));
	m_plblHostname->SetBorderState(uiRect::BORDER_NONE);
	m_plblHostname->GetUiRect().SetGradient(0);
	m_plblHostname->SetForegroundColour(wxColour(255,255,255));
	m_plblHostname->SetBackgroundColour(wxColour(61,146,146));
	wxFont m_plblHostnameFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblHostname->SetFont(m_plblHostnameFont);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTCPTransmission::OnbtnCloseClick);
	//*)

	m_plstType = new wmList(Panel3, wxNewId(), wxPoint(0,110), wxSize(195,38), wmList::STYLE_SELECT, 0, wxSize(-1,32), 3, wxSize(1,0));
	m_plstType->SetBackgroundColour(wxColour(0,0,0));
    m_plstType->SetButtonColour(wxColour(50,100,0));


    m_plblHG = new wmLabel(Panel3, wxNewId(), _("Histogram Granularity"), wxPoint(0,180), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblHG->SetBorderState(uiRect::BORDER_NONE);
	m_plblHG->GetUiRect().SetGradient(0);
	m_plblHG->SetForegroundColour(wxColour(255,255,255));
	m_plblHG->SetBackgroundColour(wxColour(0,128,64));

	m_plstGranularity = new wmList(Panel3, wxNewId(), wxPoint(0,202), wxSize(200,34), wmList::STYLE_NORMAL, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstGranularity->SetBackgroundColour(wxColour(0,0,0));


	m_plblHR = new wmLabel(Panel3, wxNewId(), _("Histogram Resolution"), wxPoint(0,245), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblHR->SetBorderState(uiRect::BORDER_NONE);
	m_plblHR->GetUiRect().SetGradient(0);
	m_plblHR->SetForegroundColour(wxColour(255,255,255));
	m_plblHR->SetBackgroundColour(wxColour(0,128,64));

	m_plstResolution = new wmList(Panel3, wxNewId(), wxPoint(0,267), wxSize(200,34), wmList::STYLE_NORMAL, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstResolution->SetBackgroundColour(wxColour(0,0,0));



    m_plstGranularity->AddButton("Decrease");
    m_plstGranularity->AddButton("Reset");
    m_plstGranularity->AddButton("Increase");

    m_plstResolution->AddButton("Decrease");
    m_plstResolution->AddButton("Reset");
    m_plstResolution->AddButton("Increase");

    m_pbtnRange = new wmButton(Panel3, wxNewId(), _("Refresh Range"), wxPoint(5,320), wxSize(75,45), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));
	m_pbtnClear = new wmButton(Panel3, wxNewId(), _("Clear Subscriber Graphs"), wxPoint(85,320), wxSize(110,45), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));


	m_plstType->AddButton("Line Graph");
    m_plstType->AddButton("Bar Chart");
    m_plstType->AddButton("Histogram");



	m_pGraph->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));
	m_pHistogram->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));

	m_plstOptions->AddButton(KBPS);
	m_plstOptions->AddButton(PLPS);
	m_plstOptions->AddButton(JITTER);
	m_plstOptions->AddButton(DELAY);
	m_plstOptions->AddButton(LOSS_RATIO);

	IOManager::Get().RegisterForRTCPTransmission(this);
	IOManager::Get().RegisterForRTSPTransmission(this);

	Connect(wxID_ANY, wxEVT_RTCP_TRANSMISSION, (wxObjectEventFunction)&pnlRTCPTransmission::OnRTCPTransmissionEvent);
	Connect(wxID_ANY, wxEVT_ODS_CONNECTION, (wxObjectEventFunction)&pnlRTCPTransmission::OnConnectionEvent);
	Connect(wxID_ANY, wxEVT_ODS_DISCONNECTION, (wxObjectEventFunction)&pnlRTCPTransmission::OnDisconnectionEvent);

	Connect(ID_M_PLST1, wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlRTCPTransmission::OnSubscriberSelected);
	Connect(ID_M_PLST2, wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlRTCPTransmission::OnGraphSelected);
	Connect(m_plstType->GetId(), wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlRTCPTransmission::OnlstTypeSelected);
	Connect(m_pbtnClear->GetId(), wxEVT_BUTTON, (wxObjectEventFunction)&pnlRTCPTransmission::OnbtnClearClick);
	Connect(m_pbtnRange->GetId(), wxEVT_BUTTON, (wxObjectEventFunction)&pnlRTCPTransmission::OnbtnRangeClick);

    Connect(m_plstResolution->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTCPTransmission::OnlstResolutionPressed);
    Connect(m_plstGranularity->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTCPTransmission::OnlstGranularityPressed);


	m_sGraph = DELAY;
}

pnlRTCPTransmission::~pnlRTCPTransmission()
{
	//(*Destroy(pnlRTCPTransmission)
	//*)
}


void pnlRTCPTransmission::OnbtnCloseClick(wxCommandEvent& event)
{
    Settings::Get().Write("Splash", "Screen", "Main");
}


void pnlRTCPTransmission::OnRTCPTransmissionEvent(const RTCPTransmissionEvent& event)
{
    pmlLog(pml::LOG_TRACE) << "pnlRTCPTransmission::OnRTCPTransmissionEvent: " << event.GetFromAddress();

    wxString sSubscriber(wxString::Format("%s:%u", event.GetFromAddress().c_str(), event.GetRTCPPort()));

    subscriber& sub = AddSubscriber(sSubscriber, 0);

    if(sub.pStats)// && sub.pStats->GetLastReceivedTime() != event.GetLastReceivedTime())
    {
        sub.nLastOctets = sub.pStats->GetTotalOctets();
        sub.dtLast = sub.pStats->GetLastReceivedTime();
        delete sub.pStats;
        sub.pStats = nullptr;
    }
    else
    {
        sub.nFirstOctets = event.GetTotalOctets();
    }

    sub.pStats = dynamic_cast<RTCPTransmissionEvent*>(event.Clone());

    StoreGraphs(sSubscriber, sub);



}

pnlRTCPTransmission::subscriber& pnlRTCPTransmission::AddSubscriber(const wxString& sIpAddress, unsigned int nSSRC)
{
    auto ins = m_mSubscribers.insert(std::make_pair(sIpAddress, subscriber()));
    if(ins.second)
    {
        m_plstSubscribers->AddButton(sIpAddress);
        m_plstSubscribers->Refresh();
        m_plstSubscribers->Update();

        AddGraphs(sIpAddress);

        ins.first->second.nSSRC = nSSRC;
    }

    if(m_sSelected == wxEmptyString)
    {
        m_plstSubscribers->SelectButton(sIpAddress, true);
    }
    else if(sIpAddress == m_sSelected)
    {
        ShowSubscriber();
    }
    return ins.first->second;
}

void pnlRTCPTransmission::ShowGraph()
{
    m_pGraph->HideAllGraphs();
    m_pGraph->ShowGraph(m_sSelected+m_sGraph, true);
    m_pHistogram->ShowGraph(m_sSelected+m_sGraph);
    m_plblGraph->SetLabel(m_sGraph);
}

void pnlRTCPTransmission::OnSubscriberSelected(const wxCommandEvent& event)
{
    auto itSubscriber= m_mSubscribers.find(event.GetString());
    if(itSubscriber != m_mSubscribers.end())
    {
        m_plblSubscriber->SetLabel(itSubscriber->first);
        m_plblSSRC->SetLabel(wxString::Format("%X", itSubscriber->second.nSSRC));
        m_sSelected = event.GetString();

        ShowGraph();
        ShowSubscriber();

    }
}

void pnlRTCPTransmission::ShowSubscriber()
{
    auto itSubscriber= m_mSubscribers.find(m_sSelected);
    if(itSubscriber != m_mSubscribers.end())
    {
        if(itSubscriber->second.pStats)
        {

            m_plblJitter->SetLabel(wxString::Format("%.6f ms", itSubscriber->second.pStats->GetJitter()));


            wxTimeSpan ts(itSubscriber->second.pStats->GetLastReceivedTime() - itSubscriber->second.pStats->GetFirstReceivedTime());
            double dKbpsAv = static_cast<double>(itSubscriber->second.pStats->GetTotalOctets()-itSubscriber->second.nFirstOctets)/ts.GetSeconds().ToDouble();
            dKbpsAv/=125.0;
            double dPacketsAv = static_cast<double>(itSubscriber->second.pStats->GetTotalPacketsLost())/ts.GetSeconds().ToDouble();

            m_plblKbAv->SetLabel(wxString::Format("%.2f", dKbpsAv));
            m_plblPacketsAv->SetLabel(wxString::Format("%.2f", dPacketsAv));
            m_plblRRGap->SetLabel(ts.Format("%H:%M:%S.%l"));

            if(itSubscriber->second.dtLast < itSubscriber->second.pStats->GetLastReceivedTime())
            {
                wxTimeSpan tsDiff(itSubscriber->second.pStats->GetLastReceivedTime() - itSubscriber->second.dtLast);
                m_plblRRGap->SetLabel(tsDiff.Format("%H:%M:%S.%l"));

                m_plblKbMax->SetLabel(wxString::Format("%.2f", itSubscriber->second.dKbpsMax));
                m_plblKbMin->SetLabel(wxString::Format("%.2f", itSubscriber->second.dKbpsMin));

                m_plblPacketsMax->SetLabel(wxString::Format("%.2f", itSubscriber->second.dLostpsMax));
                m_plblPacketsMin->SetLabel(wxString::Format("%.2f", itSubscriber->second.dLostpsMin));

            }


            m_plblPacketFirst->SetLabel(wxString::Format("%u", itSubscriber->second.pStats->GetFirstPacketNumber()));
            m_plblPacketLatest->SetLabel(wxString::Format("%u", itSubscriber->second.pStats->GetLastPacketNumber()));

            m_plblPacketsLost->SetLabel(wxString::Format("%u", itSubscriber->second.pStats->GetTotalPacketsLost()));
            m_plblPacketsTotal->SetLabel(wxString::Format("%u", itSubscriber->second.pStats->GetTotalPackets()));
            m_plblRRFirst->SetLabel(itSubscriber->second.pStats->GetFirstReceivedTime().Format("%y-%m-%d %H:%M:%S:%l"));


            m_plblRRLast->SetLabel(itSubscriber->second.pStats->GetLastReceivedTime().Format("%y-%m-%d %H:%M:%S:%l"));
            m_plblRRSR->SetLabel(wxString::Format("%.3f s", static_cast<double>(itSubscriber->second.pStats->GetSRRRTime())/65536.0));
            m_plblRoundtrip->SetLabel(wxString::Format("%u us", itSubscriber->second.pStats->GetRoundTripDelay()));

        }
    }
    else
    {
        pmlLog(pml::LOG_TRACE) << "pnlRTCPTransmission::OnSubscriberSelected: Subscriber not found!";
    }
}

void pnlRTCPTransmission::ClearStats()
{
    m_plblJitter->SetLabel("");
    m_plblKbAv->SetLabel("");
    m_plblPacketsAv->SetLabel("");
    m_plblRRGap->SetLabel("");
    m_plblRRGap->SetLabel("");
    m_plblKbMax->SetLabel("");
    m_plblKbMin->SetLabel("");
    m_plblPacketsMax->SetLabel("");
    m_plblPacketsMin->SetLabel("");

    m_plblPacketFirst->SetLabel("");
    m_plblPacketLatest->SetLabel("");

    m_plblPacketsLost->SetLabel("");
    m_plblPacketsTotal->SetLabel("");
    m_plblRRFirst->SetLabel("");


    m_plblRRLast->SetLabel("");
    m_plblRRSR->SetLabel("");
    m_plblRoundtrip->SetLabel("");
    m_plblSubscriber->SetLabel("");


}

void pnlRTCPTransmission::StoreGraphs(const wxString& sIpAddress, subscriber& sub)
{
    if(sub.dtLast < sub.pStats->GetLastReceivedTime())
    {
        wxTimeSpan tsDiff(sub.pStats->GetLastReceivedTime() - sub.dtLast);
        double dkbps = static_cast<double>(sub.pStats->GetTotalOctets() -sub.nLastOctets)/tsDiff.GetMilliseconds().ToDouble();
        dkbps*=8;


        double dLost = static_cast<double>(sub.pStats->GetPacketsLostBetweenRR())/tsDiff.GetMilliseconds().ToDouble();
        dLost *= 1000.0;

        double dRatio = static_cast<double>(sub.pStats->GetPacketLossRatio())/256.0;


        sub.dKbpsMax = std::max(dkbps, sub.dKbpsMax);
        sub.dKbpsMin = std::min(dkbps, sub.dKbpsMin);

        sub.dLostpsMax = std::max(dLost, sub.dLostpsMax);
        sub.dLostpsMin = std::min(dLost, sub.dLostpsMin);

        m_pGraph->AddPeak(sIpAddress+KBPS, dkbps);
        m_pGraph->AddPeak(sIpAddress+PLPS, dLost);
        m_pGraph->AddPeak(sIpAddress+LOSS_RATIO, dRatio);
        m_pGraph->AddPeak(sIpAddress+DELAY, sub.pStats->GetRoundTripDelay());
        m_pGraph->AddPeak(sIpAddress+JITTER, sub.pStats->GetJitter());

        m_pHistogram->AddPeak(sIpAddress+KBPS, dkbps);
        m_pHistogram->AddPeak(sIpAddress+PLPS, dLost);
        m_pHistogram->AddPeak(sIpAddress+LOSS_RATIO, dRatio);
        m_pHistogram->AddPeak(sIpAddress+DELAY, sub.pStats->GetRoundTripDelay());
        m_pHistogram->AddPeak(sIpAddress+JITTER, sub.pStats->GetJitter());
    }



}

void pnlRTCPTransmission::OnConnectionEvent(const wxCommandEvent& event)
{
    pmlLog() << "Subscriber: " << event.GetString() << ":" << event.GetInt() << " [" << event.GetExtraLong() << "] opened connection.";
    AddSubscriber(wxString::Format("%s:%u", event.GetString().c_str(), event.GetExtraLong()), (unsigned int)event.GetClientData());
}

void pnlRTCPTransmission::OnDisconnectionEvent(const wxCommandEvent& event)
{
    pmlLog(pml::LOG_DEBUG) << "Disconnection: " << event.GetString() << ":" << event.GetInt() << " [" << event.GetExtraLong() << "]";
    auto itSubscriber= m_mSubscribers.find(wxString::Format("%s:%u", event.GetString().c_str(), event.GetExtraLong()));

    if(itSubscriber != m_mSubscribers.end())
    {
        pmlLog() << "Subscriber: " << itSubscriber->first << " connection closed.\n"
        << "Total time: " << (wxDateTime::Now()-itSubscriber->second.dtConnection).Format("%H:%M:%S:%l") << "\n";
        if(itSubscriber->second.pStats)
        {
            wxTimeSpan ts(itSubscriber->second.pStats->GetLastReceivedTime() - itSubscriber->second.pStats->GetFirstReceivedTime());
            double dKbpsAv = static_cast<double>(itSubscriber->second.pStats->GetTotalOctets())/ts.GetSeconds().ToDouble();
            dKbpsAv/=125.0;

            pmlLog() << "RTCP Stats\n"
            << "Total Packets:\t" << itSubscriber->second.pStats->GetTotalPackets() << "\n"
            << "Loast Packets:\t" << itSubscriber->second.pStats->GetTotalPacketsLost() << "\n"
            << "Average Bitrate:\t" << dKbpsAv << "kb/s";

        }
        pmlLog();

        if(m_sSelected == itSubscriber->first)
        {
            ClearStats();
        }

        ClearGraphs(itSubscriber->first);

        size_t nButton = m_plstSubscribers->FindButton(itSubscriber->first);
        m_plstSubscribers->DeleteButton(nButton);
        m_mSubscribers.erase(itSubscriber);


    }
}


void pnlRTCPTransmission::AddGraphs(const wxString& sSource)
{
    m_pGraph->AddGraph(sSource+KBPS, wxColour(0,255,0), 1e6, false);
    m_pHistogram->AddGraph(sSource+KBPS, wxColour(0,255,0),1.0);

    m_pGraph->AddGraph(sSource+PLPS, wxColour(255,0,0), 1e6, false);
    m_pHistogram->AddGraph(sSource+PLPS, wxColour(255,0,0), 1.0);


    m_pGraph->AddGraph(sSource+JITTER, wxColour(0,0,255), 1e6, false);
    m_pHistogram->AddGraph(sSource+JITTER, wxColour(0,0,255), 0.1);


    m_pGraph->AddGraph(sSource+DELAY, wxColour(255,255,0), 1e6, false);
    m_pHistogram->AddGraph(sSource+DELAY, wxColour(255,255,0), 10.0);


    m_pGraph->AddGraph(sSource+LOSS_RATIO, wxColour(255,200,0), 1e6, false);
    m_pHistogram->AddGraph(sSource+LOSS_RATIO, wxColour(255,200,0), 1.0);


}


void pnlRTCPTransmission::OnGraphSelected(const wxCommandEvent& event)
{
    m_sGraph = event.GetString();
    if(m_sSelected.empty() == false)
    {
        ShowGraph();
    }
}


void pnlRTCPTransmission::OnbtnClearClick(wxCommandEvent& event)
{
    ClearGraphs(m_sSelected);
}

void pnlRTCPTransmission::OnlstTypeSelected(wxCommandEvent& event)
{
    m_pGraph->Show(event.GetString() != "Histogram");
    m_pHistogram->Show(event.GetString() == "Histogram");

    m_pGraph->ShowBarGraph(event.GetString() == "Bar Chart");
}


void pnlRTCPTransmission::OnlstResolutionPressed(const wxCommandEvent& event)
{
    m_pHistogram->ChangeResolution(m_sSelected+m_sGraph, event.GetInt());
}

void pnlRTCPTransmission::OnlstGranularityPressed(const wxCommandEvent& event)
{
    m_pHistogram->ChangeGranularity(m_sSelected+m_sGraph, event.GetInt());
}

void pnlRTCPTransmission::ClearGraphs(const wxString& sSubscriber)
{
    m_pGraph->ClearGraph(sSubscriber+KBPS);
    m_pHistogram->ClearGraph(sSubscriber+KBPS);
    m_pGraph->ClearGraph(sSubscriber+PLPS);
    m_pHistogram->ClearGraph(sSubscriber+PLPS);
    m_pGraph->ClearGraph(sSubscriber+JITTER);
    m_pHistogram->ClearGraph(sSubscriber+JITTER);
    m_pGraph->ClearGraph(sSubscriber+DELAY);
    m_pHistogram->ClearGraph(sSubscriber+DELAY);
    m_pGraph->ClearGraph(sSubscriber+LOSS_RATIO);
    m_pHistogram->ClearGraph(sSubscriber+LOSS_RATIO);
}


void pnlRTCPTransmission::OnbtnRangeClick(wxCommandEvent& event)
{
    m_pGraph->RecalculateRange(m_sSelected+m_sGraph);
}
