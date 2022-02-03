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
#include "log.h"

using namespace std::placeholders;

Json::Value ConvertToJson(const std::string& str)
{
    Json::Value jsData;
    try
    {
        std::stringstream ss;
        ss.str(str);
        ss >> jsData;

    }
    catch(const Json::RuntimeError& e)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS: " << "Unable to convert '" << str << "' to JSON: " << e.what();
    }
    return jsData;
}

pml::restgoose::response ConvertPostDataToJson(const std::vector<pml::restgoose::partData>& vData)
{
    pml::restgoose::response resp;
    if(vData.size() == 1)
    {
        resp.nHttpCode = 200;
        resp.jsonData = ConvertToJson(vData[0].data.Get());
    }
    else if(vData.size() > 1)
    {
        resp.nHttpCode = 200;
        resp.jsonData.clear();
        for(size_t i = 0; i < vData.size(); i++)
        {
            if(vData[i].name.Get().empty() == false)
            {
                resp.jsonData[vData[i].name.Get()] = vData[i].data.Get();
            }
        }
    }
    return resp;
}

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
    m_Server.Init(Settings::Get().Read("RemoteApi", "Port", 8090), endpoint("/x-pam"), true);


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

    //m_Server.AddWebsocketEndpoint(endpoint("/x-pam/settings"), std::bind(&RemoteApi::WSAuthenticate, this, _1,_2,_3), std::bind(&RemoteApi::WSMessage, this, _1,_2), std::bind(&RemoteApi::WSClose, this, _1,_2));
    m_Server.AddWebsocketEndpoint(endpoint("/x-pam/monitor"), std::bind(&RemoteApi::WSAuthenticate, this, _1,_2,_3), std::bind(&RemoteApi::WSMessage, this, _1,_2), std::bind(&RemoteApi::WSClose, this, _1,_2));



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

    for(auto pairSection : m_mSettings)
    {
        resp.jsonData[pairSection.first.ToStdString()] = Json::Value(Json::objectValue);
        for(auto pairData : pairSection.second.mKeys)
        {
            Json::Value jsKey;
            jsKey["current"] = Settings::Get().Read(pairSection.first, pairData.first, "").ToStdString();

            if(pairData.second.setEnum.empty() == false)
            {
                Json::Value jsEnum(Json::arrayValue);
                for(auto sOption : pairData.second.setEnum)
                {
                    jsEnum.append(sOption.ToStdString());
                }
                jsKey["enum"] = jsEnum;
            }
            else if(pairData.second.setEnumNum.empty() == false)
            {
                Json::Value jsEnum(Json::arrayValue);
                for(auto nOption : pairData.second.setEnumNum)
                {
                    jsEnum.append(nOption);
                }
                jsKey["enum"] = jsEnum;
            }
            else
            {
                jsKey["range"]["minimum"] = pairData.second.dRange.first;
                jsKey["range"]["maximum"] = pairData.second.dRange.second;
            }

            resp.jsonData[pairSection.first.ToStdString()][pairData.first.ToStdString()] = jsKey;
        }
    }
    return resp;
}

pml::restgoose::response RemoteApi::PatchSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    auto request = ConvertPostDataToJson(vData);
    pml::restgoose::response resp;

    if(request.nHttpCode == 200)
    {
        //array of data to change
        if(request.jsonData.isArray())
        {
            for(Json::ArrayIndex ai = 0; ai < request.jsonData.size(); ai++)
            {
                auto check = CheckJsonSettingPatch(request.jsonData[ai]);
                if(check.nHttpCode == 400)
                {
                    resp.nHttpCode = 400;
                }
                resp.jsonData.append(check.jsonData);
            }

            if(resp.nHttpCode == 200)   //error somewhere
            {
                DoPatchSettings(request.jsonData);
            }
            return resp;
        }
        else
        {
            return pml::restgoose::response(400, "Data received is not a valid JSON array");
        }
    }
    else
    {
        return pml::restgoose::response(400, "Data received could not be converted to JSON");
    }
}

