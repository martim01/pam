#include "generatorpluginbuilder.h"
#include "generatorpluginfactory.h"
#include "wmswitcherpanel.h"
#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/xml/xml.h>
#include <wx/stdpaths.h>
#include "settings.h"
#include "log.h"

using namespace std;

GeneratorPluginFactory* GeneratorPluginFactory::Get()
{
    static GeneratorPluginFactory wmf;
    return &wmf;
}

void GeneratorPluginFactory::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void GeneratorPluginFactory::Register(GeneratorPluginBuilder* pBuilder)
{
    pBuilder->SetHandler(m_pHandler);
    pBuilder->CreatePanels(m_pswpGenerators);

    m_mBuilders.insert(make_pair(pBuilder->GetName(), pBuilder));
}

GeneratorPluginFactory::~GeneratorPluginFactory()
{
    UnloadAll();
}

void GeneratorPluginFactory::SetSwitcherPanels(wmSwitcherPanel* pswpGenerators)
{
    m_pswpGenerators = pswpGenerators;
}

bool GeneratorPluginFactory::LoadGeneratorLibrary(const wxString& sLibrary)
{
    wxLogNull ln;

    pml::Log::Get() << "Generator Plugin\t" << "Load " << sLibrary << std::endl;
    map<wxString, wxDynamicLibrary*>::iterator itLib = m_mLibraries.find(sLibrary);
    if(itLib == m_mLibraries.end())
    {
        wxString sCan = wxDynamicLibrary::CanonicalizeName(sLibrary);

        wxString sDir(Settings::Get().GetGeneratorPluginDirectory());

        #ifdef PAMBASE_DEBUG
            wxString sLib(wxString::Format(wxT("%s/debug/%s"),sDir.c_str(), sCan.c_str()));
        #else
            wxString sLib(wxString::Format(wxT("%s/%s"), sDir.c_str(), sCan.c_str()));
        #endif


        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);

        if(pLib && pLib->IsLoaded())
        {
            if(pLib->HasSymbol(wxT("CreateGeneratorBuilder")))
            {
                typedef void (*RegPtr)();
                RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("CreateGeneratorBuilder"));
                if(ptr)
                {
                    (*ptr)();
                    m_mLibraries.insert(make_pair(sLibrary, pLib));
                    pml::Log::Get() << "Generator Plugin\t" << "Loaded " << sLibrary << std::endl;
                    return true;
                }
                else
                {
                    pml::Log::Get(pml::Log::LOG_ERROR) << "Generator Plugin\t" << sLib << " cannot execute function CreateMonitorBuilder" << std::endl;
                }
            }
            else
            {
                pml::Log::Get(pml::Log::LOG_ERROR) << "Generator Plugin\t" << sLib << " has no function CreateMonitorBuilder" << std::endl;

            }
        }
        else
        {
            pml::Log::Get(pml::Log::LOG_ERROR) << "Generator Plugin\t" << "Could not load " << sLib << std::endl;
        }
        delete pLib;
    }
    else
    {
        typedef void (*RegPtr)();

        RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("CreateGeneratorBuilder"));
        if(ptr)
        {
            (*ptr)();
            return true;
        }
    }
    return false;
}


void GeneratorPluginFactory::UnloadAll()
{
    for(map<wxString, GeneratorPluginBuilder*>::iterator itBuilder = m_mBuilders.begin(); itBuilder != m_mBuilders.end(); ++itBuilder)
    {
        delete itBuilder->second;
    }
    m_mBuilders.clear();

    for(map<wxString, wxDynamicLibrary*>::iterator itBuilder = m_mLibraries.begin(); itBuilder != m_mLibraries.end(); ++itBuilder)
    {
        delete itBuilder->second;
    }
    m_mLibraries.clear();

}


map<wxString, GeneratorPluginBuilder*>::iterator GeneratorPluginFactory::GeneratorPluginFactory::GetPluginBegin()
{
    return m_mBuilders.begin();
}
map<wxString, GeneratorPluginBuilder*>::iterator GeneratorPluginFactory::GeneratorPluginFactory::GetPluginEnd()
{
    return m_mBuilders.end();
}

map<wxString, GeneratorPluginBuilder*>::iterator GeneratorPluginFactory::FindPlugin(const wxString& sName)
{
    return m_mBuilders.find(sName);
}


plugin GeneratorPluginFactory::GetPluginDetails(const wxString& sDir, const wxString& sLibrary)
{


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



    wxLogNull ln;
    if(itLib != m_mLibraries.end())
    {

        if(itLib->second->HasSymbol(wxT("GetGeneratorPluginName")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetGeneratorPluginName"));
            aPlugin.sName = (*ptr)();
        }
        if(itLib->second->HasSymbol(wxT("GetVersion")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetVersion"));
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
        wxString sLib = sDir + wxT("/") + wxDynamicLibrary::CanonicalizeName(sLibrary);

        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);
        if(pLib)
        {
            if(pLib->IsLoaded())
            {
                if(pLib->HasSymbol(wxT("GetGeneratorPluginName")))
                {
                    typedef wxString (*RegPtr)();
                    RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("GetGeneratorPluginName"));
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
                    pml::Log::Get(pml::Log::LOG_ERROR) << "Generator Plugin\t" << "Could not find correct symbols in lib " << sLib << std::endl;
                }
            }
            else
            {
                pml::Log::Get(pml::Log::LOG_ERROR) << "Generator Plugin\t" << "Could not load lib " << sLib << std::endl;
            }
            delete pLib;
        }
        else
        {
            pml::Log::Get(pml::Log::LOG_ERROR) << "Generator Plugin\t" << "Could not create lib " << sLib << std::endl;
        }
    }
    return aPlugin;
}

wxString GeneratorPluginFactory::GetPluginDirectory()
{
     return Settings::Get().GetGeneratorPluginDirectory();
}
