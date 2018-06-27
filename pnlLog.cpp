#include "pnlLog.h"
#include <wx/tokenzr.h>
#include "settings.h"
#include "pnlLogControl.h"
#include "wmlogevent.h"
#include <wx/log.h>

//(*InternalHeaders(pnlLog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlLog)
const long pnlLog::ID_HTMLWINDOW1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlLog,wxPanel)
	//(*EventTable(pnlLog)
	//*)
END_EVENT_TABLE()

pnlLog::pnlLog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pControl(0)
{
	//(*Initialize(pnlLog)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_phtmlLog = new wxTouchScreenHtml(this, ID_HTMLWINDOW1, wxPoint(0,0), wxSize(620,480), 0, _T("ID_HTMLWINDOW1"));
	//*)


	m_sTableStart = wxT("<table border=\"1\" width=\"100%\"><tr><td width=\"20%\"><b>Time</b></td><td width=\"80%\"><b>Message</b></td></tr>");
	m_sTableEnd = wxT("<\table>");

	m_phtmlLog->SetPageTouch(m_sTableStart+m_sTableEnd);

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

    sLogEntry.Replace(wxT("\n"), wxT("<br>"));

    m_sTableMiddle << wxT("<tr><td>") << wxDateTime::UNow().Format(wxT("%H:%M:%S:%l")) << wxT("</td><td>") << sLogEntry << wxT("</td></tr>");

    m_phtmlLog->Freeze();
    m_phtmlLog->SetPageTouch(m_sTableStart+m_sTableMiddle+m_sTableEnd);
    m_phtmlLog->Thaw();

}


void pnlLog::Home()
{
    m_phtmlLog->Home();
}

void pnlLog::End()
{
    m_phtmlLog->End();
}

void pnlLog::PageUp()
{
    m_phtmlLog->PageUp();
}

void pnlLog::PageDown()
{
    m_phtmlLog->PageDown();
}

void pnlLog::ScrollLock(bool bLock)
{
    m_phtmlLog->SetScrollLock(bLock);
}

void pnlLog::Clear()
{
    m_sTableMiddle = wxEmptyString;
    m_phtmlLog->SetPageTouch(m_sTableStart+m_sTableMiddle+m_sTableEnd);
}




void pnlLog::OnLog(wmLogEvent& event)
{
    Log(event.GetLogMessage());
}

