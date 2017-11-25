#include "pnlLog.h"
#include <wx/tokenzr.h>
#include "settings.h"
#include "pnlLogControl.h"

//(*InternalHeaders(pnlLog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlLog)
const long pnlLog::ID_LISTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlLog,wxPanel)
	//(*EventTable(pnlLog)
	//*)
END_EVENT_TABLE()

pnlLog::pnlLog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pControl(0)
{
	//(*Initialize(pnlLog)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_plclLog = new wxListCtrl(this, ID_LISTCTRL1, wxPoint(0,0), wxSize(600,480), wxLC_REPORT|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_LISTCTRL1"));

	Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_COL_END_DRAG,(wxObjectEventFunction)&pnlLog::OnlclLogColumnEndDrag);
	//*)
	m_bScrollLock = false;

	m_nPage = 0;
    m_plclLog->SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));

	m_plclLog->InsertColumn(0, wxT("Time"), wxLIST_FORMAT_CENTER, Settings::Get().Read(wxT("Log"), wxT("Column_Time"), 80));
	m_plclLog->InsertColumn(1, wxT("Entry"), wxLIST_FORMAT_LEFT, Settings::Get().Read(wxT("Log"), wxT("Column_Entry"), 500));

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
void pnlLog::Log(const wxString& sLogEntry)
{
    size_t nIndex;
    bool bMore;
    wxArrayString as(wxStringTokenize(sLogEntry, wxT("\n")));
    for(int i = 0; i < as.GetCount(); i++)
    {
        if(as[i] != wxEmptyString)
        {
            int nLine(0);
            bMore = true;
            do
            {
                if(m_vLogPages.empty() || m_vLogPages.back().nLines == 26)
                {
                    m_vLogPages.push_back(logPage());
                }

                if(nLine == 0)
                {
                    m_vLogPages.back().sTime[m_vLogPages.back().nLines] = wxDateTime::UNow().Format(wxT("%H:%M:%S:%l"));
                }
                else
                {
                    m_vLogPages.back().sTime[m_vLogPages.back().nLines] = wxT("--:--:--:----");
                }
               if(as[i].length() < 80)
               {
                    m_vLogPages.back().sEntry[m_vLogPages.back().nLines] = as[i];
                    bMore = false;
                }
                else
                {
                    m_vLogPages.back().sEntry[m_vLogPages.back().nLines] = as[i].Left(80);
                    as[i] = as[i].Mid(80);
                }
                m_vLogPages.back().nLines++;
                nLine++;
            }while(bMore);
        }
    }

    if(!m_bScrollLock)
    {
        End();
    }
    else
    {
        ShowLog();
    }
}


void pnlLog::Home()
{
    m_nPage = 0;
    ShowLog();
}

void pnlLog::End()
{
    if(m_vLogPages.empty() == false)
    {
        m_nPage = m_vLogPages.size()-1;
        ShowLog();
    }
}

void pnlLog::PageUp()
{
    if(m_vLogPages.empty() == false && m_nPage > 0)
    {
        m_nPage--;
        ShowLog();
    }
}

void pnlLog::PageDown()
{
    if(m_vLogPages.empty() == false && m_nPage < m_vLogPages.size()-1)
    {
        m_nPage++;
        ShowLog();
    }
}

void pnlLog::ScrollLock(bool bLock)
{
    m_bScrollLock = bLock;
}

void pnlLog::Clear()
{
    m_vLogPages.clear();
    m_nPage = 0;
    m_plclLog->DeleteAllItems();

    if(m_pControl)
    {
        m_pControl->UpdateControl(0,0);
    }
}

void pnlLog::OnlclLogColumnEndDrag(wxListEvent& event)
{
}


void pnlLog::ShowLog()
{
    if(m_vLogPages.empty() == false)
    {
        m_plclLog->Freeze();
        m_plclLog->DeleteAllItems();

        for(int i = 0; i <= m_vLogPages[m_nPage].nLines; i++)
        {
            size_t nIndex = m_plclLog->InsertItem(m_plclLog->GetItemCount(), m_vLogPages[m_nPage].sTime[i]);
            m_plclLog->SetItem(nIndex, 1, m_vLogPages[m_nPage].sEntry[i]);
        }

        m_plclLog->Thaw();

        if(m_pControl)
        {
            m_pControl->UpdateControl(m_nPage, m_vLogPages.size()-1);
        }
    }
    else
    {
        if(m_pControl)
        {
            m_pControl->UpdateControl(0,0);
        }
    }


}


size_t pnlLog::GetCurrentPage() const
{
    return m_nPage;
}

size_t pnlLog::GetPageCount() const
{
    return m_vLogPages.size()-1;
}
