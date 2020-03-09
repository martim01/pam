#ifndef DLGENGINEERING_H
#define DLGENGINEERING_H

//(*Headers(dlgEngineering)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/dialog.h>
//*)

class dlgEngineering: public wxDialog
{
	public:

		dlgEngineering(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgEngineering();

		//(*Declarations(dlgEngineering)
		wmButton* m_pbtnClose;
		wmButton* m_pbtnReset;
		wmLabel* m_pLbl1;
		//*)

	protected:

		//(*Identifiers(dlgEngineering)
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(dlgEngineering)
		void OnbtnCloseClick(wxCommandEvent& event);
		void OnbtnResetClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
