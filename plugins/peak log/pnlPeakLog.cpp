#include "pnlPeakLog.h"
#include "session.h"
#include "timedbuffer.h"

//(*InternalHeaders(pnlPeakLog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlPeakLog)
const long pnlPeakLog::ID_M_PLBL37 = wxNewId();
const long pnlPeakLog::ID_CUSTOM7 = wxNewId();
const long pnlPeakLog::ID_CUSTOM8 = wxNewId();
const long pnlPeakLog::ID_CUSTOM9 = wxNewId();
const long pnlPeakLog::ID_CUSTOM12 = wxNewId();
const long pnlPeakLog::ID_M_PLBL1 = wxNewId();
const long pnlPeakLog::ID_M_PLBL2 = wxNewId();
const long pnlPeakLog::ID_M_PLBL3 = wxNewId();
const long pnlPeakLog::ID_M_PLBL4 = wxNewId();
const long pnlPeakLog::ID_M_PLST1 = wxNewId();
const long pnlPeakLog::ID_M_PBTN2 = wxNewId();
const long pnlPeakLog::ID_M_PBTN1 = wxNewId();
//*)
const wxString pnlPeakLog::GRAPH_LINES[8] = {wxT("Ch 1"), wxT("Ch 2"), wxT("Ch 3"), wxT("Ch 4"), wxT("Ch 5"), wxT("Ch 6"), wxT("Ch 7"), wxT("Ch 8")};
const wxColour pnlPeakLog::COLOUR_LINES[8] = {*wxRED, *wxGREEN, wxColour(255,165,0), wxColour(255,255,0), wxColour(0,0,255), wxColour(148,0,211), wxColour(238,130,238), wxColour(255,255,255)};

BEGIN_EVENT_TABLE(pnlPeakLog,wxPanel)
	//(*EventTable(pnlPeakLog)
	//*)
END_EVENT_TABLE()

pnlPeakLog::pnlPeakLog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) //: m_ploud(0)
{
	//(*Initialize(pnlPeakLog)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plblTitle = new wmLabel(this, ID_M_PLBL37, _("Peak Level Log"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_plblTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblTitle->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_plblTitleFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblTitle->SetFont(m_plblTitleFont);
	m_pLevelGraph_Day = new LevelGraph(this,ID_CUSTOM7, wxPoint(0,70),wxSize(72,350), 56400,0, -50);
	m_pLevelGraph_Hour = new LevelGraph(this,ID_CUSTOM8, wxPoint(72,70),wxSize(360,350), 470, 0,-50);
	m_pLevelGraph_Minute = new LevelGraph(this,ID_CUSTOM9, wxPoint(432,70),wxSize(120,350),24,0,-50);
	m_pLevelGraph_Second = new LevelGraph(this,ID_CUSTOM12, wxPoint(552,70),wxSize(47,350),1,0,-50);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Day"), wxPoint(0,40), wxSize(72,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,64));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Hour"), wxPoint(72,40), wxSize(360,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,64));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Minute"), wxPoint(432,40), wxSize(120,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(64,0,64));
	m_pLbl4 = new wmLabel(this, ID_M_PLBL4, _("Second"), wxPoint(552,40), wxSize(47,30), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(64,0,64));
	m_plstGraphs = new wmList(this, ID_M_PLST1, wxPoint(5,430), wxSize(400,50), 0, 0, wxSize(-1,-1), 8, wxSize(5,0));
	m_plstGraphs->SetBackgroundColour(wxColour(0,0,0));
	m_plstGraphs->SetButtonColour(wxColour(wxT("#004000")));
	m_pbtnType = new wmButton(this, ID_M_PBTN2, _("Log Loudness"), wxPoint(410,432), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnType->SetBackgroundColour(wxColour(37,62,90));
	m_pbtnType->SetColourSelected(wxColour(wxT("#1D3147")));
	m_pbtnClear = new wmButton(this, ID_M_PBTN1, _("Clear Graphs"), wxPoint(505,432), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClear->SetBackgroundColour(wxColour(128,64,0));
	m_pbtnClear->SetColourSelected(wxColour(wxT("#5E2F00")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPeakLog::OnlstGraphsSelected);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPeakLog::OnbtnTypeClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPeakLog::OnbtnClearClick);
	//*)

	SetPosition(pos);
	SetSize(size);



    m_bLoudness = false;
}

pnlPeakLog::~pnlPeakLog()
{
	//(*Destroy(pnlPeakLog)
	//*)
//	delete m_ploud;
}


void pnlPeakLog::InputSession(const session& aSession)
{
//    if(m_ploud)
//    {
//        delete m_ploud;
//    }
    m_nChannels = 0;
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nChannels = min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
    }
//    m_ploud = new loud(m_nChannels);

    m_pLevelGraph_Day->DeleteAllGraphs();
    m_pLevelGraph_Hour->DeleteAllGraphs();
    m_pLevelGraph_Minute->DeleteAllGraphs();
    m_pLevelGraph_Second->DeleteAllGraphs();

    m_plstGraphs->Clear();

    AddLines(m_pLevelGraph_Day);
    AddLines(m_pLevelGraph_Hour);
    AddLines(m_pLevelGraph_Minute);
    AddLines(m_pLevelGraph_Second);

    for(int i = 0; i < m_nChannels; i++)
    {
        m_pLevelGraph_Day->AddGraph(GRAPH_LINES[i], COLOUR_LINES[i]);
        m_pLevelGraph_Hour->AddGraph(GRAPH_LINES[i], COLOUR_LINES[i]);
        m_pLevelGraph_Minute->AddGraph(GRAPH_LINES[i], COLOUR_LINES[i]);
        m_pLevelGraph_Second->AddGraph(GRAPH_LINES[i], COLOUR_LINES[i]);

        m_pLevelGraph_Day->SetLimit(GRAPH_LINES[i], 0,-50);
        m_pLevelGraph_Hour->SetLimit(GRAPH_LINES[i], 0,-50);
        m_pLevelGraph_Minute->SetLimit(GRAPH_LINES[i], 0,-50);
        m_pLevelGraph_Second->SetLimit(GRAPH_LINES[i], 0,-50);


        m_plstGraphs->AddButton(wxString::Format(wxT("Hide %s"), GRAPH_LINES[i].c_str()));
    }
}

