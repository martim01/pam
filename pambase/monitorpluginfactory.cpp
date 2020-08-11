#include "monitorpluginfactory.h"
#include "monitorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/xml/xml.h>
#include "log.h"
#include "settings.h"
#include <wx/stdpaths.h>

using namespace std;

MonitorPluginFactory* MonitorPluginFactory::Get()
{
    static MonitorPluginFactory wmf;
    return &wmf;
}

void MonitorPluginFactory::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void MonitorPluginFactory::Register(MonitorPluginBuilder* pBuilder)
{
    pBuilder->SetHandler(m_pHandler);
    if(m_pswpMonitor && m_pswpOptions)
    {
        pBuilder->CreatePanels(m_pswpMonitor, m_pswpOptions);
        pBuilder->LoadSettings();
    }

    m_mBuilders.insert(make_pair(pBuilder->GetName(), pBuilder));
}

MonitorPluginFactory::~MonitorPluginFactory()
{
    UnloadAll();
}

void MonitorPluginFactory::SetSwitcherPanels(wmSwitcherPanel* pswpMonitor, wmSwitcherPanel* pswpOptions)
{
    m_pswpMonitor = pswpMonitor;
    m_pswpOptions = pswpOptions;
}

wxString MonitorPluginFactory::GetPluginDirectory()
{
     return Settings::Get().GetMonitorPluginDirectory();
}

bool MonitorPluginFactory::LoadLibrary(const wxString& sLibrary)
{
    //wxLogNull ln;
    pml::Log::Get(pml::Log::LOG_INFO) << "Monitor Plugin\t" << "Load '" << sLibrary << "'" << std::endl;
    map<wxString, wxDynamicLibrary*>::iterator itLib = m_mLibraries.find(sLibrary);
    if(itLib == m_mLibraries.end())
    {
        wxString sCan = wxDynamicLibrary::CanonicalizeName(sLibrary);


        wxString sDir(GetPluginDirectory());


        #ifdef PAMBASE_DEBUG
            wxString sLib(wxString::Format(wxT("%s/debug/%s"),sDir.c_str(), sCan.c_str()));
        #else
            wxString sLib(wxString::Format(wxT("%s/%s"), sDir.c_str(), sCan.c_str()));
        #endif

        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);


        if(pLib && pLib->IsLoaded())
        {
            if(pLib->HasSymbol(wxT("CreateMonitorBuilder")))
            {
                typedef void (*RegPtr)();
                RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("CreateMonitorBuilder"));
                if(ptr)
                {
                    (*ptr)();
                    m_mLibraries.insert(make_pair(sLibrary, pLib));
                    pml::Log::Get(pml::Log::LOG_INFO) << "Monitor Plugin\t" << "Loaded '" << sLib << "'" << std::endl;
                    return true;
                }
                else
                {
                    pml::Log::Get(pml::Log::LOG_ERROR) << "Monitor Plugin\t" << "'"  << sLib << "' cannot execute function CreateMonitorBuilder" << std::endl;
                }
            }
            else
            {
                pml::Log::Get(pml::Log::LOG_ERROR) << "Monitor Plugin\t" << "'" << sLib << "' has no function CreateMonitorBuilder" << std::endl;
            }
        }
        else
        {
            pml::Log::Get(pml::Log::LOG_ERROR) << "Monitor Plugin\t" << "Could not load '" << sLib << "'" << std::endl;
        }
        delete pLib;
    }
    else
    {
        typedef void (*RegPtr)();

        RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("CreateMonitorBuilder"));
        if(ptr)
        {
            (*ptr)();
            return true;
        }
    }
    return false;
}


void MonitorPluginFactory::UnloadAll(bool bDeletePanel)
{
    for(map<wxString, MonitorPluginBuilder*>::iterator itBuilder = m_mBuilders.begin(); itBuilder != m_mBuilders.end(); ++itBuilder)
    {
        if(bDeletePanel)
        {
            itBuilder->second->DeletePanels();
        }
        delete itBuilder->second;
    }
    m_mBuilders.clear();

    for(map<wxString, wxDynamicLibrary*>::iterator itBuilder = m_mLibraries.begin(); itBuilder != m_mLibraries.end(); ++itBuilder)
    {
        delete itBuilder->second;
    }
    m_mLibraries.clear();

}


