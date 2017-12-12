#include "pnlLog.h"
#include <wx/tokenzr.h>
#include "settings.h"
#include "pnlLogControl.h"
#include "wmlogevent.h"

//(*InternalHeaders(pnlLog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlLog)
const long pnlLog::ID_M_PLST1 = wxNewId();
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
	m_plstLog = new wmList(this, ID_M_PLST1, wxPoint(5,5), wxSize(590,470), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,20), 1, wxSize(0,0));
	wxFont m_plstLogFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plstLog->SetFont(m_plstLogFont);
	m_plstLog->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstLog->SetPressedButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstLog->SetSelectedButtonColour(wxColour(wxT("#8080FF")));
	m_plstLog->SetTextButtonColour(wxColour(wxT("#000000")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlLog::OnlstLogSelected);
	//*)
	m_bScrollLock = false;

	m_nPage = 0;
    m_plstLog->SetTextAlign(wxALIGN_LEFT);
    m_plstLog->SetGradient(0);
    m_plstLog->SetBorderStyle(uiRect::BORDER_NONE);
//    m_plblLog->SetTextAlign(wxALIGN_LEFT | wxALIGN_TOP);


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
                if(m_vLogPages.empty() || m_vLogPages.back().nLines == 22)
                {
                    m_vLogPages.push_back(logPage());
                }

                m_vLogPages.back().nId[m_vLogPages.back().nLines] = m_vLogPages.back().nEntries;
                if(nLine == 0)
                {
                    m_vLogPages.back().sTime[m_vLogPages.back().nLines] = wxDateTime::UNow().Format(wxT("%H:%M:%S:%l  "));

                }
                else
                {
                    m_vLogPages.back().sTime[m_vLogPages.back().nLines] = wxDateTime::UNow().Format(wxT("              "));
                }
               if(as[i].length() < 70)
               {
                    m_vLogPages.back().sEntry[m_vLogPages.back().nLines] = as[i];
                    bMore = false;
                }
                else
                {
                    int nLength(60);
                    do
                    {
                        if(as[i].GetChar(nLength-1) == wxT(' '))
                        {
                            break;
                        }
                        else
                        {
                            ++nLength;
                        }
                    }while(nLength < 70);

                    m_vLogPages.back().sEntry[m_vLogPages.back().nLines] = as[i].Left(nLength);

                    as[i] = as[i].Mid(nLength);
                }
                m_vLogPages.back().nLines++;
                nLine++;

            }while(bMore);
            m_vLogPages.back().nEntries++;
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
    m_plstLog->Clear();

    if(m_pControl)
    {
        m_pControl->UpdateControl(0,0);
    }
}



void pnlLog::ShowLog()
{
    if(m_vLogPages.empty() == false)
    {
        m_plstLog->Freeze();
        m_plstLog->Clear();


        for(int i = 0; i <= m_vLogPages[m_nPage].nLines; i++)
        {
            m_plstLog->AddButton(wxString::Format(wxT("%s%s"), m_vLogPages[m_nPage].sTime[i].c_str(), m_vLogPages[m_nPage].sEntry[i].c_str()), wxNullBitmap, (void*)m_vLogPages[m_nPage].nId[i]);
        }


        m_plstLog->Thaw();

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


void pnlLog::OnLog(wmLogEvent& event)
{

    Log(event.GetLogMessage());
}

void pnlLog::OnlstLogSelected(wxCommandEvent& event)
{
    m_plstLog->Freeze();
    m_plstLog->SelectAll(false, false);
    int nId = (int)event.GetClientData();
    for(size_t i = 0; i < m_plstLog->GetItemCount(); i++)
    {
        int nButtonId = (int)m_plstLog->GetButtonData(i);
        if(nButtonId == nId)
        {
            m_plstLog->SelectButton(i, false);
        }
    }
    m_plstLog->Thaw();
}
