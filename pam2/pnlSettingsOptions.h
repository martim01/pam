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
		//*)

	protected:

		//(*Identifiers(pnlSettingsOptions)
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlSettingsOptions)
		void OnbtnExitClick(wxCommandEvent& event);
		void OnbtnExitHeld(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
