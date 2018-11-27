#ifndef PNLSETTINGSPLUGINS_H
#define PNLSETTINGSPLUGINS_H

//(*Headers(pnlSettingsPlugins)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

#include "plugin.h"

class pnlSettingsPlugins: public wxPanel
{
	public:

		pnlSettingsPlugins(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlSettingsPlugins();

		//(*Declarations(pnlSettingsPlugins)
		wmList* m_plstPossible;
		wmButton* m_pbtnClear;
		wmLabel* m_pLbl1;
		wmButton* m_pbtnPossiblePrevious;
		wxPanel* Panel1;
		wmList* m_plstCurrent;
		wmButton* m_pbtnUp;
		wmButton* m_pbtnPossibleNext;
		wmList* m_plstPluginType;
		wmLabel* m_plblDetails;
		wmButton* m_pbtnApply;
		wmLabel* m_pLbl2;
		wmLabel* m_plblCurrentVersion;
		wmButton* m_pbtnCurrentPrevious;
		wmButton* m_pbtnDown;
		wmLabel* m_plblVersion;
		wmButton* m_pbtnCurrentNext;
		wmButton* m_pbtnSet;
		wmLabel* m_plblCurrentDetails;
		//*)

	protected:

		//(*Identifiers(pnlSettingsPlugins)
		static const long ID_M_PLST3;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN9;
		static const long ID_M_PBTN10;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN6;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN8;
		static const long ID_PANEL1;
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
		void OnbtnApplyHeld(wxCommandEvent& event);
		void OnlstPossiblePaged(wxCommandEvent& event);
		void OnlstCurrentPaged(wxCommandEvent& event);
		void OnbtnPossiblePreviousClick(wxCommandEvent& event);
		void OnbtnPossibleNextClick(wxCommandEvent& event);
		void OnbtnCurrentPreviousClick(wxCommandEvent& event);
		void OnbtnCurrentNextClick(wxCommandEvent& event);
		void OnlstPluginTypeSelected(wxCommandEvent& event);
		//*)

		void ShowMonitorPlugins();
		void ShowTestPlugins();
		void ShowGeneratorPlugins();

		void ClearLists();

		void EnablePageButtons();



		std::map<wxString, plugin> m_mPossible;
        unsigned int m_nSelected;
        std::map<wxString, plugin>::iterator m_itPossible;
        unsigned int m_nPossible;

        int m_nType;
        wxString m_sSection;
		DECLARE_EVENT_TABLE()
};


DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_PLUGINS_APPLY,-1)
#endif
