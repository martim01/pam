#include "PamUsageEnvironment.h"
#include <stdio.h>
#include <wx/log.h>
#include "wmlogevent.h"

////////// PamUsageEnvironment //////////

#if defined(__WIN32__) || defined(_WIN32)
extern "C" int initializeWinsockIfNecessary();
#endif




PamUsageEnvironment::PamUsageEnvironment(TaskScheduler& taskScheduler, wxEvtHandler* pHandler)
    : BasicUsageEnvironment0(taskScheduler),
    m_pHandler(pHandler)
{
#if defined(__WIN32__) || defined(_WIN32)
    if (!initializeWinsockIfNecessary())
    {
        setResultErrMsg("Failed to initialize 'winsock': ");
        reportBackgroundError();
        internalError();
    }
#endif
}

PamUsageEnvironment::~PamUsageEnvironment()
{
}

PamUsageEnvironment*
PamUsageEnvironment::createNew(TaskScheduler& taskScheduler, wxEvtHandler* pHandler)
{
    return new PamUsageEnvironment(taskScheduler, pHandler);
}

int PamUsageEnvironment::getErrno() const
{
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
    return WSAGetLastError();
#else
    return errno;
#endif
}

UsageEnvironment& PamUsageEnvironment::operator<<(char const* str)
{
    if (str == NULL) str = "(NULL)"; // sanity check
    m_sLog << wxString::FromAscii(str);


    if(m_sLog.Find(wxT("\n")) != wxNOT_FOUND)
    {
        wmLog::Get()->Log(wxT("AoIP"), m_sLog);
        m_sLog = wxEmptyString;
    }
    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(int i)
{
    m_sLog << i;

    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(unsigned u)
{
    m_sLog << u;
    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(double d)
{
    m_sLog << d;
    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(void* p)
{
    m_sLog << wxString::Format(wxT("%p"), p);
    return *this;
}
