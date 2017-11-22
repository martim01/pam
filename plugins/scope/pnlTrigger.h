#ifndef PNLTRIGGER_H
#define PNLTRIGGER_H

//(*Headers(pnlTrigger)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class ScopeBuilder;

class pnlTrigger: public wxPanel
{
	public:

		pnlTrigger(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlTrigger();

		//(*Declarations(pnlTrigger)
		wmButton* m_pbtnTriggerDown1000;
		wmButton* m_pbtnTriggerDown1;
		wmButton* m_pbtnTriggerUp100;
		wmButton* m_pbtnTriggerDown10;
		wmButton* m_pbtnTriggerUp1000;
		wmButton* m_pbtnTriggerUp10;
		wmButton* m_pbtnTriggerUp1;
		wmButton* m_pbtnTriggerDown100;
		//*)

		void EnableButtons(bool bEnable);
	protected:

		//(*Identifiers(pnlTrigger)
		static const long ID_M_PBTN21;
		static const long ID_M_PBTN22;
		static const long ID_M_PBTN23;
		static const long ID_M_PBTN24;
		static const long ID_M_PBTN25;
		static const long ID_M_PBTN26;
		static const long ID_M_PBTN27;
		static const long ID_M_PBTN28;
		//*)

	private:

		//(*Handlers(pnlTrigger)
		void OnbtnTriggerUp1000Click(wxCommandEvent& event);
		void OnbtnTriggerDown1000Click(wxCommandEvent& event);
		void OnbtnTriggerUp100Click(wxCommandEvent& event);
		void OnbtnTriggerDown100Click(wxCommandEvent& event);
		void OnbtnTriggerUp10Click(wxCommandEvent& event);
		void OnbtnTriggerDown10Click(wxCommandEvent& event);
		void OnbtnTriggerUp1Click(wxCommandEvent& event);
		void OnbtnTriggerDown1Click(wxCommandEvent& event);
		//*)

		ScopeBuilder* m_pBuilder;

		void AlterTrigger(double dMod);

		DECLARE_EVENT_TABLE()
};

#endif
