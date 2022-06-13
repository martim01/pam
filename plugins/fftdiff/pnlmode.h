#ifndef PNLMODE_H
#define PNLMODE_H

//(*Headers(pnlMode)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class fftdiffBuilder;
class SettingEvent;
class pnlMode: public wxPanel
{
	public:

		pnlMode(wxWindow* parent, fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMode();

        void SelectCursorMode(bool bSelect);
		wmButton* m_pbtnFFT_Cursor;
		wmButton* m_pbtnFFT_Hold;


	protected:

		static const long ID_M_PBTN1;
		static const long ID_M_PBTN34;

	private:


		void OnbtnFFT_PeakResetClick(wxCommandEvent& event);

		fftdiffBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
