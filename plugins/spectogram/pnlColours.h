#ifndef PNLCOLOURS_H
#define PNLCOLOURS_H

//(*Headers(pnlColours)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class SpectogramBuilder;

class pnlColours: public wxPanel
{
	public:

		pnlColours(wxWindow* parent,SpectogramBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlColours();

		//(*Declarations(pnlColours)
		wmList* m_plstColours;
		//*)

	protected:

		//(*Identifiers(pnlColours)
		static const long ID_M_PLST3;
		//*)

	private:


		SpectogramBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
