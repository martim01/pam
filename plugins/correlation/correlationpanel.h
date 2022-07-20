#pragma once

#include <vector>
#include "pmpanel.h"
#include <wx/pen.h>

class LevelGraph;
class correlationBuilder;
class timedbuffer;
class session;

class correlationPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(correlationPanel);

		/** @brief default constructor
        **/
		correlationPanel() : pmPanel(){}

		/** @brief Creates the panel
        *   @param parent pointer to the parent window
        *   @param pBuilder pointer to the correlationBuilder object that built the plugin
		*   @param id the wxWindowId of the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
		correlationPanel(wxWindow* parent,correlationBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~correlationPanel();

		//(*Declarations(correlationPanel)
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

		//(*Identifiers(correlationPanel)
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(correlationPanel)
		//*)

		correlationBuilder* m_pBuilder;

		LevelGraph* m_pGraph;

		unsigned long m_nInputChannels = 2;
		unsigned long m_nX = 0;
		unsigned long m_nY = 1;

		static const wxString GRAPH;
		DECLARE_EVENT_TABLE()
};
