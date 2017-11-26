#ifndef PNLMODE_H
#define PNLMODE_H

//(*Headers(pnlMode)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class AngleMetersBuilder;

class pnlMode: public wxPanel
{
	public:

		pnlMode(wxWindow* parent, AngleMetersBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMode();

		//(*Declarations(pnlMode)
		wmList* m_plstMeters_Display;
		//*)

	protected:

		//(*Identifiers(pnlMode)
		static const long ID_M_PLST14;
		//*)

	private:

		//(*Handlers(pnlMode)
		void OnlstMeters_DisplaySelected(wxCommandEvent& event);
		//*)

		AngleMetersBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
