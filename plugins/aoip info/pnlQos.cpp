#include "pnlQos.h"

//(*InternalHeaders(pnlQos)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "settingevent.h"
#include "settings.h"
#include "rtpframeevent.h"
#include "log.h"

//(*IdInit(pnlQos)
const long pnlQos::ID_CUSTOM12 = wxNewId();
const long pnlQos::ID_CUSTOM1 = wxNewId();
const long pnlQos::ID_PANEL4 = wxNewId();
const long pnlQos::ID_M_PLBL90 = wxNewId();
const long pnlQos::ID_M_PBTN29 = wxNewId();
const long pnlQos::ID_M_PBTN1 = wxNewId();
const long pnlQos::ID_M_PBTN2 = wxNewId();
const long pnlQos::ID_M_PBTN3 = wxNewId();
const long pnlQos::ID_M_PLBL89 = wxNewId();
const long pnlQos::ID_M_PLBL35 = wxNewId();
const long pnlQos::ID_M_PLBL85 = wxNewId();
const long pnlQos::ID_M_PLBL32 = wxNewId();
const long pnlQos::ID_M_PLBL31 = wxNewId();
const long pnlQos::ID_M_PLBL30 = wxNewId();
const long pnlQos::ID_M_PLBL29 = wxNewId();
const long pnlQos::ID_M_PLBL33 = wxNewId();
const long pnlQos::ID_M_PLBL28 = wxNewId();
const long pnlQos::ID_M_PLBL27 = wxNewId();
const long pnlQos::ID_M_PLBL26 = wxNewId();
const long pnlQos::ID_M_PLBL7 = wxNewId();
const long pnlQos::ID_M_PLBL25 = wxNewId();
const long pnlQos::ID_M_PLBL24 = wxNewId();
const long pnlQos::ID_M_PLBL23 = wxNewId();
const long pnlQos::ID_M_PLBL22 = wxNewId();
const long pnlQos::ID_M_PLBL21 = wxNewId();
const long pnlQos::ID_M_PLBL20 = wxNewId();
const long pnlQos::ID_M_PLBL19 = wxNewId();
const long pnlQos::ID_M_PLBL18 = wxNewId();
const long pnlQos::ID_M_PLBL17 = wxNewId();
const long pnlQos::ID_M_PLBL6 = wxNewId();
const long pnlQos::ID_M_PLBL15 = wxNewId();
const long pnlQos::ID_M_PLBL16 = wxNewId();
const long pnlQos::ID_M_PLBL14 = wxNewId();
const long pnlQos::ID_M_PLBL5 = wxNewId();
const long pnlQos::ID_M_PLBL12 = wxNewId();
const long pnlQos::ID_M_PLBL13 = wxNewId();
const long pnlQos::ID_M_PLBL11 = wxNewId();
const long pnlQos::ID_M_PLBL3 = wxNewId();
const long pnlQos::ID_M_PLBL4 = wxNewId();
const long pnlQos::ID_M_PLBL10 = wxNewId();
const long pnlQos::ID_M_PLBL9 = wxNewId();
const long pnlQos::ID_M_PLBL8 = wxNewId();
const long pnlQos::ID_M_PLBL1 = wxNewId();
const long pnlQos::ID_M_PLBL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlQos,wxPanel)
	//(*EventTable(pnlQos)
	//*)
END_EVENT_TABLE()

