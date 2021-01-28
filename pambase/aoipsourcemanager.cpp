#include "aoipsourcemanager.h"
#include "settings.h"
#include "log.h"
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "dnssd.h"
#include "wxzxposter.h"
#include "mdns.h"
#include "sapserver.h"
#include "wxsaphandler.h"
#include <iostream>


wxDEFINE_EVENT(wxEVT_ASM_DISCOVERY, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ASM_DISCOVERY_FINISHED, wxCommandEvent);

AoipSourceManager::AoipSourceManager() :
    m_pBrowser(nullptr),
    m_pPoster(nullptr),
    m_pSapWatcher(nullptr)
{

    LoadSources();


	Connect(wxID_ANY, wxEVT_ZC_RESOLVED, (wxObjectEventFunction)&AoipSourceManager::OnDiscovery);
	Connect(wxID_ANY, wxEVT_ZC_FINISHED, (wxObjectEventFunction)&AoipSourceManager::OnDiscoveryFinished);
	Connect(wxID_ANY, wxEVT_SAP, (wxObjectEventFunction)&AoipSourceManager::OnSap);

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
                    pml::Log::Get(pml::Log::LOG_WARN) << "AoIP Source Manager\tDuplicate source index: " << nIndex << std::endl;
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
        pml::Log::Get(pml::Log::LOG_INFO) << "AoIP Source Manager\tLoaded " << m_mSources.size() << "AES67 sources" << std::endl;
        return true;
    }
    pml::Log::Get(pml::Log::LOG_WARN) << "AoIP Source Manager\tUnable to load aoipsources.xml" << std::endl;
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


void AoipSourceManager::OnDiscovery(wxCommandEvent& event)
{
    std::shared_ptr<pml::dnsInstance> pInstance = m_pPoster->GetInstance(event.GetInt());
    if(pInstance)
    {

        wxString sIdentifier;
        if(pInstance->sService == "_rtsp._tcp")
        {
            sIdentifier = (wxString(pInstance->sName).BeforeFirst('(')+" {"+wxString(pInstance->sService).AfterFirst('_').BeforeFirst('.')+"}");
        }
        else if(pInstance->sService == "_sipuri._udp")
        {
            sIdentifier = (wxString(pInstance->sName).AfterFirst(' ').BeforeFirst('(')+" {"+wxString(pInstance->sService).AfterFirst('_').BeforeFirst('.')+"}");
        }


        if(m_setDiscover.insert(std::make_pair(sIdentifier, wxString(pInstance->sHostIP))).second)
        {
            wxString sAddress;
            if(pInstance->sService == "_rtsp._tcp")
            {
                if(pInstance->nPort == 554)
                {
                    sAddress = (wxString::Format("rtsp://%s/by-name/%s", wxString(pInstance->sHostIP).c_str(), wxString(pInstance->sName).c_str()));
                }
                else
                {
                    sAddress = (wxString::Format("rtsp://%s:%d/by-name/%s", wxString(pInstance->sHostIP).c_str(), pInstance->nPort, wxString(pInstance->sName).c_str()));
                }
                AddSource(wxString::Format("%s(%s)", sIdentifier.c_str(), wxString(pInstance->sHostIP).c_str()), sAddress);
            }
            else if(pInstance->sService == "_sipuri._udp")
            {
                sAddress = wxString(pInstance->sName).BeforeFirst(' ');
                AddSource(wxString::Format("%s(%s)", sIdentifier.c_str(),wxString(pInstance->sHostIP).c_str()), sAddress);
            }

            wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ASM_DISCOVERY);
            pEvent->SetString(wxString::Format("[%s] %s = %s:%lu", wxString(pInstance->sService).c_str(), wxString(pInstance->sName).c_str(), wxString(pInstance->sHostIP).c_str(), pInstance->nPort));
            pEvent->SetInt(1);
            wxQueueEvent(m_pDiscoveryHandler, pEvent);

        }
    }
}


