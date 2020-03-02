#ifndef PNLUPDATE_H
#define PNLUPDATE_H

//(*Headers(pnlUpdate)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)
#include <wx/dir.h>
#include <map>
#include <wx/timer.h>
#include "pnlUSB.h"
class pnlUpdate: public wxPanel
{
	public:

		pnlUpdate(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, unsigned long n=0, const wxString& s=wxEmptyString);
		virtual ~pnlUpdate();

		//(*Declarations(pnlUpdate)
		pnlUSB* m_pnlUSB;
		wmButton* m_pbtnCheck;
		wmButton* m_pbtnSelect;
		wmButton* m_pbtnSetHttp;
		wmButton* m_pbtnShareSet;
		wmButton* m_pbtnUp;
		wmEdit* m_pedtHTTP;
		wmEdit* m_pedtShare;
		wmKeyboard* m_pKbd1;
		wmKeyboard* m_pKbd2;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl8;
		wmLabel* m_plblHTTPCurrent;
		wmLabel* m_plblLocalCurrent;
		wmLabel* m_plblLocation;
		wmLabel* m_plblShareCurrent;
		wmLabel* m_plblShareSet;
		wmList* m_plstFolders;
		wmList* m_plstType;
		wmSwitcherPanel* m_pswpType;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* pnlShare;
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
		static const long ID_M_PLBL10;
		static const long ID_M_PEDT2;
		static const long ID_M_PLBL11;
		static const long ID_M_PBTN5;
		static const long ID_PANEL3;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL4;
		static const long ID_PANEL2;
		static const long ID_PANEL4;
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
		void OnbtnShareSetClick(wxCommandEvent& event);
		void OnbtnDetectClick(wxCommandEvent& event);
		void OnlstUsbSelected(wxCommandEvent& event);
		//*)
        void OnShown(wxShowEvent& event);

		void ShowDirectories();
		void ShowDrives();


		wxString m_sLocation;

		DECLARE_EVENT_TABLE()
};

#endif
