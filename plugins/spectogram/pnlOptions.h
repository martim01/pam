#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class SpectogramBuilder;
class SettingEvent;
class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent, SpectogramBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

        void SelectCursorMode(bool bSelect);
		wmButton* m_pbtnLinear;

	protected:

    private:


		void OnbtnLinearClick(wxCommandEvent& event);

		SpectogramBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif

