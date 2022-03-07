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

		wmList* m_plstMeters_Display;

	protected:

		static const long ID_M_PLST14;

	private:

		AngleMetersBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
