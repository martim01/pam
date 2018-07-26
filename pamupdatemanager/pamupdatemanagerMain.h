#pragma once

//(*Headers(pamupdatemanagerDialog)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/html/htmlwin.h>
#include "wmlist.h"
//*)
#include "wxtouchscreenhtml.h"

class pamupdatemanagerDialog: public wxDialog
{
	public:

		pamupdatemanagerDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pamupdatemanagerDialog();

		//(*Declarations(pamupdatemanagerDialog)
		wmLabel* m_pLbl4;
		wmButton* m_pbtnDependencies;
		wmLabel* m_pLbl3;
		wmButton* m_pbtnSelectNone;
		wmButton* m_pbtnSelectAll;
		wxPanel* Panel4;
		wmLabel* m_pLbl1;
		wmList* m_plstLog;
		wxPanel* Panel1;
		wxPanel* Panel3;
		wmLabel* m_pLbl5;
		wmLabel* m_plblSettings;
		wmList* m_plstUpdates;
		wxTimer m_timerUpdate;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnChangelog;
		wmButton* m_pbtnUpdate;
		wxTouchScreenHtml* m_phtmlChangelog;
		wxPanel* Panel2;
		wxTouchScreenHtml* m_phtmlDependencies;
		wmSwitcherPanel* m_pswpMain;
		wmButton* m_pbtnClose;
		//*)

	protected:

		//(*Identifiers(pamupdatemanagerDialog)
		static const long ID_M_PLBL37;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST1;
		static const long ID_PANEL1;
		static const long ID_HTMLWINDOW1;
		static const long ID_PANEL2;
		static const long ID_HTMLWINDOW2;
		static const long ID_PANEL3;
		static const long ID_M_PLBL5;
		static const long ID_M_PLST2;
		static const long ID_PANEL4;
		static const long ID_M_PSWP1;
		static const long ID_TIMER1;
		//*)

	private:

		//(*Handlers(pamupdatemanagerDialog)
		void OnbtnCloseClick(wxCommandEvent& event);
		void OnbtnSelectAllClick(wxCommandEvent& event);
		void OnbtnSelectNoneClick(wxCommandEvent& event);
		void OnbtnChangelogClick(wxCommandEvent& event);
		void OnbtnDependenciesClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OnlstUpdatesSelected(wxCommandEvent& event);
		void OntimerUpdateTrigger(wxTimerEvent& event);
		//*)

        wxString GetMonitorPluginVersion(const wxString& sPlugin);
        wxString GetTestPluginVersion(const wxString& sPlugin);

        std::map<wxString, wxString> m_mUpdate;
        bool m_bUpdate;
		DECLARE_EVENT_TABLE()
};
