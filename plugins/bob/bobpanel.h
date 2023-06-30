#pragma once

//(*Headers(bobPanel)
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"

class bobBuilder;
class timedbuffer;
class session;

class bobPanel: public pmPanel
{
	public:
	
		wxDECLARE_DYNAMIC_CLASS(bobPanel);

		/** @brief default constructor
        **/
		bobPanel() : pmPanel(){}
		
		/** @brief Creates the panel
        *   @param parent pointer to the parent window
        *   @param pBuilder pointer to the bobBuilder object that built the plugin
		*   @param id the wxWindowId of the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
		bobPanel(wxWindow* parent,bobBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~bobPanel();

		//(*Declarations(bobPanel)
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

		//(*Identifiers(bobPanel)
		//*)
		
		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(bobPanel)
		//*)
		
		bobBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};
