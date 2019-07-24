#include "batterypanel.h"
#include "batterybuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "myprocess.h"


//(*InternalHeaders(batteryPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(batteryPanel)
const long batteryPanel::ID_M_PMETER1 = wxNewId();
const long batteryPanel::ID_M_PLBL1 = wxNewId();
const long batteryPanel::ID_M_PLBL4 = wxNewId();
const long batteryPanel::ID_M_PLBL2 = wxNewId();
const long batteryPanel::ID_M_PLBL5 = wxNewId();
const long batteryPanel::ID_M_PLBL3 = wxNewId();
const long batteryPanel::ID_M_PLBL6 = wxNewId();
const long batteryPanel::ID_TIMER_CHECK = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(batteryPanel, pmPanel);

batteryPanel::batteryPanel(wxWindow* parent,batteryBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlTest)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&batteryPanel::OnLeftUp);

	Connect(wxID_ANY, wxEVT_PROCESS_FINISHED, (wxObjectEventFunction)&batteryPanel::OnProcess);

	SetSize(size);
	SetPosition(pos);
}

batteryPanel::~batteryPanel()
{
	//(*Destroy(batteryPanel)
	//*)
}

void batteryPanel::SetAudioData(const timedbuffer* pBuffer)
{

}

void batteryPanel::InputSession(const session& aSession)
{

}

void batteryPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void batteryPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}



void batteryPanel::OntimerCheckTrigger(wxTimerEvent& event)
{
    //run the script
   // m_pProcess = new MyProcess(this);
    //if(!wxExecute("sudo ./status.py", wxEXEC_ASYNC, m_pProcess))
    //{
     //   delete m_pProcess;
      //  m_pProcess = 0;
       // m_plblError->SetLabel(wxT("Process Failed!"));
    //}
}


void batteryPanel::OnProcess(wxCommandEvent& event)
{

}
