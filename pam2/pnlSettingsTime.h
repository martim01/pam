#ifndef PNLSETTINGSTIME_H
#define PNLSETTINGSTIME_H

//(*Headers(pnlSettingsTime)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

class pnlSettingsTime: public wxPanel
{
	public:

		pnlSettingsTime(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettingsTime();

		//(*Declarations(pnlSettingsTime)
		wmButton* m_pbtnLTC;
		wmButton* m_pbtnNTP;
		wmButton* m_pbtnNTPServerDeleteAll;
		wmButton* m_pbtnNtpServerAdd;
		wmButton* m_pbtnNtpServerDelete;
		wmButton* m_pbtnNtpServerEdit;
		wmButton* m_pbtnPTP;
		wmLabel* m_pLbl3;
		wmList* m_plstNTPServers;
		//*)

	protected:

		//(*Identifiers(pnlSettingsTime)
		static const long ID_M_PBTN22;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN6;
		//*)

	private:

		//(*Handlers(pnlSettingsTime)
		void OnbtnPTPClick(wxCommandEvent& event);
		void OnbtnLTCClick(wxCommandEvent& event);
		void OnbtnNTPClick(wxCommandEvent& event);
		void OnlstNTPServersSelected(wxCommandEvent& event);
		void OnbtnNtpServerAddClick(wxCommandEvent& event);
		void OnbtnNtpServerEditClick(wxCommandEvent& event);
		void OnbtnNtpServerDeleteClick(wxCommandEvent& event);
		void OnbtnNTPServerDeleteAllClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
