#pragma once

//(*Headers(MP3StreamerPanel)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"
#include "mp3encoder.h"

class MP3StreamerBuilder;
class timedbuffer;
class session;
class OnDemandStreamer;

class MP3StreamerPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(MP3StreamerPanel);

		/** @brief default constructor
        **/
		MP3StreamerPanel() : pmPanel(){}

		/** @brief Creates the panel
        *   @param parent pointer to the parent window
        *   @param pBuilder pointer to the MP3StreamerBuilder object that built the plugin
		*   @param id the wxWindowId of the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
		MP3StreamerPanel(wxWindow* parent,MP3StreamerBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~MP3StreamerPanel();

		//(*Declarations(MP3StreamerPanel)
		wmButton* m_pbtnStream;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_plblConnections;
		wmLabel* m_plblUnicastUrl;
		wmList* m_plstConnections;
		wxPanel* m_ppnlUnicast;
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

		//(*Identifiers(MP3StreamerPanel)
		static const long ID_M_PLBL3;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST1;
		static const long ID_PANEL1;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(MP3StreamerPanel)
		void OnbtnStreamClick(wxCommandEvent& event);
		//*)

		void OnODSAnnounce(wxCommandEvent& event);
		void OnODSConnection(wxCommandEvent& event);
		void OnODSDisconnection(wxCommandEvent& event);

		MP3StreamerBuilder* m_pBuilder;
		MP3Encoder m_Encoder;
        OnDemandStreamer* m_pUnicastStreamer;
		bool m_bStreaming;
        unsigned int m_nPortCount;
		DECLARE_EVENT_TABLE()
};

