#ifndef POPHUB_PNLMODE_H
#define POPHUB_PNLMODE_H

//(*Headers(pnlMode)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class pophubBuilder;

class pnlMode: public wxPanel
{
	public:

		pnlMode(wxWindow* parent, pophubBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMode();

		wmList* m_plstMeters_Display;

	protected:

		static const long ID_M_PLST14;

	private:

		pophubBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
