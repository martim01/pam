#include "lineuppanel.h"
#include "lineupbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "fftAlgorithm.h"
#include "levelcalculator.h"
#include "ppmtypes.h"
#include <wx/log.h>
//(*InternalHeaders(lineupPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;
//(*IdInit(lineupPanel)
const long lineupPanel::ID_M_PLBL8 = wxNewId();
const long lineupPanel::ID_M_PLBL16 = wxNewId();
const long lineupPanel::ID_M_PLBL9 = wxNewId();
const long lineupPanel::ID_M_PLBL6 = wxNewId();
const long lineupPanel::ID_M_PBTN1 = wxNewId();
const long lineupPanel::ID_M_PLBL17 = wxNewId();
const long lineupPanel::ID_M_PLBL10 = wxNewId();
const long lineupPanel::ID_M_PLBL7 = wxNewId();
const long lineupPanel::ID_M_PLBL60 = wxNewId();
const long lineupPanel::ID_M_PLBL21 = wxNewId();
const long lineupPanel::ID_M_PLBL13 = wxNewId();
const long lineupPanel::ID_M_PLBL14 = wxNewId();
const long lineupPanel::ID_M_PLBL18 = wxNewId();
const long lineupPanel::ID_M_PLBL1 = wxNewId();
const long lineupPanel::ID_M_PLBL2 = wxNewId();
const long lineupPanel::ID_M_PLBL11 = wxNewId();
const long lineupPanel::ID_M_PLBL19 = wxNewId();
const long lineupPanel::ID_M_PLBL12 = wxNewId();
const long lineupPanel::ID_M_PLBL23 = wxNewId();
const long lineupPanel::ID_M_PLBL22 = wxNewId();
const long lineupPanel::ID_M_PLBL15 = wxNewId();
const long lineupPanel::ID_M_PLBL24 = wxNewId();
const long lineupPanel::ID_M_PLBL3 = wxNewId();
const long lineupPanel::ID_M_PLBL20 = wxNewId();
const long lineupPanel::ID_M_PLBL4 = wxNewId();
const long lineupPanel::ID_M_PLBL5 = wxNewId();
const long lineupPanel::ID_M_PLBL25 = wxNewId();
const long lineupPanel::ID_M_PLBL26 = wxNewId();
const long lineupPanel::ID_M_PLBL27 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(lineupPanel,pmPanel);

BEGIN_EVENT_TABLE(lineupPanel,wxPanel)
	//(*EventTable(lineupPanel)
	//*)
END_EVENT_TABLE()

