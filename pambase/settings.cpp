#include "settings.h"
#include <wx/log.h>
#include <wx/stdpaths.h>
#include "settingevent.h"
#include <wx/filename.h>
#include "version.h"
#include "pmcontrol.h"
#include "pmpanel.h"

#ifdef __WXMSW__
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


using   namespace std;


Settings::Settings()
{
    //wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str())
    m_timerSave.SetOwner(this, wxNewId());
    Connect(m_timerSave.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&Settings::OnTimerSave);
}

void Settings::ReadSettings(const wxString& sFullPath)
{
    m_iniManager.ReadIniFile(sFullPath);
    m_sFullPath = sFullPath;
    CreatePaths();
}


Settings& Settings::Get()
{
    static Settings settingManager;
    return settingManager;
}


wxString Settings::Read(const wxString& sSection, const wxString& sKey, const wxString& sDefault)
{
    return m_iniManager.GetIniString(sSection, sKey, sDefault);
}

int Settings::Read(const wxString& sSection, const wxString& sKey, int nDefault)
{
    return m_iniManager.GetIniInt(sSection, sKey, nDefault);
}

double Settings::Read(const wxString& sSection, const wxString& sKey, double dDefault)
{
    return m_iniManager.GetIniDouble(sSection, sKey, dDefault);
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, const wxString& sValue)
{
    bool bDone(true);
    if(m_iniManager.GetIniString(sSection, sKey, wxEmptyString) != sValue)
    {
        m_iniManager.SetSectionValue(sSection, sKey,sValue);

        if(m_timerSave.IsRunning() == false)
        {
            m_timerSave.Start(500, true);
        }

        wxString sHandler(wxString::Format(wxT("%s/%s"),sSection.c_str(),sKey.c_str()));
        for(multimap<wxString, wxEvtHandler*>::const_iterator itHandler = m_mmHandlers.lower_bound(sHandler); itHandler != m_mmHandlers.upper_bound(sHandler); ++itHandler)
        {
            SettingEvent* pEvent = new SettingEvent(sSection, sKey, sValue);
            wxQueueEvent(itHandler->second, pEvent);
        }

    }
    return bDone;
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, int nValue)
{
    return Write(sSection, sKey, wxString::Format(wxT("%d"), nValue));
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, double dValue)
{
    return Write(sSection, sKey, wxString::Format(wxT("%f"), dValue));
}


bool Settings::GetSectionDataBegin(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itBegin) const
{
    const iniSection* pSection = m_iniManager.GetSection(sSection);
    if(pSection)
    {
        itBegin = pSection->GetDataBegin();
        return true;
    }
    return false;
}

bool Settings::GetSectionDataEnd(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itEnd) const
{
    const iniSection* pSection = m_iniManager.GetSection(sSection);
    if(pSection)
    {
        itEnd = pSection->GetDataEnd();
        return true;
    }
    return false;
}



void Settings::AddHandler(const wxString& sSection, const wxString& sKey, wxEvtHandler* pHandler)
{
    if(pHandler)
    {
        m_mmHandlers.insert(make_pair(wxString::Format(wxT("%s/%s"), sSection.c_str(), sKey.c_str()), pHandler));
    }
}

void Settings::RemoveHandler(wxEvtHandler* pHandler)
{
    for(multimap<wxString, wxEvtHandler*>::iterator itHandler = m_mmHandlers.begin(); itHandler != m_mmHandlers.end();)
    {
        if(itHandler->second == pHandler)
        {
            multimap<wxString, wxEvtHandler*>::iterator  itDelete(itHandler);
            ++itHandler;
            m_mmHandlers.erase(itDelete);
        }
        else
        {
            ++itHandler;
        }
    }
}


bool Settings::RemoveKey(const wxString& sSection, const wxString& sKey)
{
    m_iniManager.RemoveSectionValue(sSection, sKey);
    return m_iniManager.WriteIniFile(m_sFullPath);
}

bool Settings::RemoveSection(const wxString& sSection)
{
    m_iniManager.RemoveSection(sSection);
    return m_iniManager.WriteIniFile(m_sFullPath);
}


void Settings::ResetFile()
{
    m_iniManager.RemoveAllSections();
    m_iniManager.SetSectionValue("Discovery", "RTSP","1");
    m_iniManager.SetSectionValue("Discovery", "SAP","0");
    m_iniManager.SetSectionValue("General", "ShowOptions","0");
    m_iniManager.SetSectionValue("Generator", "Shape","0");
    m_iniManager.SetSectionValue("Input", "Device","0");
    m_iniManager.SetSectionValue("Input", "Reset","0");
    m_iniManager.SetSectionValue("Input", "Type","Disabled");
    m_iniManager.SetSectionValue("Main", "Monitor","Settings");
    m_iniManager.SetSectionValue("Noise", "Colour","0");
    m_iniManager.SetSectionValue("Output", "Destination", "Disabled");
    m_iniManager.SetSectionValue("Output", "Device","3");
    m_iniManager.SetSectionValue("Output", "Left", "0");
    m_iniManager.SetSectionValue("Output", "Right", "0");
    m_iniManager.SetSectionValue("Output", "Source", "Input");
    m_iniManager.SetSectionValue("Settings", "_Options", "0");
    m_iniManager.SetSectionValue("Update", "Type", "USB");

    m_iniManager.WriteIniFile(m_sFullPath);

}

