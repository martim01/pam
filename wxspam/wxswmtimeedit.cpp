#include "wxswmtimeedit.h"
#include "wmtimeedit.h"

#include "icons/wmtimeedit16.xpm"
#include "icons/wmtimeedit32.xpm"

namespace
{
    wxsRegisterItem<wxswmTimeEdit> Reg(_T("wmTimeEdit"),
                                    wxsTWidget,
                                     wxT("wxWindows"),
                                     wxT("Matthew Martin"),
                                     wxT("matthew.martin@bbc.co.uk"),
                                     wxT(""),
                                     _T("wammacs"),
                                     40,
                                     wxT("m_pedt"),
                                     wxsCPP,
                                     1,0,
                                     wmtimeedit32_xpm,
                                     wmtimeedit16_xpm
                                     );

    WXS_ST_BEGIN(wxswmTimeEditStyles,_T(""))
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxswmTimeEditEvents)
        WXS_EVI(EVT_TEXT,wxEVT_COMMAND_TEXT_UPDATED,wxCommandEvent,Text)
        WXS_EVI(EVT_TEXT_ENTER,wxEVT_COMMAND_TEXT_ENTER,wxCommandEvent,TextEnter)
    WXS_EV_END()


}

wxswmTimeEdit::wxswmTimeEdit(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxswmTimeEditEvents,
        wxswmTimeEditStyles),
    m_nDateFormat(wmTimeEdit::DATE_YYYYMMDD),
    m_nTimeFormat(wmTimeEdit::TIME_HMS),
    m_nFrames(0),
    m_bTimeFirst(true),
    m_sSeparatorDate(wxT("/")),
    m_sSeparatorTime(wxT(":"))
{}

void wxswmTimeEdit::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("\"wmtimeedit.h\""),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %d, %d, %d, %d, wxT('%s'), wxT('%s'), %P, %S);\n"), m_nDateFormat,
                                                                      m_nTimeFormat,
                                                                      m_nFrames,
                                                                      m_bTimeFirst,
                                                                      m_sSeparatorDate.Left(1).c_str(),
                                                                      m_sSeparatorTime.Left(1).c_str());
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxswmTimeEdit::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxswmTimeEdit::OnBuildPreview(wxWindow* Parent,long Flags)
{
    if(m_sSeparatorDate == wxEmptyString)
    {
        m_sSeparatorDate = wxT("/");
    }
    if(m_sSeparatorTime == wxEmptyString)
    {
        m_sSeparatorTime = wxT(":");
    }
    wmTimeEdit* Preview = new wmTimeEdit(Parent,GetId(),m_nDateFormat, m_nTimeFormat, m_nFrames, m_bTimeFirst, m_sSeparatorDate.GetChar(0), m_sSeparatorTime.GetChar(0) ,Pos(Parent),Size(Parent));

    Preview->SetDateTime(wxDateTime::Now());
    return SetupWindow(Preview,Flags);
}

void wxswmTimeEdit::OnEnumWidgetProperties(long Flags)
{

    static const long Values_Date[] = {wmTimeEdit::DATE_NONE, wmTimeEdit::DATE_DDMMYY, wmTimeEdit::DATE_MMDDYYYY, wmTimeEdit::DATE_YYYYMMDD, wmTimeEdit::DATE_DDMMYY, wmTimeEdit::DATE_YYMMDD};
    static const wxChar* Names_Date[] = {wxT("None"), wmTimeEdit::DDMMYYYY, wmTimeEdit::MMDDYYYY, wmTimeEdit::YYYYMMDD, wmTimeEdit::DDMMYY, wmTimeEdit::MMDDYY, wmTimeEdit::YYMMDD, NULL };


    WXS_ENUM(wxswmTimeEdit, m_nDateFormat, _("Date Format"), _T("date_format"), Values_Date, Names_Date, m_nDateFormat);

    static const long Values_Time[] = {wmTimeEdit::TIME_NONE, wmTimeEdit::TIME_HM, wmTimeEdit::TIME_HMS, wmTimeEdit::TIME_HMSmS, wmTimeEdit::TIME_HMSFr};
    static const wxChar* Names_Time[] = {wxT("None"), wmTimeEdit::HM, wmTimeEdit::HMS, wmTimeEdit::HMSmS, wmTimeEdit::HMSFr, NULL};

    WXS_ENUM(wxswmTimeEdit, m_nTimeFormat, _("Time Format"), _T("time_format"), Values_Time, Names_Time, m_nTimeFormat);

    WXS_STRING(wxswmTimeEdit,m_sSeparatorDate,_("Date Separator"),_T("date_seaparator"),_T("/"),false)
    WXS_STRING(wxswmTimeEdit,m_sSeparatorTime,_("Time Separator"),_T("time_seaparator"),_T(":"),false)
    WXS_BOOL(wxswmTimeEdit, m_bTimeFirst, _("Time First"), _T("time_first"), m_bTimeFirst);
    WXS_LONG(wxswmTimeEdit,m_nFrames,_("Frames/Sec"),_T("frames"),m_nFrames)

}