lineupPanel::lineupPanel(wxWindow* parent,lineupBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(lineupPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL8, _("Level:"), wxPoint(5,5), wxSize(120,81), 0, _T("ID_M_PLBL8"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl2Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL16, _("L"), wxPoint(125,5), wxSize(20,40), 0, _T("ID_M_PLBL16"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl4Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plblLevelL = new wmLabel(this, ID_M_PLBL9, _("1000 kHz"), wxPoint(146,5), wxSize(150,40), 0, _T("ID_M_PLBL9"));
	m_plblLevelL->SetBorderState(uiRect::BORDER_NONE);
	m_plblLevelL->GetUiRect().SetGradient(0);
	m_plblLevelL->SetForegroundColour(wxColour(255,255,255));
	m_plblLevelL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblLevelLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblLevelL->SetFont(m_plblLevelLFont);
	m_plblRangeL = new wmLabel(this, ID_M_PLBL6, _("-12dBFS"), wxPoint(297,5), wxSize(150,40), 0, _T("ID_M_PLBL6"));
	m_plblRangeL->SetBorderState(uiRect::BORDER_NONE);
	m_plblRangeL->GetUiRect().SetGradient(0);
	m_plblRangeL->SetForegroundColour(wxColour(255,255,255));
	m_plblRangeL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblRangeLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblRangeL->SetFont(m_plblRangeLFont);
	m_pbtnReset = new wmButton(this, ID_M_PBTN1, _("Reset"), wxPoint(450,5), wxSize(110,82), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnReset->SetBackgroundColour(wxColour(0,0,160));
	wxFont m_pbtnResetFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnReset->SetFont(m_pbtnResetFont);
	m_pLbl5 = new wmLabel(this, ID_M_PLBL17, _("R"), wxPoint(125,46), wxSize(20,40), 0, _T("ID_M_PLBL17"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl5Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl5->SetFont(m_pLbl5Font);
	m_plblLevelR = new wmLabel(this, ID_M_PLBL10, _("1000 kHz"), wxPoint(146,46), wxSize(150,40), 0, _T("ID_M_PLBL10"));
	m_plblLevelR->SetBorderState(uiRect::BORDER_NONE);
	m_plblLevelR->GetUiRect().SetGradient(0);
	m_plblLevelR->SetForegroundColour(wxColour(255,255,255));
	m_plblLevelR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblLevelRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblLevelR->SetFont(m_plblLevelRFont);
	m_plblRangeR = new wmLabel(this, ID_M_PLBL7, _("-12dBFS"), wxPoint(297,46), wxSize(150,40), 0, _T("ID_M_PLBL7"));
	m_plblRangeR->SetBorderState(uiRect::BORDER_NONE);
	m_plblRangeR->GetUiRect().SetGradient(0);
	m_plblRangeR->SetForegroundColour(wxColour(255,255,255));
	m_plblRangeR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblRangeRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblRangeR->SetFont(m_plblRangeRFont);
	m_plblInput = new wmLabel(this, ID_M_PLBL60, _("Frequency:"), wxPoint(5,100), wxSize(120,81), 0, _T("ID_M_PLBL60"));
	m_plblInput->SetBorderState(uiRect::BORDER_NONE);
	m_plblInput->GetUiRect().SetGradient(0);
	m_plblInput->SetForegroundColour(wxColour(255,255,255));
	m_plblInput->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblInputFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblInput->SetFont(m_plblInputFont);
	m_pLbl9 = new wmLabel(this, ID_M_PLBL21, _("R"), wxPoint(125,141), wxSize(20,40), 0, _T("ID_M_PLBL21"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl9Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl9->SetFont(m_pLbl9Font);
	m_plblDominantHzR = new wmLabel(this, ID_M_PLBL13, _("1000 kHz"), wxPoint(146,141), wxSize(150,40), 0, _T("ID_M_PLBL13"));
	m_plblDominantHzR->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantHzR->GetUiRect().SetGradient(0);
	m_plblDominantHzR->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantHzR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantHzRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantHzR->SetFont(m_plblDominantHzRFont);
	m_plblDominantdBR = new wmLabel(this, ID_M_PLBL14, _("-12dBFS"), wxPoint(297,141), wxSize(150,40), 0, _T("ID_M_PLBL14"));
	m_plblDominantdBR->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantdBR->GetUiRect().SetGradient(0);
	m_plblDominantdBR->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantdBR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantdBRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantdBR->SetFont(m_plblDominantdBRFont);
	m_pLbl6 = new wmLabel(this, ID_M_PLBL18, _("L"), wxPoint(125,100), wxSize(20,40), 0, _T("ID_M_PLBL18"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl6Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl6->SetFont(m_pLbl6Font);
	m_plblDominantHzL = new wmLabel(this, ID_M_PLBL1, _("1000 kHz"), wxPoint(146,100), wxSize(150,40), 0, _T("ID_M_PLBL1"));
	m_plblDominantHzL->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantHzL->GetUiRect().SetGradient(0);
	m_plblDominantHzL->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantHzL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantHzLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantHzL->SetFont(m_plblDominantHzLFont);
	m_plblDominantdBL = new wmLabel(this, ID_M_PLBL2, _("-12dBFS"), wxPoint(297,100), wxSize(150,40), 0, _T("ID_M_PLBL2"));
	m_plblDominantdBL->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantdBL->GetUiRect().SetGradient(0);
	m_plblDominantdBL->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantdBL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantdBLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantdBL->SetFont(m_plblDominantdBLFont);
	m_pLbl3 = new wmLabel(this, ID_M_PLBL11, _("Distortion:"), wxPoint(5,190), wxSize(120,81), 0, _T("ID_M_PLBL11"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl3Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_pLbl7 = new wmLabel(this, ID_M_PLBL19, _("L"), wxPoint(125,190), wxSize(20,40), 0, _T("ID_M_PLBL19"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl7Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	m_plblDistortionL = new wmLabel(this, ID_M_PLBL12, wxEmptyString, wxPoint(146,190), wxSize(150,40), 0, _T("ID_M_PLBL12"));
	m_plblDistortionL->SetBorderState(uiRect::BORDER_NONE);
	m_plblDistortionL->GetUiRect().SetGradient(0);
	m_plblDistortionL->SetForegroundColour(wxColour(255,255,255));
	m_plblDistortionL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDistortionLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDistortionL->SetFont(m_plblDistortionLFont);
	m_plblDistortionMaxL = new wmLabel(this, ID_M_PLBL23, wxEmptyString, wxPoint(297,190), wxSize(150,40), 0, _T("ID_M_PLBL23"));
	m_plblDistortionMaxL->SetBorderState(uiRect::BORDER_NONE);
	m_plblDistortionMaxL->GetUiRect().SetGradient(0);
	m_plblDistortionMaxL->SetForegroundColour(wxColour(255,255,255));
	m_plblDistortionMaxL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDistortionMaxLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDistortionMaxL->SetFont(m_plblDistortionMaxLFont);
	m_pLbl10 = new wmLabel(this, ID_M_PLBL22, _("R"), wxPoint(125,231), wxSize(20,40), 0, _T("ID_M_PLBL22"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(0);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl10Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl10->SetFont(m_pLbl10Font);
	m_plblDistortionR = new wmLabel(this, ID_M_PLBL15, wxEmptyString, wxPoint(146,231), wxSize(150,40), 0, _T("ID_M_PLBL15"));
	m_plblDistortionR->SetBorderState(uiRect::BORDER_NONE);
	m_plblDistortionR->GetUiRect().SetGradient(0);
	m_plblDistortionR->SetForegroundColour(wxColour(255,255,255));
	m_plblDistortionR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDistortionRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDistortionR->SetFont(m_plblDistortionRFont);
	m_plblDistortionMaxR = new wmLabel(this, ID_M_PLBL24, wxEmptyString, wxPoint(297,231), wxSize(150,40), 0, _T("ID_M_PLBL24"));
	m_plblDistortionMaxR->SetBorderState(uiRect::BORDER_NONE);
	m_plblDistortionMaxR->GetUiRect().SetGradient(0);
	m_plblDistortionMaxR->SetForegroundColour(wxColour(255,255,255));
	m_plblDistortionMaxR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDistortionMaxRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDistortionMaxR->SetFont(m_plblDistortionMaxRFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL3, _("Phase:"), wxPoint(5,290), wxSize(120,40), 0, _T("ID_M_PLBL3"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl1Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pLbl8 = new wmLabel(this, ID_M_PLBL20, wxEmptyString, wxPoint(125,290), wxSize(20,40), 0, _T("ID_M_PLBL20"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl8Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl8->SetFont(m_pLbl8Font);
	m_plblPhaseSamples = new wmLabel(this, ID_M_PLBL4, _("Samples"), wxPoint(146,290), wxSize(150,40), 0, _T("ID_M_PLBL4"));
	m_plblPhaseSamples->SetBorderState(uiRect::BORDER_NONE);
	m_plblPhaseSamples->GetUiRect().SetGradient(0);
	m_plblPhaseSamples->SetForegroundColour(wxColour(255,255,255));
	m_plblPhaseSamples->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblPhaseSamplesFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblPhaseSamples->SetFont(m_plblPhaseSamplesFont);
	m_plblPhaseDegrees = new wmLabel(this, ID_M_PLBL5, _("Degrees"), wxPoint(297,290), wxSize(150,40), 0, _T("ID_M_PLBL5"));
	m_plblPhaseDegrees->SetBorderState(uiRect::BORDER_NONE);
	m_plblPhaseDegrees->GetUiRect().SetGradient(0);
	m_plblPhaseDegrees->SetForegroundColour(wxColour(255,255,255));
	m_plblPhaseDegrees->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblPhaseDegreesFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblPhaseDegrees->SetFont(m_plblPhaseDegreesFont);
	m_pLbl11 = new wmLabel(this, ID_M_PLBL25, _("LineUp:"), wxPoint(5,350), wxSize(120,40), 0, _T("ID_M_PLBL25"));
	m_pLbl11->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl11->GetUiRect().SetGradient(0);
	m_pLbl11->SetForegroundColour(wxColour(255,255,255));
	m_pLbl11->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl11Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl11->SetFont(m_pLbl11Font);
	m_pLbl12 = new wmLabel(this, ID_M_PLBL26, wxEmptyString, wxPoint(125,350), wxSize(20,40), 0, _T("ID_M_PLBL26"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl12Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_plblAudio = new wmLabel(this, ID_M_PLBL27, wxEmptyString, wxPoint(146,350), wxSize(301,40), 0, _T("ID_M_PLBL27"));
	m_plblAudio->SetBorderState(uiRect::BORDER_NONE);
	m_plblAudio->GetUiRect().SetGradient(0);
	m_plblAudio->SetForegroundColour(wxColour(255,255,255));
	m_plblAudio->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblAudioFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblAudio->SetFont(m_plblAudioFont);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&lineupPanel::OnbtnResetClick);
	//*)

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&lineupPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);

	m_nChannels = 2;
    m_nChannel[0] = 0;
	m_nChannel[1] = 1;

	m_vBufferL.reserve(4096);
    m_vBufferR.reserve(4096);

    long nBins = 4096;
    m_vfft_out.resize(nBins+1);

    m_nSampleSize = (m_vfft_out.size()-1)*2*m_nChannels;
    m_pLevelCalc = new LevelCalculator(-70.0);

    m_dDominantFrequencyMax[0] = 0.0;
    m_dDominantFrequencyMax[1] = 0.0;
    m_dDominantFrequencyMin[0] = 48000.0;
    m_dDominantFrequencyMin[1] = 48000.0;

    m_bFirstLevel = true;
    m_bFirstDistortion = true;
	m_pCalc = 0;
	std::map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().FindType(wxT("Digital"));
    if(itType != PPMTypeManager::Get().GetTypeEnd())
    {
        m_pLevelCalc->SetMode(itType->second.nType);
        m_pLevelCalc->SetDynamicResponse(itType->second.dRiseTime, itType->second.dRisedB, 200, itType->second.dFalldB);
    }

	Connect(wxID_ANY, wxEVT_OFFSET_DONE, (wxObjectEventFunction)&lineupPanel::OnOffsetDone);
}

lineupPanel::~lineupPanel()
{
	//(*Destroy(lineupPanel)
	//*)
}

void lineupPanel::SetAudioData(const timedbuffer* pBuffer)
{
    m_pLevelCalc->CalculateLevel(pBuffer);
    m_plblLevelL->SetLabel(wxString::Format(wxT("%.2f dBFS"), m_pLevelCalc->GetLevel(0)));
    m_plblLevelR->SetLabel(wxString::Format(wxT("%.2f dBFS"), m_pLevelCalc->GetLevel(1)));


    m_glitsDetector.SetAudioData(pBuffer);

    if(m_bFirstLevel)
    {
        m_dLevelMax[0] = m_pLevelCalc->GetLevel(0);
        m_dLevelMax[1] = m_pLevelCalc->GetLevel(1);
        m_dLevelMin[0] = m_pLevelCalc->GetLevel(0);
        m_dLevelMin[1] = m_pLevelCalc->GetLevel(1);
        m_bFirstLevel = false;
    }
    else
    {
        m_dLevelMax[0] = std::max(m_pLevelCalc->GetLevel(0), m_dLevelMax[0]);
        m_dLevelMax[1] = std::max(m_pLevelCalc->GetLevel(1), m_dLevelMax[1]);
        m_dLevelMin[0] = std::min(m_pLevelCalc->GetLevel(0), m_dLevelMin[0]);
        m_dLevelMin[1] = std::min(m_pLevelCalc->GetLevel(1), m_dLevelMin[1]);
    }

    m_plblRangeL->SetLabel(wxString::Format(wxT("%.3f dB"), (m_dLevelMax[0]-m_dLevelMin[0])));
    m_plblRangeR->SetLabel(wxString::Format(wxT("%.3f dB"), (m_dLevelMax[1]-m_dLevelMin[1])));

    for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
    {
        m_lstBufferL.push_back(pBuffer->GetBuffer()[i]);
        m_lstBufferR.push_back(pBuffer->GetBuffer()[i]);
    }

    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=m_nChannels)
    {
        if(m_vBufferL.size() == 4096)
        {
            if(m_pCalc == 0)
            {
                m_pCalc = new OffsetCalculator(this, m_vBufferL, m_vBufferR);
                m_pCalc->Create();
                m_pCalc->Run();
            }
            break;
        }
        else if(m_pCalc == 0)
        {
            m_vBufferL.push_back(pBuffer->GetBuffer()[i+m_nChannel[0]]);
            m_vBufferR.push_back(pBuffer->GetBuffer()[i+m_nChannel[1]]);
        }
    }

    switch(m_glitsDetector.GetType())
    {
    case GlitsDetector::GD_GLITS_LR:
        m_plblAudio->SetBackgroundColour(wxColour(0,128,64));
        m_plblAudio->SetLabel(wxT("GLITS"));
        break;
    case GlitsDetector::GD_GLITS_RL:
        m_plblAudio->SetBackgroundColour(wxColour(128,0,0));
        m_plblAudio->SetLabel(wxT("GLITS Legs Reversed"));
        break;
    case GlitsDetector::GD_MONO:
        if(m_dDistortionMax[0] == 0.0 && m_dDistortionMax[1] == 0.0)
        {
            m_plblAudio->SetBackgroundColour(wxColour(0,128,64));
            m_plblAudio->SetLabel(wxT("Mono Tone"));
        }
        else
        {
            m_plblAudio->SetBackgroundColour(wxColour(255,128,0));
            m_plblAudio->SetLabel(wxT("Unknown"));
        }
        break;
    case GlitsDetector::GD_EBU_LR:
        m_plblAudio->SetBackgroundColour(wxColour(0,128,64));
        m_plblAudio->SetLabel(wxT("EBU"));
        break;
    case GlitsDetector::GD_EBU_RL:
        m_plblAudio->SetBackgroundColour(wxColour(128,0,0));
        m_plblAudio->SetLabel(wxT("EBU Legs Reversed"));
        break;
    default:
        m_plblAudio->SetBackgroundColour(wxColour(255,128,0));
        m_plblAudio->SetLabel(wxT("Unknown"));
        break;
    }
}

void lineupPanel::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
        m_nChannels = std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nSampleRate = 48000;
        m_nChannels = 2;
    }

    m_nSampleSize = (m_vfft_out.size()-1)*2*m_nChannels;

    m_pLevelCalc->InputSession(aSession);
}

void lineupPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void lineupPanel::OnLeftUp(wxMouseEvent& event)
{

}


void lineupPanel::OnOffsetDone(wxCommandEvent& event)
{
    m_pCalc = 0;

    //copy the data in to the buffer and do the fft
    while(m_lstBufferL.size() > m_nSampleSize)
    {
        DoFFT();
    }

    m_vBufferL.clear();
    m_vBufferR.clear();
    m_vBufferL.reserve(4096);
    m_vBufferR.reserve(4096);

    m_dOffsetSamples = event.GetInt();

    double dDegPerSample = 360.0/(48000.0/m_dDominantFrequency[0]);
    double dPhase = dDegPerSample*m_dOffsetSamples;

    m_plblDominantHzL->SetLabel(wxString::Format(wxT("%.f Hz"), m_dDominantFrequency[0]));

    m_plblDominantdBL->SetLabel(wxString::Format(wxT("%.2f Hz"), m_dDominantFrequencyMax[0]-m_dDominantFrequencyMin[0]));

    m_plblDominantHzR->SetLabel(wxString::Format(wxT("%.f Hz"), m_dDominantFrequency[1]));
    m_plblDominantdBR->SetLabel(wxString::Format(wxT("%.2f Hz"), m_dDominantFrequencyMax[1]-m_dDominantFrequencyMin[1]));


    m_plblPhaseSamples->SetLabel(wxString::Format(wxT("%.0f"), m_dOffsetSamples));
    m_plblPhaseDegrees->SetLabel(wxString::Format(wxT("%.2f'"), dPhase));
}

void lineupPanel::DoFFT()
{
    FFTAlgorithm fft;
    double dDistortion = fft.GetTHDistortion(m_lstBufferL, m_nSampleRate, m_nChannels, false, 5, m_vfft_out.size(), m_nSampleSize/2);
    if(m_bFirstDistortion)
    {
        m_dDistortionMax[0] = dDistortion;
    }
    else
    {
        m_dDistortionMax[0] = std::max(dDistortion, m_dDistortionMax[0]);
    }

    m_dDominantFrequency[0] = fft.GetFundamentalBinFrequency();
    if(fft.GetFundamentalAmplitude() > -60.0)
    {
        m_dDominantFrequencyMax[0] = max(m_dDominantFrequencyMax[0], m_dDominantFrequency[0]);
        m_dDominantFrequencyMin[0] = min(m_dDominantFrequencyMin[0], m_dDominantFrequency[0]);
    }

    m_plblDistortionL->SetLabel(wxString::Format(wxT("%.2f%"), dDistortion));


    dDistortion = fft.GetTHDistortion(m_lstBufferR, m_nSampleRate, m_nChannels, true, 5, m_vfft_out.size(), m_nSampleSize/2);

    m_dDominantFrequency[1] = fft.GetFundamentalBinFrequency();
    if(fft.GetFundamentalAmplitude() > -60.0)
    {
        m_dDominantFrequencyMax[1] = max(m_dDominantFrequencyMax[1], m_dDominantFrequency[1]);
        m_dDominantFrequencyMin[1] = min(m_dDominantFrequencyMin[1], m_dDominantFrequency[1]);
    }

    m_plblDistortionR->SetLabel(wxString::Format(wxT("%.2f%"), dDistortion));
    if(m_bFirstDistortion)
    {
        m_dDistortionMax[1] = dDistortion;
        m_bFirstDistortion = false;
    }
    else
    {
        m_dDistortionMax[1] = std::max(dDistortion, m_dDistortionMax[1]);
    }

    m_plblDistortionMaxL->SetLabel(wxString::Format(wxT("%.2f%"), m_dDistortionMax[0]));
    m_plblDistortionMaxR->SetLabel(wxString::Format(wxT("%.2f%"), m_dDistortionMax[1]));
}

void lineupPanel::OnbtnResetClick(wxCommandEvent& event)
{
    m_bFirstLevel = true;
    m_bFirstDistortion = true;

    m_dDominantFrequencyMax[0] = 0.0;
    m_dDominantFrequencyMax[1] = 0.0;
    m_dDominantFrequencyMin[0] = 48000.0;
    m_dDominantFrequencyMin[1] = 48000.0;

}
