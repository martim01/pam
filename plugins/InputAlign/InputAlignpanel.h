#pragma once

//(*Headers(InputAlignPanel)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"

class InputAlignBuilder;
class timedbuffer;
class session;

class InputAlignPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(InputAlignPanel);

		InputAlignPanel(){}
		InputAlignPanel(wxWindow* parent,InputAlignBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~InputAlignPanel();

		//(*Declarations(InputAlignPanel)
		wmButton* m_pbtnAlignAll;
		wmButton* m_pbtnAlignSelected;
		wmEdit* m_pedtInputLevel;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_plblCurrent;
		wmLabel* m_plblGain;
		wmList* m_plstChannel1;
		//*)

		/**	@brief Called when there is some audio data to pass to the meter
		*	@param pBuffer const pointer to the timedbuffer containing the audio data
		**/
        void SetAudioData(const timedbuffer* pBuffer);

		/** @brief Called when there is a change to an input session (e.g. samplerate, bit depth, number of channels)
		*	@param aSession session object containing the new session information
		**/
		void InputSession(const session& aSession);

		/** @brief Called when the output channel mapping changes
		*	@param vChannels vector containing the output channels. The size of the vector reflects the number of output channels and each entry represents the particular channel
		**/
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(InputAlignPanel)
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL1;
		static const long ID_M_PEDT1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL5;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL6;
		//*)


	private:

		//(*Handlers(InputAlignPanel)
		void OnlstChannelSelected(wxCommandEvent& event);
		void OnbtnAlignSelectedClick(wxCommandEvent& event);
		void OnbtnAlignAllClick(wxCommandEvent& event);
		//*)
        double GetGain();
        double ConvertGainToRatio(double dGain);
        double ConvertRatioToGain(double dRatio);

		InputAlignBuilder* m_pBuilder;
        unsigned int m_nSelectedChannel;

        double m_dPeak;
        unsigned int m_nTotalChannels;
		DECLARE_EVENT_TABLE()
};
