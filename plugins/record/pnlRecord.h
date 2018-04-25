#ifndef PNLRECORD_H
#define PNLRECORD_H

//(*Headers(pnlRecord)
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include <wx/timer.h>
#include "wmedit.h"
//*)
#include "soundfile.h"

class timedbuffer;
class session;

class pnlRecord: public wxPanel
{
	public:

		pnlRecord(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlRecord();

		//(*Declarations(pnlRecord)
		wmLabel* m_plblTime;
		wmButton* m_pbtnRecord;
		wmButton* m_pbtnClear;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl12;
		wmEdit* m_pedtFile;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnDate;
		wxTimer m_timerSecond;
		//*)

        void SetAudioData(const timedbuffer* pBuffer);
        void InputSession(const session& aSession);
	protected:

		//(*Identifiers(pnlRecord)
		static const long ID_M_PLBL37;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PEDT1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_TIMER1;
		//*)

	private:

		//(*Handlers(pnlRecord)
		void OnbtnRecordClick(wxCommandEvent& event);
		void OntimerSecondTrigger(wxTimerEvent& event);
		void OnbtnDateClick(wxCommandEvent& event);
		void OnedtFileText(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)

        bool m_bRecording;
        wxDateTime m_dtRecording;
        SoundFile m_sf;

        unsigned int m_nInputChannels;
        unsigned int m_nSampleRate;

		DECLARE_EVENT_TABLE()
};

#endif
