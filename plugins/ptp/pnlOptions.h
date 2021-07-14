#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmlist.h"
#include <wx/panel.h>
//*)
#include "wmlabel.h"
#include "wmbutton.h"

class ptpBuilder;
class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent, ptpBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

		//(*Declarations(pnlOptions)
		wmList* m_plstWindow;
		//*)
        wmList* m_plstGranularity;
        wmList* m_plstResolution;
        wmLabel* m_plblQoS;
        wmLabel* m_plblHR;
        wmLabel* m_plblHG;
        wmButton* m_pbtnClear;

	protected:

		//(*Identifiers(pnlOptions)
		static const long ID_M_PLST5;
		//*)

	private:

	    void OnbtnClearClick(wxCommandEvent& event);
        void OnlstGranularityPressed(wxCommandEvent& event);
        void OnlstResolutionPressed(wxCommandEvent& event);

		//(*Handlers(pnlOptions)
		void OnlstWindowSelected(wxCommandEvent& event);
		//*)
        ptpBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
