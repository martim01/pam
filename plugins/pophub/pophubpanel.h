#ifndef POPHUB_PANEL_H
#define POPHUB_PANEL_H

#include <array>
#include <memory>
#include <vector>


#include <wx/panel.h>

#include "pmpanel.h"
#include "ppmtypes.h"
#include "session.h"

class pophubBuilder;
class timedbuffer;


class AngleMeter;
class CorrelationBar;
class LevelCalculator;
class LevelMeter;
class wmButton;
class wmLabel;

class pnlR128;

class pophubPanel: public pmPanel
{
	public:
	
		wxDECLARE_DYNAMIC_CLASS(pophubPanel);

		pophubPanel() : pmPanel(){}
		
		/** @brief Creates the panel
        *   @param parent pointer to the parent window
        *   @param pBuilder pointer to the pophubBuilder object that built the plugin
		*   @param id the wxWindowId of the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
		pophubPanel(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pophubPanel();

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

		void OnLeftUp(wxMouseEvent& event);

	private:
		struct meter
		{
			bool bMS{false};
			int nInput{0};
		};

		void ViewChanged(unsigned long nView);
		void ShowRadio();
		void ShowRadioLoudness();
		void ShowNews();
		void ShowNewsLoudness();
		void ShowNewsGain();
		void ShowWorkshop();
		void ShowTV();

		void ShowOutputMonitorMeters(unsigned int nTop);
		void ShowNewsMeters(unsigned int nTop);
		void SetMeterSettings(int nView);

		void SetMode(const wxString& sMode);

		void SetMeterDetails(int nView, int nSide, const wxString& sLabel);

		void DestroyControls();

		void SetSpeed(unsigned long nSpeed);
		void SetScale(const wxString& sTitle, const ppmtype& aType);
		void SetLightColours();

		pophubBuilder* m_pBuilder;

		int m_nInputChannels = 2;

		
		std::array<AngleMeter*,2> m_meters{nullptr,nullptr};
		std::array<meter, 2> m_meterDetails;
		
		std::array<wmLabel*, 2> m_barLabels{nullptr, nullptr};
		std::array<LevelMeter*,2> m_barMeters{nullptr, nullptr};
		LevelMeter* m_pLevels = nullptr;

		std::array<wmLabel*,2> m_meterLabel{nullptr,nullptr};
		std::array<wmButton*,2> m_meterSelect{nullptr,nullptr};
		std::array<CorrelationBar*,2> m_phasebars{nullptr,nullptr};
		std::shared_ptr<LevelCalculator> m_pCalculator = nullptr;
		
		pnlR128* m_ppnlR128 = nullptr;
		session m_session;

		enum enumSide {kLeft, kRight};

		DECLARE_EVENT_TABLE()
};


#endif