#include "remoteapi.h"
#include "settings.h"
#include "settingevent.h"
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"
#include "generatorpluginfactory.h"
#include <algorithm>
#include <functional>
#include <wx/filename.h>
#include <wx/dir.h>
#include "aoipsourcemanager.h"

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

    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/wav"), std::bind(&RemoteApi::GetWavFiles, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::POST, endpoint("/x-pam/wav"), std::bind(&RemoteApi::PostWavFile, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::HTTP_DELETE, endpoint("/x-pam/wav"), std::bind(&RemoteApi::DeleteWavFile, this, _1,_2,_3,_4));

    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/aoip"), std::bind(&RemoteApi::GetAoipSources, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::PATCH, endpoint("/x-pam/aoip"), std::bind(&RemoteApi::PatchAoipSources, this, _1,_2,_3,_4));

    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins"), std::bind(&RemoteApi::GetPlugins, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/monitor"), std::bind(&RemoteApi::GetPluginsMonitor, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/test"), std::bind(&RemoteApi::GetPluginsTest, this, _1,_2,_3,_4));
    m_Server.AddEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/generator"), std::bind(&RemoteApi::GetPluginsGenerator, this, _1,_2,_3,_4));

    m_Server.AddNotFoundCallback(std::bind(&RemoteApi::NotFound, this, _1,_2,_3,_4));

    m_Server.Init(Settings::Get().Read("RemoteApi", "Port", 8090), endpoint("/x-pam"), true);

}
pml::restgoose::response RemoteApi::NotFound(const query& theQuery,  const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp(404);
    resp.jsonData["path"] = theEndpoint.Get();
    return resp;
}

pml::restgoose::response RemoteApi::GetRoot(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    resp.jsonData.append("settings/");
    resp.jsonData.append("plugins/");
    resp.jsonData.append("wav/");
    resp.jsonData.append("aoip/");



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

bool RemoteApi::WSAuthenticate(const endpoint& theEndpoint, const userName& theUser, const ipAddress& thePeer)
{
    if(m_setWS.find(theEndpoint) != m_setWS.end())
    {
        return true;
    }
    return false;
}

bool RemoteApi::WSMessage(const endpoint& theEndpoint, const Json::Value& theMessage)
{
    //we don't allow messages
    return false;
}

void RemoteApi::WSClose(const endpoint& theEndpoint, const ipAddress& thePeer)
{

}

bool RemoteApi::AddPluginEndpoint(const httpMethod& method, const endpoint& theEndpoint, std::function<pml::restgoose::response(const query&, const std::vector<pml::restgoose::partData>&, const endpoint&, const userName&)> func)
{
    return m_Server.AddEndpoint(method, theEndpoint, func);
}

bool RemoteApi::AddPluginWebsocketEndpoint(const endpoint& theEndpoint)
{
    if(m_setWS.insert(theEndpoint).second)
    {
        return m_Server.AddWebsocketEndpoint(theEndpoint, std::bind(&RemoteApi::WSAuthenticate, this, _1,_2,_3), std::bind(&RemoteApi::WSMessage, this, _1, _2), std::bind(&RemoteApi::WSClose, this, _1, _2));
    }
    return false;
}

pml::restgoose::response RemoteApi::GetWavFiles(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    resp.jsonData = Json::Value(Json::arrayValue);

    wxArrayString asFiles;
    wxDir::GetAllFiles(Settings::Get().GetWavDirectory(), &asFiles, wxT("*.wav"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        resp.jsonData.append(fn.GetName().ToStdString());
    }
    return resp;
}

pml::restgoose::response RemoteApi::PostWavFile(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    return resp;
}

pml::restgoose::response RemoteApi::DeleteWavFile(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    return resp;
}

pml::restgoose::response RemoteApi::GetAoipSources(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    resp.jsonData = Json::Value(Json::arrayValue);
    for(auto pairSource : AoipSourceManager::Get().GetSources())
    {
        if(pairSource.first > 0)
        {
            Json::Value jsSource;
            jsSource["index"] = pairSource.first;
            jsSource["name"] = pairSource.second.sName.ToStdString();
            jsSource["type"] = pairSource.second.sType.ToStdString();
            jsSource["details"] = pairSource.second.sDetails.ToStdString();
            jsSource["SDP"] = pairSource.second.sSDP.ToStdString();
            jsSource["tags"] = Json::Value(Json::arrayValue);
            for(auto tag : pairSource.second.setTags)
            {
                jsSource["tags"].append(tag.ToStdString());
            }
            resp.jsonData.append(jsSource);
        }
    }
    return resp;
}

pml::restgoose::response RemoteApi::PatchAoipSources(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    return resp;
}

