#ifndef PNLLEVELS_H
#define PNLLEVELS_H

#include <vector>
//(*Headers(pnlLevels)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)

class MaxMinGraph;

class timedbuffer;
struct session;

class pnlLevels: public wxPanel
{
	public:

		pnlLevels(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLevels();

		//(*Declarations(pnlLevels)
		wmLabel* m_pLbl12;
		wmButton* m_pbtnReset;
		//*)

        void SetAudioData(const timedbuffer* pBuffer);
		void ResetTest();

		void InputSession(const session& aSession);

	protected:

		//(*Identifiers(pnlLevels)
		static const long ID_M_PLBL37;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlLevels)
		void OnbtnResetClick(wxCommandEvent& event);
		//*)

		void CreateGraphs(unsigned int nChannels);

		std::vector<MaxMinGraph*> m_vGraph;
		std::vector<wmLabel*> m_vLabel;
		std::vector<double> m_vLevelPeakMax;
		std::vector<double> m_vLevelPeakMin;


		DECLARE_EVENT_TABLE()
};

#endif
