#ifndef PNLSETTINGSINPUT_H
#define PNLSETTINGSINPUT_H

//(*Headers(pnlSettingsInput)
#include <wx/panel.h>
//*)

class pnlSettingsInput: public wxPanel
{
	public:

		pnlSettingsInput(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettingsInput();

		//(*Declarations(pnlSettingsInput)
		//*)

	protected:

		//(*Identifiers(pnlSettingsInput)
		//*)

	private:

		//(*Handlers(pnlSettingsInput)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
