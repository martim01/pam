#ifndef PNLSETTINGSPROFILES_H
#define PNLSETTINGSPROFILES_H

//(*Headers(pnlSettingsProfiles)
#include "wmbutton.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include <wx/filename.h>


class pnlSettingsProfiles: public wxPanel
{
	public:

		pnlSettingsProfiles(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlSettingsProfiles();

		//(*Declarations(pnlSettingsProfiles)
		wmButton* m_pbtnRename;
		wmList* m_plstProfiles;
		wmButton* m_pbtnDelete;
		wmButton* m_pbtnSave;
		wmButton* m_pbtnLoad;
		//*)

	protected:

		//(*Identifiers(pnlSettingsProfiles)
		static const long ID_M_PLST1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN2;
		//*)

	private:

		//(*Handlers(pnlSettingsProfiles)
		void OnlstProfilesSelected(wxCommandEvent& event);
		void OnbtnLoadClick(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnRenameClick(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		//*)

		void PopulateList();
        bool SaveFile(wxFileName fnFrom, wxFileName fnTo);

		wxFileName m_fnSelected;

		wxFileName m_fnPam;
		DECLARE_EVENT_TABLE()
};

#endif
