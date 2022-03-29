#ifndef PNLAOIPMANUAL_H
#define PNLAOIPMANUAL_H

//(*Headers(pnlAoipManual)
#include "pnlChannelMapping.h"
#include "wmbutton.h"
#include "wmedit.h"
#include "wmipeditpnl.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)

class pnlAoipManual: public wxPanel
{
	public:

		pnlAoipManual(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlAoipManual();

		//(*Declarations(pnlAoipManual)
		pnlChannelMapping* m_ppnlMapping;
		wmButton* m_pbtnBits;
		wmButton* m_pbtnChannels;
		wmButton* m_pbtnMapping;
		wmButton* m_pbtnRtpMap;
		wmButton* m_pbtnSampleRate;
		wmButton* m_pbtnStream;
		wmEdit* m_pedtPort;
		wmIpEditPnl* m_pipServer;
		wmKeyboard* m_pkbd;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl8;
		wmLabel* m_pLbl9;
		wmSwitcherPanel* m_pswpMain;
		wxPanel* m_ppnlSettings;
		//*)

	protected:

		//(*Identifiers(pnlAoipManual)
		static const long ID_M_PLBL8;
		static const long ID_M_PIP1;
		static const long ID_M_PLBL1;
		static const long ID_M_PEDT3;
		static const long ID_M_PLBL2;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL4;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN13;
		static const long ID_M_PKBD2;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL11;
		static const long ID_M_PBTN4;
		static const long ID_PANEL1;
		static const long ID_CHANNEL_MAPPING;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlAoipManual)
		void OnbtnSampleRateClick(wxCommandEvent& event);
		void OnbtnStreamClick(wxCommandEvent& event);
		void OnbtnRtpMapClick(wxCommandEvent& event);
		void OnbtnChannelsClick(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnMappingClick(wxCommandEvent& event);
		//*)
		void OnbtnBitsClick(wxCommandEvent& event);
		void OnIpChanged(wxCommandEvent& event);
		void OnedtRTPPort(wxCommandEvent& event);

		void OnbtnBackClick(wxCommandEvent& event);

        wxString CreateSDP();

		void OnSettingChanged(SettingEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