pml::restgoose::response RemoteApi::CheckJsonSettingPatch(const Json::Value& jsPatch)
{
    pml::restgoose::response resp(400);

    if(jsPatch.isObject() && jsPatch.isMember("section") && jsPatch.isMember("key") && jsPatch.isMember("value"))
    {

        resp.jsonData = jsPatch;

        auto itSection = m_mSettings.find(wxString(jsPatch["section"].asString()));
        if(itSection != m_mSettings.end())
        {
            auto itKey = itSection->second.mKeys.find(wxString(jsPatch["key"].asString()));
            if(itKey != itSection->second.mKeys.end())
            {
                auto check = CheckJsonSettingPatchConstraint(jsPatch, itKey->second);
                if(check.nHttpCode == 400)
                {
                    resp.jsonData["reason"] = check.jsonData;
                }
                else
                {
                    //success
                    resp.nHttpCode = 200;
                    resp.jsonData["reason"] = "Success";
                }
            }
            else
            {
                resp.jsonData["reason"] = "Section "+jsPatch["section"].asString()+" has no key "+jsPatch["key"].asString();
            }
        }
        else
        {
            resp.jsonData["reason"] = "Settings has no section "+jsPatch["section"].asString();
        }
    }
    else
    {
        resp.jsonData["reason"] = "JSON is invalid";
    }
    return resp;

}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const RemoteApi::section::constraint& aConstraint)
{
    if(aConstraint.setEnum.empty() == false)
    {
        auto itValue = aConstraint.setEnum.find(wxString(jsPatch["value"].asString()));
        if(itValue != aConstraint.setEnum.end())
        {
            return pml::restgoose::response(200);
        }
        else
        {
            return pml::restgoose::response(400, jsPatch["value"].asString()+" is not a valid value");
        }
    }
    else if(aConstraint.setEnumNum.empty() == false)
    {
        if(jsPatch["value"].isConvertibleTo(Json::intValue))
        {
            auto itValue = aConstraint.setEnumNum.find(jsPatch["value"].asInt());
            if(itValue != aConstraint.setEnumNum.end())
            {
                return true;
            }
            else
            {
                return pml::restgoose::response(400, jsPatch["value"].asString()+" is not a valid value");
            }
        }
        else
        {
            return pml::restgoose::response(400, jsPatch["value"].asString()+" is not convertible to an integeger");
        }
    }
    else if(jsPatch["value"].isConvertibleTo(Json::realValue) && jsPatch["value"].asDouble() >= aConstraint.dRange.first && jsPatch["value"].asDouble() <= aConstraint.dRange.second)
    {
        return true;
    }
    else
    {
        return pml::restgoose::response(400, jsPatch["value"].asString()+" is not convertible to a double or is out of range");
    }
}

void RemoteApi::DoPatchSettings(const Json::Value& jsArray)
{
    for(Json::ArrayIndex ai = 0; ai < jsArray.size(); ai++)
    {
        Settings::Get().Write(jsArray[ai]["section"].asString(), jsArray[ai]["key"].asString(), jsArray[ai]["value"].asString());
    }
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
    pmlLog() << "RemoteApi\tWebsocket connection: " << theEndpoint.Get() << " from " << thePeer.Get();
    return true;
}

bool RemoteApi::WSMessage(const endpoint& theEndpoint, const Json::Value& theMessage)
{
    //we don't allow messages
    return false;
}

void RemoteApi::WSClose(const endpoint& theEndpoint, const ipAddress& thePeer)
{
    pmlLog() << "RemoteApi\tWebsocket closed: " << theEndpoint.Get() << " from " << thePeer.Get();
}

bool RemoteApi::AddPluginEndpoint(const httpMethod& method, const endpoint& theEndpoint, std::function<pml::restgoose::response(const query&, const std::vector<pml::restgoose::partData>&, const endpoint&, const userName&)> func)
{
    return m_Server.AddEndpoint(method, theEndpoint, func);
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

void RemoteApi::SendPluginWebsocketMessage(const Json::Value& jsMessage)
{
    m_Server.SendWebsocketMessage({endpoint("/x-pam/monitor")}, jsMessage);
}


void RemoteApi::RegisterRemoteApiStringEnum(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(setEnum)));
}

void RemoteApi::RegisterRemoteApiRange(const wxString& sSection, const wxString& sKey, const std::pair<double, double>& dRange)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(dRange)));
}

void RemoteApi::RegisterRemoteApiIntEnum(const wxString& sSection, const wxString& sKey, const std::set<int>& setEnum)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(setEnum)));
}
