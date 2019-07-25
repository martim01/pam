#pragma once

//(*Headers([CLASS_PREFIX]Panel)
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"

class [CLASS_PREFIX]Builder;
class timedbuffer;
class session;

class [CLASS_PREFIX]Panel: public pmPanel
{
	public:
		
		wxDECLARE_DYNAMIC_CLASS([CLASS_PREFIX]Panel);

		[CLASS_PREFIX]Panel(){}
		[CLASS_PREFIX]Panel(wxWindow* parent,[CLASS_PREFIX]Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~[CLASS_PREFIX]Panel();

		//(*Declarations([CLASS_PREFIX]Panel)
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

		//(*Identifiers([CLASS_PREFIX]Panel)
		//*)
		
		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers([CLASS_PREFIX]Panel)
		//*)
		
		[CLASS_PREFIX]Builder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};
