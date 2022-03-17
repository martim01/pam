#pragma once

//(*Headers(LTCPanel)
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"

class SettingEvent;
class LTCBuilder;
class timedbuffer;
class session;
class LtcDecoder;

class LTCPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(LTCPanel);

		LTCPanel(){}
		LTCPanel(wxWindow* parent,LTCBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LTCPanel();

		//(*Declarations(LTCPanel)
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl4;
		wmLabel* m_pLblColourTitle;
		wmLabel* m_pLblDateTitle;
		wmLabel* m_pLblFPSTitle;
		wmLabel* m_pLblModeTitle;
		wmLabel* m_pLblVolumeTitle;
		wmLabel* m_plblClock;
		wmLabel* m_plblClockTitle;
		wmLabel* m_plblColour;
		wmLabel* m_plblDateFormat;
		wmLabel* m_plblFPS;
		wmLabel* m_plblLTCDate;
		wmLabel* m_plblLTCTime;
		wmLabel* m_plblLTCVolume;
		wmLabel* m_plblListTitle;
		wmLabel* m_plblMode;
		wmLabel* m_plblRaw;
		wmList* m_plstChannels;
		wmList* m_plstDate;
		//*)

		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(LTCPanel)
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL16;
		static const long ID_M_PLBL17;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL18;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL15;
		static const long ID_M_PLBL19;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(LTCPanel)
		void OnlstDateSelected(wxCommandEvent& event);
		//*)

		void OnSettingEvent(SettingEvent& event);

		LTCBuilder* m_pBuilder;
		LtcDecoder* m_pDecoder;

		unsigned int m_nInputChannels;
		DECLARE_EVENT_TABLE()
};

