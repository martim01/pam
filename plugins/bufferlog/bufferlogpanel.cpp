#include "bufferlogpanel.h"
#include "bufferlogbuilder.h"
#include "timedbuffer.h"
#include "session.h"

//(*InternalHeaders(bufferlogPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(bufferlogPanel)
const long bufferlogPanel::ID_M_PLST1 = wxNewId();
const long bufferlogPanel::ID_M_PLBL1 = wxNewId();
const long bufferlogPanel::ID_M_PLBL2 = wxNewId();
const long bufferlogPanel::ID_M_PLBL4 = wxNewId();
const long bufferlogPanel::ID_M_PLBL3 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(bufferlogPanel,pmPanel);

BEGIN_EVENT_TABLE(bufferlogPanel,wxPanel)
	//(*EventTable(bufferlogPanel)
	//*)
END_EVENT_TABLE()

bufferlogPanel::bufferlogPanel(wxWindow* parent,bufferlogBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(bufferlogPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstBuffer = new wmList(this, ID_M_PLST1, wxPoint(0,0), wxSize(200,480), 0, 1, wxSize(-1,25), 1, wxSize(0,0));
	m_plstBuffer->SetBackgroundColour(wxColour(0,0,0));
	m_plstBuffer->SetBorderStyle(3);
	m_plstBuffer->SetButtonColour(wxColour(wxT("#000000")));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Default Buffer"), wxPoint(220,0), wxSize(100,25), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(0,128,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
	m_plblBuffer = new wmLabel(this, ID_M_PLBL2, wxEmptyString, wxPoint(320,0), wxSize(100,25), 0, _T("ID_M_PLBL2"));
	m_plblBuffer->SetBorderState(uiRect::BORDER_NONE);
	m_plblBuffer->GetUiRect().SetGradient(0);
	m_plblBuffer->SetForegroundColour(wxColour(0,128,255));
	m_plblBuffer->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL4, _("Buffers Less"), wxPoint(220,50), wxSize(100,25), 0, _T("ID_M_PLBL4"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(0,128,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	m_plblLess = new wmLabel(this, ID_M_PLBL3, wxEmptyString, wxPoint(320,50), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_plblLess->SetBorderState(uiRect::BORDER_NONE);
	m_plblLess->GetUiRect().SetGradient(0);
	m_plblLess->SetForegroundColour(wxColour(0,128,255));
	m_plblLess->SetBackgroundColour(wxColour(0,0,0));
	//*)

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&bufferlogPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);
	m_nDefaultBuffer = 0;
	m_nLess = 0;
}

bufferlogPanel::~bufferlogPanel()
{
	//(*Destroy(bufferlogPanel)
	//*)
}

void bufferlogPanel::SetAudioData(const timedbuffer* pBuffer)
{
    size_t nIndex(0);
    m_plstBuffer->Freeze();
	if(m_nDefaultBuffer < pBuffer->GetBufferSize())
    {
        m_nDefaultBuffer = pBuffer->GetBufferSize();
        m_plblBuffer->SetLabel(wxString::Format(wxT("%u"), m_nDefaultBuffer));

        nIndex = m_plstBuffer->AddButton(wxString::Format(wxT("%u"), pBuffer->GetBufferSize()), wxNullBitmap, 0, wmList::wmENABLED, *wxGREEN);
        m_nLess = 0;
    }
    else if(m_nDefaultBuffer > pBuffer->GetBufferSize())
    {
        nIndex = m_plstBuffer->AddButton(wxString::Format(wxT("%u"), pBuffer->GetBufferSize()), wxNullBitmap, 0, wmList::wmENABLED, *wxRED);
        ++m_nLess;
    }
    else
    {
        nIndex = m_plstBuffer->AddButton(wxString::Format(wxT("%u"), pBuffer->GetBufferSize()), wxNullBitmap, 0, wmList::wmENABLED, *wxBLACK);
    }
    m_plstBuffer->Thaw();
    m_plstBuffer->ShowButton(nIndex,wmList::BOTTOM,false);
}

void bufferlogPanel::InputSession(const session& aSession)
{

}

void bufferlogPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void bufferlogPanel::OnLeftUp(wxMouseEvent& event)
{

}


