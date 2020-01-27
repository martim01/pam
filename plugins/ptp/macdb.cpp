#include "macdb.h"
#include <wx/xml/xml.h>
#include "settings.h"
#include "wmlogevent.h"

bool MacDb::LoadXml()
{
    wxXmlDocument xmlDoc;
    if(xmlDoc.Load(wxString::Format(wxT("%s/macaddress.xml"), Settings::Get().GetDocumentDirectory().c_str())))
    {
        for(wxXmlNode* pRecord = xmlDoc.GetRoot()->GetChildren(); pRecord; pRecord = pRecord->GetNext())
        {
            if(pRecord->GetName() == "record")
            {
                wxString sMac, sVendor;
                for(wxXmlNode* pEntry = pRecord->GetChildren(); pEntry; pEntry = pEntry->GetNext())
                {
                    if(pEntry->GetName() == "mac")
                    {
                        sMac = pEntry->GetNodeContent();
                    }
                    else if(pEntry->GetName() == "vendor")
                    {
                        sVendor = pEntry->GetNodeContent();
                    }
                }
                if(sMac.empty() == false)
                {
                    m_mVendors.insert(std::make_pair(sMac.MakeLower(), sVendor));
                }
            }
        }
        wmLog::Get()->Log(wxString::Format("Vendoers: %zu", m_mVendors.size()));
        return true;
    }
    else
    {
        wmLog::Get()->Log("Failed to open file");
        return false;
    }
}


wxString MacDb::GetVendor(const wxString& sMac) const
{
    auto itMac = m_mVendors.find(sMac.Left(8));


    if(itMac != m_mVendors.end())
    {
        if(itMac->first.length() == 8)
        {
            return itMac->second.BeforeFirst(' ')+"\n";
        }
        else
        {
            return wxEmptyString;
            //@todo
        }
    }
    else
    {
        wmLog::Get()->Log(wxString::Format("GetVendor %s: Not Found", sMac.Left(8).c_str()));
        return wxEmptyString;
    }
}
