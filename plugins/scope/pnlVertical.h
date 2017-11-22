#ifndef PNLVERTICAL_H
#define PNLVERTICAL_H

//(*Headers(pnlVertical)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class ScopeBuilder;

class pnlVertical: public wxPanel
{
	public:

		pnlVertical(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlVertical();

		//(*Declarations(pnlVertical)
		wmButton* m_pbtnVerticalOut10;
		wmButton* m_pbtnVerticalOut2;
		wmButton* m_pbtnVerticalIn1;
		wmButton* m_pbtnVerticalIn10;
		wmButton* m_pbtnVerticalIn2;
		wmButton* m_pbtnVerticalOut1;
		wmButton* m_pbtnVerticalOut4;
		wmButton* m_pbtnVerticalIn4;
		//*)

	protected:

		//(*Identifiers(pnlVertical)
		static const long ID_M_PBTN13;
		static const long ID_M_PBTN14;
		static const long ID_M_PBTN15;
		static const long ID_M_PBTN16;
		static const long ID_M_PBTN17;
		static const long ID_M_PBTN18;
		static const long ID_M_PBTN19;
		static const long ID_M_PBTN20;
		//*)

	private:

		//(*Handlers(pnlVertical)
		void OnbtnVerticalIn10Click(wxCommandEvent& event);
		void OnbtnVerticalOut10Click(wxCommandEvent& event);
		void OnbtnVerticalIn4Click(wxCommandEvent& event);
		void OnbtnVerticalOut4Click(wxCommandEvent& event);
		void OnbtnVerticalIn2Click(wxCommandEvent& event);
		void OnbtnVerticalOut2Click(wxCommandEvent& event);
		void OnbtnVerticalIn1Click(wxCommandEvent& event);
		void OnbtnVerticalOut1Click(wxCommandEvent& event);
		//*)

		ScopeBuilder* m_pBuilder;

		void AlterVerticalZoom(double dMod);

		DECLARE_EVENT_TABLE()
};

#endif
