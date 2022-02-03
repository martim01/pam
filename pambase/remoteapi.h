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


        void RegisterRemoteApiStringEnum(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum);
        void RegisterRemoteApiIntEnum(const wxString& sSection, const wxString& sKey, const std::set<int>& setEnum);
        void RegisterRemoteApiRange(const wxString& sSection, const wxString& sKey, const std::pair<double, double>& dRange);



        bool AddPluginEndpoint(const httpMethod& method, const endpoint& theEndpoint, std::function<pml::restgoose::response(const query&, const std::vector<pml::restgoose::partData>&, const endpoint&, const userName&)> func);
        void SendPluginWebsocketMessage(const Json::Value& jsMessage);

        RemoteApi();
        ~RemoteApi();

        pml::restgoose::Server m_Server;


        struct section
        {
            struct constraint
            {
                constraint(const std::set<wxString>& setE) : setEnum(setE), dRange{0.0,0.0}{}
                constraint(const std::set<int>& setE) : setEnumNum(setE), dRange{0.0,0.0}{}
                constraint(const std::pair<double, double>& dR) : dRange(dR){}
                std::set<wxString> setEnum;
                std::set<int> setEnumNum;
                std::pair<double,double> dRange;

            };
            std::map<wxString, constraint> mKeys;
        };

        pml::restgoose::response CheckJsonSettingPatch(const Json::Value& jsPatch);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const RemoteApi::section::constraint& aConstraint);
        void DoPatchSettings(const Json::Value& jsArray);

        std::map<wxString, section> m_mSettings;

};
