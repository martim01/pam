#pragma once
#include "log.h"
#include <wx/event.h>

class wxLogOutput : public LogOutput
{
    public:
        wxLogOutput(){}
        void Flush(int nLogLevel, const std::stringstream&  logStream);
    private:
};




