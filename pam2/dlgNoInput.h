#ifndef DLGNOINPUT_H
#define DLGNOINPUT_H

//(*Headers(dlgNoInput)
#include "wmlabel.h"
#include <wx/dialog.h>
//*)

class dlgNoInput: public wxDialog
{
	public:

		dlgNoInput(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgNoInput();

		//(*Declarations(dlgNoInput)
		wmLabel* m_pLbl1;
		//*)

	protected:

		//(*Identifiers(dlgNoInput)
		static const long ID_M_PLBL1;
		//*)

	private:

		//(*Handlers(dlgNoInput)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
