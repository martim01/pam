#pragma once
#include "RestGoose.h"
#include <functional>

class RemoteApi
{
    public:
        static RemoteApi& Get();

        void Run();

    protected:
        //bool WSAuthenticate(const endpoint& theEndpoint, const userName& theUser, const ipAddress& thePeer);
        //bool WSMessage(const endpoint& theEndpoint, const Json::Value& theMessage);
        //void WSClose(const endpoint& theEndpoint, const ipAddress& thePeer);


        pml::restgoose::response GetRoot(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response PatchSettings(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

        pml::restgoose::response GetPlugins(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetPluginsMonitor(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetPluginsTest(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);
        pml::restgoose::response GetPluginsGenerator(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

    private:
        RemoteApi();
        ~RemoteApi();

        pml::restgoose::Server m_Server;
};
