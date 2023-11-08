#include "pnlLog.h"
#include <wx/tokenzr.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlLogControl.h"
#include "wxlogoutput.h"
#include <wx/log.h>
#include "logelement.h"
#include <wx/dcclient.h>
#include "wxlogoutput.h"
#include "log.h"
#include "logtofile.h"
//(*InternalHeaders(pnlLog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;


BEGIN_EVENT_TABLE(pnlLog,wxPanel)
	//(*EventTable(pnlLog)
	//*)
END_EVENT_TABLE()

pnlLog::pnlLog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pControl(0), m_nLogLevel(pml::LOG_TRACE)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLogList = new wmListAdv(this, wxNewId(), wxPoint(0,0), wxSize(600,480), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
	m_pLogList->SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));
	m_pLogList->SetBackgroundColour(wxColour(100,100,180));
	//*)

	m_nLogLevel = Settings::Get().Read("Log", "Level", pml::LOG_INFO);
	Settings::Get().AddHandler(this,"Log","Level");
	Settings::Get().AddHandler(this,"Log","ScrollLock");
    Settings::Get().AddHandler(this,"Log","LogToFile");
    
	Bind(wxEVT_SETTING_CHANGED, &pnlLog::OnSettingChanged, this);


    m_nLogOutput = pml::LogStream::AddOutput(std::make_unique<wxLogOutput>(this));
    pml::LogStream::SetOutputLevel(pml::LOG_INFO);

    if(Settings::Get().Read("Log", "LogToFile", false))
    {
        m_nLogToFile = pml::LogStream::AddOutput(std::make_unique<pml::LogToFile>("/var/log/pam"));
    }

	Connect(wxID_ANY,wxEVT_PMLOG,(wxObjectEventFunction)&pnlLog::OnLog);
}

pnlLog::~pnlLog()
{
	//(*Destroy(pnlLog)
	//*)
	pml::LogStream::RemoveOutput(m_nLogOutput);
	Settings::Get().RemoveHandler(this);
}

void pnlLog::SetLogControl(pnlLogControl* pControl)
{
    m_pControl = pControl;
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



void pnlLog::OnLog(wxCommandEvent& event)
{
    wxClientDC dc(this);
    dc.SetFont(m_pLogList->GetFont());

    m_pLogList->AddElement(std::make_shared<LogElement>(dc, GetClientSize().x, event.GetString(), event.GetInt()), true, [this](std::shared_ptr<advElement> pElement)->bool{
                                                       auto pLogElement = std::dynamic_pointer_cast<LogElement>(pElement);
                                                       return (pLogElement && pLogElement->GetMessageLevel() >= m_nLogLevel);
                                                       });
    m_pLogList->Refresh();

    if(!m_bScrollLock)
    {
        End();
    }

}

void pnlLog::Filter(int nFilter)
{
    m_nLogLevel = nFilter;
    m_pLogList->Filter([this](std::shared_ptr<advElement> pElement)->bool{
                                                       auto pLogElement = std::dynamic_pointer_cast<LogElement>(pElement);
                                                       return (pLogElement && pLogElement->GetMessageLevel() >= m_nLogLevel);
                                                       });

}

void pnlLog::OnSettingChanged(SettingEvent& event)
{
    if(event.GetSection()=="Log")
    {
        if(event.GetKey()=="Level")
        {
            Filter(event.GetValue(3L));
        }
        else if(event.GetKey() == "ScrollLock")
        {
            ScrollLock(event.GetValue(false));
        }
        else if(event.GetKey() == "LogToFile")
        {
            if(event.GetValue(false) && m_nLogToFile == -1)
            {
                m_nLogToFile = pml::LogStream::AddOutput(std::make_unique<pml::LogToFile>("/var/log/pam"));
            }
            else if(!event.GetValue(false) && m_nLogToFile != -1)
            {
                pml::LogStream::RemoveOutput(m_nLogToFile);
                m_nLogToFile = -1;
            }
        }
    }
}
