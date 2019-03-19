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


	m_phtmlLog->SetPage(wxEmptyString);

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

    m_lstLog.push_back(sLogEntry);

    ShowLog();

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
    m_bScrollLock = bLock;
}

void pnlLog::Clear()
{
    m_phtmlLog->SetPage(wxEmptyString  );
}




void pnlLog::OnLog(wmLogEvent& event)
{
    Log(event.GetLogMessage());
}


void pnlLog::ShowLog()
{
    wxString sLog;
    for(std::list<wxString>::iterator itLog = m_lstLog.begin(); itLog != m_lstLog.end(); ++itLog)
    {
        wxString sLogEntry(*itLog);

        sLogEntry.Replace(wxT("\n"), wxT("<br>"));
        wxString sTab(wxT("<p style='font-size: small'>"));

        if(sLogEntry.Find(wxT("**TESTS**")) != wxNOT_FOUND)
        {
            sLogEntry = sLogEntry.Mid(9);
            sTab = wxT("<p bgcolor=\"#ffaaaa\">");
        }

        sLog <<  sTab << wxDateTime::UNow().Format(wxT("%H:%M:%S:%l")) << wxT("&nbsp;&nbsp;&nbsp;") << sLogEntry << wxT("</p><hr>");
    }
    wxPoint pntView = m_phtmlLog->GetViewStart();

    m_phtmlLog->Freeze();
    m_phtmlLog->SetPage(sLog);
    m_phtmlLog->Thaw();


    if(!m_bScrollLock)
    {
        m_phtmlLog->Refresh();
        m_phtmlLog->Update();
        m_phtmlLog->End();
    }
    else
    {
        m_phtmlLog->Scroll(pntView);
    }
}
