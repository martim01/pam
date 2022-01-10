#pragma once

//(*Headers(pamupdatemanagerDialog)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/timer.h>
//*)
#include "wmlistadv.h"
#include <wx/filename.h>

class pamupdatemanagerDialog: public wxDialog
{
	public:

		pamupdatemanagerDialog(wxWindow* parent, const wxString& sDevice, const wxFileName& fnUpdate, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pamupdatemanagerDialog();

		//(*Declarations(pamupdatemanagerDialog)
		wmButton* m_pbtnCancel;
		wmButton* m_pbtnUpdate;
		wmEdit* m_pedtPassword;
		wmKeyboard* m_pkeyboard;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_plblTitle;
		wmSwitcherPanel* m_pswpMain;
		wxPanel* m_ppnlPassword;
		wxPanel* m_ppnlProgress;
		wxPanel* m_ppnlRelease;
		wxTimer m_timerStart;
		//*)

		wmListAdv* m_plstRelease;

	protected:

		//(*Identifiers(pamupdatemanagerDialog)
		static const long ID_M_PLBL2;
		static const long ID_PANEL1;
		static const long ID_M_PLBL3;
		static const long ID_PANEL2;
		static const long ID_M_PEDT1;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL1;
		static const long ID_PANEL3;
		static const long ID_M_PSWP1;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN1;
		static const long ID_TIMER2;
		//*)

	private:

		//(*Handlers(pamupdatemanagerDialog)
		void OnedtPasswordTextEnter(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OntimerStartTrigger(wxTimerEvent& event);
		//*)

        wxString m_sDevice;
        wxFileName m_fnUpdate,

		DECLARE_EVENT_TABLE()
};
