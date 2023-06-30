#ifndef PNLSCALE_H
#define PNLSCALE_H

//(*Headers(pnlScale)
#include "wmbutton.h"
#include <wx/panel.h>
//*)
#include "wmlist.h"

class R128Builder;

class pnlScale: public wxPanel
{
	public:

		pnlScale(wxWindow* parent,R128Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlScale();

		wmButton* m_pbtnZero;
		wmList* m_plstMode;

	protected:

		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;

	private:

		R128Builder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
