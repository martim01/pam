#include "ppmtypes.h"
#include "settings.h"
#include <wx/xml/xml.h>
#include <wx/tokenzr.h>
#include <wx/stdpaths.h>
using namespace std;

PPMTypeManager& PPMTypeManager::Get()
{
    static PPMTypeManager manager;
    return manager;
}

map<wxString, ppmtype>::const_iterator PPMTypeManager::GetTypeBegin()
{
    return m_mTypes.begin();
}
map<wxString, ppmtype>::const_iterator PPMTypeManager::GetTypeEnd()
{
    return m_mTypes.end();
}

map<wxString, ppmtype>::const_iterator PPMTypeManager::FindType(const wxString& sType)
{
    return m_mTypes.find(sType);
}

PPMTypeManager::PPMTypeManager()
{
    LoadTypes();
}

void PPMTypeManager::LoadTypes()
{
    wxXmlDocument xmlDoc;
    wxString sFile = wxString::Format(wxT("%s/pam/ppmtypes.xml"), wxStandardPaths::Get().GetDocumentsDir().c_str());
    if(xmlDoc.Load(sFile) && xmlDoc.GetRoot())
    {
        for(wxXmlNode* pTopNode = xmlDoc.GetRoot()->GetChildren(); pTopNode; pTopNode = pTopNode->GetNext())
        {
            if(pTopNode->GetName().CmpNoCase(wxT("ppm_type")) == 0)
            {
                ppmtype atype;
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
                    else if(pNode->GetName().CmpNoCase(wxT("offset")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dOffset);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("risetime")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dRiseTime);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("risedb")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dRisedB);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("falltime")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dFallTime);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("falldb")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dFalldB);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("scaling")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dScaling);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("type")) == 0)
                    {
                        pNode->GetNodeContent().ToLong(&atype.nType);
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("units")) == 0)
                    {
                        atype.sUnit = pNode->GetNodeContent();
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("label_ref")) == 0)
                    {
                        atype.sReference = pNode->GetNodeContent();
                    }
                    else if(pNode->GetName().CmpNoCase(wxT("overmod")) == 0)
                    {
                        pNode->GetNodeContent().ToDouble(&atype.dOverMod);
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
