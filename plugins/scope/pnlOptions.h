#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmbutton.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class ScopeBuilder;
class SettingEvent;
class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent, ScopeBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

		//(*Declarations(pnlOptions)
		wmList* m_plstScope_Mode;
		wmButton* m_pbtnScope_Memory;
		wmButton* m_pbtnScope_ClearMemory;
		wmButton* m_pbtnScope_ResetSlide;
		wmButton* m_pbtnScope_Autotrigger;
		//*)

	protected:

		//(*Identifiers(pnlOptions)
		static const long ID_M_PLST21;
		static const long ID_M_PBTN30;
		static const long ID_M_PBTN32;
		static const long ID_M_PBTN29;
		static const long ID_M_PBTN33;
		//*)

	private:

		//(*Handlers(pnlOptions)
		void OnlstScope_ModeSelected(wxCommandEvent& event);
		void OnbtnScope_AutotriggerClick(wxCommandEvent& event);
		void OnbtnScope_ResetSlideClick(wxCommandEvent& event);
		void OnbtnScope_ClearMemoryClick(wxCommandEvent& event);
		void OnbtnScope_MemoryClick(wxCommandEvent& event);
		//*)

		void OnSettingChanged(SettingEvent& event);

		ScopeBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
