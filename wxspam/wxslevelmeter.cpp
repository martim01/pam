#include "wxslevelmeter.h"
#include "levelmeter.h"
#include <prep.h>
#include <vector>
#include "icons/levelmeter16.xpm"
#include "icons/levelmeter32.xpm"
#include "wxspropertylevels.h"

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
            long Languages,rer
            unsigned short VerHi,
            unsigned short VerLo,
            wxString Bmp32FileName,
            wxString Bmp16FileName
            */
    wxsRegisterItem<wxsLevelMeter> Reg(_T("LevelMeter"),
                                     wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("pam"),
                                     340,
                                     wxT("m_pMeter"),
                                     wxsCPP,
                                     1,0,
                                     levelmeter32_xpm,
                                     levelmeter16_xpm
                                     );

     WXS_ST_BEGIN(wxsLevelMeterStyles,_T(""))
    WXS_ST_END()

    WXS_EV_BEGIN(wxsLevelMeterEvents)
    WXS_EV_END()
}

wxsLevelMeter::wxsLevelMeter(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsLevelMeterEvents,
        wxsLevelMeterStyles),
    m_nMin(-70),
    m_bLevelOnly(false)
{}

void wxsLevelMeter::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"levelmeter.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %d, %d, %P, %S);\n"),m_sText.wx_str(), m_nMin, m_bLevelOnly);

            BuildSetupWindowCode();

            double dOverMod;
            if(m_clrNormal.GetColour().IsOk() && m_clrOver.GetColour().IsOk() && m_details.sOverMod.ToDouble(&dOverMod))
            {
                Codef(_T("%ASetLightColours(wxColour(wxT(\"%s\")), %s, wxColour(wxT(\"%s\")));\n"), m_clrNormal.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str(), m_details.sOverMod.c_str(), m_clrOver.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
            }


            if(m_details.sTitle.empty() == false)
            {
                Codef(_T("std::vector<double> vLevels;\n"));
                for(size_t i = 0; i < m_details.asLevels.GetCount(); i++)
                {
                    double dLevel;
                    if(m_details.asLevels[i].ToDouble(&dLevel))
                    {
                        Codef(_T("vLevels.push_back(%s);\n"), m_details.asLevels[i].c_str());
                    }
                }
                Codef(_T("%ASetLevels(vLevels, %s, %t, %t, %t, %s);\n"), m_details.sOffset.c_str(), m_details.sUnits.c_str(), m_details.sTitle.c_str(), m_details.sReference.c_str(), m_details.sScaling.c_str());
            }

            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxsLevelMeter::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxsLevelMeter::OnBuildPreview(wxWindow* Parent,long Flags)
{
    LevelMeter* Preview = new LevelMeter(Parent,GetId(), m_sText, m_nMin, m_bLevelOnly,Pos(Parent),Size(Parent));

    double dOverMod;
    if(m_clrNormal.GetColour().IsOk() && m_clrOver.GetColour().IsOk() && m_details.sOverMod.ToDouble(&dOverMod))
    {
        Preview->SetLightColours(m_clrNormal.GetColour(), dOverMod, m_clrOver.GetColour());
    }


    if(m_details.sTitle.empty() == false)
    {
        std::vector<double> vLevels;
        for(size_t i = 0; i < m_details.asLevels.GetCount(); i++)
        {
            double dLevel;
            if(m_details.asLevels[i].ToDouble(&dLevel))
            {
                vLevels.push_back(dLevel);
            }
        }
        double dOffset(0.0);
        double dScaling(0.0);
        m_details.sOffset.ToDouble(&dOffset);
        m_details.sScaling.ToDouble(&dScaling);

        Preview->SetLevels(vLevels, dOffset,m_details.sUnits, m_details.sTitle, m_details.sReference,dScaling);
    }
    else
    {
        std::vector<double> vLevels;
        vLevels.push_back(m_nMin);
        vLevels.push_back(m_nMin/2);
        vLevels.push_back(m_nMin/4);
        vLevels.push_back(m_nMin/8);

        Preview->SetLevels(vLevels, 0, wxT("dbFS"), m_sText);
    }

    Preview->ShowValue(-18.0);

    return SetupWindow(Preview,Flags);
}

void wxsLevelMeter::OnEnumWidgetProperties(cb_unused long Flags)
{
    WXS_STRING(wxsLevelMeter,m_sText,_("Label"),_T("label"),_T(""),false);
    WXS_BOOL(wxsLevelMeter, m_bLevelOnly, _T("Levels Only"), _T("levels_only"), m_bLevelOnly)

    WXS_LONG(wxsLevelMeter, m_nMin, wxT("Min dB"), wxT("meter_min"), m_nMin);

    WXS_COLOUR(wxsLevelMeter, m_clrNormal, wxT("Normal"), wxT("meter_normal"));
    WXS_COLOUR(wxsLevelMeter, m_clrOver, wxT("Over Mod"), wxT("meter_over"));

    WXS_LEVELS(wxsLevelMeter, m_details, wxT("Levels"), wxT("level"));
}
