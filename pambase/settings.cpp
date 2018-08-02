#include "settings.h"
#include <wx/log.h>
#include <wx/stdpaths.h>
#include "settingevent.h"
#include <wx/filename.h>
#include "version.h"

using   namespace std;


Settings::Settings()
{
    m_iniManager.ReadIniFile(wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));

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
        bDone = m_iniManager.WriteIniFile(wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));

        if(bDone)
        {
            wxString sHandler(wxString::Format(wxT("%s/%s"),sSection.c_str(),sKey.c_str()));
            for(multimap<wxString, wxEvtHandler*>::const_iterator itHandler = m_mmHandlers.lower_bound(sHandler); itHandler != m_mmHandlers.upper_bound(sHandler); ++itHandler)
            {
                SettingEvent event(sSection, sKey);
                wxPostEvent(itHandler->second, event);
            }
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
    return m_iniManager.WriteIniFile(wxString::Format(wxT("%s/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));
}

bool Settings::RemoveSection(const wxString& sSection)
{
    m_iniManager.RemoveSection(sSection);
    return m_iniManager.WriteIniFile(wxString::Format(wxT("%s/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));
}


wxString Settings::GetExecutableDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Executable"), wxString::Format(wxT("%s/bin"), GetDocumentDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Executable"), wxT("C:\\Progam Files\\pam2"));
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
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Core"), wxString::Format(wxT("%s/lib"), GetDocumentDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("Core"), wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxT('\\')));
    #endif // __WXGNU__
}

wxString Settings::GetMonitorPluginDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("MonitorPlugins"), wxString::Format(wxT("%s/monitor"), GetCoreLibDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("MonitorPlugins"), wxStandardPaths::Get().GetPluginsDir());
    #endif // __WXGNU__
}

wxString Settings::GetTestPluginDirectory() const
{
    #ifdef __WXGNU__
    return m_iniManager.GetIniString(wxT("Paths"), wxT("TestPlugins"), wxString::Format(wxT("%s/test"), GetCoreLibDirectory().c_str()));
    #else
    return m_iniManager.GetIniString(wxT("Paths"), wxT("TestPlugins"), wxStandardPaths::Get().GetPluginsDir());
    #endif // __WXGNU__
}


bool Settings::HideCursor()
{
    return (m_iniManager.GetIniInt(wxT("General"), wxT("Cursor"), 1) == 0);
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
}


wxString Settings::GetLibraryVersion() const
{
    return wxString::Format(wxT("%d.%d.%d.%d"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD, AutoVersion::REVISION);
}
