#pragma once

//(*Headers(LTCGeneratorPanel)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

#include <vector>
#include "pmpanel.h"

class LTCGeneratorBuilder;
class timedbuffer;
class session;
class SettingEvent;

class LTCGeneratorPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(LTCGeneratorPanel);

		LTCGeneratorPanel(){}
		LTCGeneratorPanel(wxWindow* parent,LTCGeneratorBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LTCGeneratorPanel();

		//(*Declarations(LTCGeneratorPanel)
		wmLabel* m_pLbl4;
		wmLabel* m_plblDays;
		wmLabel* m_pLbl3;
		wmButton* m_pbtnMonthMinus;
		wmLabel* m_pLbl7;
		wmButton* m_pbtnMonthPlus;
		wxPanel* pnlTime;
		wmList* m_plstData;
		wmLabel* m_pLbl1;
		wmButton* m_pbtnDayMinus;
		wmButton* m_ptbnMinutePlus;
		wmButton* m_pbtnDayPlus;
		wmLabel* m_plblMinutes;
		wmLabel* m_plblMonths;
		wmLabel* m_pLbl10;
		wmButton* m_pbtnHourPlus;
		wmLabel* m_pLbl5;
		wmList* m_plstTime;
		wmButton* m_pbtnYearMinus;
		wmLabel* m_plblHours;
		wmList* m_plstUserData;
		wmList* m_plstFPS;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl8;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnSecondPlus;
		wmLabel* m_pLbl9;
		wmButton* m_pbtnYearPlus;
		wmLabel* m_plblSeconds;
		wmButton* m_pbtnHourMinus;
		wmButton* m_ptbnSecondMinus;
		wmButton* m_pbtnMinuteMinus;
		wmLabel* m_plblYears;
		//*)

	protected:

		//(*Identifiers(LTCGeneratorPanel)
		static const long ID_M_PLBL1;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLST4;
		static const long ID_M_PLBL5;
		static const long ID_M_PBTN2;
		static const long ID_M_PLBL11;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL8;
		static const long ID_M_PBTN7;
		static const long ID_M_PLBL14;
		static const long ID_M_PBTN8;
		static const long ID_M_PLBL6;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL12;
		static const long ID_M_PBTN4;
		static const long ID_M_PLBL9;
		static const long ID_M_PBTN9;
		static const long ID_M_PLBL15;
		static const long ID_M_PBTN10;
		static const long ID_M_PLBL7;
		static const long ID_M_PBTN5;
		static const long ID_M_PLBL13;
		static const long ID_M_PBTN6;
		static const long ID_M_PLBL10;
		static const long ID_M_PBTN11;
		static const long ID_M_PLBL16;
		static const long ID_M_PBTN12;
		static const long ID_PANEL1;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(LTCGeneratorPanel)
		void OnlstFPSSelected(wxCommandEvent& event);
		void OnlstUserDataSelected(wxCommandEvent& event);
		void OnlstDataSelected(wxCommandEvent& event);
		void OnbtnYearMinusClick(wxCommandEvent& event);
		void OnbtnYearPlusClick(wxCommandEvent& event);
		void OnbtnMonthMinusClick(wxCommandEvent& event);
		void OnbtnMonthPlusClick(wxCommandEvent& event);
		void OnbtnDayMinusClick(wxCommandEvent& event);
		void OnbtnDayPlusClick(wxCommandEvent& event);
		void OnbtnHourMinusClick(wxCommandEvent& event);
		void OnbtnHourPlusClick(wxCommandEvent& event);
		void OnbtnMinuteMinusClick(wxCommandEvent& event);
		void OnbtnMinutePlusClick(wxCommandEvent& event);
		void OnbtnSecondMinusClick(wxCommandEvent& event);
		void OnbtnSecondPlusClick(wxCommandEvent& event);
		void OnlstTimeSelected(wxCommandEvent& event);
		//*)

		void LoadSettings();
		void OnSettingEvent(SettingEvent& event);

		LTCGeneratorBuilder* m_pBuilder;

		int m_nYear;
		int m_nMonth;
		int m_nDay;
		int m_nHour;
		int m_nMinute;
		int m_nSecond;

        wxString m_sOffset;
		DECLARE_EVENT_TABLE()
};

