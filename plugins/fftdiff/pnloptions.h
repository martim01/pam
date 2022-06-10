#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class FFTBuilder;
class SettingEvent;
class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent, FFTBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

        void SelectCursorMode(bool bSelect);
		wmButton* m_pbtnFFT_Cursor;
		wmButton* m_pbtnFFT_Hold;
		wmButton* m_pbtnFFT_Peak;
		wmButton* m_pbtnFFT_PeakReset;

		wmButton* m_pbtnFFT_Trough;
		wmButton* m_pbtnFFT_TroughReset;

	protected:

		static const long ID_M_PBTN1;
		static const long ID_M_PBTN34;

	private:


		void OnbtnFFT_PeakResetClick(wxCommandEvent& event);

		FFTBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
