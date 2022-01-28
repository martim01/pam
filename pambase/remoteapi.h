#pragma once
#include "RestGoose.h"
#include <functional>
#include <set>

class RemoteApi
{
    public:
        static RemoteApi& Get();

        void Run();

        bool AddPluginWebsocketEndpoint(const endpoint& theEndpoint);
        bool AddPluginEndpoint(const httpMethod& method, const endpoint& theEndpoint, std::function<pml::restgoose::response(const query&, const std::vector<pml::restgoose::partData>&, const endpoint&, const userName&)> func);

    protected:
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

    private:
        RemoteApi();
        ~RemoteApi();

        pml::restgoose::Server m_Server;

        std::set<endpoint> m_setWS;
};
