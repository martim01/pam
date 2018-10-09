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
#include <map>
#include <wx/timer.h>
class pnlUpdate: public wxPanel
{
	public:

		pnlUpdate(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, unsigned long n=0, const wxString& s=wxEmptyString);
		virtual ~pnlUpdate();

		//(*Declarations(pnlUpdate)
		wmLabel* m_pLbl4;
		wmLabel* m_plblShareCurrent;
		wmLabel* m_pLbl3;
		wmButton* m_pbtnCheck;
		wmList* m_plstType;
		wmLabel* m_pLbl7;
		wmButton* m_pbtnShareSet;
		wmEdit* m_pedtHTTP;
		wmLabel* m_plblUSB;
		wxPanel* pnlShare;
		wmButton* m_pbtnDetect;
		wmLabel* m_plblShareSet;
		wmLabel* m_plblLocation;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl1;
		wmKeyboard* m_pKbd2;
		wxPanel* Panel1;
		wmButton* m_pbtnUp;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl8;
		wmButton* m_pbtnSetHttp;
		wmSwitcherPanel* m_pswpType;
		wmLabel* m_plblHTTPCurrent;
		wmLabel* m_plblLocalCurrent;
		wxPanel* pnlUSB;
		wmList* m_plstUsb;
		wxPanel* Panel2;
		wmButton* m_pbtnSelect;
		wmEdit* m_pedtShare;
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
		static const long ID_M_PLBL13;
		static const long ID_M_PBTN6;
		static const long ID_M_PLST3;
		static const long ID_M_PLBL12;
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
		void OnTimerUSB(wxTimerEvent& event);
		void ShowDirectories();
		void ShowDrives();

		void CheckUSB();

		wxTimer m_timerUSB;
		wxString m_sLocation;
		std::map<wxString, wxString> m_mUsb;
		DECLARE_EVENT_TABLE()
};

#endif
