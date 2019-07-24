#pragma once

//(*Headers(batteryPanel)
#include "levelmeter.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include <wx/timer.h>
//*)

#include <vector>
#include "pmpanel.h"

class MyProcess;
class batteryBuilder;
class timedbuffer;
class session;

class batteryPanel: public wxPanel
{
	public:
        wxDECLARE_DYNAMIC_CLASS(batteryPanel);

        batteryPanel(){}
		batteryPanel(wxWindow* parent,batteryBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~batteryPanel();

		//(*Declarations(batteryPanel)
		LevelMeter* m_pMeter1;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_plblError;
		wmLabel* m_plblInput;
		wmLabel* m_plblStatus;
		wxTimer m_timerCheck;
		//*)
		wmLabel* m_pLbl4;
		wmLabel* m_plblCharge;
		wmLabel* m_plblRaw;

		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(batteryPanel)
		static const long ID_M_PMETER1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL6;
		static const long ID_TIMER_CHECK;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(batteryPanel)
		void OntimerCheckTrigger(wxTimerEvent& event);
		//*)
        void OnProcess(wxCommandEvent& event);

        void CheckStatus();
		batteryBuilder* m_pBuilder;
		MyProcess*  m_pProcess;

};

