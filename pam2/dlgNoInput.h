#ifndef DLGNOINPUT_H
#define DLGNOINPUT_H

//(*Headers(dlgNoInput)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/dialog.h>
//*)

class dlgNoInput: public wxDialog
{
	public:

		dlgNoInput(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgNoInput();

		//(*Declarations(dlgNoInput)
		wmButton* m_pbtnInput;
		wmLabel* m_pLbl1;
		//*)

	protected:

		//(*Identifiers(dlgNoInput)
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(dlgNoInput)
		void OnbtnInputClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
