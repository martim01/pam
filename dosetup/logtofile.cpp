#include "logtofile.h"
#include <syslog.h>
#include <chrono>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <sstream>

std::string CreatePath(std::string sPath)
{
    if(sPath[sPath.length()-1] != '/' && sPath[sPath.length()-1] != '\\')
    {
        sPath+= '/';
    }
    return sPath;
}

std::vector<std::string> SplitString(std::string str, char cSplit)
{
    std::vector<std::string> vSplit;
    std::istringstream f(str);
    std::string s;

    while (getline(f, s, cSplit))
    {
        if(s.empty() == false)
        {
            vSplit.push_back(s);
        }
    }
    return vSplit;
}

bool do_mkdir(const std::string& sPath, mode_t mode)
{
    struct stat st;
    bool bStatus(true);


    if (stat(sPath.c_str(), &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(sPath.c_str(), mode) != 0 && errno != EEXIST)
        {
	    std::stringstream ss;
            ss <<"do_mkdir("<<sPath<<") " << strerror(errno);
	    syslog(LOG_WARNING, ss.str().c_str());
            bStatus = false;
        }
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
	std::stringstream ss;
        ss <<"do_mkdir("<<sPath<<") " << strerror(errno);
	syslog(LOG_WARNING, ss.str().c_str());
        bStatus = false;
    }

    return bStatus;
}

bool mkpath(const std::string& sPath, mode_t mode)
{

    std::vector<std::string> vSplit(SplitString(sPath, '/'));
    std::stringstream ssPath;

    for(size_t i = 0; i < vSplit.size(); i++)
    {
        if(sPath[0] == '/' && i == 0)
        {
            ssPath << '/';
        }
        ssPath << vSplit[i];
        if(do_mkdir(ssPath.str(), mode) == false)
        {
            return false;
        }

        ssPath << "/";
    }

    return true;
}


LogToFile::LogToFile(const std::string& sRootPath,int nTimestamp, pml::LogOutput::enumTS eResolution) : LogOutput(nTimestamp, eResolution),
m_sRootPath(CreatePath(sRootPath))
{
}

void LogToFile::OpenFile(const std::string& sFilePath, const std::string& sFileName)
{
    if(m_ofLog.is_open())
    {
        m_ofLog.close();
    }

    m_sFilePath = sFilePath;
    m_sFileName = sFileName;

    if(!mkpath(m_sFilePath, 0755))
    {
        std::stringstream ss;
        ss << "Unable to create log file "  << sFilePath;
        syslog(LOG_WARNING, ss.str().c_str());
    }
    std::string sFile(m_sFilePath+m_sFileName);
    m_ofLog.open(sFile, std::fstream::app);
    //chande the permissions
    chmod(sFile.c_str(), 0664);
}

void LogToFile::Flush(pml::enumLevel eLogLevel, const std::stringstream&  logStream, const std::string& sPrefix)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ssFilePath;
    std::stringstream ssFileName;
    ssFilePath << m_sRootPath;
    ssFileName << std::put_time(localtime(&in_time_t), "%Y-%m-%dT%H") << ".log";

    if(m_ofLog.is_open() == false || ssFilePath.str() != m_sFilePath || ssFileName.str() != m_sFileName)
    {
        OpenFile(ssFilePath.str(), ssFileName.str());
    }

    if(m_ofLog.is_open())
    {
        m_ofLog << Timestamp().str();
        m_ofLog << pml::LogStream::STR_LEVEL[eLogLevel] << "\t[" << sPrefix << "]\t" << logStream.str();
        m_ofLog.flush();
    }
    else
    {
        syslog(LOG_INFO, logStream.str().c_str());
    }
}