void pnlPeakLog::AddLines(LevelGraph* pGraph)
{
    pGraph->ClearLines();
    pGraph->AddLine(-36.0, wxPen(*wxWHITE, 1));
    pGraph->AddLine(-24.0, wxPen(*wxWHITE, 1));
    pGraph->AddLine(-18.0, wxPen(wxColour(0,0,255), 1));
    pGraph->AddLine(-12.0, wxPen(wxColour(255,200,0), 1));
    pGraph->AddLine(-6.0, wxPen(wxColour(255,0,0), 1));

}

void pnlPeakLog::SetAudioData(const timedbuffer* pBuffer)
{
    vector<float> vPeak(m_nChannels, 0.0);

    if(!m_bLoudness)
    {
        //find largest value
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+= m_nChannels)
        {
            for(size_t j = 0; j < vPeak.size(); j++)
            {
                vPeak[j] = max((double)fabs(pBuffer->GetBuffer()[i+j]), (double)vPeak[j]);
            }
        }

        for(int j=0; j < vPeak.size(); j++)
        {
            vPeak[j] = 20*log10(vPeak[j]);
        }
    }
    else
    {
//        if(m_ploud)
//        {
//            m_ploud->calcIntermediate(m_nChannels, pBuffer->GetBufferSize()/m_nChannels, pBuffer->GetBuffer());
//            for(int j=0; j < vPeak.size(); j++)
//            {
//                vPeak[j] = m_ploud->getValue(j);
//            }
//        }
    }
    for(int j=0; j < vPeak.size(); j++)
    {
        m_pLevelGraph_Day->AddPeak(GRAPH_LINES[j], vPeak[j]);
        m_pLevelGraph_Day->AddPeak(GRAPH_LINES[j], vPeak[j]);

        m_pLevelGraph_Hour->AddPeak(GRAPH_LINES[j],vPeak[j]);
        m_pLevelGraph_Hour->AddPeak(GRAPH_LINES[j],vPeak[j]);

        m_pLevelGraph_Minute->AddPeak(GRAPH_LINES[j], vPeak[j]);
        m_pLevelGraph_Minute->AddPeak(GRAPH_LINES[j], vPeak[j]);

        m_pLevelGraph_Second->AddPeak(GRAPH_LINES[j], vPeak[j]);
        m_pLevelGraph_Second->AddPeak(GRAPH_LINES[j], vPeak[j]);


    }

}

