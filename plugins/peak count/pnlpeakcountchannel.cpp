#include "pnlpeakcountchannel.h"

//(*InternalHeaders(pnlPeakCountChannel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlPeakCountChannel)
const long pnlPeakCountChannel::ID_M_PLBL9 = wxNewId();
const long pnlPeakCountChannel::ID_M_PLBL1 = wxNewId();
const long pnlPeakCountChannel::ID_M_PLBL3 = wxNewId();
const long pnlPeakCountChannel::ID_M_PLBL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlPeakCountChannel,wxPanel)
	//(*EventTable(pnlPeakCountChannel)
	//*)
END_EVENT_TABLE()

pnlPeakCountChannel::pnlPeakCountChannel(wxWindow* parent,unsigned int nChannel, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlPeakCountChannel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plbLChannel = new wmLabel(this, ID_M_PLBL9, wxEmptyString, wxPoint(0,0), wxSize(70,0), 0, _T("ID_M_PLBL9"));
	m_plbLChannel->SetBorderState(uiRect::BORDER_FLAT);
	m_plbLChannel->SetForegroundColour(wxColour(255,255,255));
	m_plbLChannel->SetBackgroundColour(wxColour(64,0,64));
	wxFont m_plbLChannelFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plbLChannel->SetFont(m_plbLChannelFont);
	m_plblCount = new wmLabel(this, ID_M_PLBL1, wxEmptyString, wxPoint(0,30), wxSize(70,25), 0, _T("ID_M_PLBL1"));
	m_plblCount->SetBorderState(uiRect::BORDER_FLAT);
	m_plblCount->SetForegroundColour(wxColour(255,255,255));
	m_plblCount->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblCountFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblCount->SetFont(m_plblCountFont);
	m_plblPeaks = new wmLabel(this, ID_M_PLBL3, wxEmptyString, wxPoint(0,60), wxSize(70,25), 0, _T("ID_M_PLBL3"));
	m_plblPeaks->SetBorderState(uiRect::BORDER_FLAT);
	m_plblPeaks->SetForegroundColour(wxColour(255,255,255));
	m_plblPeaks->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblPeaksFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblPeaks->SetFont(m_plblPeaksFont);
	m_plblPercent = new wmLabel(this, ID_M_PLBL2, wxEmptyString, wxPoint(0,90), wxSize(70,25), 0, _T("ID_M_PLBL2"));
	m_plblPercent->SetBorderState(uiRect::BORDER_FLAT);
	m_plblPercent->SetForegroundColour(wxColour(255,255,255));
	m_plblPercent->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblPercentFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblPercent->SetFont(m_plblPercentFont);
	//*)
	SetSize(wxSize(70,130));
	SetPosition(pos);

	ResetTest();
	m_plbLChannel->SetLabel(wxString::Format(wxT("Ch %d"), nChannel));
}

pnlPeakCountChannel::~pnlPeakCountChannel()
{
	//(*Destroy(pnlPeakCountChannel)
	//*)
}


void pnlPeakCountChannel::AddPeaks(unsigned int nSamples, unsigned int nPeaks, unsigned int nTotal)
{
    m_nPeaks+= nPeaks;
    m_nSamples+=nSamples;
    m_nTotal += nTotal;

    UpdateLabels();
}

void pnlPeakCountChannel::UpdateLabels()
{
    m_plblCount->SetLabel(wxString::Format(wxT("%d"), m_nSamples));
    m_plblPeaks->SetLabel(wxString::Format(wxT("%d"), m_nPeaks));
    if(m_nTotal != 0)
    {
        m_plblPercent->SetLabel(wxString::Format(wxT("%.2f%%"), (static_cast<double>(m_nSamples)/static_cast<double>(m_nTotal))*100.0));
    }
    else
    {
        m_plblPercent->SetLabel(wxEmptyString);
    }
}

void pnlPeakCountChannel::ResetTest()
{
    m_nPeaks = 0;
    m_nSamples = 0;
    m_nTotal = 0;
    m_dLastAmplitude = -80.0;
    UpdateLabels();
}
