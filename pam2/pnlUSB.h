#ifndef PNLUSB_H
#define PNLUSB_H
#include <wx/timer.h>
#include "usbchecker.h"

//(*Headers(pnlUSB)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

class pnlUSB: public wxPanel
{
	public:

		pnlUSB(wxWindow* parent, const wxString& sFileType, const wxString& sSelectLabel, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int nStyle=0,  const wxString& sN=wxEmptyString);
		virtual ~pnlUSB();

		void SetSection(const wxString& sSection){}
		void StartCheck();

		wmLabel* m_plblUSB;
        wmList* m_plstLog;
        wmButton* m_pbtnCheck;
        wmButton* m_pbtnCancel;
		wmButton* m_pbtnUpload;
		wmList* m_plstFiles;
		wmLabel* m_pstHostname;

        wxString m_sSelectedFile;
        wxString m_sSelectedDevice;

	protected:

		wxString m_sFilename;

	private:


		void OnbtnCancelClick(wxCommandEvent& event);

		void OnbtnCheckClick(wxCommandEvent& event);


		void OnUsbFound(const wxCommandEvent& event);
		void OnUsbFileFound(const wxCommandEvent& event);
		void OnUsbFinished(const wxCommandEvent& event);
		void OnUsbError(const wxCommandEvent& event);
        void OnFileSelected(const wxCommandEvent& event);


		void CheckUSB();

		UsbChecker m_checker;

		std::map<wxString, wxString> m_mFiles;

		DECLARE_EVENT_TABLE()
};

#endif
