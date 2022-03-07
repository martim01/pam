#ifndef PNLOFFSET_H
#define PNLOFFSET_H

//(*Headers(pnlRouting)
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "wmbutton.h"
#include "wmlabel.h"
class ScopeBuilder;

class pnlOffset: public wxPanel
{
	public:

		pnlOffset(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOffset();


        wmLabel* m_plblOffset;
        wmLabel* m_plblMove;
		wmList* m_plstChannels;
		wmButton* m_pbtnDown5;
		wmButton* m_pbtnDown;
		wmButton* m_pbtnReset;
		wmButton* m_pbtnUp;
		wmButton* m_pbtnUp5;


		void SetNumberOfChannels(unsigned int nChannels);

	protected:

		//(*Identifiers(pnlRouting)
		static const long ID_M_PLST16;
		static const long ID_M_PLST1;
		//*)

	private:

		void OnlstChannelSelected(wxCommandEvent& event);
		void OnbtnDownPressed(wxCommandEvent& event);
		void OnbtnUpPressed(wxCommandEvent& event);
		void OnbtnDown5Pressed(wxCommandEvent& event);
		void OnbtnUp5Pressed(wxCommandEvent& event);
		void OnbtnResetPressed(wxCommandEvent& event);

		ScopeBuilder* m_pBuilder;

		unsigned long m_nChannel;

		DECLARE_EVENT_TABLE()
};

#endif

