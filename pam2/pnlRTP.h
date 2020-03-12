#ifndef PNLRTP_H
#define PNLRTP_H

//(*Headers(pnlRTP)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/html/htmlwin.h>
#include <wx/notebook.h>
#include <wx/panel.h>
//*)

#include <set>
#include <queue>
#include "wxtouchscreenhtml.h"
#include "pnlUSB.h"

class DNSServiceBrowser;
class SapWatchThread;
class RtpThread;
class wmListAdv;
class SettingEvent;


class pnlRTP: public wxPanel
{
	public:

		pnlRTP(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlRTP();

		//(*Declarations(pnlRTP)
		pnlUSB* m_pnlUSB;
		wmButton* m_pbtnAdd;
		wmButton* m_pbtnCancel;
		wmButton* m_pbtnConfirm;
		wmButton* m_pbtnDelete;
		wmButton* m_pbtnDeleteAll;
		wmButton* m_pbtnDiscover;
		wmButton* m_pbtnImport;
		wmButton* m_pbtnImportBack;
		wmButton* m_pbtnImportImport;
		wmButton* m_pbtnManual;
		wmButton* m_pbtnSAP;
		wmButton* m_pbtnStartDiscovery;
		wmButton* m_pbtnUpdate;
		wmEdit* m_pedtName;
		wmEdit* m_pedtUrl;
		wmKeyboard* m_pkeyboard;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl7;
		wmLabel* m_pLbl8;
		wmLabel* m_plblDiscovering;
		wmLabel* m_plblImportProgress;
		wmList* m_plstFiles;
		wmList* m_plstServices;
		wmList* m_plstSources;
		wmSwitcherPanel* m_pSwp1;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* pnlDiscovery;
		wxTouchScreenHtml* m_phtmlResults;
		//*)

		wmListAdv* m_pList;


		void ListSources();

	protected:

		//(*Identifiers(pnlRTP)
		static const long ID_M_PLST1;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN11;
		static const long ID_M_PBTN6;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_PANEL1;
		static const long ID_M_PLBL1;
		static const long ID_M_PEDT1;
		static const long ID_M_PLBL2;
		static const long ID_M_PEDT2;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL6;
		static const long ID_PANEL2;
		static const long ID_M_PLBL7;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN9;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL8;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN10;
		static const long ID_HTMLWINDOW1;
		static const long ID_PANEL3;
		static const long ID_PANEL5;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST3;
		static const long ID_M_PBTN12;
		static const long ID_M_PBTN13;
		static const long ID_M_PLBL10;
		static const long ID_PANEL4;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlRTP)
		void OnbtnAddClick(wxCommandEvent& event);
		void OnedtNameTextEnter(wxCommandEvent& event);
		void OnedtUrlTextEnter(wxCommandEvent& event);
		void OnbtnConfirmClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		void OnlstSourcesSelected(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnDiscoverClick(wxCommandEvent& event);
		void OnbtnDeleteAllHeld(wxCommandEvent& event);
		void OnlstServicesSelected(wxCommandEvent& event);
		void OnbtnSAPClick(wxCommandEvent& event);
		void OnbtnStartDiscoveryClick(wxCommandEvent& event);
		void OnbtnManualClick(wxCommandEvent& event);
		void OnbtnImportClick(wxCommandEvent& event);
		void OnlstImportFilesSelected(wxCommandEvent& event);
		void OnbtnImportImportClick(wxCommandEvent& event);
		//*)

		void OnDiscovery(wxCommandEvent& event);
		void OnDiscoveryFinished(wxCommandEvent& event);
		void OnSap(wxCommandEvent& event);
		void OnSDPReceived(wxCommandEvent& event);

		void DecodeSap(const wxString& sData);

		void GetSDP(const wxString& sUrl);
		void GetSDP();
		void OnRTPClosed(wxCommandEvent& event);
		void OnSettingEvent(SettingEvent& event);

        void ImportSources(const wxString& sFileName);

        void FillInEdit();
		unsigned int m_nSelectedSource;


        DNSServiceBrowser* m_pBrowser;
        SapWatchThread* m_pSapWatch;
        size_t m_nDiscovered;
        std::set<std::pair<wxString, wxString> > m_setDiscover;

        std::queue<wxString> m_queueUrl;

        RtpThread* m_pThread;


        wxArrayString m_asServices;

		DECLARE_EVENT_TABLE()
};

#endif
