#include "wxspropertyadditems.h"
#include <globals.h>
#include <manager.h>
#include <projectmanager.h>
#include <cbproject.h>
#include <cbfunctor.h>
#include <logmanager.h>


using namespace std;
// Helper macro for fetching variable
#define ITEMS   wxsVARIABLE(pObject,m_nItemOffset,lItems_t)
#define STRING   wxsVARIABLE(pObject,m_nColourOffset,wxString)


wxsPropertyAddItems::wxsPropertyAddItems(long nItemOffset, long nColourOffset, const wxString& PGName,const wxString& DataName,int Priority):
    wxsCustomEditorProperty(PGName,DataName,Priority),
    m_nItemOffset(nItemOffset),
    m_nColourOffset(nColourOffset)
{}

bool wxsPropertyAddItems::ShowEditor(wxsPropertyContainer* pObject)
{
    dlgAddItems dlg(0, ITEMS, STRING);
    if(dlg.ShowModal() == wxID_OK)
    {
        ITEMS = dlg.GetItems();
        return true;
    }
    return false;
}

bool wxsPropertyAddItems::XmlRead(wxsPropertyContainer* pObject,TiXmlElement* pElement)
{
    ITEMS.clear();

    if(!pElement)
        return false;


    for ( TiXmlElement* pItem = pElement->FirstChildElement("item"); pItem; pItem = pItem->NextSiblingElement("item") )
    {
        button_wxs item;
        TiXmlElement* pChild = pItem->FirstChildElement("text");
        if(pChild)
        {
            item.sText = cbC2U(pChild->GetText());
        }

        pChild = pItem->FirstChildElement("clr_button");
        if(pChild)
        {
            item.clrButton.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrButton = wxNullColour;
        }
        pChild = pItem->FirstChildElement("clr_button_pressed");
        if(pChild)
        {
            item.clrPressed.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrPressed = wxNullColour;
        }
        pChild = pItem->FirstChildElement("clr_button_selected");
        if(pChild)
        {
            item.clrSelected.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrSelected = wxNullColour;
        }
        pChild = pItem->FirstChildElement("clr_button_flashing");
        if(pChild)
        {
            item.clrFlashing.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrFlashing = wxNullColour;
        }

        pChild = pItem->FirstChildElement("clr_text");
        if(pChild)
        {
            item.clrText.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrText = wxNullColour;
        }
        pChild = pItem->FirstChildElement("clr_text_pressed");
        if(pChild)
        {
            item.clrTextPressed.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrTextPressed = wxNullColour;
        }
        pChild = pItem->FirstChildElement("clr_text_selected");
        if(pChild)
        {
            item.clrTextSelected.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrTextSelected = wxNullColour;
        }
        pChild = pItem->FirstChildElement("clr_text_flashing");
        if(pChild)
        {
            item.clrTextFlashing.Set(cbC2U(pChild->GetText()));
        }
        else
        {
            item.clrTextFlashing = wxNullColour;
        }

        pChild = pItem->FirstChildElement("enabled");
        if(pChild)
        {
            long nEnabled;
            cbC2U(pChild->GetText()).ToLong(&nEnabled);
            item.nEnabled = nEnabled;
        }
        ITEMS.push_back(item);

    }


    return true;
}

bool wxsPropertyAddItems::XmlWrite(wxsPropertyContainer* pObject,TiXmlElement* pElement)
{
    for(lItems_t::iterator itItem = ITEMS.begin(); itItem != ITEMS.end(); ++itItem)
    {
        TiXmlElement xmlItem("item");
        AppendProperty(&xmlItem, wxT("text"), (*itItem).sText);

        if((*itItem).clrButton.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_button"), (*itItem).clrButton.GetAsString(wxC2S_HTML_SYNTAX));
        }
        if((*itItem).clrPressed.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_button_pressed"), (*itItem).clrPressed.GetAsString(wxC2S_HTML_SYNTAX));
        }
        if((*itItem).clrSelected.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_button_selected"), (*itItem).clrSelected.GetAsString(wxC2S_HTML_SYNTAX));
        }
        if((*itItem).clrFlashing.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_button_flashing"), (*itItem).clrFlashing.GetAsString(wxC2S_HTML_SYNTAX));
        }

        if((*itItem).clrText.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_text"), (*itItem).clrText.GetAsString(wxC2S_HTML_SYNTAX));
        }
        if((*itItem).clrTextPressed.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_text_pressed"), (*itItem).clrTextPressed.GetAsString(wxC2S_HTML_SYNTAX));
        }
        if((*itItem).clrTextSelected.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_text_selected"), (*itItem).clrTextSelected.GetAsString(wxC2S_HTML_SYNTAX));
        }
        if((*itItem).clrTextFlashing.IsOk())
        {
            AppendProperty(&xmlItem, wxT("clr_text_flashing"), (*itItem).clrTextFlashing.GetAsString(wxC2S_HTML_SYNTAX));
        }

        AppendProperty(&xmlItem, wxT("enabled"), (wxString::Format(wxT("%d"), (*itItem).nEnabled)));
        AppendProperty(&xmlItem, wxT("slide"), (wxString::Format(wxT("%d"), (*itItem).nSlideAllowed)));
        AppendProperty(&xmlItem, wxT("slide_text"), (*itItem).sSlide);

        pElement->InsertEndChild(xmlItem);
    }

    return true;
}

void wxsPropertyAddItems::AppendProperty(TiXmlElement* pParent, wxString sProperty, wxString sValue)
{
    TiXmlElement xml(cbU2C(sProperty));
    xml.InsertEndChild(TiXmlText(cbU2C(sValue)));
    pParent->InsertEndChild(xml);
}

bool wxsPropertyAddItems::PropStreamRead(wxsPropertyContainer* pObject,wxsPropertyStream* pStream)
{

    return true;
}

bool wxsPropertyAddItems::PropStreamWrite(wxsPropertyContainer* pObject,wxsPropertyStream* pStream)
{

    return true;
}

wxString wxsPropertyAddItems::GetStr(wxsPropertyContainer* pObject)
{

    return _("Click to add items...");

}


