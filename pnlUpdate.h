#ifndef PNLUPDATE_H
#define PNLUPDATE_H

//(*Headers(pnlUpdate)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmedit.h"
#include "wmlist.h"
//*)
#include <wx/dir.h>

class pnlUpdate: public wxPanel
{
	public:

		pnlUpdate(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, unsigned long n=0, const wxString& s=wxEmptyString);
		virtual ~pnlUpdate();

		//(*Declarations(pnlUpdate)
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl3;
		wmEdit* m_pEdt2;
		wmButton* m_pbtnCheck;
		wmList* m_plstType;
		wmEdit* m_pedtHTTP;
		wmLabel* m_plblLocation;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl1;
		wmKeyboard* m_pKbd2;
		wxPanel* Panel1;
		wmButton* m_pbtnUp;
		wxPanel* Panel3;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmButton* m_pbtnSetHttp;
		wmSwitcherPanel* m_pswpType;
		wmLabel* m_pLbl2;
		wmLabel* m_plblHTTPCurrent;
		wmLabel* m_plblLocalCurrent;
		wxPanel* Panel2;
		wmButton* m_pbtnSelect;
		wmList* m_plstFolders;
		//*)

	protected:

		//(*Identifiers(pnlUpdate)
		static const long ID_M_PLBL3;
		static const long ID_M_PLST2;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL5;
		static const long ID_M_PEDT1;
		static const long ID_M_PLBL6;
		static const long ID_M_PBTN4;
		static const long ID_PANEL1;
		static const long ID_M_PKBD2;
		static const long ID_M_PLBL2;
		static const long ID_M_PEDT2;
		static const long ID_PANEL3;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL4;
		static const long ID_PANEL2;
		static const long ID_M_PSWP1;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlUpdate)
		void OnlstTypeSelected(wxCommandEvent& event);
		void OnlstFoldersSelected(wxCommandEvent& event);
		void OnbtnUpClick(wxCommandEvent& event);
		void OnbtnSelectClick(wxCommandEvent& event);
		void OnbtnCheckClick(wxCommandEvent& event);
		void OnbtnSetHttpClick(wxCommandEvent& event);
		//*)

		void ShowDirectories();

		void ShowDrives();
		wxString m_sLocation;
		DECLARE_EVENT_TABLE()
};

#endif
