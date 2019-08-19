#ifndef WMIPEDITPNL_H
#define WMIPEDITPNL_H

//(*Headers(wmipeditpnl)
#include <wx/panel.h>
#include "wmedit.h"
//*)
#include "pmpanel.h"


class PAMBASE_IMPEXPORT wmipeditpnl: public pmPanel
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmipeditpnl)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmipeditpnl);
    #endif // WXSPAM
	public:

	    wmipeditpnl();

		wmipeditpnl(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, unsigned long nStyle=0, const wxString& sEmpty=wxEmptyString);
		virtual ~wmipeditpnl();

		wxString GetValue();
		void SetValue(const wxString& sValue);

		//(*Declarations(wmipeditpnl)
		wmEdit* m_pEdt2;
		wmEdit* m_pEdt1;
		wmEdit* m_pEdt4;
		wmEdit* m_pEdt3;
		//*)

		bool SetBackgroundColour(const wxColour& colour);
		bool SetForegroundColour(const wxColour& colour);
        bool Enable(bool bEnable=true);
	protected:

		//(*Identifiers(wmipeditpnl)
		static const long ID_M_PEDT1;
		static const long ID_M_PEDT2;
		static const long ID_M_PEDT3;
		static const long ID_M_PEDT4;
		//*)

	private:

		//(*Handlers(wmipeditpnl)
		void Onm_pEdt1Text(wxCommandEvent& event);
		void Onm_pEdt2Text(wxCommandEvent& event);
		void Onm_pEdt3Text(wxCommandEvent& event);
		void Onm_pEdt4Text(wxCommandEvent& event);
		void OnSetFocus(wxFocusEvent& event);
		void OnResize(wxSizeEvent& event);
		//*)

		void Onm_pEdt1TextBack(wxCommandEvent& event);
		void Onm_pEdt2TextBack(wxCommandEvent& event);
		void Onm_pEdt3TextBack(wxCommandEvent& event);
		void Onm_pEdt4TextBack(wxCommandEvent& event);

		void Onm_pEdt1TextTab(wxCommandEvent& event);
		void Onm_pEdt2TextTab(wxCommandEvent& event);
		void Onm_pEdt3TextTab(wxCommandEvent& event);
		void Onm_pEdt4TextTab(wxCommandEvent& event);

		void CheckEdit(wmEdit* pCurrent, wmEdit* pNext);

		void SizeRects();

		DECLARE_EVENT_TABLE()
};

#endif
