#ifndef PNLSETTINGSPLUGINS_H
#define PNLSETTINGSPLUGINS_H

//(*Headers(pnlSettingsPlugins)
#include "wmbutton.h"
#include "wmlabel.h"
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
		wxPanel* Panel1;
		wmList* m_plstCurrent;
		wmButton* m_pbtnUp;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnDown;
		wmButton* m_pbtnSet;
		wmButton* m_pbtnPlugin;
		//*)

	protected:

		//(*Identifiers(pnlSettingsPlugins)
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_PANEL1;
		static const long ID_M_PBTN5;
		//*)

	private:

		//(*Handlers(pnlSettingsPlugins)
		void OnlstPossibleSelected(wxCommandEvent& event);
		void OnlstCurrentSelected(wxCommandEvent& event);
		void OnpbtnSetHeld(wxCommandEvent& event);
		void OnbtnClearHeld(wxCommandEvent& event);
		void OnbtnUpClick(wxCommandEvent& event);
		void OnbtnDownClick(wxCommandEvent& event);
		void OnPanel1Paint(wxPaintEvent& event);
		void OnbtnPluginClick(wxCommandEvent& event);
		//*)

		void ShowMonitorPlugins();
		void ShowTestPlugins();

		void ClearLists();


		std::map<wxString, wxString> m_mPossible;
        unsigned int m_nSelected;
        std::map<wxString, wxString>::iterator m_itPossible;
        unsigned int m_nPossible;

        bool m_bTests;
        wxString m_sSection;
		DECLARE_EVENT_TABLE()
};

#endif
