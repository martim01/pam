#pragma once

#include "wmbutton.h"
#include "wmkeyboard.h"
#include <wx/panel.h>
#include <wx/dialog.h>
#include "wmedit.h"
#include "wmlist.h"

class dlgWiFi : public wxDialog
{
	public:

		dlgWiFi(wxWindow* parent, const wxString& sWiFi, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgWiFi();

		wmKeyboard* m_pKbd1;
		wmEdit* m_pedtPassword;
		wmButton* m_pbtnCancel;
		wmList* m_plstWiFi;
		wmButton* m_pbtnSave;
		//*)

		unsigned long m_nChannels;

	protected:


	private:

		void OnedtPasswordText(wxCommandEvent& event);
		void OnedtPasswordTextEnter(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnlstWiFiSelected(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

