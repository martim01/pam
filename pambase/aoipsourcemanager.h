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
    AoIPSource(unsigned int nI,const wxString& sN=wxEmptyString, const wxString& sD=wxEmptyString, const wxString& sdp=wxEmptyString) :
        nIndex(nI),
        sName(sN),
        sDetails(sD),
        sSDP(sdp)
        {}
    unsigned int nIndex;
    wxString sName;
    wxString sType;
    wxString sDetails;
    wxString sSDP;
    std::set<wxString> setTags;
};


class wxZCPoster;

namespace pml
{
    class Browser;
};

namespace sapserver
{
    class SapServer;
};


class PAMBASE_IMPEXPORT AoipSourceManager : public wxEvtHandler
{
    public:
        static AoipSourceManager& Get();

        bool LoadSources();
        bool SaveSources();

        std::map<unsigned int, AoIPSource>::const_iterator GetSourceBegin() const;
        std::map<unsigned int, AoIPSource>::const_iterator GetSourceEnd() const;
        AoIPSource FindSource(unsigned int nIndex) const;
        AoIPSource FindSource(const wxString& sName) const;

        bool AddSource(const wxString& sName, const wxString& sDetails, const wxString& sSDP=wxEmptyString, unsigned int nIndex = 0);
        bool EditSource(unsigned int nIndex, const wxString& sName, const wxString& sDetails);
        bool SetSourceTags(unsigned int nIndex, const std::set<wxString>& setTags);

        void DeleteSource(unsigned int nIndex);
        void DeleteSource(const wxString& sName);
        void DeleteAllSources();

        void StartDiscovery(wxEvtHandler* pHandler, const std::set<std::string>& setServices, std::set<std::string>& setSAP);
        void StopDiscovery();


    private:
        AoipSourceManager();
        ~AoipSourceManager();
        void OnDiscovery(wxCommandEvent& event);
        void OnDiscoveryFinished(wxCommandEvent& event);
        void OnSap(wxCommandEvent& event);

        unsigned int GenerateIndex();

        wxXmlNode* NewTextNode(const wxString& sKey, const wxString& sValue,wxXmlNodeType type=wxXML_TEXT_NODE);

        std::map<unsigned int, AoIPSource> m_mSources;


        //discovery
        wxEvtHandler* m_pDiscoveryHandler;
        std::unique_ptr<pml::Browser> m_pBrowser;
        std::shared_ptr<wxZCPoster> m_pPoster;

        std::unique_ptr<sapserver::SapServer> m_pSapWatcher;

        enum {LOCAL=0, ORGANISATION, GLOBAL};


        unsigned long m_nDiscovered;
        std::set<std::pair<wxString, wxString> > m_setDiscover;

};

wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ASM_DISCOVERY, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ASM_DISCOVERY_FINISHED, wxCommandEvent);
