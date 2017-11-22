#include "testpluginfactory.h"
#include "testpluginbuilder.h"
#include "wmswitcherpanel.h"
#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/xml/xml.h>


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
 //   wxLogNull ln;

    map<wxString, wxDynamicLibrary*>::iterator itLib = m_mLibraries.find(sLibrary);
    if(itLib == m_mLibraries.end())
    {
        wxString sLib;
        #ifdef __WXDEBUG__
        sLib = wxString::Format(wxT("lib\\debug\\%s"), sLibrary.c_str());
        #else
        sLib = wxString::Format(wxT("lib\\%s"), sLibrary.c_str());
        #endif // __WXDEBUG__
        wxDynamicLibrary* pLib = new wxDynamicLibrary(wxDynamicLibrary::CanonicalizeName(sLib));

        if(pLib && pLib->IsLoaded())
        {
            if(pLib->HasSymbol(wxT("CreateBuilders")))
            {
                typedef void (*RegPtr)();
                RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("CreateBuilders"));
                if(ptr)
                {
                    (*ptr)();
                    m_mLibraries.insert(make_pair(sLibrary, pLib));
                    return true;
                }
            }
        }
        delete pLib;
    }
    else
    {
        typedef void (*RegPtr)();

        RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("CreateBuilders"));
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
