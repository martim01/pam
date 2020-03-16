#ifndef PNLSETTINGSOPTIONS_H
#define PNLSETTINGSOPTIONS_H

//(*Headers(pnlSettingsOptions)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class pnlSettingsOptions: public wxPanel
{
	public:

		pnlSettingsOptions(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettingsOptions();

		//(*Declarations(pnlSettingsOptions)
		wmButton* m_pbtnExit;
		wmButton* m_pbtnReboot;
		wmButton* m_pbtnShutdown;
		wmButton* m_pbtnTerminal;
		//*)

	protected:

		//(*Identifiers(pnlSettingsOptions)
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		//*)

	private:

		//(*Handlers(pnlSettingsOptions)
		void OnbtnExitClick(wxCommandEvent& event);
		void OnbtnExitHeld(wxCommandEvent& event);
		void OnbtnRebootHeld(wxCommandEvent& event);
		void OnbtnShutdownHeld(wxCommandEvent& event);
		void OnbtnTerminalClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
