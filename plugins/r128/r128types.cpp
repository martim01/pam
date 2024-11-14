#include "r128types.h"
#include "settings.h"
#include <wx/xml/xml.h>
#include <wx/tokenzr.h>
#include <wx/stdpaths.h>
#include "log.h"

using namespace std;

R128TypeManager& R128TypeManager::Get()
{
    static R128TypeManager manager;
    return manager;
}

map<wxString, r128type>::const_iterator R128TypeManager::GetTypeBegin()
{
    return m_mTypes.begin();
}
map<wxString, r128type>::const_iterator R128TypeManager::GetTypeEnd()
{
    return m_mTypes.end();
}

map<wxString, r128type>::const_iterator R128TypeManager::FindType(const wxString& sType)
{
    return m_mTypes.find(sType);
}

R128TypeManager::R128TypeManager()
{
    LoadTypes();
}

std::set<wxString> R128TypeManager::GetTypes()
{
    std::set<wxString> setTypes;
    for(const auto& pairType : m_mTypes)
    {
        setTypes.insert(pairType.first);
    }
    return setTypes;
}

void R128TypeManager::LoadTypes()
{
    wxXmlDocument xmlDoc;
    wxString sFile = wxString::Format(wxT("%s/pam/r128types.xml"), wxStandardPaths::Get().GetDocumentsDir().c_str());
    if(xmlDoc.Load(sFile) && xmlDoc.GetRoot())
    {
        for(wxXmlNode* pTopNode = xmlDoc.GetRoot()->GetChildren(); pTopNode; pTopNode = pTopNode->GetNext())
        {
            if(pTopNode->GetName().CmpNoCase(wxT("r128_type")) == 0)
            {
                r128type atype;
                wxString sName;
                for(wxXmlNode* pNode = pTopNode->GetChildren(); pNode; pNode = pNode->GetNext())
                {
                    if(pNode->GetName().CmpNoCase(wxT("name")) == 0)
                    {
                        sName = pNode->GetNodeContent();
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("scale")) == 0)
                    {
                        wxArrayString asScale(wxStringTokenize(pNode->GetNodeContent(), wxT(",")));
                        atype.vLevels.resize(asScale.size());
                        for(size_t i = 0; i < asScale.GetCount(); i++)
                        {
                            double dValue;
                            asScale[i].ToDouble(&atype.vLevels[i]);
                        }
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("max")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dMax);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("min")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dMin);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("target")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dTarget);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("ok")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dOk);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("high")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dHigh);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("colour_low")) == 0)
                    {
                        wxColour clr(pNode->GetNodeContent());
                        if(clr.IsOk())
                        {
                            atype.clrLow = clr;
                        }
                        else
                        {
                            atype.clrLow = wxColour(0,0,100);
                        }
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("colour_ok")) == 0)
                    {
                        wxColour clr(pNode->GetNodeContent());
                        if(clr.IsOk())
                        {
                            atype.clrOk = clr;
                        }
                        else
                        {
                            atype.clrOk = wxColour(50,255,50);
                        }
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("colour_high")) == 0)
                    {
                        wxColour clr(pNode->GetNodeContent());
                        if(clr.IsOk())
                        {
                            atype.clrHigh = clr;
                        }
                        else
                        {
                            atype.clrOk = wxColour(100,0,0);
                        }
                    }
                 }
                 if(sName.empty() == false)
                 {
                     m_mTypes.insert(make_pair(sName, atype));
                 }
            }
        }
    }
}
