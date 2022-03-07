#include "pnlRouting.h"
#include "lissajoubuilder.h"

//(*InternalHeaders(pnlRouting)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlRouting)
const long pnlRouting::ID_M_PLBL2 = wxNewId();
const long pnlRouting::ID_M_PLST1 = wxNewId();
const long pnlRouting::ID_M_PLST2 = wxNewId();
const long pnlRouting::ID_M_PLBL1 = wxNewId();
const long pnlRouting::ID_M_PLST3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlRouting,wxPanel)
	//(*EventTable(pnlRouting)
	//*)
END_EVENT_TABLE()

pnlRouting::pnlRouting(wxWindow* parent,LissajouBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlRouting)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Y Axis"), wxPoint(0,125), wxSize(200,20), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,168,0));
	m_plstY = new wmList(this, ID_M_PLST1, wxPoint(0,145), wxSize(200,62), wmList::STYLE_SELECT, 0, wxSize(-1,30), 4, wxSize(-1,-1));
	m_plstY->SetBackgroundColour(wxColour(0,0,0));
	m_plstX = new wmList(this, ID_M_PLST2, wxPoint(0,60), wxSize(200,62), wmList::STYLE_SELECT, 0, wxSize(-1,30), 4, wxSize(-1,-1));
	m_plstX->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("X Axis"), wxPoint(0,40), wxSize(200,20), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(168,0,0));
	m_plstMonitor = new wmList(this, ID_M_PLST3, wxPoint(0,0), wxSize(200,35), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(2,1));
	m_plstMonitor->SetBackgroundColour(wxColour(0,0,0));
	m_plstMonitor->SetButtonColour(wxColour(wxT("#0000A0")));
	m_plstMonitor->SetSelectedButtonColour(wxColour(wxT("#FF8000")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstYSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstXSelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstMonitorSelected);
	//*)

	m_plstMonitor->AddButton(wxT("Follow"));
	m_plstMonitor->AddButton(wxT("Force"));
	m_plstMonitor->AddButton(wxT("Indy"));

	m_plstMonitor->ConnectToSetting(m_pBuilder->GetSection(), "Monitor", size_t(0));

	m_plstX->ConnectToSetting(m_pBuilder->GetSection(), "Axis_X", size_t(0));
	m_plstY->ConnectToSetting(m_pBuilder->GetSection(), "Axis_Y", size_t(1));
}

pnlRouting::~pnlRouting()
{
	//(*Destroy(pnlRouting)
	//*)
}


void pnlRouting::OnlstYSelected(wxCommandEvent& event)
{
}

void pnlRouting::OnlstXSelected(wxCommandEvent& event)
{
}


void pnlRouting::SetNumberOfChannels(unsigned int nChannels)
{
    m_plstX->Freeze();
    m_plstY->Freeze();
    m_plstX->Clear();
    m_plstY->Clear();

    if(nChannels != 2)
    {
        for(unsigned int i = 0; i < nChannels; i++)
        {
            m_plstX->AddButton(wxString::Format(wxT("CH: %d"), i));
            m_plstY->AddButton(wxString::Format(wxT("CH: %d"), i));
        }
    }
    else
    {
            m_plstX->AddButton(wxT("Left"));
            m_plstY->AddButton(wxT("Left"));
            m_plstX->AddButton(wxT("Right"));
            m_plstY->AddButton(wxT("Right"));
    }
    m_plstX->Thaw();
    m_plstY->Thaw();

    int nX = m_pBuilder->ReadSetting(wxT("Axis_X"), 0);
    if(nX >= nChannels)
        nX = 0;

    int nY = m_pBuilder->ReadSetting(wxT("Axis_Y"), 1);
    if(nY >= nChannels)
        nY = 1;

    m_plstX->SelectButton(nX, true);
    m_plstY->SelectButton(nY, true);

}

void pnlRouting::OnlstMonitorSelected(wxCommandEvent& event)
{

    switch(event.GetInt())
    {
        case 0:
           EnableLists(false);
           break;
        case 1:
            EnableLists(true);
            break;
        case 2:
            EnableLists(true);
            break;
    }
}


void pnlRouting::EnableLists(bool bEnable)
{
    m_plstX->Enable(bEnable);
    m_plstY->Enable(bEnable);
}


void pnlRouting::SetAxis(unsigned int nLeft, unsigned int nRight)
{
    m_plstX->SelectButton(nLeft,false);
    m_plstY->SelectButton(nRight,false);
}
