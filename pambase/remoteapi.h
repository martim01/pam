#pragma once
#include "RestGoose.h"
#include <functional>
#include <set>
#include <map>
#include <wx/string.h>
#include <wx/event.h>
class SettingEvent;
class wxFileName;

class RemoteApi : public wxEvtHandler
{
    public:
        static RemoteApi& Get();


    protected:
        friend class MonitorPluginBuilder;
        friend class TestPluginBuilder;
        friend class GeneratorPluginBuilder;
        friend class pam2Dialog;
        friend class Settings;

        bool WSAuthenticate(const endpoint& theEndpoint,const query& theQuery, const userName& theUser, const ipAddress& thePeer);
        bool WSMessage(const endpoint& theEndpoint, const Json::Value& theMessage);
        void WSClose(const endpoint& theEndpoint, const ipAddress& thePeer);

        pml::restgoose::response ExtraEndpoint(const httpMethod& theMethod, const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

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


        void RegisterRemoteApiEnum(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault);
        void RegisterRemoteApiEnum(const wxString& sSection, const wxString& sKey, const std::map<int, wxString>& setEnum, int nDefault);
        void RegisterRemoteApiRangeDouble(const wxString& sSection, const wxString& sKey, const std::pair<double, double>& dRange, double dDefault);
        void RegisterRemoteApiRangeInt(const wxString& sSection, const wxString& sKey, const std::pair<int, int>& nRange, int nDefault);
        void RegisterRemoteApiCallback(const wxString& sSection, const wxString& sKey, std::function<std::set<wxString>()> func, const wxString& sDefault);
        void RegisterRemoteApiCallback(const wxString& sSection, const wxString& sKey, std::function<std::map<int, wxString>()> func, int nDefault);
        void RegisterRemoteApi(const wxString& sSection, const wxString& sKey, const wxString& sDefault);
        void RegisterRemoteApiCSV(const wxString& sSection, const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault);



        bool AddPluginEndpoint(const httpMethod& method, const endpoint& theEndpoint, std::function<pml::restgoose::response(const query&, const std::vector<pml::restgoose::partData>&, const endpoint&, const userName&)> func);
        void SendPluginWebsocketMessage(const Json::Value& jsMessage);
        void SendSettingWebsocketMessage(const Json::Value& jsMessage);

        RemoteApi();
        ~RemoteApi();

        bool WebsocketsActive();
        void OnSettingEvent(SettingEvent& event);

        pml::restgoose::Server m_Server;

        Json::Value GetSectionJson(const wxString& sSection, const wxString& sKeyFilter);


        Json::Value GetJson(const std::set<wxString>& setEnum);
        Json::Value GetJson(const std::map<int,wxString>& mEnum);
        Json::Value GetJson(std::function<std::set<wxString>()> func);
        Json::Value GetJson(std::function<std::map<int, wxString>()> func);

        pml::restgoose::response WavEndpoint(const httpMethod& theMethod, const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

        pml::restgoose::response DownloadWav(const wxFileName& fnWav);
        pml::restgoose::response DeleteWav(const wxFileName& fnWav);

        struct section
        {
            struct constraint
            {
                enum enumType{FREE, STRING_ENUM, INT_ENUM, INT_RANGE, DOUBLE_RANGE, STRING_CALLBACK, INT_CALLBACK, CSV};

                constraint(const wxString& sDefault) : dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::FREE){initial.s = sDefault;}
                constraint(const std::set<wxString>& setE,const wxString& sDefault) : setEnum(setE), dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::STRING_ENUM){initial.s = sDefault;}
                constraint(const std::set<wxString>& setE,const wxString& sDefault, bool bPlaceholder) : setEnum(setE), dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::CSV){initial.s = sDefault;}
                constraint(const std::map<int, wxString>& mE, int nDefault) : mEnumNum(mE), dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::INT_ENUM){initial.n = nDefault;}
                constraint(const std::pair<double, double>& dR, double dDefault) : dRange(dR), nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::DOUBLE_RANGE){initial.d = dDefault;}
                constraint(const std::pair<int, int>& nR, int nDefault) : dRange{0.0,0.0}, nRange(nR), funcStringEnum(nullptr), funcIntEnum(nullptr), eType(enumType::INT_RANGE){initial.n = nDefault;}
                constraint(std::function<std::set<wxString>()> func,const wxString& sDefault) : dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(func), funcIntEnum(nullptr), eType(enumType::STRING_CALLBACK){initial.s = sDefault;}
                constraint(std::function<std::map<int, wxString>()> func, int nDefault) : dRange{0.0,0.0}, nRange{0,0}, funcStringEnum(nullptr), funcIntEnum(func), eType(enumType::INT_CALLBACK){initial.n = nDefault;}

                std::set<wxString> setEnum;
                std::map<int, wxString> mEnumNum;
                std::pair<double,double> dRange;
                std::pair<int,int> nRange;

                std::function<std::set<wxString>()> funcStringEnum;
                std::function<std::map<int, wxString>()> funcIntEnum;

                enumType eType;

                struct def
                {
                    double d;
                    int n;
                    wxString s;
                };
                def initial;
            };
            std::map<wxString, constraint> mKeys;
        };

        Json::Value GetSectionJson(const wxString& sSectionName, const section& aSection, const wxString& sKeyFilter);

        pml::restgoose::response CheckJsonSettingPatch(const Json::Value& jsPatch, wxString sSection);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const RemoteApi::section::constraint& aConstraint);
        pml::restgoose::response CheckJsonSettingPatchConstraintCSV(const Json::Value& jsPatch, const std::set<wxString>& setEnum);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::set<wxString>& setEnum);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::map<int, wxString>& mEnum);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::pair<double, double>& dRange);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, const std::pair<int, int>& dRange);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, std::function<std::set<wxString>()> func);
        pml::restgoose::response CheckJsonSettingPatchConstraint(const Json::Value& jsPatch, std::function<std::map<int, wxString>()> func);

        pml::restgoose::response DoPatchSettings(const std::vector<pml::restgoose::partData>& vData, const wxString& sSection="");
        void DoPatchSettings(const Json::Value& jsArray, const wxString& sSection);


        pml::restgoose::response CheckJsonAoipPatch(const Json::Value& jsPatch);
        pml::restgoose::response DoPatchAoip(const Json::Value& jsArray);
        pml::restgoose::response DoPatchAoipAdd(const Json::Value& jsPatch);
        pml::restgoose::response DoPatchAoipUpdate(const Json::Value& jsPatch);
        pml::restgoose::response DoPatchAoipDelete(const Json::Value& jsPatch);

        std::map<wxString, section> m_mSettings;
        bool m_bWebsocketsActive;

};
