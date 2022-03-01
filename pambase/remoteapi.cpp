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
#include <wx/tokenzr.h>
#include "soundfile.h"
#include <map>

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

std::map<wxString, wxString> RemoteApi::ConvertQueryToMap(const query& theQuery)
{

    wxArrayString asQuery = wxStringTokenize(wxString(theQuery.Get()), "&");
    std::map<wxString, wxString> mQuery;

    for(size_t i = 0; i < asQuery.GetCount(); i++)
    {
        mQuery.insert({asQuery[i].Before('=').Lower(), asQuery[i].After('=')});
    }
    return mQuery;
}



void RemoteApi::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == "RemoteApi")
    {
        if(event.GetKey() == "Websockets")
        {
            m_bWebsocketsActive = event.GetValue(false);
        }
        else if(event.GetKey() == "Enable")
        {
            if(event.GetValue(false))
            {
                m_Server.Run(true);
            }
            else
            {
                m_Server.Stop();
            }
        }
        else if(event.GetKey() == "Interface")
        {
            m_Server.SetInterface(ipAddress(event.GetValue().ToStdString()), Settings::Get().Read("RemoteApi", "Port", 8090));
        }
        else if(event.GetKey() == "Port")
        {
            m_Server.SetInterface(ipAddress(Settings::Get().Read("RemoteApi", "Interface", "0.0.0.0").ToStdString()), event.GetValue(8090l));
        }
    }
    else if(WebsocketsActive())
    {
        auto itSection = m_mSettings.find(event.GetSection());
        if(itSection != m_mSettings.end() && itSection->second.mKeys.find(event.GetKey()) != itSection->second.mKeys.end())
        {
            Json::Value jsMessage;
            jsMessage["setting"]["section"] = event.GetSection().ToStdString();
            jsMessage["setting"]["key"] = event.GetKey().ToStdString();
            switch(event.GetType())
            {
                case SettingEvent::SETTING_STRING:
                    jsMessage["setting"]["value"] = event.GetValue().ToStdString();
                    break;
                case SettingEvent::SETTING_LONG:
                    jsMessage["setting"]["value"] = (int)event.GetValue(0l);
                    break;
                case SettingEvent::SETTING_DOUBLE:
                    jsMessage["setting"]["value"] = event.GetValue(0.0);
                    break;
                case SettingEvent::SETTING_BOOL:
                    jsMessage["setting"]["value"] = event.GetValue(false);
                    break;
            }
            SendSettingWebsocketMessage(jsMessage);
        }
    }
}


RemoteApi& RemoteApi::Get()
{
    static RemoteApi api;
    return api;
}

RemoteApi::~RemoteApi()
{
    m_Server.Stop();
    Settings::Get().RemoveHandler(this);
}


RemoteApi::RemoteApi() : m_bWebsocketsActive(Settings::Get().Read("RemoteApi", "Websockets",0))
{
    m_Server.Init(ipAddress(Settings::Get().Read("RemoteApi", "Interface", "0.0.0.0").ToStdString()), Settings::Get().Read("RemoteApi", "Port", 8090), endpoint("/x-pam"), true);


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

    m_Server.AddWebsocketEndpoint(endpoint("/x-pam/ws/plugin"), std::bind(&RemoteApi::WSAuthenticate, this, _1,_2,_3), std::bind(&RemoteApi::WSMessage, this, _1,_2), std::bind(&RemoteApi::WSClose, this, _1,_2));
    m_Server.AddWebsocketEndpoint(endpoint("/x-pam/ws/setting"), std::bind(&RemoteApi::WSAuthenticate, this, _1,_2,_3), std::bind(&RemoteApi::WSMessage, this, _1,_2), std::bind(&RemoteApi::WSClose, this, _1,_2));
    m_Server.AddWebsocketEndpoint(endpoint("/x-pam/ws"), std::bind(&RemoteApi::WSAuthenticate, this, _1,_2,_3), std::bind(&RemoteApi::WSMessage, this, _1,_2), std::bind(&RemoteApi::WSClose, this, _1,_2));

    Settings::Get().AddHandler(this);
    Bind(wxEVT_SETTING_CHANGED, &RemoteApi::OnSettingEvent, this);

    if(Settings::Get().Read("RemoteApi","Enable",false))
    {
        m_Server.Run(true);
    }

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
    auto mQuery = ConvertQueryToMap(theQuery);
    wxString sSection = mQuery["section"];
    wxString sKey = mQuery["key"];


    pml::restgoose::response resp;

    for(auto pairSection : m_mSettings)
    {
        if(sSection.empty() || sSection.CmpNoCase(pairSection.first) == 0)
        {
            resp.jsonData[pairSection.first.ToStdString()] = GetSectionJson(pairSection.first, pairSection.second, sKey);
        }
    }
    return resp;
}

