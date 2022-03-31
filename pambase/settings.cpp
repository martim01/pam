#include "settings.h"
#include <wx/log.h>
#include <wx/stdpaths.h>
#include "settingevent.h"
#include <wx/filename.h>
#include "pambase_version.h"
#include "pmcontrol.h"
#include "pmpanel.h"
#include "pam2_paths.h"

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
    Bind(wxEVT_TIMER, &Settings::OnTimerSave, this, m_timerSave.GetId());

    AddHandler(this);
    Bind(wxEVT_SETTING_CHANGED, &Settings::OnSettingChanged, this);
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

bool Settings::WriteInternal(const wxString& sSection, const wxString& sKey, const wxString& sValue, SettingEvent::enumType eType)
{
    bool bDone(true);
    if(m_iniManager.GetIniString(sSection, sKey, wxEmptyString) != sValue)
    {
        m_iniManager.SetSectionValue(sSection, sKey,sValue);



        std::set<wxEvtHandler*> setHandlers;
        GetHandlers("/", setHandlers);                  //registered for all changes
        GetHandlers(sSection+"/", setHandlers);         //registered for all changes to this section
        GetHandlers(sSection+"/"+sKey, setHandlers);    //registered for change to this section key pair

        for(auto pHandler : setHandlers)
        {
            SettingEvent* pEvent = new SettingEvent(sSection, sKey, sValue, eType);
            wxQueueEvent(pHandler, pEvent);
        }
    }
    return bDone;
}

void Settings::GetHandlers(const wxString& sHandlers, std::set<wxEvtHandler*>& setHandlers)
{
    for(multimap<wxString, wxEvtHandler*>::const_iterator itHandler = m_mmHandlers.lower_bound(sHandlers); itHandler != m_mmHandlers.upper_bound(sHandlers); ++itHandler)
    {
        setHandlers.insert(itHandler->second);
    }
}

void Settings::OnSettingChanged(SettingEvent& event)
{
    //start the timer to save the ini file within 500ms
    if(m_timerSave.IsRunning() == false)
    {
        m_timerSave.Start(500, true);
    }
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, const wxString& sValue)
{
    WriteInternal(sSection, sKey, sValue, SettingEvent::enumType::SETTING_STRING);

    return true;
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, int nValue)
{
    WriteInternal(sSection, sKey, wxString::Format(wxT("%d"), nValue), SettingEvent::enumType::SETTING_LONG);

    return true;
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, unsigned int nValue)
{
    WriteInternal(sSection, sKey, wxString::Format(wxT("%u"), nValue), SettingEvent::enumType::SETTING_LONG);
    return true;
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, double dValue)
{
    WriteInternal(sSection, sKey, wxString::Format(wxT("%f"), dValue), SettingEvent::enumType::SETTING_DOUBLE);

    return true;
}


bool Settings::GetSectionDataBegin(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itBegin)
{
    auto pSection = m_iniManager.GetSection(sSection);
    if(pSection)
    {
        itBegin = pSection->GetDataBegin();
        return true;
    }
    return false;
}

bool Settings::GetSectionDataEnd(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itEnd)
{
    auto pSection = m_iniManager.GetSection(sSection);
    if(pSection)
    {
        itEnd = pSection->GetDataEnd();
        return true;
    }
    return false;
}



void Settings::AddHandler(wxEvtHandler* pHandler, const wxString& sSection, const wxString& sKey)
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
    m_iniManager.DeleteSections();
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
    wxString sPath = m_iniManager.GetIniString("Paths", "Executable", "");
    if(sPath == "")
    {
        wxArrayString asOutput;
        wxArrayString asError;
        long nResult = wxExecute("realpath `which pam2`", asOutput, asError);

        if(nResult == 0 && asOutput.Count() != 0)
        {
            sPath = asOutput[0].BeforeLast('/');
            m_iniManager.SetSectionValue("Paths", "Executable", sPath);
        }
    }
    return sPath;
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Executable"), wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxT('\\')));
    #endif // __WXGNU__
}

wxString Settings::GetConfigDirectory() const
{
    return pml::pam2::CONFIG_PATH;

}

wxString Settings::GetDocumentDirectory() const
{

    return pml::pam2::CONFIG_PATH;
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
    #ifdef _pambase_
    return pml::pambase::VERSION_STRING;
    #else
    return "";
    #endif
}


void Settings::OnTimerSave(wxTimerEvent& event)
{
    m_iniManager.WriteIniFile(m_sFullPath);
}


const multimap<wxString, wxString>& Settings::GetInterfaces()
{
    if(m_mmInterfaces.empty())
    {
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
                                m_mmInterfaces.insert(make_pair(wxString(pCurrAddresses->FriendlyName), wxString::FromUTF8(inet_ntoa(sa_in->sin_addr))));
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
                m_mmInterfaces.insert(std::make_pair(wxString::FromUTF8(ifa->ifa_name), wxString::FromUTF8(addressBuffer)));
            }
        }
        if (ifAddrStruct!=NULL)
        {
            freeifaddrs(ifAddrStruct);
        }
        #endif // __WXMSW__
    }
    return m_mmInterfaces;
}


const mapSection& Settings::GetSections()
{
    return m_iniManager.GetSections();
}

std::shared_ptr<iniSection> Settings::GetSection(const wxString& sSection)
{

    return m_iniManager.GetSection(sSection);
}

std::string GetChannelMapping()
{
    return Settings::Get().Read("Server", "ChannelMapping", "St").ToStdString();
}
