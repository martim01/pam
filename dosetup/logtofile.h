#pragma once
#include "log.h"
#include <string>
#include <fstream>

class LogToFile : public pml::LogOutput
{
    public:
        LogToFile(const std::string& sRootPath, int nTimestamp=TS_TIME, enumTS eResolution=TSR_MILLISECOND);
        virtual ~LogToFile(){}
        void Flush(pml::enumLevel eLogLevel, const std::stringstream&  logStream) override;

    private:

        void OpenFile(const std::string& sFilePath, const std::string& sFileName);
        std::string m_sRootPath;
        std::string m_sFilePath;
        std::string m_sFileName;

        std::ofstream m_ofLog;
	bool m_bLogToConsole;
};


