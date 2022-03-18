#ifndef PNLR128METERS_H
#define PNLR128METERS_H

//(*Headers(pnlMeters)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class R128Builder;

class pnlMeters: public wxPanel
{
	public:

		pnlMeters(wxWindow* parent, R128Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
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
		R128Builder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
