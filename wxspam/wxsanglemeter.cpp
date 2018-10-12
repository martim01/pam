#include "wxsanglemeter.h"
#include "anglemeter.h"
#include <prep.h>
#include <vector>
#include "icons/anglemeter16.xpm"
#include "icons/anglemeter32.xpm"
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
    wxsRegisterItem<wxsAngleMeter> Reg(_T("AngleMeter"),
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
                                     anglemeter32_xpm,
                                     anglemeter16_xpm
                                     );

     WXS_ST_BEGIN(wxsAngleMeterStyles,_T(""))
    WXS_ST_END()

    WXS_EV_BEGIN(wxsAngleMeterEvents)
    WXS_EV_END()
}

wxsAngleMeter::wxsAngleMeter(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsAngleMeterEvents,
        wxsAngleMeterStyles),
    m_nMin(-70),
    m_nRouting(0)
{}

void wxsAngleMeter::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"anglemeter.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %t, %d, %d, 1, %P, %S);\n"),m_sText.wx_str(), m_nMin, m_nRouting);

            BuildSetupWindowCode();


            if(m_clrMeter1.GetColour().IsOk() && m_clrMeter2.GetColour().IsOk())
            {
                Codef(_T("%ASetMeterColour(wxColour(wxT(\"%s\")), wxColour(wxT(\"%s\")));\n"), m_clrMeter1.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str(), m_clrMeter2.GetColour().GetAsString(wxC2S_HTML_SYNTAX).c_str());
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
                Codef(_T("%ASetLevels(vLevels, %s, %s, %t, %t, %s, %t);\n"), m_details.sOffset.c_str(), m_details.sScaling.c_str(), m_details.sTitle.c_str(), m_details.sUnits.c_str(), m_details.sOverMod.c_str(), m_details.sReference.c_str());
            }

            return;
        }

        case wxsUnknownLanguage: // fall through
        default:
        {
            wxsCodeMarks::Unknown(_T("wxsAngleMeter::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxsAngleMeter::OnBuildPreview(wxWindow* Parent,long Flags)
{
    AngleMeter* Preview = new AngleMeter(Parent,GetId(),m_sText, m_nMin, m_nRouting, 1,Pos(Parent),Size(Parent));
    if(m_clrMeter1.GetColour().IsOk() && m_clrMeter2.GetColour().IsOk())
    {
        Preview->SetMeterColour(m_clrMeter1.GetColour(), m_clrMeter2.GetColour());
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
        double dOverMod(0.0);
        m_details.sOffset.ToDouble(&dOffset);
        m_details.sScaling.ToDouble(&dScaling);
        m_details.sOverMod.ToDouble(&dOverMod);

        Preview->SetLevels(vLevels, dOffset, dScaling, m_details.sTitle, m_details.sUnits, dOverMod, m_details.sReference);
    }
    else
    {
        std::vector<double> vLevels;
        vLevels.push_back(m_nMin);
        vLevels.push_back(m_nMin/2);
        vLevels.push_back(m_nMin/4);
        vLevels.push_back(m_nMin/8);

        Preview->SetLevels(vLevels, 0, 1, m_sText, wxEmptyString, 0, wxEmptyString);
    }

    double dValue[2] = {-20.0,-16.0};
    Preview->ShowValue(dValue);

    return SetupWindow(Preview,Flags);
}

void wxsAngleMeter::OnEnumWidgetProperties(cb_unused long Flags)
{
    WXS_STRING(wxsAngleMeter,m_sText,_("Label"),_T("label"),_T(""),false);

    static const long Values[] = {AngleMeter::MONO, AngleMeter::LEFT_RIGHT, AngleMeter::MONO_STEREO};
    static const wxChar* Names[] = {wxT("Mono"), wxT("Stereo"), wxT("M+S"), NULL };

    WXS_ENUM(wxsAngleMeter, m_nRouting, _("Meter Type"), _T("meter_type"), Values, Names, 0);
    WXS_LONG(wxsAngleMeter, m_nMin, wxT("Min dB"), wxT("meter_min"), m_nMin);

    WXS_COLOUR(wxsAngleMeter, m_clrMeter1, wxT("Meter 1"), wxT("meter_1"));
    WXS_COLOUR(wxsAngleMeter, m_clrMeter2, wxT("Meter 2"), wxT("meter_2"));

    WXS_LEVELS(wxsAngleMeter, m_details, wxT("Levels"), wxT("level"));
}
