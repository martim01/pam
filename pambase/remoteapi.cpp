#include "remoteapi.h"
#include "settings.h"
#include "settingevent.h"
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"
#include "generatorpluginfactory.h"
#include <algorithm>
#include <functional>

using namespace std::placeholders;

RemoteApi& RemoteApi::Get()
{
    static RemoteApi api;
    return api;
}

RemoteApi::~RemoteApi()
{
    m_Server.Stop();
}

void RemoteApi::Run()
{
    m_Server.Run(true);
}

RemoteApi::RemoteApi()
{
    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam"), std::bind(&RemoteApi::GetRoot, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/settings"), std::bind(&RemoteApi::GetSettings, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::PATCH, endpoint("/x-pam/settings"), std::bind(&RemoteApi::PatchSettings, this, _1,_2,_3,_4));

    m_Server.Init(Settings::Get().Read("RemoteApi", "Port", 8090), endpoint("/x-pam"), true);

}

pml::restgoose::response RemoteApi::GetRoot(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    resp.jsonData.append("settings/");
    resp.jsonData.append("plugins/");
    return resp;
}

pml::restgoose::response RemoteApi::GetSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    for(auto pairSection : Settings::Get().GetSections())
    {
        resp.jsonData[pairSection.first.ToStdString()] = Json::Value(Json::objectValue);
        for(auto pairData : pairSection.second->GetData())
        {
            resp.jsonData[pairSection.first.ToStdString()][pairData.first.ToStdString()] = pairData.second.ToStdString();
        }
    }
    return resp;
}

pml::restgoose::response RemoteApi::PatchSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp(400);
    return resp;
}

pml::restgoose::response RemoteApi::GetPlugins(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    resp.jsonData.append("monitor/");
    resp.jsonData.append("test/");
    resp.jsonData.append("generator/");
    return resp;
}
pml::restgoose::response RemoteApi::GetPluginsMonitor(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    std::for_each(MonitorPluginFactory::Get()->GetPluginBegin(), MonitorPluginFactory::Get()->GetPluginEnd(), [&](const std::pair<wxString, MonitorPluginBuilder*>& pairPlugin)
                  {
                      resp.jsonData.append(pairPlugin.first.ToStdString()+"/");
                  });
    return resp;
}

pml::restgoose::response RemoteApi::GetPluginsTest(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    std::for_each(TestPluginFactory::Get()->GetPluginBegin(), TestPluginFactory::Get()->GetPluginEnd(), [&](const std::pair<wxString, TestPluginBuilder*>& pairPlugin)
                  {
                      resp.jsonData.append(pairPlugin.first.ToStdString()+"/");
                  });
    return resp;
}

pml::restgoose::response RemoteApi::GetPluginsGenerator(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    std::for_each(GeneratorPluginFactory::Get()->GetPluginBegin(), GeneratorPluginFactory::Get()->GetPluginEnd(), [&](const std::pair<wxString, GeneratorPluginBuilder*>& pairPlugin)
                  {
                      resp.jsonData.append(pairPlugin.first.ToStdString()+"/");
                  });
    return resp;
}