Json::Value RemoteApi::GetSectionJson(const wxString& sSection, const wxString& sKeyFilter)
{
    auto itSection = m_mSettings.find(sSection);
    if(itSection != m_mSettings.end())
    {
        return GetSectionJson(itSection->first, itSection->second, sKeyFilter);
    }
    return Json::Value(Json::objectValue);
}

Json::Value RemoteApi::GetSectionJson(const wxString& sSectionName, const section& aSection, const wxString& sKeyFilter)
{
    Json::Value jsSection;
    for(auto pairData : aSection.mKeys)
    {
        if(sKeyFilter.empty() || sKeyFilter.CmpNoCase(pairData.first) == 0)
        {
            Json::Value jsKey;

            switch(pairData.second.eType)
            {
                case section::constraint::enumType::STRING_ENUM:
                    jsKey["enum"] = GetJson(pairData.second.setEnum);
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.s).ToStdString();
                    break;
                case section::constraint::enumType::INT_ENUM:
                    jsKey["enum"] = GetJson(pairData.second.mEnumNum);
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.n);
                    break;
                case section::constraint::enumType::STRING_CALLBACK:
                    jsKey["enum"] = GetJson(pairData.second.funcStringEnum);
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.s).ToStdString();
                    break;
                case section::constraint::enumType::INT_CALLBACK:
                    jsKey["enum"] = GetJson(pairData.second.funcIntEnum);
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.n);
                    break;break;
                case section::constraint::enumType::INT_RANGE:
                    jsKey["range"]["minimum"] = pairData.second.nRange.first;
                    jsKey["range"]["maximum"] = pairData.second.nRange.second;
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.n);
                    break;
                case section::constraint::enumType::DOUBLE_RANGE:
                    jsKey["range"]["minimum"] = pairData.second.dRange.first;
                    jsKey["range"]["maximum"] = pairData.second.dRange.second;
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.d);
                    break;
                case section::constraint::enumType::CSV:
                    jsKey["csv"] = GetJson(pairData.second.setEnum);
                    jsKey["current"] = Settings::Get().Read(sSectionName, pairData.first, pairData.second.initial.s).ToStdString();
                    break;
            }

            jsSection[pairData.first.ToStdString()] = jsKey;
        }
    }
    return jsSection;
}

Json::Value RemoteApi::GetJson(const std::set<wxString>& setEnum)
{
    Json::Value jsEnum(Json::arrayValue);
    for(auto sOption : setEnum)
    {
        jsEnum.append(sOption.ToStdString());
    }
    return jsEnum;
}

Json::Value RemoteApi::GetJson(const std::map<int,wxString>& mEnum)
{
    Json::Value jsEnum(Json::arrayValue);
    for(auto pairOption : mEnum)
    {
        Json::Value jsV;
        jsV["label"] = pairOption.second.ToStdString();
        jsV["value"] = pairOption.first;
        jsEnum.append(jsV);
    }
    return jsEnum;
}


Json::Value RemoteApi::GetJson(std::function<std::set<wxString>()> func)
{
    if(func)
    {
        return GetJson(func());
    }
    else
    {
        return Json::Value(Json::arrayValue);
    }
}

Json::Value RemoteApi::GetJson(std::function<std::map<int, wxString>()> func)
{
    if(func)
    {
        return GetJson(func());
    }
    else
    {
        return Json::Value(Json::arrayValue);
    }
}

pml::restgoose::response RemoteApi::PatchSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    return DoPatchSettings(vData);
}

