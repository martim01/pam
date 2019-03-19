#include "pnlLog.h"
#include <wx/tokenzr.h>
#include "settings.h"
#include "pnlLogControl.h"
#include "wmlogevent.h"
#include <wx/log.h>
#include "logelement.h"
#include <wx/dcclient.h>

//(*InternalHeaders(pnlLog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;


BEGIN_EVENT_TABLE(pnlLog,wxPanel)
	//(*EventTable(pnlLog)
	//*)
END_EVENT_TABLE()

pnlLog::pnlLog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pControl(0)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLogList = new wmListAdv(this, wxNewId(), wxPoint(0,0), wxSize(600,480), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
	m_pLogList->SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));
	m_pLogList->SetBackgroundColour(wxColour(100,100,180));
	//*)



	wmLog::Get()->SetTarget(this);
	Connect(wxID_ANY,wxEVT_WMLOG,(wxObjectEventFunction)&pnlLog::OnLog);
}

pnlLog::~pnlLog()
{
	//(*Destroy(pnlLog)
	//*)
}

void pnlLog::SetLogControl(pnlLogControl* pControl)
{
    m_pControl = pControl;
}
void pnlLog::Log(wxString sLogEntry)
{
    #ifdef __WXDEBUG__
        cout << sLogEntry.mb_str() << endl;
    #endif // __WXDEBUG__

    wxClientDC dc(this);
    dc.SetFont(m_pLogList->GetFont());

    bool bTest(false);
    if(sLogEntry.Find(wxT("**TESTS**")) != wxNOT_FOUND)
    {
        sLogEntry = sLogEntry.Mid(9);
        bTest = true;
    }

    m_pLogList->AddElement(new LogElement(dc, GetClientSize().x, sLogEntry, bTest));
    //m_pLogList->Refresh();
    if(!m_bScrollLock)
    {
        End();
    }
}


void pnlLog::Home()
{
    m_pLogList->ShowElement(0, wmListAdv::TOP);
}

void pnlLog::End()
{
    m_pLogList->ShowElement(m_pLogList->GetElementCount()-1, wmListAdv::BOTTOM);
}

void pnlLog::PageUp()
{
    m_pLogList->ScrollVertical(480);
}

void pnlLog::PageDown()
{
    m_pLogList->ScrollVertical(-480);
}

void pnlLog::ScrollLock(bool bLock)
{
    m_bScrollLock = bLock;
}

void pnlLog::Clear()
{
    m_pLogList->Clear();
}




void pnlLog::OnLog(wmLogEvent& event)
{
    Log(event.GetLogMessage());
}


