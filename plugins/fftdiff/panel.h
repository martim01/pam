#pragma once

//(*Headers(fftdiffPanel)
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"

class fftdiffBuilder;
class timedbuffer;
class session;

class fftdiffPanel: public pmPanel
{
	public:
	
		wxDECLARE_DYNAMIC_CLASS(fftdiffPanel);

		/** @brief default constructor
        **/
		fftdiffPanel() : pmPanel(){}
		
		/** @brief Creates the panel
        *   @param parent pointer to the parent window
        *   @param pBuilder pointer to the fftdiffBuilder object that built the plugin
		*   @param id the wxWindowId of the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
		fftdiffPanel(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~fftdiffPanel();

		//(*Declarations(fftdiffPanel)
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

		//(*Identifiers(fftdiffPanel)
		//*)
		
		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(fftdiffPanel)
		//*)
		
		fftdiffBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};