pml::restgoose::response RemoteApi::DoPatchSettings(const std::vector<pml::restgoose::partData>& vData, const wxString& sSection)
{
    pml::restgoose::response resp;
    auto request = ConvertPostDataToJson(vData);
    if(request.nHttpCode == 200)
    {
        //array of data to change
        if(request.jsonData.isArray())
        {
            for(Json::ArrayIndex ai = 0; ai < request.jsonData.size(); ai++)
            {
                auto check = CheckJsonSettingPatch(request.jsonData[ai], sSection);
                if(check.nHttpCode == 400)
                {
                    resp.nHttpCode = 400;
                }
                resp.jsonData.append(check.jsonData);
            }

            if(resp.nHttpCode == 200)   //no error
            {
                DoPatchSettings(request.jsonData, sSection);
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

pml::restgoose::response RemoteApi::CheckJsonSettingPatch(const Json::Value& jsPatch, wxString sSection)
{
    pml::restgoose::response resp(400);

    if(jsPatch.isObject() && (jsPatch.isMember("section") || sSection.empty() == false) && jsPatch.isMember("key") && jsPatch.isMember("value"))
    {
        if(sSection.empty())
        {
            sSection = wxString(jsPatch["section"].asString());
        }

        resp.jsonData = jsPatch;

        auto itSection = m_mSettings.find(sSection);
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
                resp.nHttpCode = 404;
                resp.jsonData["reason"] = "Section "+sSection.ToStdString()+" has no key "+jsPatch["key"].asString();
            }
        }
        else
        {
            resp.nHttpCode = 404;
            resp.jsonData["reason"] = "Settings has no section "+sSection.ToStdString();
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
    switch(aConstraint.eType)
    {
        case section::constraint::enumType::STRING_ENUM:
            return CheckJsonSettingPatchConstraint(jsPatch["value"], aConstraint.setEnum);
        case section::constraint::enumType::INT_ENUM:
            return CheckJsonSettingPatchConstraint(jsPatch["value"], aConstraint.mEnumNum);
        case section::constraint::enumType::INT_RANGE:
            return CheckJsonSettingPatchConstraint(jsPatch["value"], aConstraint.nRange);
        case section::constraint::enumType::DOUBLE_RANGE:
            return CheckJsonSettingPatchConstraint(jsPatch["value"], aConstraint.dRange);
        case section::constraint::enumType::STRING_CALLBACK:
            return CheckJsonSettingPatchConstraint(jsPatch["value"], aConstraint.funcStringEnum);
        case section::constraint::enumType::INT_CALLBACK:
            return CheckJsonSettingPatchConstraint(jsPatch["value"], aConstraint.funcIntEnum);
        case section::constraint::enumType::CSV:
            return CheckJsonSettingPatchConstraintCSV(jsPatch["value"], aConstraint.setEnum);
        default:
            return pml::restgoose::response(200);
    }
}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsValue, const std::set<wxString>& setEnum)
{
    auto itValue = setEnum.find(wxString(jsValue.asString()));
    if(itValue != setEnum.end())
    {
        return pml::restgoose::response(200);
    }
    else
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not a valid value");
    }
}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraintCSV(const Json::Value& jsValue, const std::set<wxString>& setEnum)
{
    wxArrayString asValues = wxStringTokenize(wxString(jsValue.asString()), ",");
    if(asValues.GetCount() == 0)
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not a valid value");
    }

    for(size_t i = 0; i < asValues.GetCount(); i++)
    {
        auto itValue = setEnum.find(asValues[i]);
        if(itValue == setEnum.end())
        {
            return pml::restgoose::response(400, jsValue.asString()+" is not a valid value");
        }
    }
    return pml::restgoose::response(200);
}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsValue, const std::map<int, wxString>& mEnum)
{
    if(jsValue.isConvertibleTo(Json::intValue))
    {
        auto itValue = mEnum.find(jsValue.asInt());
        if(itValue != mEnum.end())
        {
            return pml::restgoose::response(200);
        }
        else
        {
            return pml::restgoose::response(400, jsValue.asString()+" is not a valid value");
        }
    }
    else
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not convertible to an integer");
    }

}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsValue, const std::pair<double, double>& dRange)
{
    if(jsValue.isConvertibleTo(Json::realValue) == false)
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not convertible to a double");
    }
    else if (jsValue.asDouble() >= dRange.first && jsValue.asDouble() <= dRange.second)
    {
        return pml::restgoose::response(200);
    }
    else
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not within range");
    }
}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsValue, const std::pair<int, int>& nRange)
{
    if(jsValue.isConvertibleTo(Json::intValue) == false)
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not convertible to an integer");
    }
    else if (jsValue.asInt() >= nRange.first && jsValue.asInt() <= nRange.second)
    {
        return pml::restgoose::response(200);
    }
    else
    {
        return pml::restgoose::response(400, jsValue.asString()+" is not within range");
    }
}

pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsValue, std::function<std::set<wxString>()> func)
{
    if(func)
    {
        return CheckJsonSettingPatchConstraint(jsValue, func());
    }
    else
    {
        return pml::restgoose::response(400, "Enum is empty");
    }
}
pml::restgoose::response RemoteApi::CheckJsonSettingPatchConstraint(const Json::Value& jsValue, std::function<std::map<int, wxString>()> func)
{
    if(func)
    {
        return CheckJsonSettingPatchConstraint(jsValue, func());
    }
    else
    {
        return pml::restgoose::response(400, "Enum is empty");
    }
}

void RemoteApi::DoPatchSettings(const Json::Value& jsArray, const wxString& sSection)
{
    for(Json::ArrayIndex ai = 0; ai < jsArray.size(); ai++)
    {
        if(sSection.empty())
        {
            Settings::Get().Write(jsArray[ai]["section"].asString(), jsArray[ai]["key"].asString(), jsArray[ai]["value"].asString());
        }
        else
        {
            Settings::Get().Write(sSection, jsArray[ai]["key"].asString(), jsArray[ai]["value"].asString());
        }
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
    wxString sFilename;
    wxString sLocation;
    for(const auto& data : vData)
    {
        if(data.name.Get() == "name")
        {
            sFilename = wxString(data.data.Get());
        }
        else if(data.name.Get() == "file")
        {
            sLocation = wxString(data.filepath.Get());
        }
    }
    pmlLog() << "Name: " << sFilename.ToStdString() << "\tLocation: " << sLocation.ToStdString();
    if(sFilename.empty() == false && sLocation.empty() == false)
    {
        //Check it is a wav file
        SoundFile sf;
        if(sf.OpenToRead(sLocation) == false)
        {
            wxRemoveFile(sLocation);
            resp.jsonData["reason"] = "Uploaded file is not a wav file";
        }
        else
        {
            wxFileName fn;
            fn.Assign(Settings::Get().GetWavDirectory(), sFilename+".wav");
            if(wxRenameFile(sLocation, fn.GetFullPath()))
            {
                resp.nHttpCode = 200;
                resp.jsonData["reason"] = "Success";
            }
            else
            {
                wxRemoveFile(sLocation);
                resp.jsonData["reason"] = "Unable to store wav file in directory";
            }
        }
    }
    else
    {
        resp.jsonData["reason"] = "Filename not set, or no file uploaded";
    }
    return resp;
}

pml::restgoose::response RemoteApi::DeleteWavFile(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    auto request = ConvertPostDataToJson(vData);
    pml::restgoose::response resp;

    if(request.nHttpCode == 200)
    {
        //array of data to change
        if(request.jsonData.isArray())
        {
            resp.nHttpCode = 200;
            resp.jsonData["reason"] = "Success";
            for(Json::ArrayIndex ai = 0; ai < request.jsonData.size(); ++ai)
            {
                if(request.jsonData[ai].isString())
                {
                    wxFileName fn;
                    fn.Assign(Settings::Get().GetWavDirectory(), wxString(request.jsonData[ai].asString()+".wav"));
                    if(wxRemoveFile(fn.GetFullPath()))
                    {
                        resp.jsonData["files"].append("Deleted "+request.jsonData[ai].asString());
                    }
                    else
                    {
                        resp.nHttpCode = 404;
                        resp.jsonData["files"].append("Failed to delete "+request.jsonData[ai].asString());
                    }
                }
            }
        }
        else
        {
            resp.jsonData["reason"] = "Passed Json is not an array";
        }
    }
    else
    {
        resp.jsonData["reason"] = "JSON is invalid";
    }
    return resp;
}

pml::restgoose::response RemoteApi::GetAoipSources(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    auto mQuery = ConvertQueryToMap(theQuery);

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
            if(jsSource["type"] = pairSource.second.sType == "SDP")
            {
                jsSource["SDP"] = pairSource.second.sSDP.ToStdString();
            }
            else
            {
                jsSource["details"] = pairSource.second.sDetails.ToStdString();
            }
            jsSource["tags"] = Json::Value(Json::arrayValue);
            for(auto tag : pairSource.second.setTags)
            {
                jsSource["tags"].append(tag.ToStdString());
            }

            //check if the source matches the query string
            bool bAdd(true);
            for(auto pairQuery : mQuery)
            {
                if(jsSource.isMember(pairQuery.first.ToStdString()))
                {
                    if(jsSource[pairQuery.first.ToStdString()].isString() && jsSource[pairQuery.first.ToStdString()].asString() != pairQuery.second.ToStdString())
                    {
                        bAdd = false;
                    }
                    else if(jsSource[pairQuery.first.ToStdString()].isArray())
                    {
                        bool bAdd = false;
                        for(Json::ArrayIndex ai = 0; ai < jsSource["tags"].size(); ++ai)
                        {
                            if(pairQuery.second.ToStdString().find(jsSource["tags"].asString()+",") != std::string::npos ||
                               pairQuery.second.ToStdString().find(","+jsSource["tags"].asString()) != std::string::npos)
                            {
                                bAdd = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(bAdd)
            {
                resp.jsonData.append(jsSource);
            }
        }
    }
    return resp;
}

pml::restgoose::response RemoteApi::PatchAoipSources(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
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
                auto check = CheckJsonAoipPatch(request.jsonData[ai]);
                if(check.nHttpCode != 200)
                {
                    resp.nHttpCode = 400;
                }
                resp.jsonData.append(check.jsonData);
            }

            if(resp.nHttpCode != 200)   //error somewhere
            {
                return resp;
            }
            else
            {
                return DoPatchAoip(request.jsonData);
            }
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

pml::restgoose::response RemoteApi::CheckJsonAoipPatch(const Json::Value& jsPatch)
{
    pml::restgoose::response resp(400);
    if(jsPatch.isMember("action") == false || jsPatch["action"].isString() == false)
    {
        resp.jsonData["reason"] = "No action passed";
        return resp;
    }
    if(jsPatch["action"].asString() == "update")
    {
        if(jsPatch.isMember("index") == false || jsPatch["index"].isInt() == false)
        {
            resp.jsonData["reason"] = "Action is update but no index passed";
            return resp;
        }
        if(jsPatch.isMember("tags") == true && jsPatch["tags"].isArray() == false)
        {
            resp.jsonData["reason"] = "Action is update with tags, but not an array";
            return resp;
        }
    }
    else if(jsPatch["action"].asString() == "delete" && (jsPatch.isMember("index") == false || jsPatch["index"].isInt() == false))
    {
        resp.jsonData["reason"] = "Action is delete but no index passed";
        return resp;
    }
    else if(jsPatch["action"].asString() == "add")
    {
        if(jsPatch.isMember("name") == false || jsPatch["name"].isString() == false)
        {
            resp.jsonData["reason"] = "Action is add but no name passed";
            return resp;
        }
        if(jsPatch.isMember("type") == false || jsPatch["type"].isString() == false)
        {
            resp.jsonData["reason"] = "Action is add but no type passed";
            return resp;
        }
        if((jsPatch["type"].asString() == "SDP" || jsPatch["type"].asString() == "sdp") && (jsPatch.isMember("SDP") == false || jsPatch["SDP"].isString() == false))
        {
            resp.jsonData["reason"] = "Action is add and type SDP but no SDP passed";
            return resp;
        }
        if((jsPatch["type"].asString() == "RTSP" || jsPatch["type"].asString() == "rtsp") &&
           (jsPatch.isMember("details") == false || jsPatch["details"].isString() == false))
        {
            resp.jsonData["reason"] = "Action is add and type RTSP but no RTSP details passed";
            return resp;
        }
    }
    resp.nHttpCode = 200;
    resp.jsonData["reason"] = "Success";
    return resp;
}

pml::restgoose::response RemoteApi::DoPatchAoip(const Json::Value& jsArray)
{
    pml::restgoose::response resp(200);

    for(Json::ArrayIndex ai = 0; ai < jsArray.size(); ai++)
    {
        if(jsArray[ai]["action"].asString() == "add")
        {
            resp.jsonData.append(DoPatchAoipAdd(jsArray[ai]).jsonData);
        }
        else if(jsArray[ai]["action"].asString() == "update")
        {
            resp.jsonData.append(DoPatchAoipUpdate(jsArray[ai]).jsonData);
        }
        else if(jsArray[ai]["action"].asString() == "delete")
        {
            resp.jsonData.append(DoPatchAoipDelete(jsArray[ai]).jsonData);
        }
    }
    return resp;
}

pml::restgoose::response RemoteApi::DoPatchAoipAdd(const Json::Value& jsSource)
{
    bool bSuccess(false);
    if(jsSource["type"].asString() == "SDP" || jsSource["type"].asString() == "sdp")
    {
        bSuccess = AoipSourceManager::Get().AddSource(jsSource["name"].asString(), "SDP", jsSource["SDP"].asString());
    }
    else if(jsSource["type"].asString() == "RTSP" || jsSource["type"].asString() == "rtsp")
    {
        bSuccess = AoipSourceManager::Get().AddSource(jsSource["name"].asString(), jsSource["details"].asString());
    }
    return bSuccess ? pml::restgoose::response(200, jsSource["name"].asString()+" added") : pml::restgoose::response(400);

}

pml::restgoose::response RemoteApi::DoPatchAoipUpdate(const Json::Value& jsPatch)
{
    auto source = AoipSourceManager::Get().FindSource(jsPatch["index"].asInt());
    if(source.nIndex == 0)
    {
        return pml::restgoose::response(404, "Source not found");
    }
    if(jsPatch.isMember("name"))
    {
        AoipSourceManager::Get().SetSourceName(jsPatch["index"].asInt(), jsPatch["name"].asString());
    }
    if(jsPatch.isMember("details"))
    {
        AoipSourceManager::Get().SetSourceDetails(jsPatch["index"].asInt(), jsPatch["details"].asString());
    }
    if(jsPatch.isMember("SDP"))
    {
        AoipSourceManager::Get().SetSourceSDP(jsPatch["index"].asInt(), jsPatch["SDP"].asString());
    }
    if(jsPatch.isMember("type"))
    {
        AoipSourceManager::Get().SetSourceType(jsPatch["index"].asInt(), jsPatch["type"].asString());
    }
    if(jsPatch.isMember("tags"))
    {
        std::set<wxString> setTags;
        for(Json::ArrayIndex ai = 0; ai < jsPatch["tags"].size(); ai++)
        {
            setTags.insert(jsPatch["tags"][ai].asString());
        }
        AoipSourceManager::Get().SetSourceTags(jsPatch["index"].asInt(), setTags);
    }
    return pml::restgoose::response(200, jsPatch["index"].asString()+" updated");
}

pml::restgoose::response RemoteApi::DoPatchAoipDelete(const Json::Value& jsPatch)
{
    auto source = AoipSourceManager::Get().FindSource(jsPatch["index"].asInt());
    if(source.nIndex == 0)
    {
        return pml::restgoose::response(404, "Source not found");
    }
    AoipSourceManager::Get().DeleteSource(jsPatch["index"].asInt());
    return pml::restgoose::response(200, jsPatch["index"].asString()+" deleted");
}




void RemoteApi::SendPluginWebsocketMessage(const Json::Value& jsMessage)
{
    m_Server.SendWebsocketMessage({endpoint("/x-pam/ws/plugin")}, jsMessage);
}

void RemoteApi::SendSettingWebsocketMessage(const Json::Value& jsMessage)
{
    m_Server.SendWebsocketMessage({endpoint("/x-pam/ws/setting")}, jsMessage);
}

void RemoteApi::RegisterRemoteApiCSV(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(setEnum, sDefault, true)));
}

void RemoteApi::RegisterRemoteApiEnum(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(setEnum, sDefault)));
}