map<wxString, MonitorPluginBuilder*>::iterator MonitorPluginFactory::MonitorPluginFactory::GetPluginBegin()
{
    return m_mBuilders.begin();
}
map<wxString, MonitorPluginBuilder*>::iterator MonitorPluginFactory::MonitorPluginFactory::GetPluginEnd()
{
    return m_mBuilders.end();
}

map<wxString, MonitorPluginBuilder*>::iterator MonitorPluginFactory::FindPlugin(const wxString& sName)
{
    return m_mBuilders.find(sName);
}


map<wxString, wxDynamicLibrary*>::const_iterator MonitorPluginFactory::GetLibraryBegin() const
{
    return m_mLibraries.begin();
}

map<wxString, wxDynamicLibrary*>::const_iterator MonitorPluginFactory::GetLibraryEnd() const
{
    return m_mLibraries.end();
}

map<wxString, wxDynamicLibrary*>::const_iterator MonitorPluginFactory::FindLibray(const wxString& sLibrary) const
{
    return m_mLibraries.find(sLibrary);
}


plugin MonitorPluginFactory::GetPluginDetails(const wxString& sDir, const wxString& sLibrary)
{
    wxLogNull ln;

    plugin aPlugin;
    aPlugin.sLibrary = sLibrary;

    map<wxString, wxDynamicLibrary*>::const_iterator itLib = m_mLibraries.begin();
    for(; itLib != m_mLibraries.end(); ++itLib)
    {
        if(itLib->first.CmpNoCase(sLibrary) == 0)
        {
            break;
        }
    }
    if(itLib != m_mLibraries.end())
    {
        if(itLib->second->HasSymbol(wxT("GetPluginName")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetPluginName"));
            aPlugin.sName = (*ptr)();
        }
        if(itLib->second->HasSymbol(wxT("GetPluginVersion")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetPluginVersion"));
            aPlugin.sVersion = (*ptr)();
        }
        if(itLib->second->HasSymbol(wxT("GetDetails")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetDetails"));
            aPlugin.sDetails = (*ptr)();
        }
    }
    else
    {
        #ifdef PAMBASE_DEBUG
        wxString sLib = sDir + wxT("/debug/") + wxDynamicLibrary::CanonicalizeName(sLibrary);
        #else
        wxString sLib = sDir + wxT("/") + wxDynamicLibrary::CanonicalizeName(sLibrary);
        #endif // PAMBASE_DEBUG

        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);
        if(pLib)
        {
            if(pLib->IsLoaded())
            {
                if(pLib->HasSymbol(wxT("GetPluginName")) && pLib->HasSymbol(wxT("CreateMonitorBuilder")))
                {
                    typedef wxString (*RegPtr)();
                    RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("GetPluginName"));
                    if(ptr)
                    {
                        aPlugin.sName = (*ptr)();
                    }
                    if(pLib->HasSymbol(wxT("GetPluginVersion")))
                    {
                        typedef wxString (*RegPtr)();
                        RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("GetPluginVersion"));
                        aPlugin.sVersion = (*ptr)();
                    }
                    if(pLib->HasSymbol(wxT("GetDetails")))
                    {
                        typedef wxString (*RegPtr)();
                        RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("GetDetails"));
                        aPlugin.sDetails = (*ptr)();
                    }
                }
                else
                {
                    pml::Log::Get(pml::Log::LOG_ERROR) << "Monitor Plugin\t" << "Could not find correct symbols in lib " << sLib << std::endl;
                }
            }
            else
            {
                pml::Log::Get(pml::Log::LOG_ERROR) << "Monitor Plugin\t" << "Could not load lib " << sLib << std::endl;
            }
            delete pLib;
        }
        else
        {
            pml::Log::Get(pml::Log::LOG_ERROR) << "Monitor Plugin\t" << "Could not create lib " << sLib << std::endl;
        }
    }
    return aPlugin;
}
