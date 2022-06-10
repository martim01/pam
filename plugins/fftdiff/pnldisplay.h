#ifndef PNLDISPLAY_H
#define PNLDISPLAY_H

//(*Headers(pnlDisplay)
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "wmbutton.h"

class FFTBuilder;

class pnlDisplay: public wxPanel
{
	public:

		pnlDisplay(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDisplay();

		//(*Declarations(pnlDisplay)
		wmList* m_plstFFT_Display;
		//*)
        wmButton* m_pbtnColour;

	protected:

		//(*Identifiers(pnlDisplay)
		static const long ID_M_PLST4;
		//*)

	private:

		//(*Handlers(pnlDisplay)
		void OnlstFFT_DisplaySelected(wxCommandEvent& event);
		//*)
        void OnbtnColour(wxCommandEvent& event);
        FFTBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
