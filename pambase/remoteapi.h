#pragma once
#include "RestGoose.h"
#include <functional>
#include <set>
#include <map>
#include <wx/string.h>

class RemoteApi
{
    public:
        static RemoteApi& Get();

        void Run();



    protected:
        friend class MonitorPluginBuilder;
        friend class TestPluginBuilder;
        friend class GeneratorPluginBuilder;
        friend class pam2Dialog;

        bool WSAuthenticate(const endpoint& theEndpoint, const userName& theUser, const ipAddress& thePeer);
        bool WSMessage(const endpoint& theEndpoint, const Json::Value& theMessage);
        void WSClose(const endpoint& theEndpoint, const ipAddress& thePeer);

        pml::restgoose::response NotFound(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

        pml::restgoose::response GetRoot(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response PatchSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

        pml::restgoose::response GetPlugins(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetPluginsMonitor(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetPluginsTest(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetPluginsGenerator(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);


        pml::restgoose::response GetWavFiles(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response PostWavFile(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response DeleteWavFile(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

        pml::restgoose::response GetAoipSources(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response PatchAoipSources(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);


        void RegisterRemoteApiEnum(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum);
        void RegisterRemoteApiEnum(const wxString& sSection, const wxString& sKey, const std::map<int, wxString>& setEnum);
        void RegisterRemoteApiRangeDouble(const wxString& sSection, const wxString& sKey, const std::pair<double, double>& dRange);
        void RegisterRemoteApiRangeInt(const wxString& sSection, const wxString& sKey, const std::pair<int, int>& dRange);
        void RegisterRemoteApiCallback(const wxString& sSection, const wxString& sKey, std::function<std::set<wxString>()> func);
        void RegisterRemoteApiCallback(const wxString& sSection, const wxString& sKey, std::function<std::map<int, wxString>()> func);
        void RegisterRemoteApi(const wxString& sSection, const wxString& sKey);



        bool AddPluginEndpoint(const httpMethod& method, const endpoint& theEndpoint, std::function<pml::restgoose::response(const query&, const std::vector<pml::restgoose::partData>&, const endpoint&, const userName&)> func);
        void SendPluginWebsocketMessage(const Json::Value& jsMessage);

        RemoteApi();
        ~RemoteApi();

        bool WebsocketsActive();


        pml::restgoose::Server m_Server;


        Json::Value GetJson(const std::set<wxString>& setEnum);
        Json::Value GetJson(const std::map<int,wxString>& mEnum);
        Json::Value GetJson(std::function<std::set<wxString>()> func);
        Json::Value GetJson(std::function<std::map<int, wxString>()> func);

        struct section
        {
            struct constraint
            {
                enum enumType{FREE, STRING_ENUM, INT_ENUM, INT_RANGE, DOUBLE_RANGE, STRING_CALLBACK, INT_CALLBACK};

                constraint() : dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::FREE){}
                constraint(const std::set<wxString>& setE) : setEnum(setE), dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::STRING_ENUM){}
                constraint(const std::map<int, wxString>& mE) : mEnumNum(mE), dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::INT_ENUM){}
                constraint(const std::pair<double, double>& dR) : dRange(dR), nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::DOUBLE_RANGE){}
                constraint(const std::pair<int, int>& nR) : dRange{0.0,0.0}, nRange(nR), funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::INT_RANGE){}
                constraint(std::function<std::set<wxString>()> func) : dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(func), funcIntEnum(nullptr), eType(enumType::STRING_CALLBACK){}
                constraint(std::function<std::map<int, wxString>()> func) : dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(func), eType(enumType::INT_CALLBACK){}

                std::set<wxString> setEnum;
                std::map<int, wxString> mEnumNum;
                std::pair<double,double> dRange;
                std::pair<int,int> nRange;

                std::function<std::set<wxString>()> funcStringEnum;
                std::function<std::map<int, wxString>()> funcIntEnum;


                enumType eType;
            };
            std::map<wxString, constraint> mKeys;
        };

        pml::restgoose::response CheckJsonSettingPatch(const Json::Value& jsPatch);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const RemoteApi::section::constraint& aConstraint);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::set<wxString>& setEnum);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::map<int, wxString>& mEnum);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::pair<double, double>& dRange);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::pair<int, int>& dRange);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, std::function<std::set<wxString>()> func);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, std::function<std::map<int, wxString>()> func);

        void DoPatchSettings(const Json::Value& jsArray);

        std::map<wxString, section> m_mSettings;
        bool m_bWebsocketsActive;

};
