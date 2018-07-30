#ifndef DLGEDITNAME_H
#define DLGEDITNAME_H

//(*Headers(dlgEditName)
#include "wmbutton.h"
#include "wmkeyboard.h"
#include <wx/panel.h>
#include <wx/dialog.h>
#include "wmedit.h"
#include "wmlist.h"
//*)

class dlgEditName: public wxDialog
{
	public:

		dlgEditName(wxWindow* parent,wxString sName, long nChannel=-1, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgEditName();

		//(*Declarations(dlgEditName)
		wxPanel* m_ppnlChannel;
		wmKeyboard* m_pKbd1;
		wmEdit* m_pedtName;
		wmButton* m_pbtnCancel;
		wmList* m_plstChannels;
		wmButton* m_pbtnSave;
		//*)

		unsigned long m_nChannels;

	protected:

		//(*Identifiers(dlgEditName)
		static const long ID_M_PKBD1;
		static const long ID_M_PEDT1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PLST1;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(dlgEditName)
		void OnedtNameText(wxCommandEvent& event);
		void OnedtNameTextEnter(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnlstChannelsSelected(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
