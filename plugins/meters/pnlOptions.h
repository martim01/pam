#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmbutton.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class MetersBuilder;

class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent,MetersBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

		//(*Declarations(pnlOptions)
		wmList* m_plstMeter_Peaks;
		wmButton* m_pbnFreeze;
		wmButton* m_pbtnMeterClear;
		//*)

	protected:

		//(*Identifiers(pnlOptions)
		static const long ID_M_PBTN11;
		static const long ID_M_PBTN12;
		static const long ID_M_PLST25;
		//*)

	private:

		//(*Handlers(pnlOptions)
		void OnlstMeter_PeaksSelected(wxCommandEvent& event);
		void OnbnFreezeClick(wxCommandEvent& event);
		void OnbtnMeterClearClick(wxCommandEvent& event);
		void OnlstM3M6Selected(wxCommandEvent& event);
		void OnlstSpeedSelected(wxCommandEvent& event);
		void OnbtnM3M6Click(wxCommandEvent& event);
		void OnbtnShadingClick(wxCommandEvent& event);
		//*)

		MetersBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
