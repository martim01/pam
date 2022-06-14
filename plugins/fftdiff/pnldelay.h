#ifndef PNLDELAY_H
#define PNLDELAY_H

//(*Headers(pnlDelay)
#include "wmbutton.h"
#include <wx/panel.h>
//*)
#include "wmlist.h"
#include "wmlabel.h"
class fftdiffBuilder;
class SettingEvent;
class pnlDelay: public wxPanel
{
	public:

		pnlDelay(wxWindow* parent, fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDelay();

        void SelectCursorMode(bool bSelect);

		wmList* m_plstMode;

		wmLabel* m_plblWindow;
		wmList* m_plstWindow;

		wmLabel* m_plblAccuracy;
		wmList* m_plstAccuracy;
		wmButton* m_pbtnCalculate;

	protected:


	private:


		void OnbtnCalculateClick(wxCommandEvent& event);

		fftdiffBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
