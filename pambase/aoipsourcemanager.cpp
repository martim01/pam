#include "aoipsourcemanager.h"
#include "settings.h"
#include "wmlogevent.h"
#include <wx/log.h>


AoipSourceManager::AoipSourceManager()
{
    LoadSources();
}

AoipSourceManager& AoipSourceManager::Get()
{
    static AoipSourceManager ai;
    return ai;
}


bool AoipSourceManager::LoadSources()
{
    wxLogNull ln;
    wxXmlDocument xmlDoc;
    if(xmlDoc.Load(wxString::Format("%s/aoipsources.xml", Settings::Get().GetDocumentDirectory().c_str())) && xmlDoc.GetRoot())
    {
        for(wxXmlNode* pNode = xmlDoc.GetRoot()->GetChildren(); pNode; pNode = pNode->GetNext())
        {
            unsigned long nIndex;
            if(pNode->GetName().CmpNoCase("source") == 0 && pNode->GetAttribute("index").ToULong(&nIndex))
            {
                auto insertSource = m_mSources.insert(std::make_pair(nIndex, AoIPSource(nIndex)));
                if(insertSource.second == false)
                {
                    wmLog::Get()->Log(wxString::Format("AoIP Source Manager: Duplicate source index %d", nIndex));
                }
                else
                {
                    for(wxXmlNode* pSourceNode = pNode->GetChildren(); pSourceNode; pSourceNode = pSourceNode->GetNext())
                    {
                        if(pSourceNode->GetName().CmpNoCase("name") == 0)
                        {
                            insertSource.first->second.sName = pSourceNode->GetNodeContent();
                        }
                        else if(pSourceNode->GetName().CmpNoCase("details") == 0)
                        {
                            insertSource.first->second.sDetails = pSourceNode->GetNodeContent();
                        }
                        else if(pSourceNode->GetName().CmpNoCase("sdp") == 0)
                        {
                            insertSource.first->second.sSDP = pSourceNode->GetNodeContent();
                        }
                        else if(pSourceNode->GetName().CmpNoCase("type") == 0)
                        {
                            insertSource.first->second.sType = pSourceNode->GetNodeContent();
                        }
                        if(pSourceNode->GetName().CmpNoCase("tags") == 0)
                        {
                            for(wxXmlNode* pTagNode = pSourceNode->GetChildren(); pTagNode; pTagNode = pTagNode->GetNext())
                            {
                                insertSource.first->second.setTags.insert(pTagNode->GetNodeContent());
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}


std::map<unsigned int, AoIPSource>::const_iterator AoipSourceManager::GetSourceBegin() const
{
    return m_mSources.begin();
}

std::map<unsigned int, AoIPSource>::const_iterator AoipSourceManager::GetSourceEnd() const
{
    return m_mSources.end();
}

AoIPSource AoipSourceManager::FindSource(unsigned int nIndex) const
{
    auto itSource = m_mSources.find(nIndex);
    if(itSource != m_mSources.end())
    {
        return itSource->second;
    }
    else
    {
        return AoIPSource(0);
    }
}

AoIPSource AoipSourceManager::FindSource(const wxString& sName) const
{
    for(auto pairSource : m_mSources)
    {
        if(pairSource.second.sName.CmpNoCase(sName) == 0)
        {
            return pairSource.second;
        }
    }
    return AoIPSource(0);
}

bool AoipSourceManager::AddSource(const wxString& sName, const wxString& sDetails, const wxString& sSDP, unsigned int nIndex)
{
    if(nIndex == 0)
    {
        nIndex = GenerateIndex();
    }
    if(m_mSources.insert(std::make_pair(nIndex, AoIPSource(nIndex, sName, sDetails, sSDP))).second)
    {
        return SaveSources();
    }
    else
    {
        return false;
    }
}

unsigned int AoipSourceManager::GenerateIndex()
{
    if(m_mSources.empty() == false)
    {
        auto itEnd = m_mSources.end();
        --itEnd;
        return itEnd->first+1;
    }
    else
    {
        return 1;
    }
}

bool AoipSourceManager::EditSource(unsigned int nIndex, const wxString& sName, const wxString& sDetails)
{
    auto itSource = m_mSources.find(nIndex);
    if(itSource == m_mSources.end())
    {
        return false;
    }
    itSource->second.sName = sName;
    itSource->second.sDetails = sDetails;
    return SaveSources();
}

bool AoipSourceManager::SetSourceTags(unsigned int nIndex, const std::set<wxString>& setTags)
{
    auto itSource = m_mSources.find(nIndex);
    if(itSource == m_mSources.end())
    {
        return false;
    }
    itSource->second.setTags = setTags;
    return SaveSources();
}


bool AoipSourceManager::SaveSources()
{
    wxXmlDocument xmlDoc;
    xmlDoc.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, "pam"));
    for(auto pairSource : m_mSources)
    {
        wxXmlNode* pSource = new wxXmlNode(wxXML_ELEMENT_NODE, "source");
        pSource->AddAttribute("index", wxString::Format("%u", pairSource.first));
        pSource->AddChild(NewTextNode("name", pairSource.second.sName));
        pSource->AddChild(NewTextNode("details", pairSource.second.sDetails));
        pSource->AddChild(NewTextNode("sdp", pairSource.second.sSDP, wxXML_CDATA_SECTION_NODE));

        if(pairSource.second.setTags.empty() == false)
        {
            wxXmlNode* pTags = new wxXmlNode(wxXML_ELEMENT_NODE, "tags");
            for(auto tag : pairSource.second.setTags)
            {
                pTags->AddChild(NewTextNode("tag", tag));
            }
            pSource->AddChild(pTags);
        }
        xmlDoc.GetRoot()->AddChild(pSource);
    }
    return xmlDoc.Save(wxString::Format("%s/aoipsources.xml", Settings::Get().GetDocumentDirectory().c_str()));
}


wxXmlNode* AoipSourceManager::NewTextNode(const wxString& sKey, const wxString& sValue, wxXmlNodeType type)
{
    wxXmlNode* pKey = new wxXmlNode(wxXML_ELEMENT_NODE, sKey);
    wxXmlNode* pValue = new wxXmlNode(type, "", sValue);
    pKey->AddChild(pValue);
    return pKey;
}


void AoipSourceManager::DeleteSource(unsigned int nIndex)
{
    m_mSources.erase(nIndex);
    SaveSources();
}

void AoipSourceManager::DeleteSource(const wxString& sName)
{
    for(auto pairSource : m_mSources)
    {
        if(pairSource.second.sName.CmpNoCase(sName) == 0)
        {
            m_mSources.erase(pairSource.first);
            SaveSources();
            break;
        }
    }
}

void AoipSourceManager::DeleteAllSources()
{
    m_mSources.clear();
    SaveSources();
}
