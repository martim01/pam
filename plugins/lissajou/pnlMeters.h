#ifndef PNLMETERS_H
#define PNLMETERS_H

//(*Headers(pnlMeters)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class LissajouBuilder;

class pnlMeters: public wxPanel
{
	public:

		pnlMeters(wxWindow* parent, LissajouBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMeters();

		//(*Declarations(pnlMeters)
		wmList* m_plstMode;
		//*)

	protected:

		//(*Identifiers(pnlMeters)
		static const long ID_M_PLST19;
		//*)

	private:

		//(*Handlers(pnlMeters)
		void OnlstMetersSelected(wxCommandEvent& event);
		void OnlstMeterTypeSelected(wxCommandEvent& event);
		//*)
		LissajouBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
