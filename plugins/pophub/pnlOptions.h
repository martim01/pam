#ifndef POPHUB_PNLOPTIONS_H
#define POPHUB_PNLOPTIONS_H

#include "wmbutton.h"
#include "wmlist.h"
#include "wmlabel.h"
#include <wx/panel.h>


class pophubBuilder;

class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

		wmButton* m_pbtnM3M6;
		wmLabel* m_plblSpeed;
		wmLabel* m_plblPeaks;
		wmList* m_plstSpeed;
		wmList* m_plstMeterPeaks;
		
		wmButton* m_pbtnLabelCurrent;
		wmButton* m_pbtnLabelPeak;

	protected:

	private:

		//(*Handlers(pnlOptions)
		void OnlstTextSelected(wxCommandEvent& event);
		//*)

		pophubBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
