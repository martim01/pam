#ifndef PNLSETTINGSPLUGINS_H
#define PNLSETTINGSPLUGINS_H

//(*Headers(pnlSettingsPlugins)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class pnlSettingsPlugins: public wxPanel
{
	public:

		pnlSettingsPlugins(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlSettingsPlugins();

		//(*Declarations(pnlSettingsPlugins)
		wmList* m_plstPossible;
		wmButton* m_pbtnClear;
		wmLabel* m_pLbl1;
		wmSwitcherPanel* m_pswpPlugins;
		wxPanel* Panel1;
		wmList* m_plstCurrent;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnSet;
		//*)

	protected:

		//(*Identifiers(pnlSettingsPlugins)
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_PANEL1;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlSettingsPlugins)
		void OnlstPossibleSelected(wxCommandEvent& event);
		void OnlstCurrentSelected(wxCommandEvent& event);
		void OnpbtnSetHeld(wxCommandEvent& event);
		void OnbtnClearHeld(wxCommandEvent& event);
		//*)

		std::map<wxString, wxString> m_mPossible;
        unsigned int m_nSelected;
        std::map<wxString, wxString>::iterator m_itPossible;
		DECLARE_EVENT_TABLE()
};

#endif
