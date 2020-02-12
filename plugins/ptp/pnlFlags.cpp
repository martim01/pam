#include "pnlFlags.h"
#include "ptpstructs.h"
//(*InternalHeaders(pnlFlags)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlFlags)
const long pnlFlags::ID_M_PLBL35 = wxNewId();
const long pnlFlags::ID_M_PLBL39 = wxNewId();
const long pnlFlags::ID_M_PLBL45 = wxNewId();
const long pnlFlags::ID_M_PLBL53 = wxNewId();
const long pnlFlags::ID_M_PLBL65 = wxNewId();
const long pnlFlags::ID_M_PLBL66 = wxNewId();
const long pnlFlags::ID_M_PLBL67 = wxNewId();
const long pnlFlags::ID_M_PLBL68 = wxNewId();
const long pnlFlags::ID_M_PLBL69 = wxNewId();
const long pnlFlags::ID_M_PLBL70 = wxNewId();
const long pnlFlags::ID_M_PLBL71 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlFlags,wxPanel)
	//(*EventTable(pnlFlags)
	//*)
END_EVENT_TABLE()

const wxColour pnlFlags::CLR_OFF = wxColour(160,160,160);
const wxColour pnlFlags::CLR_ON = wxColour(100,180,100);

pnlFlags::pnlFlags(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, int nStyle, const wxString& sEmpty)
{
	//(*Initialize(pnlFlags)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plblFlagsProfile1 = new wmLabel(this, ID_M_PLBL35, _("Profile 1"), wxPoint(0,0), wxSize(69,20), 0, _T("ID_M_PLBL35"));
	m_plblFlagsProfile1->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsProfile1->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsProfile1->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsProfile1->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsProfile2 = new wmLabel(this, ID_M_PLBL39, _("Profile 2"), wxPoint(70,0), wxSize(69,20), 0, _T("ID_M_PLBL39"));
	m_plblFlagsProfile2->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsProfile2->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsProfile2->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsProfile2->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsUnicast = new wmLabel(this, ID_M_PLBL45, _("Unicast"), wxPoint(140,0), wxSize(69,20), 0, _T("ID_M_PLBL45"));
	m_plblFlagsUnicast->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsUnicast->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsUnicast->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsUnicast->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlags2Step = new wmLabel(this, ID_M_PLBL53, _("2-Step"), wxPoint(210,0), wxSize(69,20), 0, _T("ID_M_PLBL53"));
	m_plblFlags2Step->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlags2Step->GetUiRect().SetGradient(wxWEST);
	m_plblFlags2Step->SetForegroundColour(wxColour(255,255,255));
	m_plblFlags2Step->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsAlternate = new wmLabel(this, ID_M_PLBL65, _("Alternate"), wxPoint(0,22), wxSize(69,20), 0, _T("ID_M_PLBL65"));
	m_plblFlagsAlternate->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsAlternate->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsAlternate->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsAlternate->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsFreq = new wmLabel(this, ID_M_PLBL66, _("Freq Trace"), wxPoint(70,22), wxSize(69,20), 0, _T("ID_M_PLBL66"));
	m_plblFlagsFreq->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsFreq->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsFreq->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsFreq->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsTime = new wmLabel(this, ID_M_PLBL67, _("Time Trace"), wxPoint(140,22), wxSize(69,20), 0, _T("ID_M_PLBL67"));
	m_plblFlagsTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsTime->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsTime->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsTime->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsScale = new wmLabel(this, ID_M_PLBL68, _("TimeScale"), wxPoint(210,22), wxSize(69,20), 0, _T("ID_M_PLBL68"));
	m_plblFlagsScale->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsScale->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsScale->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsScale->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlagsUTC = new wmLabel(this, ID_M_PLBL69, _("UTC Rble"), wxPoint(0,44), wxSize(69,20), 0, _T("ID_M_PLBL69"));
	m_plblFlagsUTC->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlagsUTC->GetUiRect().SetGradient(wxWEST);
	m_plblFlagsUTC->SetForegroundColour(wxColour(255,255,255));
	m_plblFlagsUTC->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlags59 = new wmLabel(this, ID_M_PLBL70, _("LI 59"), wxPoint(70,44), wxSize(69,20), 0, _T("ID_M_PLBL70"));
	m_plblFlags59->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlags59->GetUiRect().SetGradient(wxWEST);
	m_plblFlags59->SetForegroundColour(wxColour(255,255,255));
	m_plblFlags59->SetBackgroundColour(wxColour(92,122,224));
	m_plblFlags61 = new wmLabel(this, ID_M_PLBL71, _("LI 61"), wxPoint(140,44), wxSize(69,20), 0, _T("ID_M_PLBL71"));
	m_plblFlags61->SetBorderState(uiRect::BORDER_NONE);
	m_plblFlags61->GetUiRect().SetGradient(wxWEST);
	m_plblFlags61->SetForegroundColour(wxColour(255,255,255));
	m_plblFlags61->SetBackgroundColour(wxColour(92,122,224));
	//*)
	SetPosition(pos);
	SetSize(size);
}

pnlFlags::~pnlFlags()
{
	//(*Destroy(pnlFlags)
	//*)
}

void pnlFlags::ShowFlags(unsigned short nFlags)
{
    m_plblFlagsAlternate->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::ALTERNATE_MASTER) ? CLR_ON : CLR_OFF);
    m_plblFlags2Step->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::TWO_STEP) ? CLR_ON : CLR_OFF);
    m_plblFlagsUnicast->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::UNICAST) ? CLR_ON : CLR_OFF);
    m_plblFlagsProfile1->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::PROFILE1) ? CLR_ON : CLR_OFF);
    m_plblFlagsProfile2->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::PROFILE2) ? CLR_ON : CLR_OFF);
    m_plblFlags61->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::LI_61) ? CLR_ON : CLR_OFF);
    m_plblFlags59->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::LI_59) ? CLR_ON : CLR_OFF);
    m_plblFlagsUTC->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::UTC_OFFSET_VALID) ? CLR_ON : CLR_OFF);
    m_plblFlagsScale->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::TIMESCALE) ? CLR_ON : CLR_OFF);
    m_plblFlagsTime->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::TIME_TRACEABLE) ? CLR_ON : CLR_OFF);
    m_plblFlagsFreq->SetBackgroundColour( (nFlags & ptpmonkey::ptpV2Header::FREQ_TRACEABLE) ? CLR_ON : CLR_OFF);

}