wxString Settings::GetExecutableDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Executable"), wxString::Format(wxT("%s/bin"), GetDocumentDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Executable"), wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxT('\\')));
    #endif // __WXGNU__
}

wxString Settings::GetConfigDirectory() const
{
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Config"), wxString::Format(wxT("%s/pam"), wxStandardPaths::Get().GetUserConfigDir().c_str()));

}

wxString Settings::GetDocumentDirectory() const
{

    return m_iniManager.GetIniString(wxT("Paths"), wxT("Docs"), wxString::Format(wxT("%s/pam"), wxStandardPaths::Get().GetDocumentsDir().c_str()));
}

wxString Settings::GetLogDirectory() const
{
    return wxString::Format(wxT("%s/logs"), GetDocumentDirectory().c_str());
}

wxString Settings::GetWavDirectory() const
{
    return wxString::Format(wxT("%s/wav"), GetDocumentDirectory().c_str());
}

wxString Settings::GetTempDirectory() const
{
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Temp"), wxStandardPaths::Get().GetTempDir());
}

wxString Settings::GetCoreLibDirectory() const
{
     #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Core"), wxT("/usr/local/lib/pam2"));
    #else

    return m_iniManager.GetIniString(wxT("Paths"), wxT("Core"),  wxString::Format(wxT("%s/lib"), GetExecutableDirectory().c_str()));
    #endif // __WXGNU__
}

wxString Settings::GetMonitorPluginDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("MonitorPlugins"), wxString::Format(wxT("%s/monitor"), GetCoreLibDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("MonitorPlugins"), wxString::Format(wxT("%s/monitor"), GetCoreLibDirectory().c_str()));
    #endif // __WXGNU__
}

wxString Settings::GetTestPluginDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("TestPlugins"), wxString::Format(wxT("%s/test"), GetCoreLibDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("TestPlugins"), wxString::Format(wxT("%s/test"), GetCoreLibDirectory().c_str()));
    #endif // __WXGNU__
}

wxString Settings::GetGeneratorPluginDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("GeneratorPlugins"), wxString::Format(wxT("%s/generator"), GetCoreLibDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("GeneratorPlugins"), wxString::Format(wxT("%s/generator"), GetCoreLibDirectory().c_str()));
    #endif // __WXGNU__
}


void Settings::CreatePaths()
{
    if(!wxFileName::DirExists(GetConfigDirectory()))
    {
        wxFileName::Mkdir(GetConfigDirectory(), 0777, wxPATH_MKDIR_FULL);
    }

    if(!wxFileName::DirExists(GetDocumentDirectory()))
    {
        wxFileName::Mkdir(GetDocumentDirectory(), 0777, wxPATH_MKDIR_FULL);
    }

    if(!wxFileName::DirExists(GetLogDirectory()))
    {
        wxFileName::Mkdir(GetLogDirectory(), 0777, wxPATH_MKDIR_FULL);
    }

    if(!wxFileName::DirExists(GetWavDirectory()))
    {
        wxFileName::Mkdir(GetWavDirectory(), 0777, wxPATH_MKDIR_FULL);
    }
}


wxString Settings::GetLibraryVersion() const
{
    return wxString::Format(wxT("%d.%d.%d.%d"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD, AutoVersion::REVISION);
}


void Settings::OnTimerSave(wxTimerEvent& event)
{
    m_iniManager.WriteIniFile(m_sFullPath);
}


multimap<wxString, wxString> Settings::GetInterfaces() const
{
    multimap<wxString, wxString> mmInterfaces;

    #ifdef __WXMSW__

    //DWORD dwSize = 0;
    unsigned int i = 0;
    // Set the flags to pass to GetAdaptersAddresses
    // default to unspecified address family (both)
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    DWORD dwRetVal = 0;
    ULONG outBufLen = 15000;
    ULONG Iterations = 0;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    do
    {
        pAddresses = (IP_ADAPTER_ADDRESSES *) HeapAlloc(GetProcessHeap(), 0, outBufLen);
        if (pAddresses == NULL)
        {
            return multimap<wxString, wxString>();
        }

        dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            HeapFree(GetProcessHeap(),0, pAddresses);
            pAddresses = NULL;
        }
        else
        {
            break;
        }
        Iterations++;
    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

    if (dwRetVal == NO_ERROR)
    {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses)
        {
            if (pCurrAddresses->PhysicalAddressLength != 0)
            {
                pUnicast = pCurrAddresses->FirstUnicastAddress;
                if (pUnicast != NULL)
                {
                    for (i = 0; pUnicast != NULL; i++)
                    {
                        if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
                        {
                            //char buff[100];
                            sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
                            mmInterfaces.insert(make_pair(wxString::FromAscii(pCurrAddresses->AdapterName), wxString::FromAscii(inet_ntoa(sa_in->sin_addr))));
                        }
                        pUnicast = pUnicast->Next;
                    }
                }
            }

            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    if (pAddresses)
    {
        HeapFree(GetProcessHeap(),0, pAddresses);
    }
    #else
    ifaddrs * ifAddrStruct=NULL;
    ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET)
        { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            mmInterfaces.insert(make_pair(wxString::FromAscii(ifa->ifa_name), wxString::FromAscii(addressBuffer)));
        }
    }
    if (ifAddrStruct!=NULL)
    {
        freeifaddrs(ifAddrStruct);
    }
    #endif // __WXMSW__
    return mmInterfaces;
}
