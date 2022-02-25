#pragma once
#include <wx/string.h>
#include <map>
#include <set>
#include <wx/xml/xml.h>
#include "dlldefine.h"
#include <wx/event.h>

#include <memory>

struct PAMBASE_IMPEXPORT AoIPSource
{
    AoIPSource(int nI,const wxString& sN=wxEmptyString, const wxString& sD=wxEmptyString, const wxString& sdp=wxEmptyString) :
        nIndex(nI),
        sName(sN),
        sDetails(sD),
        sType(sD.BeforeFirst(':')),
        sSDP(sdp)
        {
            if(sType.empty())
            {
                sType = "SDP";
            }

        }
    int nIndex;
    wxString sName;
    wxString sDetails;
    wxString sType;
    wxString sSDP;
    std::set<wxString> setTags;
};


class wxZCPoster;

namespace pml
{
    namespace dnssd
    {
        class Browser;
    };

    class SapServer;
};


class PAMBASE_IMPEXPORT AoipSourceManager : public wxEvtHandler
{
    public:
        static AoipSourceManager& Get();

        bool LoadSources();
        bool SaveSources();

        const std::map<int, AoIPSource>& GetSources() const { return m_mSources;}
        std::map<int, wxString> GetSourceNames();

        AoIPSource FindSource(int nIndex) const;
        AoIPSource FindSource(const wxString& sName) const;

        bool AddSource(const wxString& sName, const wxString& sDetails, const wxString& sSDP=wxEmptyString, int nIndex = 0);
        bool EditSource(int nIndex, const wxString& sName, const wxString& sDetails);
        bool SetSourceName(int nIndex, const wxString& sName);
        bool SetSourceDetails(int nIndex, const wxString& sDetails);
        bool SetSourceSDP(int nIndex, const wxString& sSDP);
        bool SetSourceTags(int nIndex, const std::set<wxString>& setTags);
        bool SetSourceType(int nIndex, const wxString& sType);

        void DeleteSource(int nIndex);
        void DeleteSource(const wxString& sName);
        void DeleteAllSources();

        void StartDiscovery(wxEvtHandler* pHandler, const std::set<std::string>& setServices, std::set<std::string>& setSAP);
        void StopDiscovery();

        enum {SOURCE_MANUAL_A=-1, SOURCE_MANUAL_B = -2, SOURCE_NMOS_A=-3, SOURCE_NMOS_B=-4};

    private:
        AoipSourceManager();
        ~AoipSourceManager();
        void OnDiscovery(wxCommandEvent& event);
        void OnDiscoveryFinished(wxCommandEvent& event);
        void OnSap(wxCommandEvent& event);

        int GenerateIndex();

        wxXmlNode* NewTextNode(const wxString& sKey, const wxString& sValue,wxXmlNodeType type=wxXML_TEXT_NODE);

        std::map<int, AoIPSource> m_mSources;


        //discovery
        wxEvtHandler* m_pDiscoveryHandler;
        std::unique_ptr<pml::dnssd::Browser> m_pBrowser;
        std::shared_ptr<wxZCPoster> m_pPoster;

        std::unique_ptr<pml::SapServer> m_pSapWatcher;

        enum {LOCAL=0, ORGANISATION, GLOBAL};


        unsigned long m_nDiscovered;
        std::set<std::pair<wxString, wxString> > m_setDiscover;

};

wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ASM_DISCOVERY, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ASM_DISCOVERY_FINISHED, wxCommandEvent);
