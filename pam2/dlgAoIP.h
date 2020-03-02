#ifndef DLGAOIP_H
#define DLGAOIP_H

//(*Headers(dlgAoIP)
#include "wmbutton.h"
#include <wx/dialog.h>
#include <wx/panel.h>
//*)
#include "pnlRTP.h"

class dlgAoIP: public wxDialog
{
	public:

		dlgAoIP(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgAoIP();

		//(*Declarations(dlgAoIP)
		pnlRTP* m_ppnlRTP;
		wmButton* m_pbtnFinished;
		//*)

	protected:

		//(*Identifiers(dlgAoIP)
		static const long ID_PANEL1;
		static const long ID_M_PBTN7;
		//*)

	private:

		//(*Handlers(dlgAoIP)
		void OnbtnFinishedClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
