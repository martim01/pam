#ifndef PNLSCALE_H
#define PNLSCALE_H

//(*Headers(pnlScale)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class R128Builder;

class pnlScale: public wxPanel
{
	public:

		pnlScale(wxWindow* parent,R128Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlScale();

		//(*Declarations(pnlScale)
		wmButton* m_pbtnScale;
		wmButton* m_pbtnZero;
		//*)

	protected:

		//(*Identifiers(pnlScale)
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		//*)

	private:

		//(*Handlers(pnlScale)
		void OnbtnScaleClick(wxCommandEvent& event);
		void OnbtnZeroClick(wxCommandEvent& event);
		//*)

		R128Builder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