pnlQos::pnlQos(wxWindow* parent,const wxString& sGroup, AoIPInfoBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_sGroup(sGroup)
{
	//(*Initialize(pnlQos)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	Panel1 = new wxPanel(this, ID_PANEL4, wxPoint(0,280), wxSize(600,160), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_pHistogram = new Histogram(Panel1,ID_CUSTOM12, wxPoint(0,0),wxSize(600,160));
	m_pGraph = new HistoryGraph(Panel1,ID_CUSTOM1, wxPoint(0,0),wxSize(600,160));
	m_plblTSDF = new wmLabel(this, ID_M_PLBL90, wxEmptyString, wxPoint(146,246), wxSize(109,25), 0, _T("ID_M_PLBL90"));
	m_plblTSDF->SetBorderState(uiRect::BORDER_NONE);
	m_plblTSDF->GetUiRect().SetGradient(0);
	m_plblTSDF->SetForegroundColour(wxColour(0,128,0));
	m_plblTSDF->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTSDFFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblTSDF->SetFont(m_plblTSDFFont);
	m_pbtnCurrentGraph = new wmButton(this, ID_M_PBTN29, wxEmptyString, wxPoint(262,246), wxSize(140,25), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));
	m_pbtnCurrentGraph->SetForegroundColour(wxColour(0,128,0));
	m_pbtnCurrentGraph->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCurrentGraph->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnGraphType = new wmButton(this, ID_M_PBTN1, _("Line Graph"), wxPoint(403,246), wxSize(70,25), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnGraphType->SetForegroundColour(wxColour(0,128,0));
	m_pbtnGraphType->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnGraphType->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnRange = new wmButton(this, ID_M_PBTN2, _("Range"), wxPoint(474,246), wxSize(60,25), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnRange->SetForegroundColour(wxColour(0,128,0));
	m_pbtnRange->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnRange->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnClear = new wmButton(this, ID_M_PBTN3, _("Clear"), wxPoint(535,246), wxSize(60,25), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnClear->SetForegroundColour(wxColour(0,128,0));
	m_pbtnClear->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnClear->SetColourSelected(wxColour(wxT("#800000")));
	m_pLbl45 = new wmLabel(this, ID_M_PLBL89, _("TS-DF"), wxPoint(146,225), wxSize(109,20), 0, _T("ID_M_PLBL89"));
	m_pLbl45->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl45->GetUiRect().SetGradient(0);
	m_pLbl45->SetForegroundColour(wxColour(255,255,255));
	m_pLbl45->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSJitter = new wmLabel(this, ID_M_PLBL35, wxEmptyString, wxPoint(5,246), wxSize(140,25), 0, _T("ID_M_PLBL35"));
	m_plblQoSJitter->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSJitter->GetUiRect().SetGradient(0);
	m_plblQoSJitter->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSJitter->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSJitterFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSJitter->SetFont(m_plblQoSJitterFont);
	m_pLbl39 = new wmLabel(this, ID_M_PLBL85, _("Current Graph"), wxPoint(262,225), wxSize(333,20), 0, _T("ID_M_PLBL85"));
	m_pLbl39->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl39->GetUiRect().SetGradient(0);
	m_pLbl39->SetForegroundColour(wxColour(255,255,255));
	m_pLbl39->SetBackgroundColour(wxColour(0,128,64));
	m_pLbl17 = new wmLabel(this, ID_M_PLBL32, _("Jitter"), wxPoint(5,225), wxSize(140,20), 0, _T("ID_M_PLBL32"));
	m_pLbl17->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl17->GetUiRect().SetGradient(0);
	m_pLbl17->SetForegroundColour(wxColour(255,255,255));
	m_pLbl17->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSInterMax = new wmLabel(this, ID_M_PLBL31, wxEmptyString, wxPoint(450,191), wxSize(147,25), 0, _T("ID_M_PLBL31"));
	m_plblQoSInterMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterMax->GetUiRect().SetGradient(0);
	m_plblQoSInterMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterMax->SetFont(m_plblQoSInterMaxFont);
	m_plblQoSInterAv = new wmLabel(this, ID_M_PLBL30, wxEmptyString, wxPoint(301,191), wxSize(147,25), 0, _T("ID_M_PLBL30"));
	m_plblQoSInterAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterAv->GetUiRect().SetGradient(0);
	m_plblQoSInterAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterAv->SetFont(m_plblQoSInterAvFont);
	m_plblQoSInterMin = new wmLabel(this, ID_M_PLBL29, wxEmptyString, wxPoint(5,191), wxSize(147,25), 0, _T("ID_M_PLBL29"));
	m_plblQoSInterMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInterMin->GetUiRect().SetGradient(0);
	m_plblQoSInterMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInterMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInterMin->SetFont(m_plblQoSInterMinFont);
	m_plblQoSInter = new wmLabel(this, ID_M_PLBL33, wxEmptyString, wxPoint(153,191), wxSize(147,25), 0, _T("ID_M_PLBL33"));
	m_plblQoSInter->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSInter->GetUiRect().SetGradient(0);
	m_plblQoSInter->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSInter->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSInterFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSInter->SetFont(m_plblQoSInterFont);
	m_pLbl16 = new wmLabel(this, ID_M_PLBL28, _("Inter Packet Gap Max"), wxPoint(450,170), wxSize(147,20), 0, _T("ID_M_PLBL28"));
	m_pLbl16->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl16->GetUiRect().SetGradient(0);
	m_pLbl16->SetForegroundColour(wxColour(255,255,255));
	m_pLbl16->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl15 = new wmLabel(this, ID_M_PLBL27, _("IPG Average"), wxPoint(301,170), wxSize(147,20), 0, _T("ID_M_PLBL27"));
	m_pLbl15->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl15->GetUiRect().SetGradient(0);
	m_pLbl15->SetForegroundColour(wxColour(255,255,255));
	m_pLbl15->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl14 = new wmLabel(this, ID_M_PLBL26, _("Inter Packet Gap Min"), wxPoint(5,170), wxSize(147,20), 0, _T("ID_M_PLBL26"));
	m_pLbl14->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl14->GetUiRect().SetGradient(0);
	m_pLbl14->SetForegroundColour(wxColour(255,255,255));
	m_pLbl14->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl4 = new wmLabel(this, ID_M_PLBL7, _("Inter Packet Gap"), wxPoint(153,170), wxSize(147,20), 0, _T("ID_M_PLBL7"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSPacketsMax = new wmLabel(this, ID_M_PLBL25, wxEmptyString, wxPoint(400,136), wxSize(195,25), 0, _T("ID_M_PLBL25"));
	m_plblQoSPacketsMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsMax->GetUiRect().SetGradient(0);
	m_plblQoSPacketsMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsMax->SetFont(m_plblQoSPacketsMaxFont);
	m_plblQoSPacketsAv = new wmLabel(this, ID_M_PLBL24, wxEmptyString, wxPoint(200,136), wxSize(199,25), 0, _T("ID_M_PLBL24"));
	m_plblQoSPacketsAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsAv->GetUiRect().SetGradient(0);
	m_plblQoSPacketsAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsAv->SetFont(m_plblQoSPacketsAvFont);
	m_plblQoSPacketsMin = new wmLabel(this, ID_M_PLBL23, wxEmptyString, wxPoint(5,136), wxSize(194,25), 0, _T("ID_M_PLBL23"));
	m_plblQoSPacketsMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSPacketsMin->GetUiRect().SetGradient(0);
	m_plblQoSPacketsMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSPacketsMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSPacketsMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSPacketsMin->SetFont(m_plblQoSPacketsMinFont);
	m_pLbl13 = new wmLabel(this, ID_M_PLBL22, _("Packets Lost/s Max"), wxPoint(400,115), wxSize(195,20), 0, _T("ID_M_PLBL22"));
	m_pLbl13->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl13->GetUiRect().SetGradient(0);
	m_pLbl13->SetForegroundColour(wxColour(255,255,255));
	m_pLbl13->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL21, _("Packets Lost/s Average"), wxPoint(200,115), wxSize(199,20), 0, _T("ID_M_PLBL21"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl11 = new wmLabel(this, ID_M_PLBL20, _("Packets Lost/s Min"), wxPoint(5,115), wxSize(194,20), 0, _T("ID_M_PLBL20"));
	m_pLbl11->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl11->GetUiRect().SetGradient(0);
	m_pLbl11->SetForegroundColour(wxColour(255,255,255));
	m_pLbl11->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSKbMax = new wmLabel(this, ID_M_PLBL19, wxEmptyString, wxPoint(450,81), wxSize(147,25), 0, _T("ID_M_PLBL19"));
	m_plblQoSKbMax->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbMax->GetUiRect().SetGradient(0);
	m_plblQoSKbMax->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbMax->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbMax->SetFont(m_plblQoSKbMaxFont);
	m_plblQoSKbAv = new wmLabel(this, ID_M_PLBL18, wxEmptyString, wxPoint(301,81), wxSize(148,25), 0, _T("ID_M_PLBL18"));
	m_plblQoSKbAv->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbAv->GetUiRect().SetGradient(0);
	m_plblQoSKbAv->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbAv->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbAvFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbAv->SetFont(m_plblQoSKbAvFont);
	m_plblQoSKbMin = new wmLabel(this, ID_M_PLBL17, wxEmptyString, wxPoint(5,81), wxSize(147,25), 0, _T("ID_M_PLBL17"));
	m_plblQoSKbMin->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKbMin->GetUiRect().SetGradient(0);
	m_plblQoSKbMin->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKbMin->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbMinFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKbMin->SetFont(m_plblQoSKbMinFont);
	m_plblQoSKb = new wmLabel(this, ID_M_PLBL6, wxEmptyString, wxPoint(153,81), wxSize(147,25), 0, _T("ID_M_PLBL6"));
	m_plblQoSKb->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSKb->GetUiRect().SetGradient(0);
	m_plblQoSKb->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSKb->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSKbFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSKb->SetFont(m_plblQoSKbFont);
	m_pLbl9 = new wmLabel(this, ID_M_PLBL15, _("kBits/s Average"), wxPoint(301,60), wxSize(148,20), 0, _T("ID_M_PLBL15"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl10 = new wmLabel(this, ID_M_PLBL16, _("kBits/s Max"), wxPoint(450,60), wxSize(147,20), 0, _T("ID_M_PLBL16"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(0);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl8 = new wmLabel(this, ID_M_PLBL14, _("kBits/s Min"), wxPoint(5,60), wxSize(147,20), 0, _T("ID_M_PLBL14"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL5, _("kBits/s"), wxPoint(153,60), wxSize(147,20), 0, _T("ID_M_PLBL5"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,255));
	m_plblQoSLost = new wmLabel(this, ID_M_PLBL12, wxEmptyString, wxPoint(500,26), wxSize(100,25), 0, _T("ID_M_PLBL12"));
	m_plblQoSLost->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSLost->GetUiRect().SetGradient(0);
	m_plblQoSLost->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSLost->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSLostFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSLost->SetFont(m_plblQoSLostFont);
	m_plblQoSReceived = new wmLabel(this, ID_M_PLBL13, wxEmptyString, wxPoint(400,26), wxSize(99,25), 0, _T("ID_M_PLBL13"));
	m_plblQoSReceived->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSReceived->GetUiRect().SetGradient(0);
	m_plblQoSReceived->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSReceived->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSReceivedFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSReceived->SetFont(m_plblQoSReceivedFont);
	m_plblQoSTime = new wmLabel(this, ID_M_PLBL11, wxEmptyString, wxPoint(5,26), wxSize(132,25), 0, _T("ID_M_PLBL11"));
	m_plblQoSTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoSTime->GetUiRect().SetGradient(0);
	m_plblQoSTime->SetForegroundColour(wxColour(0,128,0));
	m_plblQoSTime->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblQoSTimeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblQoSTime->SetFont(m_plblQoSTimeFont);
	m_plblRTCP_Last = new wmLabel(this, ID_M_PLBL3, wxEmptyString, wxPoint(138,26), wxSize(130,25), 0, _T("ID_M_PLBL3"));
	m_plblRTCP_Last->SetBorderState(uiRect::BORDER_NONE);
	m_plblRTCP_Last->GetUiRect().SetGradient(0);
	m_plblRTCP_Last->SetForegroundColour(wxColour(0,128,0));
	m_plblRTCP_Last->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRTCP_LastFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRTCP_Last->SetFont(m_plblRTCP_LastFont);
	m_plblRTCP_NTP = new wmLabel(this, ID_M_PLBL4, wxEmptyString, wxPoint(269,26), wxSize(132,25), 0, _T("ID_M_PLBL4"));
	m_plblRTCP_NTP->SetBorderState(uiRect::BORDER_NONE);
	m_plblRTCP_NTP->GetUiRect().SetGradient(0);
	m_plblRTCP_NTP->SetForegroundColour(wxColour(0,128,0));
	m_plblRTCP_NTP->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblRTCP_NTPFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
	m_plblRTCP_NTP->SetFont(m_plblRTCP_NTPFont);
	m_pLbl7 = new wmLabel(this, ID_M_PLBL10, _("Packets Lost"), wxPoint(500,5), wxSize(100,20), 0, _T("ID_M_PLBL10"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl6 = new wmLabel(this, ID_M_PLBL9, _("Total Packets"), wxPoint(400,5), wxSize(99,20), 0, _T("ID_M_PLBL9"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl5 = new wmLabel(this, ID_M_PLBL8, _("QoS Time"), wxPoint(5,5), wxSize(132,20), 0, _T("ID_M_PLBL8"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Last SR"), wxPoint(138,5), wxSize(130,20), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,255));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("SR NTP"), wxPoint(269,5), wxSize(130,20), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,255));

	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlQos::OnbtnRangeClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlQos::OnbtnClearClick);
	//*)


    m_pGraph->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));
    m_pHistogram->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));


    int nInterval = Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000)*1000;
	m_pGraph->AddGraph(wxT("kBit/s"), wxColour(0,255,0), nInterval, false);
    m_pHistogram->AddGraph(wxT("kBit/s"), wxColour(0,255,0), 0.1);


	m_pGraph->AddGraph(wxT("TS-DF"), wxColour(255,255,255),  nInterval, false);
	m_pHistogram->AddGraph("TS-DF", wxColour(255,255,255), 1.0);

	m_pGraph->AddGraph(wxT("Packet Gap"), wxColour(0,0,255),  nInterval, false);
	m_pHistogram->AddGraph("Packet Gap", wxColour(0,0,255), 0.1);


	m_pGraph->AddGraph(wxT("Packet Loss"), wxColour(255,0,0),  nInterval, false);
	m_pHistogram->AddGraph("Packet Loss", wxColour(255,0,0), 10.0);


	m_pGraph->AddGraph(wxT("Jitter"), wxColour(255,255,0),  nInterval, false);
	m_pHistogram->AddGraph("Jitter", wxColour(255,255,0), 0.01);

	m_pGraph->AddGraph(wxT("Timestamp"), wxColour(0,0,255),  nInterval, false);
	m_pHistogram->AddGraph("Timestamp", wxColour(0,0,255), 1000);

    m_pGraph->AddGraph(wxT("Timestamp Errors"), wxColour(255,128,0),  nInterval, false);
    m_pHistogram->AddGraph("Timestamp Errors", wxColour(255,128,0), 10);

	m_pGraph->AddGraph(wxT("Slip"), wxColour(180,200,255),  nInterval, false);
	m_pHistogram->AddGraph("Slip", wxColour(180,200,255), 10);


	m_pbtnCurrentGraph->SetPopup({"kBit/s", "Packet Gap", "Packet Loss", "Jitter", "Timestamp", "Timestamp Errors", "TS-DF", "Slip"});
	m_pbtnCurrentGraph->ConnectToSetting(pBuilder->GetSection(), "Graph_"+sGroup, "kBit/s");

	m_pbtnGraphType->SetPopup({"Line Graph", "Bar Chart", "Histogram"});
	m_pbtnGraphType->ConnectToSetting(pBuilder->GetSection(), "GraphType_"+sGroup, "Line Graph");

	pBuilder->RegisterForSettingsUpdates(this, "Graph_"+sGroup);
	pBuilder->RegisterForSettingsUpdates(this, "GraphType_"+sGroup);

	ShowGraph(pBuilder->ReadSetting("Graph_"+sGroup, "kBit/s"));
	SetGraphType(pBuilder->ReadSetting("GraphType_"+sGroup, "Line Graph"));

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pnlQos::OnSettingChanged);
}

pnlQos::~pnlQos()
{
	//(*Destroy(pnlQos)
	//*)
}

void pnlQos::QoSUpdated(qosData* pData)
{
	if(IsShownOnScreen())
    {
		m_plblQoSTime->SetLabel(pData->tsTime.Format(wxT("%H:%M:%S:%l")));
		m_plblQoSKb->SetLabel(wxString::Format(wxT("%.2f"), pData->dkbits_per_second_Now));
		m_plblQoSKbAv->SetLabel(wxString::Format(wxT("%.2f"), pData->dkbits_per_second_Av));
		m_plblQoSKbMax->SetLabel(wxString::Format(wxT("%f"), pData->dkbits_per_second_max));
		m_plblQoSKbMin->SetLabel(wxString::Format(wxT("%f"), pData->dkbits_per_second_min));
		m_plblQoSLost->SetLabel(wxString::Format(wxT("%d"), pData->nTotNumPacketsLost));
		m_plblQoSPacketsAv->SetLabel(wxString::Format(wxT("%.3f"), pData->dPacket_loss_fraction_av));
		m_plblQoSPacketsMax->SetLabel(wxString::Format(wxT("%.3f"), pData->dPacket_loss_fraction_max));
		m_plblQoSPacketsMin->SetLabel(wxString::Format(wxT("%.3f"), pData->dPacket_loss_fraction_min));
		m_plblQoSReceived->SetLabel(wxString::Format(wxT("%d"), pData->nTotNumPacketsReceived));

		m_plblQoSInter->SetLabel(wxString::Format(wxT("%f ms"), pData->dInter_packet_gap_ms_max));
		m_plblQoSInterAv->SetLabel(wxString::Format(wxT("%f ms"), pData->dInter_packet_gap_ms_av));

		m_dTotalMaxInterPacket = std::max(m_dTotalMaxInterPacket, pData->dInter_packet_gap_ms_max);
		m_dTotalMinInterPacket = std::min(m_dTotalMinInterPacket, pData->dInter_packet_gap_ms_min);
		m_plblQoSInterMax->SetLabel(wxString::Format(wxT("%f ms"), m_dTotalMaxInterPacket));
		m_plblQoSInterMin->SetLabel(wxString::Format(wxT("%f ms"), m_dTotalMinInterPacket));

		m_plblQoSJitter->SetLabel(wxString::Format(wxT("%f ms"),pData->dJitter));


		wxDateTime dtSR(time_t(pData->tvLastSR_Time.tv_sec));
		dtSR.SetMillisecond(pData->tvLastSR_Time.tv_usec/1000);

		if(dtSR.IsValid())
		{
			m_plblRTCP_Last->SetLabel(dtSR.Format("%H:%M:%S:%l"));
		}
		else
		{
			m_plblRTCP_Last->SetLabel("");
		}

		wxDateTime dtNTP(time_t(pData->tvSync.tv_sec));
		dtNTP.SetMillisecond(pData->tvSync.tv_usec/1000);
		if(dtNTP.IsValid())
		{
			m_plblRTCP_NTP->SetLabel(dtNTP.Format("%H:%M:%S:%l"));
		}
		else
		{
			m_plblRTCP_NTP->SetLabel("");
		}
	}

    m_pGraph->AddPeak(wxT("kBit/s"), pData->dkbits_per_second_Now);
    m_pHistogram->AddPeak(wxT("kBit/s"), pData->dkbits_per_second_Now);

    m_pGraph->AddPeak(wxT("Packet Gap"), pData->dInter_packet_gap_ms_max);
    m_pHistogram->AddPeak(wxT("Packet Gap"), pData->dInter_packet_gap_ms_max);

    m_pGraph->AddPeak(wxT("Packet Loss"), pData->dPacket_loss_fraction_av);
    m_pHistogram->AddPeak(wxT("Packet Loss"), pData->dPacket_loss_fraction_av);

    m_pGraph->AddPeak(wxT("Jitter"), pData->dJitter);
    m_pHistogram->AddPeak(wxT("Jitter"), pData->dJitter);


    m_pGraph->AddPeak(wxT("Timestamp Errors"), pData->nTimestampErrors);
    m_pHistogram->AddPeak(wxT("Timestamp Errors"), pData->nTimestampErrors);

	
	auto dSlip = m_dLatency-m_dInitialLatency;
	m_pGraph->AddPeak("Slip", dSlip);
	m_pHistogram->AddPeak("Slip", dSlip);

}

void pnlQos::OnbtnRangeClick(wxCommandEvent& event)
{
    m_pGraph->RecalculateRange(m_sGraph);
}

void pnlQos::OnbtnClearClick(wxCommandEvent& event)
{
    m_pGraph->ClearGraphs();
    m_pHistogram->ClearGraphs();
	m_dTotalMaxInterPacket = -1.0;
	m_dTotalMinInterPacket = std::numeric_limits<double>::max();
}

void pnlQos::ShowGraph(const wxString& sGraph)
{
    m_pGraph->HideAllGraphs();
    m_pGraph->ShowGraph(sGraph, true);
    m_pHistogram->ShowGraph(sGraph);

    m_pbtnCurrentGraph->SetLabel(sGraph);
    m_sGraph = sGraph;
}

void pnlQos::SetGraphType(const wxString& sType)
{
    m_pHistogram->Show(sType == "Histogram");
    m_pGraph->Show(sType != "Histogram");
    m_pGraph->ShowBarGraph(sType == "Bar Chart");

    m_pbtnGraphType->SetLabel(sType);
}

void pnlQos::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "Graph_"+m_sGroup)
    {
        ShowGraph(event.GetValue());
    }
    else if(event.GetKey() == "GraphType_"+m_sGroup)
    {
        SetGraphType(event.GetValue());
    }
}

void pnlQos::SetAudioData(const timedbuffer* pTimedBuffer)
{

	double dTimestamp(static_cast<double>(pTimedBuffer->GetTimestamp())/4294967296.0);
    m_pGraph->AddPeak("Timestamp",dTimestamp);
    m_pHistogram->AddPeak("Timestamp",dTimestamp);

}

void pnlQos::RtpFrame(std::shared_ptr<const rtpFrame> pFrame)
{
	m_plblTSDF->SetLabel(wxString::Format(wxT("%.0f us"), pFrame->dTSDF));
	if(pFrame->dTSDF >= 0.0)
    {
        m_pGraph->AddPeak(wxT("TS-DF"), pFrame->dTSDF);
        m_pHistogram->AddPeak(wxT("TS-DF"), pFrame->dTSDF);
    }

	m_dLatency = static_cast<double>(pFrame->timeLatency.tv_sec)*1000000.0 + static_cast<double>(pFrame->timeLatency.tv_usec);
	if(m_nLatencyCounter < 3)
	{
		++m_nLatencyCounter;
		m_dInitialLatency = m_dLatency;
	}	
}
