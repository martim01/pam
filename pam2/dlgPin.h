#ifndef DLGPIN_H
#define DLGPIN_H

//(*Headers(dlgPin)
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/dialog.h>
//*)

class dlgPin: public wxDialog
{
	public:

		dlgPin(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgPin();

		//(*Declarations(dlgPin)
		wmEdit* m_pedtPin;
		wmKeyboard* m_pkbd;
		wmLabel* m_pLbl1;
		//*)

	protected:

		//(*Identifiers(dlgPin)
		static const long ID_M_PLBL1;
		static const long ID_M_PEDT1;
		static const long ID_M_PKBD1;
		//*)

	private:

		//(*Handlers(dlgPin)
		void OnedtPinTextEnter(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
