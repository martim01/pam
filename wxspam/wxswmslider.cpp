#include "wxswmslider.h"
#include "wmslider.h"
#include <prep.h>

#include "icons/wmslider16.xpm"
#include "icons/wmslider32.xpm"

namespace
{
    /*
    wxString ClassName,
            wxsItemType Type,
            wxString License,
            wxString Author,
            wxString Email,
            wxString Site,
            wxString Category,
            long Priority,
            wxString DefaultVarName,
            long Languages,
            unsigned short VerHi,
            unsigned short VerLo,
            wxString Bmp32FileName,
            wxString Bmp16FileName
            */
    wxsRegisterItem<wxswmSlider> Reg(_T("wmSlider"),
                                     wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pSlider"),
                                     wxsCPP,
                                     1,0,
                                     wmslider32_xpm,
                                     wmslider16_xpm
                                     );

    WXS_ST_BEGIN(wxswmSliderStyles,_T(""))
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmSliderEvents)
        WXS_EVI(EVT_SLIDER_MOVE,wxEVT_SLIDER_MOVE,wxCommandEvent,Move)
    WXS_EV_END()
}

wxswmSlider::wxswmSlider(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmSliderEvents,
        wxswmSliderStyles),
    m_sLabel(_("Slider")),
    m_nMin(0),
    m_nMax(100),
    m_nStart(30)
{}

void wxswmSlider::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmslider.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %P, %S);\n"),m_sLabel.wx_str());

            BuildSetupWindowCode();

            if(m_clrSlider.GetColour().IsOk())
            {
                Codef(_T("%ASetSliderColour(wxColour(wxT(\"%s\"));\n"), m_clrSlider.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            if(m_clrButton.GetColour().IsOk())
            {
                Codef(_T("%ASetButtonColour(wxColour(wxT(\"%s\"));\n"), m_clrButton.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }
            Codef(_T("%AInit(%d,%d,%d);\n"),m_nMin, m_nMax, m_nStart);


            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmSlider::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmSlider::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wmSlider* Preview = new wmSlider(Parent,GetId(),m_sLabel,Pos(Parent),Size(Parent));
    if(m_clrSlider.GetColour().IsOk())
    {
        Preview->SetSliderColour(m_clrSlider.GetColour());
    }
    if(m_clrButton.GetColour().IsOk())
    {
        Preview->SetButtonColour(m_clrButton.GetColour());
    }
    Preview->Init(m_nMin, m_nMax, m_nStart);

    return SetupWindow(Preview,Flags);
}

void wxswmSlider::OnEnumWidgetProperties(cb_unused long Flags)
{
    WXS_STRING(wxswmSlider,m_sLabel,_("Label"),_T("label"),_T(""),false)

    WXS_COLOUR(wxswmSlider, m_clrSlider, wxT("Slider"), wxT("slider"));
    WXS_COLOUR(wxswmSlider, m_clrButton, wxT("Button"), wxT("button"));

    WXS_LONG(wxswmSlider, m_nMin, wxT("Min Value"), wxT("min"), m_nMin);
    WXS_LONG(wxswmSlider, m_nMax, wxT("Max Value"), wxT("max"), m_nMax);
    WXS_LONG(wxswmSlider, m_nStart, wxT("Start Value"), wxT("start"), m_nStart);
}