void RemoteApi::RegisterRemoteApiRangeDouble(const wxString& sSection, const wxString& sKey, const std::pair<double, double>& dRange, double dDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(dRange, dDefault)));
}

void RemoteApi::RegisterRemoteApiEnum(const wxString& sSection, const wxString& sKey, const std::map<int, wxString>& mEnum, int nDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(mEnum, nDefault)));
}

void RemoteApi::RegisterRemoteApiRangeInt(const wxString& sSection, const wxString& sKey, const std::pair<int, int>& nRange, int nDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(nRange, nDefault)));
}

void RemoteApi::RegisterRemoteApiCallback(const wxString& sSection, const wxString& sKey, std::function<std::set<wxString>()> func, const wxString& sDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(func, sDefault)));
}

void RemoteApi::RegisterRemoteApiCallback(const wxString& sSection, const wxString& sKey, std::function<std::map<int, wxString>()> func, int nDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(func, nDefault)));
}

void RemoteApi::RegisterRemoteApi(const wxString& sSection, const wxString& sKey, const wxString& sDefault)
{
    auto itSection = m_mSettings.insert(std::make_pair(sSection, section())).first;
    itSection->second.mKeys.insert(std::make_pair(sKey, section::constraint(sDefault)));
}

bool RemoteApi::WebsocketsActive()
{
    return m_bWebsocketsActive;
}



