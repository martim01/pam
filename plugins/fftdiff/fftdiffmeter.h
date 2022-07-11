#pragma once
#include <wx/window.h>
#include "uirect.h"
#include "freq_mag.h"
#include <wx/timer.h>
#include "kiss_fft.h"
#include <queue>
#include "pmcontrol.h"
#include "colourgradient.h"
#include "json/json.h"
#include "session.h"
#include "delayline.h"

class timedbuffer;
class fftdiffBuilder;
class SettingEvent;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class fftdiffMeter : public pmControl
{
    public:
        DECLARE_EVENT_TABLE()
        wxDECLARE_DYNAMIC_CLASS(fftdiffMeter);

        /** @brief default constructor
        **/
        fftdiffMeter() : pmControl(){ }

        /** @brief Constructor - made to be the same as a wxButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param label the text to write in the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param validator not used - just here to have same structure as wxButton
        *   @param name not used - just here to have same structure as wxButton
        **/
        fftdiffMeter(wxWindow *parent, fftdiffBuilder* pBuilder,
                 wxWindowID id=-1,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize);

        /** @brief Creates the button
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param label the text to write in the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param validator not used - just here to have same structure as wxButton
        *   @param name not used - just here to have same structure as wxButton
        **/
        virtual bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);



        virtual ~fftdiffMeter();

        void SetFreqMag(const freq_mag& fm);

        void SetAudioData(const nonInterlacedVector& buffer);
        void SetSampleRate(unsigned long nSampleRate);

        void SetHold(bool bHold);

        void ShowMax(bool bShow);
        void ShowMin(bool bShow);
        void ShowAverage(bool bShow);
        void ShowAverageRolling(bool bShow);
        void ResetMax();
        void ResetMin();
        void ResetAverage();

        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(500,100);
        }

        void SetPeakCutoff(int ndB)
        {
            m_nPeakCutoff = ndB;
            Refresh();
        }

        void SetFall(double ddB)
        {
            m_dFall = ddB;
        }

        void SetWindowType(int nType);
        void SetOverlap(double dPercent);
        void SetBufferSize(size_t nFrames);
        void SetNumberOfBins(size_t nBins);

        unsigned int GetDisplayType() const
        {
            return m_nDisplayType;
        }


        void SetDisplayType(unsigned int nDisplay);
        void SetMeter(short nMeter);

        void SetCursorMode(bool bSet);
        bool GetCursorMode()
        {   return m_bCursorMode;   }

        void SetColourMode(bool bOn)
        {
            m_bColour = bOn;
        }

        void TurnoffNudge();

        void SetVerticalRange(unsigned long ndB);

        unsigned long GetSampleRate() const { return m_nSampleRate;}

        enum {DISPLAY_GRAPH, DISPLAY_LINES, DISPLAY_EQ};
        enum {ANALYSE_L,ANALYSE_R, ANALYSE_L_P_R, ANALYSE_L_M_R};
        enum {WINDOW_RECTANGULAR, WINDOW_HANNING, WINDOW_HAMMING, WINDOW_BLACKMAN, WINDOW_KAISER, WINDOW_KAISERBESSEL};
        enum{FFT, OCTAVE, PEAKS};

        Json::Value CreateWebsocketMessage();
  protected:

        void OnSettingChanged(SettingEvent& event);
        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);
        void DrawFFT(wxDC& dc);
        void DrawGraph(wxDC& dc, const std::vector<float>& vSpectrum, const wxColour& clr, double dMultiplier=1.0);
        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);


        void DoFFT();

        void FFTRoutine();
        void Peaks();
        float WindowMod(float dAmplitude);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnTimerNudge(wxTimerEvent& event);

        void CheckRollingCount();


        nonInterlacedList m_buffer;

        std::vector<kiss_fft_cpx> m_vfft_out[2];

        std::queue<freq_mag> m_qResult;

        std::vector<float> m_vAmplitude;
        std::vector<float> m_vAverage;
        std::vector<float> m_vAverageRolling;
        std::vector<float> m_vAverageRollingDisplay;
        std::vector<float> m_vMax;
        std::vector<float> m_vMin;

        double m_dTotalFrames;
        double m_dRollingFrames;
        fftdiffBuilder* m_pBuilder;


        bool m_bHold;
        bool m_bShowMax;
        bool m_bShowMin;
        bool m_bShowAverage;
        bool m_bShowAverageRolling;


        double m_dBinSize;
        double m_dVerticalResolution;



        double m_dFall;
        int m_nPeakCutoff;

        wxRect m_rectGrid;

        unsigned int m_nDisplayType;

        int m_nWindowType;
        size_t m_nOverlap;

        short m_nMeterType;

        size_t m_nNumberOfSetBins;

        double m_dOverlapPercent;

        static const unsigned long FFT_SIZE = 2048;

        static const double OCTAVE_LOW_1;
        static const double OCTAVE_LOW_2;
        static const double OCTAVE_LOW_3;


        static const wxString LABEL_DISPLAY[3];
        static const wxString LABEL_ANALYSE[10];
        static const wxString LABEL_WINDOW[6];
        static const wxString LABEL_TYPE[3];

        static const wxColour COLOUR_LABEL;

        size_t m_nSampleSize;

        bool m_bCursorMode;
        size_t m_nBinSelected;
        int m_nNudge;
        enum {NONE, UP, DOWN};
        wxTimer m_timerNudge;

        uiRect m_uiAmplitude;
        uiRect m_uiAverage;
        uiRect m_uiBin;
        uiRect m_uiNudgeDown;
        uiRect m_uiNudgeUp;

        unsigned long m_nSampleRate;
        unsigned long m_nChannels;

        ColourGradient m_HeatMap;
        bool m_bColour;

        double m_dPeakLevel;
        double m_dPeakFrequency;



};

