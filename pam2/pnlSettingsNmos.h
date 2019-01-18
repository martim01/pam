#ifndef PNLSETTINGSNMOS_H
#define PNLSETTINGSNMOS_H

//(*Headers(pnlSettingsNmos)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class pnlSettingsNmos: public wxPanel
{
	public:

		pnlSettingsNmos(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlSettingsNmos();

		//(*Declarations(pnlSettingsNmos)
		wmButton* m_pbtnClient;
		wmButton* m_pbtnNmos;
		//*)

	protected:

		//(*Identifiers(pnlSettingsNmos)
		static const long ID_M_PBTN22;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlSettingsNmos)
		void OnbtnNmosClick(wxCommandEvent& event);
		void OnbtnClientClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
