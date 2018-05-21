#ifndef PNLCONTROL_H
#define PNLCONTROL_H

//(*Headers(pnlControl)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class R128Builder;

class pnlControl: public wxPanel
{
	public:

		pnlControl(wxWindow* parent,R128Builder* pBuilder,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlControl();

		//(*Declarations(pnlControl)
		wmButton* m_pbtnCalculate;
		wmButton* m_pbtnReset;
		//*)

	protected:

		//(*Identifiers(pnlControl)
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		//*)

	private:

		//(*Handlers(pnlControl)
		void OnbtnCalculateClick(wxCommandEvent& event);
		void OnbtnResetClick(wxCommandEvent& event);
		//*)

        R128Builder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
