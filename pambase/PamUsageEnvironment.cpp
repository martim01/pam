#include "PamUsageEnvironment.h"
#include <stdio.h>
#include <wx/log.h>
#include "log.h"
#include <iostream>

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
    if (str != NULL)
    {
        std::string sMsg(str);
        m_ls << str;

        if(sMsg.back() == '\n')
        {
            m_ls;
        }
    }
    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(int i)
{
    m_ls << i;

    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(unsigned u)
{
    m_ls << u;
    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(double d)
{
    m_ls << d;
    return *this;
}

UsageEnvironment& PamUsageEnvironment::operator<<(void* p)
{
    m_ls << reinterpret_cast<unsigned long>(p);
    return *this;
}
