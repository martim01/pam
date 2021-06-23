#include "testpluginfactory.h"
#include "testpluginbuilder.h"
#include "wmswitcherpanel.h"
#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/xml/xml.h>
#include <wx/stdpaths.h>
#include "settings.h"
#include "log.h"

using namespace std;

TestPluginFactory* TestPluginFactory::Get()
{
    static TestPluginFactory wmf;
    return &wmf;
}

void TestPluginFactory::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void TestPluginFactory::Register(TestPluginBuilder* pBuilder)
{
    pBuilder->SetHandler(m_pHandler);
    pBuilder->CreatePanels(m_pswpTests);

    m_mBuilders.insert(make_pair(pBuilder->GetName(), pBuilder));
}

TestPluginFactory::~TestPluginFactory()
{
    UnloadAll();
}

void TestPluginFactory::SetSwitcherPanels(wmSwitcherPanel* pswpTests)
{
    m_pswpTests = pswpTests;
}

bool TestPluginFactory::LoadTestLibrary(const wxString& sLibrary)
{
    wxLogNull ln;

    pmlLog(pml::LOG_INFO) << "Test Plugin\t" << "Load '" << sLibrary << "'";

    map<wxString, wxDynamicLibrary*>::iterator itLib = m_mLibraries.find(sLibrary);
    if(itLib == m_mLibraries.end())
    {
        wxString sCan = wxDynamicLibrary::CanonicalizeName(sLibrary);

        wxString sDir(Settings::Get().GetTestPluginDirectory());


        #ifdef PAMBASE_DEBUG
            wxString sLib(wxString::Format(wxT("%s/debug/%s"),sDir.c_str(), sCan.c_str()));
        #else
            wxString sLib(wxString::Format(wxT("%s/%s"), sDir.c_str(), sCan.c_str()));
        #endif


        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);

        if(pLib && pLib->IsLoaded())
        {
            if(pLib->HasSymbol(wxT("CreateTestBuilder")))
            {
                typedef void (*RegPtr)();
                RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("CreateTestBuilder"));
                if(ptr)
                {
                    (*ptr)();
                    m_mLibraries.insert(make_pair(sLibrary, pLib));
                    pmlLog(pml::LOG_INFO) << "Test Plugin\t" << "Loaded '" << sLibrary << "'";
                    return true;
                }
                else
                {
                    pmlLog(pml::LOG_ERROR) << "Test Plugin\t" << "'" << sLib << "' cannot execute function CreateMonitorBuilder";
                }
            }
            else
            {
                pmlLog(pml::LOG_ERROR) << "Test Plugin\t" << "'" << sLib << "' has no function CreateMonitorBuilder";

            }
        }
        else
        {
            pmlLog(pml::LOG_ERROR) << "Test Plugin\t" << "Could not load '" << sLib << "'";
        }
        delete pLib;
    }
    else
    {
        typedef void (*RegPtr)();

        RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("CreateTestBuilder"));
        if(ptr)
        {
            (*ptr)();
            return true;
        }
    }
    return false;
}


void TestPluginFactory::UnloadAll()
{
    for(map<wxString, TestPluginBuilder*>::iterator itBuilder = m_mBuilders.begin(); itBuilder != m_mBuilders.end(); ++itBuilder)
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


map<wxString, TestPluginBuilder*>::iterator TestPluginFactory::TestPluginFactory::GetPluginBegin()
{
    return m_mBuilders.begin();
}
map<wxString, TestPluginBuilder*>::iterator TestPluginFactory::TestPluginFactory::GetPluginEnd()
{
    return m_mBuilders.end();
}

map<wxString, TestPluginBuilder*>::iterator TestPluginFactory::FindPlugin(const wxString& sName)
{
    return m_mBuilders.find(sName);
}


plugin TestPluginFactory::GetPluginDetails(const wxString& sDir, const wxString& sLibrary)
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

        if(itLib->second->HasSymbol(wxT("GetTestPluginName")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetTestPluginName"));
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
                if(pLib->HasSymbol(wxT("GetTestPluginName")))
                {
                    typedef wxString (*RegPtr)();
                    RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("GetTestPluginName"));
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
                    pmlLog(pml::LOG_ERROR) << "Test Plugin\t" << "Could not find correct symbols in lib " << sLib;
                }
            }
            else
            {
                pmlLog(pml::LOG_ERROR) << "Test Plugin\t" << "Could not load lib " << sLib;
            }
            delete pLib;
        }
        else
        {
            pmlLog(pml::LOG_ERROR) << "Test Plugin\t" << "Could not create lib " << sLib;
        }
    }
    return aPlugin;
}

wxString TestPluginFactory::GetPluginDirectory()
{
     return Settings::Get().GetTestPluginDirectory();
}
