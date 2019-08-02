#pragma once
#include <wx/window.h>
#include "uirect.h"
#include <vector>
#include <list>
#include "pmcontrol.h"
#include "kiss_fft.h"
#include "colourgradient.h"

class fftphaseBuilder;
class session;
class timedbuffer;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class fftphaseMeter : public pmControl
{
    DECLARE_EVENT_TABLE()
	wxDECLARE_DYNAMIC_CLASS(fftphaseMeter);

    public:

        /** @brief default constructor
        **/
        fftphaseMeter() : pmControl() { }

        /** @brief Constructor
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the control
        *   @param label the text to write in the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the button
        **/
        fftphaseMeter(wxWindow *parent, fftphaseBuilder* pBuilder,
                 wxWindowID id = wxNewId(),
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize);

        /** @brief Creates the control
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the control
        *   @param label the text to write in the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
        virtual bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);



        virtual ~fftphaseMeter();

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


		void SetOverlap(double dPercent);
		void SetNumberOfBins(size_t nBins);
		void SetWindowType(int nType);

		void SetFall(bool bOn);
		void SetDisplayType(int nType);

        wxSize DoGetBestSize() const
        {
            return wxSize(800,480);
        }
        enum {WINDOW_RECTANGULAR, WINDOW_HANNING, WINDOW_HAMMING, WINDOW_BLACKMAN, WINDOW_KAISER, WINDOW_KAISERBESSEL};

  protected:

        void DoFFT(int nChannel);

        void OnPaint(wxPaintEvent& event);
        void DrawPhase(wxDC& dc);
        void DrawStars(wxDC& dc);
        void DrawGraph(wxDC& dc);
        virtual void OnSize(wxSizeEvent& event);

        float WindowMod(float dAmplitude);

        void OnLeftUp(wxMouseEvent& event);

        fftphaseBuilder* m_pBuilder;

        std::list<float> m_lstBuffer[2];      ///< vector containing the a-leg samples
        std::vector<kiss_fft_cpx> m_vfft_out[2];
        std::vector<float> m_vAmplitude[2];
        std::vector<float> m_vPhase[2];
        std::vector<wxColour> m_vColour;

        unsigned long m_nSampleRate;
        unsigned long m_nChannels;
        unsigned long m_nWindowType;
        unsigned long m_nOverlap;
        double m_dBinSize;
        double m_dFall;
        double m_dOverlapPercent;
        size_t m_nSampleSize;
        size_t m_nNumberOfSetBins;
        int m_nDisplayType;
        enum {GRAPH=0, STARS, PHASE};
        wxRect m_rectGrid;
        ColourGradient m_HeatMap;
        bool m_bFall;

        uiRect m_uiSettingsAnalyse;
        uiRect m_uiSettingsWindow;
        uiRect m_uiSettingsOverlap;
        uiRect m_uiSettingsBins;
        uiRect m_uiSettingsDisplay;
        uiRect m_uiSettingsMeter;
        uiRect m_uiPeakFrequency;
        uiRect m_uiPeakLevel;
        uiRect m_uiAmplitude;
        uiRect m_uiBin;
        static const wxString LABEL_WINDOW[6];

};


