#include "wxswmlines.h"
#include "wmline.h"
#include <prep.h>
#include "icons/wmlines32.xpm"
#include "icons/wmlines16.xpm"


namespace
{
    wxsRegisterItem<wxswmLines> Reg(_T("wmLineWnd"), wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pLns"),
                                     wxsCPP,
                                     1,0,
                                     wmlines32_xpm,
                                     wmlines16_xpm
                                     );

    WXS_ST_BEGIN(wxswmLinesStyles,_T(""))
        WXS_ST_CATEGORY("wmLineWnd")
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmLinesEvents)
    WXS_EV_END()
}

wxswmLines::wxswmLines(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmLinesEvents,
        wxswmLinesStyles)
{}

void wxswmLines::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmline.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %P, %S, 0);\n"));
            for(mLines_t::iterator itLine = m_mLines.begin(); itLine != m_mLines.end(); ++itLine)
            {
                Codef(_T("%AAddPercentageLine(wxT(\"%s\"), wxPoint(%d,%d), wxPoint(%d,%d), wxColour(wxT(\"%s\")), %d, %d, %d, %d, %d, %d, %d);\n"),itLine->first.c_str(), itLine->second.pntStart.x, itLine->second.pntStart.y, itLine->second.pntEnd.x,itLine->second.pntEnd.y,itLine->second.clr.GetAsString(wxC2S_HTML_SYNTAX).c_str(), itLine->second.nWidth, itLine->second.nPenStyle, itLine->second.nLineStyle, itLine->second.nStartArrow, itLine->second.nEndArrow, itLine->second.bDraw, itLine->second.nArrowSize);
            }

            //if ( IsDefault ) Codef( _T("%ASetDefault();\n"));
            BuildSetupWindowCode();
            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmLines::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmLines::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmLineWnd* Preview = new wmLineWnd(Parent,GetId(),Pos(Parent),Size(Parent),0);
    for(mLines_t::iterator itLine = m_mLines.begin(); itLine != m_mLines.end(); ++itLine)
    {
        Preview->AddPercentageLine(itLine->first, itLine->second);
    }
    //if ( IsDefault ) Preview->SetDefault();
    return SetupWindow(Preview,Flags);
}

void wxswmLines::OnEnumWidgetProperties(cb_unused long Flags)
{
    static wxsPropertyAddLines _Property(wxsOFFSET(wxswmLines,m_mLines), wxT("Lines"),wxT("lines"));
    Property(_Property);
    //WXS_ADDLINE(wxswmLines, m_mLines, wxT("Lines"), wxT("lines"));
}


void wxswmLines::OnAddExtraProperties(wxsPropertyGridManager* pGrid)
{
    wxsWidget::OnAddExtraProperties(pGrid);
}

void wxswmLines::OnExtraPropertyChanged(wxsPropertyGridManager* pGrid, wxPGId id)
{
    wxsWidget::OnExtraPropertyChanged(pGrid, id);
}

bool wxswmLines::OnXmlRead(TiXmlElement* pElement, bool bIsXrc, bool bIsExtra)
{
    return wxsWidget::OnXmlRead(pElement, bIsXrc, bIsExtra);
}

bool wxswmLines::OnXmlWrite(TiXmlElement* pElement, bool bIsXrc, bool bIsExtra)
{
    return wxsWidget::OnXmlWrite(pElement, bIsXrc, bIsExtra);
}
