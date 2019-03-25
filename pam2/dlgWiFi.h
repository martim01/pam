#pragma once

#include "wmbutton.h"
#include "wmkeyboard.h"
#include <wx/panel.h>
#include <wx/dialog.h>
#include "wmedit.h"
#include "wmlist.h"
#include "wmlabel.h"
#include <map>

class dlgWiFi : public wxDialog
{
	public:

		dlgWiFi(wxWindow* parent, const wxString& sWiFi, const wxString& sInterface, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgWiFi();

		wmKeyboard* m_pKbd1;
		wmEdit* m_pedtPassword;
		wmButton* m_pbtnCancel;
		wmList* m_plstWiFi;
		wmButton* m_pbtnSave;
		wmButton* m_pbtnScan;
		//*)
        wmLabel* m_plblPassword;
		unsigned long m_nChannels;

	protected:

        void Scan();

	private:

		void OnedtPasswordText(wxCommandEvent& event);
		void OnedtPasswordTextEnter(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnScanClick(wxCommandEvent& event);
		void OnlstWiFiSelected(wxCommandEvent& event);

		wxString m_sWifi;
		wxString m_sInterface;
		DECLARE_EVENT_TABLE()
};

