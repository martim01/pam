#include "wmtestlog.h"
#include <wx/log.h>
#include "settings.h"
#include <wx/file.h>
#include <wx/filename.h>
#include "wmlogevent.h"

using namespace std;


wmTestLog& wmTestLog::Get()
{
    static wmTestLog lg;
    return lg;
}

void wmTestLog::SetTarget(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void wmTestLog::Log(const wxString& sTest, const wxString& sMessage, bool bSend)
{
    if(m_pHandler)
    {
        wmLogEvent* plge = new wmLogEvent(sTest, sMessage, false);
        if(bSend == true)
        {
            m_pHandler->QueueEvent(plge);
        }
    }

    map<wxString, wxFile*>::iterator itFile = m_mFileLog.find(sTest);
    if(itFile != m_mFileLog.end() || m_dtLog != wxDateTime::Today())
    {
        itFile = OpenLogFile(sTest, true);
    }
	itFile->second->Write(wxString::Format(wxT("%s\t%s\r\n"), wxDateTime::Now().Format(wxT("%H:%M:%S")).c_str(), sMessage.c_str()));
}

std::map<wxString, wxFile*>::iterator wmTestLog::OpenLogFile(const wxString& sTest, bool bOpen)
{
    map<wxString, wxFile*>::iterator itFile = m_mFileLog.find(sTest);
    if(itFile != m_mFileLog.end())
    {
        delete itFile->second;
        m_mFileLog.erase(itFile);
    }
    itFile = m_mFileLog.end();

	if(bOpen)
	{
		itFile = m_mFileLog.insert(make_pair(sTest, new wxFile(wxString::Format(wxT("%s/%s.log"), Settings::Get().GetLogDirectory().c_str(), wxDateTime::Now().FormatISODate().c_str()), wxFile::write_append))).first;
		m_dtLog = wxDateTime::Today();
	}

	return itFile;
}


wmTestLog::~wmTestLog()
{
    for(map<wxString, wxFile*>::iterator itFile = m_mFileLog.begin(); itFile != m_mFileLog.end(); ++itFile)
    {
        delete itFile->second;
    }
}
