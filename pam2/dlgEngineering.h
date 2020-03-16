#ifndef DLGENGINEERING_H
#define DLGENGINEERING_H

//(*Headers(dlgEngineering)
#include "wmbutton.h"
#include <wx/dialog.h>
#include <wx/panel.h>
//*)
#include <wx/timer.h>
#include "wxbitmappanel.h"

class dlgEngineering: public wxDialog
{
	public:

		dlgEngineering(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgEngineering();

		//(*Declarations(dlgEngineering)
		wmButton* m_pbtnClose;
		wmButton* m_pbtnConfig;
		wmButton* m_pbtnFilesystem;
		wmButton* m_pbtnReset;
		wmButton* m_pbtnSoundcard;
		wxBitmapPanel* m_pBackground;
		//*)
	protected:

		//(*Identifiers(dlgEngineering)
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN1;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(dlgEngineering)
		void OnbtnCloseClick(wxCommandEvent& event);
		void OnbtnResetClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnbtnSoundcardClick(wxCommandEvent& event);
		void OnbtnConfigClick(wxCommandEvent& event);
		void OnbtnFilesystemClick(wxCommandEvent& event);
		//*)

		void OnTimeClose(wxTimerEvent& event);

		wxTimer m_timerClose;

		DECLARE_EVENT_TABLE()
};

#endif
