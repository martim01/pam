#pragma once
#include "wmlogevent.h"


void wxLogOutput::Flush(int nLogLevel, const std::stringstream&  logStream)
{
    wxString sLog(wxString::FromAscii(logStream.str().c_str()));
    sLog.Replace(wxT("\n"), wxT(""));

    wmLog::Get()->Log(wxT("NMOS"), sLog);
}
