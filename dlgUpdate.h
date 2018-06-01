#ifndef DLGUPDATE_H
#define DLGUPDATE_H

//(*Headers(dlgUpdate)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include <wx/dialog.h>
#include <wx/html/htmlwin.h>
#include "wmlist.h"
//*)

class dlgUpdate: public wxDialog
{
	public:

		dlgUpdate(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgUpdate();

		//(*Declarations(dlgUpdate)
		wmButton* m_pbtnDependencies;
		wmLabel* m_pLbl3;
		wmButton* m_pbtnSelectNone;
		wmButton* m_pbtnSelectAll;
		wmLabel* m_pLbl1;
		wxPanel* Panel1;
		wxPanel* Panel3;
		wmLabel* m_plblSettings;
		wxHtmlWindow* m_phtmlDependencies;
		wmList* m_plstUpdates;
		wxHtmlWindow* m_phtmlChangelog;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnChangelog;
		wmButton* m_pbtnUpdate;
		wxPanel* Panel2;
		wmSwitcherPanel* m_pswpMain;
		wmButton* m_pbtnClose;
		//*)

	protected:

		//(*Identifiers(dlgUpdate)
		static const long ID_M_PLBL37;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST1;
		static const long ID_PANEL1;
		static const long ID_HTMLWINDOW1;
		static const long ID_PANEL2;
		static const long ID_HTMLWINDOW2;
		static const long ID_PANEL3;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(dlgUpdate)
		void OnbtnCloseClick(wxCommandEvent& event);
		void OnbtnSelectAllClick(wxCommandEvent& event);
		void OnbtnSelectNoneClick(wxCommandEvent& event);
		void OnbtnChangelogClick(wxCommandEvent& event);
		void OnbtnDependenciesClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OnlstUpdatesSelected(wxCommandEvent& event);
		//*)

        void ShowMonitorPluginVersion(const wxString& sPlugin);
        void ShowTestPluginVersion(const wxString& sPlugin);
		DECLARE_EVENT_TABLE()
};

#endif
