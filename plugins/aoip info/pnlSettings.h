#ifndef PNLSETTINGS_H
#define PNLSETTINGS_H

//(*Headers(pnlSettings)
#include "wmlist.h"
#include <wx/panel.h>
//*)
#include "wmlabel.h"
class AoIPInfoBuilder;

class pnlSettings: public wxPanel
{
	public:

		pnlSettings(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettings();

		//(*Declarations(pnlSettings)
		wmList* m_plstInterval;
		//*)
        wmList* m_plstGranularity;
        wmList* m_plstResolution;
        wmLabel* m_plblQoS;
        wmLabel* m_plblHR;
        wmLabel* m_plblHG;
	protected:

		//(*Identifiers(pnlSettings)
		static const long ID_M_PLST16;
		//*)

	private:

		//(*Handlers(pnlSettings)
		void OnlstIntervalSelected(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)
		void OnlstGranularityPressed(wxCommandEvent& event);
		void OnlstResolutionPressed(wxCommandEvent& event);

		AoIPInfoBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
