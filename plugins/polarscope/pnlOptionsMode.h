#ifndef PNLOPTIONSMODE_H
#define PNLOPTIONSMODE_H

//(*Headers(pnlOptionsMode)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class PolarScopeBuilder;

class pnlOptionsMode: public wxPanel
{
	public:

		pnlOptionsMode(wxWindow* parent,PolarScopeBuilder* pBuilder,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptionsMode();

		//(*Declarations(pnlOptionsMode)
		wmList* m_plstMode;
		//*)

	protected:

		//(*Identifiers(pnlOptionsMode)
		static const long ID_M_PLST19;
		//*)

	private:

		//(*Handlers(pnlOptionsMode)
		void OnlstModeSelected(wxCommandEvent& event);
		//*)

		PolarScopeBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
