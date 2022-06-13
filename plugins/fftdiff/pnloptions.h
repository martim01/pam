#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class fftdiffBuilder;
class SettingEvent;
class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent, fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

        void SelectCursorMode(bool bSelect);
		wmButton* m_pbtnMax;
		wmButton* m_pbtnMaxReset;

		wmButton* m_pbtnMin;
		wmButton* m_pbtnMinReset;

		wmButton* m_pbtnAverage;
		wmButton* m_pbtnAverageReset;

	protected:

		static const long ID_M_PBTN1;
		static const long ID_M_PBTN34;

	private:


		void OnbtnMaxResetClick(wxCommandEvent& event);
		void OnbtnMinResetClick(wxCommandEvent& event);
		void OnbtnAverageResetClick(wxCommandEvent& event);

		fftdiffBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
