#include "wxspropertylevels.h"
#include <globals.h>
#include <manager.h>
#include <projectmanager.h>
#include <cbproject.h>
#include <cbfunctor.h>
#include <logmanager.h>
#include "dlgLevels.h"

using namespace std;
// Helper macro for fetching variable
#define LEVELS   wxsVARIABLE(pObject,m_nLevelOffset,levelDetails)


wxsPropertyLevels::wxsPropertyLevels(long nLevelOffset, const wxString& PGName,const wxString& DataName,int Priority):
    wxsCustomEditorProperty(PGName,DataName,Priority),
    m_nLevelOffset(nLevelOffset)
{}

bool wxsPropertyLevels::ShowEditor(wxsPropertyContainer* pObject)
{
    dlgLevels dlg(0, LEVELS);
    if(dlg.ShowModal() == wxID_OK)
    {
        LEVELS = dlg.GetDetails();
        return true;
    }
    return false;
}

bool wxsPropertyLevels::XmlRead(wxsPropertyContainer* pObject,TiXmlElement* pElement)
{
    LEVELS.asLevels.clear();

    if(!pElement)
        return false;


    //TiXmlElement* pLevel = pElement->FirstChildElement("levels");
    if(pElement)
    {
        LEVELS.sTitle = (cbC2U(pElement->Attribute("title")));
        LEVELS.sUnits = (cbC2U(pElement->Attribute("units")));
        LEVELS.sOffset = (cbC2U(pElement->Attribute("offset")));
        LEVELS.sScaling = (cbC2U(pElement->Attribute("scaling")));
        LEVELS.sReference = (cbC2U(pElement->Attribute("reference")));
        LEVELS.sOverMod = (cbC2U(pElement->Attribute("overmod")));

        for ( TiXmlElement* pItem = pElement->FirstChildElement("level"); pItem; pItem = pItem->NextSiblingElement("level") )
        {
            LEVELS.asLevels.Add(cbC2U(pItem->GetText()));
        }
    }


    return true;
}

bool wxsPropertyLevels::XmlWrite(wxsPropertyContainer* pObject,TiXmlElement* pElement)
{
    pElement->SetAttribute("title", cbU2C(LEVELS.sTitle));
    pElement->SetAttribute("units", cbU2C(LEVELS.sUnits));
    pElement->SetAttribute("offset", cbU2C(LEVELS.sOffset));
    pElement->SetAttribute("scaling", cbU2C(LEVELS.sScaling));
    pElement->SetAttribute("reference", cbU2C(LEVELS.sReference));
    pElement->SetAttribute("overmod", cbU2C(LEVELS.sOverMod));

    for(size_t i = 0; i < LEVELS.asLevels.GetCount(); i++)
    {
        TiXmlElement xmlLevel("level");
        TiXmlText xmlText(cbU2C(LEVELS.asLevels[i]));
        xmlLevel.InsertEndChild(xmlText);
        pElement->InsertEndChild(xmlLevel);
    }

    return true;
}

void wxsPropertyLevels::AppendProperty(TiXmlElement* pParent, wxString sProperty, wxString sValue)
{
    TiXmlElement xml(cbU2C(sProperty));
    xml.InsertEndChild(TiXmlText(cbU2C(sValue)));
    pParent->InsertEndChild(xml);
}

bool wxsPropertyLevels::PropStreamRead(wxsPropertyContainer* pObject,wxsPropertyStream* pStream)
{

    return true;
}

bool wxsPropertyLevels::PropStreamWrite(wxsPropertyContainer* pObject,wxsPropertyStream* pStream)
{

    return true;
}

wxString wxsPropertyLevels::GetStr(wxsPropertyContainer* pObject)
{

    return _("Click to edit levels...");

}