void pnlPeakLog::OnlstGraphsSelected(wxCommandEvent& event)
{
    bool bShow(false);
    wxString sLine(event.GetString().AfterFirst(wxT(' ')));
    wxString sNewText(wxT("Show "));
    wxColour clr(64,0,0);
    if(event.GetString().BeforeFirst(wxT(' ')) == wxT("Show"))
    {
        sNewText = wxT("Hide ");
        bShow = true;
        clr = wxColour(0,64,0);
    }
    m_plstGraphs->SetButtonText(event.GetInt(), wxString::Format(wxT("%s%s"), sNewText.c_str(), sLine.c_str()));
    m_plstGraphs->SetButtonColour(event.GetInt(), clr);

    m_pLevelGraph_Day->ShowGraph(sLine, bShow);
    m_pLevelGraph_Hour->ShowGraph(sLine, bShow);
    m_pLevelGraph_Minute->ShowGraph(sLine, bShow);
    m_pLevelGraph_Second->ShowGraph(sLine, bShow);
}

void pnlPeakLog::OnbtnClearClick(wxCommandEvent& event)
{
    ClearGraphs();
}

void pnlPeakLog::ClearGraphs()
{
    m_pLevelGraph_Day->ClearGraphs();
    m_pLevelGraph_Hour->ClearGraphs();
    m_pLevelGraph_Minute->ClearGraphs();
    m_pLevelGraph_Second->ClearGraphs();

    for(int i = 0; i < m_nChannels; i++)
    {
        m_pLevelGraph_Day->SetLimit(GRAPH_LINES[i], 0,-50);
        m_pLevelGraph_Hour->SetLimit(GRAPH_LINES[i], 0,-50);
        m_pLevelGraph_Minute->SetLimit(GRAPH_LINES[i], 0,-50);
        m_pLevelGraph_Second->SetLimit(GRAPH_LINES[i], 0,-50);
    }
}

void pnlPeakLog::SetLogType(bool bLUFS)
{
    ClearGraphs();
    m_pLevelGraph_Day->ClearZones();
    m_pLevelGraph_Hour->ClearZones();
    m_pLevelGraph_Minute->ClearZones();
    m_pLevelGraph_Second->ClearZones();

    m_bLoudness = bLUFS;

    if(m_bLoudness)
    {
        m_pLevelGraph_Day->AddZone(-70,-29, *wxRED);
        m_pLevelGraph_Day->AddZone(-19,0, *wxRED);
        m_pLevelGraph_Day->AddZone(-29,-24, wxColour(255,255,0));
        m_pLevelGraph_Day->AddZone(-22,-19, wxColour(255,255,0));
        m_pLevelGraph_Day->AddZone(-24,-22, *wxGREEN);

        m_pLevelGraph_Hour->AddZone(-70,-29, *wxRED);
        m_pLevelGraph_Hour->AddZone(-19,0, *wxRED);
        m_pLevelGraph_Hour->AddZone(-29,-24, wxColour(255,255,0));
        m_pLevelGraph_Hour->AddZone(-22,-19, wxColour(255,255,0));
        m_pLevelGraph_Hour->AddZone(-24,-22, *wxGREEN);

        m_pLevelGraph_Minute->AddZone(-70,-29, *wxRED);
        m_pLevelGraph_Minute->AddZone(-19,0, *wxRED);
        m_pLevelGraph_Minute->AddZone(-29,-24, wxColour(255,255,0));
        m_pLevelGraph_Minute->AddZone(-22,-19, wxColour(255,255,0));
        m_pLevelGraph_Minute->AddZone(-24,-22, *wxGREEN);

        m_pLevelGraph_Second->AddZone(-70,-29, *wxRED);
        m_pLevelGraph_Second->AddZone(-19,0, *wxRED);
        m_pLevelGraph_Second->AddZone(-29,-24, wxColour(255,255,0));
        m_pLevelGraph_Second->AddZone(-22,-19, wxColour(255,255,0));
        m_pLevelGraph_Second->AddZone(-24,-22, *wxGREEN);
    }
}

void pnlPeakLog::OnbtnTypeClick(wxCommandEvent& event)
{

    SetLogType(!m_bLoudness);
    if(m_bLoudness)
    {
        m_pbtnType->SetLabel(wxT("Log Peaks"));
        m_plblTitle->SetLabel(wxT("Loudness Log"));


    }
    else
    {
        m_pbtnType->SetLabel(wxT("Log Loudness"));
        m_plblTitle->SetLabel(wxT("Peak Level Log"));
    }
}





