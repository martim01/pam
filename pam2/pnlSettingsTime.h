#ifndef PNLSETTINGSTIME_H
#define PNLSETTINGSTIME_H

//(*Headers(pnlSettingsTime)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)

class pnlSettingsTime: public wxPanel
{
	public:

		pnlSettingsTime(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId);
		virtual ~pnlSettingsTime();

		//(*Declarations(pnlSettingsTime)
		wmButton* m_pbtnNTPServerDeleteAll;
		wmButton* m_pbtnNtpServerAdd;
		wmButton* m_pbtnNtpServerDelete;
		wmButton* m_pbtnNtpServerEdit;
		wmEdit* m_pedtDomain;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl1;
		wmLabel* m_plblListTitle;
		wmList* m_plstDate;
		wmList* m_plstNTPServers;
		wmList* m_plstSync;
		wmSwitcherPanel* m_pswpSettings;
		wxPanel* m_ppnlLTCS;
		wxPanel* m_ppnlNTP;
		wxPanel* m_ppnlPTP;
		//*)

	protected:

		//(*Identifiers(pnlSettingsTime)
		static const long ID_M_PLST1;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN6;
		static const long ID_PANEL1;
		static const long ID_M_PLBL8;
		static const long ID_M_PEDT1;
		static const long ID_M_PKBD1;
		static const long ID_PANEL2;
		static const long ID_M_PLBL13;
		static const long ID_M_PLST2;
		static const long ID_PANEL3;
		static const long ID_M_PSWP1;
		static const long ID_M_PLST3;
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
		void Onm_ppnlPTPPaint(wxPaintEvent& event);
		void Onm_pedtDomainText(wxCommandEvent& event);
		void OnedtDomainTextEnter(wxCommandEvent& event);
		void OnlstDateSelected(wxCommandEvent& event);
		void OnlstSyncSelected(wxCommandEvent& event);
		//*)

		void SaveNtpServers();
		int m_nSelectedServer;
		DECLARE_EVENT_TABLE()
};

#endif
