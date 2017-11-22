#ifndef PNLWINDOW_H
#define PNLWINDOW_H

//(*Headers(pnlWindow)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class FFTBuilder;

class pnlWindow: public wxPanel
{
	public:

		pnlWindow(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlWindow();

		//(*Declarations(pnlWindow)
		wmList* m_plstFFT_Window;
		//*)

	protected:

		//(*Identifiers(pnlWindow)
		static const long ID_M_PLST5;
		//*)

	private:

		//(*Handlers(pnlWindow)
		void OnlstFFT_WindowSelected(wxCommandEvent& event);

		//*)

        FFTBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
