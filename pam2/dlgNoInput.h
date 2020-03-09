#ifndef DLGNOINPUT_H
#define DLGNOINPUT_H

//(*Headers(dlgNoInput)
#include "wmbutton.h"
#include <wx/dialog.h>
//*)
#include <wx/timer.h>

class dlgNoInput: public wxDialog
{
	public:

		dlgNoInput(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgNoInput();

		//(*Declarations(dlgNoInput)
		wmButton* m_pbtnInput;
		//*)

	protected:

		//(*Identifiers(dlgNoInput)
		static const long ID_M_PBTN1;
		//*)
        void OnTimerReset(wxTimerEvent& event);
	private:

		//(*Handlers(dlgNoInput)
		void OnbtnInputClick(wxCommandEvent& event);
		//*)

		wxTimer m_timerReset;
		wxString m_sInput;
		DECLARE_EVENT_TABLE()
};

#endif
