#ifndef DLGWAV_H
#define DLGWAV_H

//(*Headers(dlgWav)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
//*)
#include "usbchecker.h"


class dlgWav: public wxDialog
{
	public:

		dlgWav(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgWav();

		//(*Declarations(dlgWav)
		pnlUSB* m_pnlUSB;
		wmButton* m_pbtnClose;
		wmButton* m_pbtnDelete;
		wmButton* m_pbtnEnd;
		wmButton* m_pbtnHome;
		wmButton* m_pbtnImport;
		wmButton* m_pbtnImportBack;
		wmButton* m_pbtnNext;
		wmButton* m_pbtnPrevious;
		wmButton* m_pbtnRename;
		wmButton* m_pbtnSave;
		wmLabel* m_plblImportProgress;
		wmList* m_plstFiles;
		wmSwitcherPanel* m_pSwp1;
		wxPanel* Panel1;
		wxPanel* Panel2;
		//*)

	protected:

		//(*Identifiers(dlgWav)
		static const long ID_M_PLST1;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN9;
		static const long ID_PANEL2;
		static const long ID_PANEL5;
		static const long ID_M_PLBL10;
		static const long ID_M_PBTN13;
		static const long ID_PANEL1;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(dlgWav)
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnRenameClick(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		void OnbtnImportClick(wxCommandEvent& event);
		void OnlstFilesSelected(wxCommandEvent& event);
		void OnbtnCloseClick(wxCommandEvent& event);
		//*)
		void OnUsbFinished(wxCommandEvent& event);
        void EnableButtons();
        void ImportWavFile(const wxFileName& fnWav);
        void OnbtnImportImportClick(wxCommandEvent& event);

		UsbChecker m_usb;
		bool m_bTransfer;

		DECLARE_EVENT_TABLE()
};

#endif
