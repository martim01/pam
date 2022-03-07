#ifndef PNLSETTINGSREMOTE_H
#define PNLSETTINGSREMOTE_H

//(*Headers(pnlSettingsRemote)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)

class SettingEvent;

class pnlSettingsRemote: public wxPanel
{
	public:

		pnlSettingsRemote(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId="");
		virtual ~pnlSettingsRemote();

		//(*Declarations(pnlSettingsRemote)
		wmButton* m_pbtnInterface;
		wmButton* m_pbtnRemote;
		wmButton* m_pbtnWebsocket;
		wmEdit* m_pedtPort;
		wmKeyboard* m_pkbd;
		wmLabel* m_pLbl1;
		//*)

	protected:

		//(*Identifiers(pnlSettingsRemote)
		static const long ID_M_PBTN22;
		static const long ID_M_PKBD2;
		static const long ID_M_PEDT3;
		static const long ID_M_PBTN6;
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlSettingsRemote)
		//*)
        void OnSettingChange(SettingEvent& event);
		DECLARE_EVENT_TABLE()
};

#endif
