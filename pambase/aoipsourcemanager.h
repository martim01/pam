#pragma once
#include <wx/string.h>
#include <map>
#include <set>
#include <wx/xml/xml.h>
#include "dlldefine.h"

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



class PAMBASE_IMPEXPORT AoipSourceManager
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

    private:
        AoipSourceManager();

        unsigned int GenerateIndex();

        wxXmlNode* NewTextNode(const wxString& sKey, const wxString& sValue,wxXmlNodeType type=wxXML_TEXT_NODE);

        std::map<unsigned int, AoIPSource> m_mSources;
};