void AoipSourceManager::OnSap(wxCommandEvent& event)
{
    wxString sIpAddress = event.GetString().BeforeFirst('\n');
    wxString sSDP = event.GetString().AfterFirst('\n');
    wxString sName;



    //is it an L24 or L16 session
    bool bCanDecode(false);
    wxArrayString asLines(wxStringTokenize(sSDP, "\n"));
    for(size_t i = 0; i < asLines.size(); i++)
    {
        if(asLines[i].find("a=rtpmap:") != wxNOT_FOUND)
        {
            unsigned long nCodec;
            if(asLines[i].AfterFirst(':').BeforeFirst(' ').ToULong(&nCodec) && nCodec > 95 && nCodec < 127)
            {
                if(asLines[i].find("L24") != wxNOT_FOUND || asLines[i].find("L16") != wxNOT_FOUND)
                {
                    bCanDecode = true;
                    break;
                }
            }
        }
    }
    if(bCanDecode)
    {
        //find the source name:
        int nStart = sSDP.Find("s=");
        if(nStart != wxNOT_FOUND)
        {
            sName = sSDP.Mid(nStart+2).BeforeFirst('\n');
        }
        nStart = sSDP.Find("o=");
        if(nStart != wxNOT_FOUND)
        {
            wxArrayString asSplit(wxStringTokenize(sSDP.Mid(nStart+2).BeforeFirst('\n')));
            if(asSplit.size() >= 6)
            {
                sIpAddress = asSplit[5];
            }
        }

        if(event.GetInt())
        {
            if(m_setDiscover.insert(std::make_pair(sName, sIpAddress)).second)
            {
                m_nDiscovered++;
                pml::Log::Get() << "AoIP Source Manager\tDiscovery: SAP response from " << sIpAddress.c_str() << std::endl;
                pml::Log::Get() << "AoIP Source Manager\tDiscovery: SDP=" << sSDP << std::endl;

                AddSource(sName, wxString::Format("sap:%s", sIpAddress.c_str()), sSDP);

                wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ASM_DISCOVERY);
                pEvent->SetString(wxString::Format("[SAP] %s = %s", sName.c_str(), sIpAddress.c_str()));
                pEvent->SetInt(1);
                wxQueueEvent(m_pDiscoveryHandler, pEvent);
            }
        }
        else if(m_setDiscover.erase(std::make_pair(sName, sIpAddress)) > 0)
        {
            wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ASM_DISCOVERY);
            pEvent->SetString(wxString::Format("[SAP] %s:%s = REMOVED", sName.c_str(), sIpAddress.c_str()));
            pEvent->SetInt(-1);
            wxQueueEvent(m_pDiscoveryHandler, pEvent);

            DeleteSource(sName);

        }
    }
}



void AoipSourceManager::StartDiscovery(wxEvtHandler* pHandler, const std::set<std::string>& setServices, std::set<std::string>& setSAP)
{
    m_pDiscoveryHandler = pHandler;

    if(setServices.empty() == false)
    {
        m_pBrowser = std::unique_ptr<pml::Browser>(new pml::Browser());
        m_pPoster = std::make_shared<wxZCPoster>(this);
        for(auto service : setServices)
        {
            m_pBrowser->AddService(std::string(service.c_str()), std::dynamic_pointer_cast<pml::ZCPoster>(m_pPoster));
        }
        m_pBrowser->StartBrowser();
    }

    if(setSAP.empty() == false)
    {
        m_pSapWatcher = std::unique_ptr<pml::SapServer>(new pml::SapServer(std::make_shared<wxSapHandler>(this)));
        m_pSapWatcher->Run();
        for(auto service : setSAP)
        {
            m_pSapWatcher->AddReceiver(IpAddress(std::string(service.c_str())));
        }
    }
}


void AoipSourceManager::StopDiscovery()
{
    m_pBrowser = nullptr;
    m_pPoster = nullptr;
    if(m_pSapWatcher)
    {
        m_pSapWatcher->Stop();
    }
    m_pSapWatcher = nullptr;
}


void AoipSourceManager::OnDiscoveryFinished(wxCommandEvent& event)
{
    if(m_pDiscoveryHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ASM_DISCOVERY_FINISHED);
        wxQueueEvent(m_pDiscoveryHandler, pEvent);
    }
}


AoipSourceManager::~AoipSourceManager()
{

}
