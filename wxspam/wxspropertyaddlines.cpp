#include "wxspropertyaddlines.h"
#include <globals.h>
#include <manager.h>
#include <projectmanager.h>
#include <cbproject.h>
#include <cbfunctor.h>
#include <logmanager.h>


using namespace std;
// Helper macro for fetching variable
#define LINES   wxsVARIABLE(pObject,m_nLineOffset,mLines_t)


wxsPropertyAddLines::wxsPropertyAddLines(long nLineOffset, const wxString& PGName,const wxString& DataName,int Priority):
    wxsCustomEditorProperty(PGName,DataName,Priority),
    m_nLineOffset(nLineOffset)
{}

bool wxsPropertyAddLines::ShowEditor(wxsPropertyContainer* pObject)
{
    dlgAddLines dlg(0, LINES);
    if(dlg.ShowModal() == wxID_OK)
    {
        LINES = dlg.GetLines();
        return true;
    }
    return false;
}

bool wxsPropertyAddLines::XmlRead(wxsPropertyContainer* pObject,TiXmlElement* pElement)
{
    LINES.clear();

    if(!pElement)
        return false;



    for ( TiXmlElement* pItem = pElement->FirstChildElement("line"); pItem; pItem = pItem->NextSiblingElement("line") )
    {
        wxString sName(cbC2U(pItem->Attribute("name")));

        long nStartX(0);
        long nStartY(0);
        long nEndX(0);
        long nEndY(0);
        wxColour clr;
        long nWidth(1);
        long nPen(wxSOLID);
        long nLine(wmLine::LINESTYLE_FLAT);
        long nStartArrow(wmLine::LINEEND_NONE);
        long nEndArrow(wmLine::LINEEND_NONE);
        long nArrowSize(7);
        bool bDraw(true);

        TiXmlElement* pChild = pItem->FirstChildElement("start");
        if(pChild)
        {
            cbC2U(pChild->Attribute("x")).ToLong(&nStartX);
            cbC2U(pChild->Attribute("y")).ToLong(&nStartY);
        }

        pChild = pItem->FirstChildElement("end");
        if(pChild)
        {
            cbC2U(pChild->Attribute("x")).ToLong(&nEndX);
            cbC2U(pChild->Attribute("y")).ToLong(&nEndY);
        }

        pChild = pItem->FirstChildElement("colour");
        if(pChild)
        {
            clr.Set(cbC2U(pChild->GetText()));
        }

        pChild = pItem->FirstChildElement("width");
        if(pChild)
        {
            cbC2U(pChild->GetText()).ToLong(&nWidth);
        }

        pChild = pItem->FirstChildElement("pen");
        if(pChild)
        {
            cbC2U(pChild->GetText()).ToLong(&nPen);
        }

        pChild = pItem->FirstChildElement("style");
        if(pChild)
        {
            cbC2U(pChild->GetText()).ToLong(&nLine);
        }

        pChild = pItem->FirstChildElement("start_arrow");
        if(pChild)
        {
            cbC2U(pChild->GetText()).ToLong(&nStartArrow);
        }

        pChild = pItem->FirstChildElement("end_arrow");
        if(pChild)
        {
            cbC2U(pChild->GetText()).ToLong(&nEndArrow);
        }

        pChild = pItem->FirstChildElement("arrow_size");
        if(pChild)
        {
            cbC2U(pChild->GetText()).ToLong(&nArrowSize);
        }

        pChild = pItem->FirstChildElement("draw");
        if(pChild)
        {
            bDraw = (cbC2U(pChild->GetText()) != wxT("0"));
        }

        LINES.insert(make_pair(sName, wmLine(wxPoint(nStartX, nStartY),
                                             wxPoint(nEndX, nEndY),
                                             clr,
                                             nWidth,
                                             nPen,
                                             nLine,
                                             nStartArrow,
                                             nEndArrow,
                                             nArrowSize,
                                             bDraw)));
    }


    return true;
}

bool wxsPropertyAddLines::XmlWrite(wxsPropertyContainer* pObject,TiXmlElement* pElement)
{

    for(mLines_t::iterator itLine = LINES.begin(); itLine != LINES.end(); ++itLine)
    {
        TiXmlElement xmlLine("line");
        xmlLine.SetAttribute("name", cbU2C(itLine->first));

        TiXmlElement xmlStart("start");
        xmlStart.SetAttribute("x", cbU2C(wxString::Format(wxT("%d"), itLine->second.pntStart.x)));
        xmlStart.SetAttribute("y", cbU2C(wxString::Format(wxT("%d"), itLine->second.pntStart.y)));
        xmlLine.InsertEndChild(xmlStart);

        TiXmlElement xmlEnd("end");
        xmlEnd.SetAttribute("x", cbU2C(wxString::Format(wxT("%d"), itLine->second.pntEnd.x)));
        xmlEnd.SetAttribute("y", cbU2C(wxString::Format(wxT("%d"), itLine->second.pntEnd.y)));
        xmlLine.InsertEndChild(xmlEnd);

        AppendProperty(&xmlLine, wxT("colour"), itLine->second.clr.GetAsString(wxC2S_HTML_SYNTAX));
        AppendProperty(&xmlLine, wxT("width"), wxString::Format(wxT("%d"),itLine->second.nWidth));
        AppendProperty(&xmlLine, wxT("pen"), wxString::Format(wxT("%d"),itLine->second.nPenStyle));
        AppendProperty(&xmlLine, wxT("style"), wxString::Format(wxT("%d"),itLine->second.nLineStyle));
        AppendProperty(&xmlLine, wxT("start_arrow"), wxString::Format(wxT("%d"),itLine->second.nStartArrow));
        AppendProperty(&xmlLine, wxT("end_arrow"), wxString::Format(wxT("%d"),itLine->second.nEndArrow));
        AppendProperty(&xmlLine, wxT("arrow_size"), wxString::Format(wxT("%d"),itLine->second.nArrowSize));
        AppendProperty(&xmlLine, wxT("draw"), wxString::Format(wxT("%d"),itLine->second.bDraw));

        pElement->InsertEndChild(xmlLine);
    }

    return true;
}

void wxsPropertyAddLines::AppendProperty(TiXmlElement* pParent, wxString sProperty, wxString sValue)
{
    TiXmlElement xml(cbU2C(sProperty));
    xml.InsertEndChild(TiXmlText(cbU2C(sValue)));
    pParent->InsertEndChild(xml);
}

bool wxsPropertyAddLines::PropStreamRead(wxsPropertyContainer* pObject,wxsPropertyStream* pStream)
{

    return true;
}

bool wxsPropertyAddLines::PropStreamWrite(wxsPropertyContainer* pObject,wxsPropertyStream* pStream)
{

    return true;
}

wxString wxsPropertyAddLines::GetStr(wxsPropertyContainer* pObject)
{

    return _("Click to edit lines...");

}


