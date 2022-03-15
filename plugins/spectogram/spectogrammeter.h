#pragma once
//#include <wx/window.h>
#include "uirect.h"
#include "freq_mag.h"
#include <wx/timer.h>
#include "kiss_fft.h"
#include <queue>
#include <list>
#include "colourgradient.h"
#include "pmcontrol.h"
#include "json/json.h"

class wxImage;
class timedbuffer;
class SpectogramBuilder;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class SpectogramMeter : public pmControl
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(SpectogramMeter);

    public:

        /** @brief default constructor
        **/
        SpectogramMeter() : pmControl(){ }

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
        SpectogramMeter(wxWindow *parent, SpectogramBuilder* pBuilder,
                 wxWindowID id,
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



        virtual ~SpectogramMeter();

        void SetFreqMag(const freq_mag& fm);

        void SetData(const timedbuffer* pBuffer);
        void SetSampleRate(unsigned long nSampleRate);

        void SetHold(bool bHold);
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

        void SetChannels(const std::vector<std::pair<unsigned char, wxString>>& vChannels);

        void SetAnalyseMode(int nMode);
        void SetWindowType(int nType);
        void SetOverlap(double dPercent);
        void SetBufferSize(size_t nFrames);
        void SetNumberOfBins(size_t nBins);

        unsigned int GetDisplayType() const
        {
            return m_nDisplayType;
        }


        void SetDisplayType(unsigned int nDisplay);

        void SetCursorMode(bool bSet);
        bool GetCursorMode()
        {   return m_bCursorMode;   }

        void TurnoffNudge();

        void SetHeatMap(int nMap);

        void SetLinear(bool bLinear);
        enum {DISPLAY_GRAPH, DISPLAY_LINES, DISPLAY_EQ};
        enum {ANALYSE_L,ANALYSE_R, ANALYSE_L_P_R, ANALYSE_L_M_R};
        enum {WINDOW_RECTANGULAR, WINDOW_HANNING, WINDOW_HAMMING, WINDOW_BLACKMAN, WINDOW_KAISER, WINDOW_KAISERBESSEL};

        enum {MAP_MONO, MAP_HOTCOLD, MAP_FIVE, MAP_PPM, MAP_SEVEN};
  protected:


        Json::Value CreateWebsocketMessage(const wxImage& anImage);

        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);
        void DrawSpectogram(wxDC& dc);
        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        void CreateScaleBitmap();
        void DoFFT();

        void FFTRoutine();
        void Peaks();
        float WindowMod(float dAmplitude);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnTimerNudge(wxTimerEvent& event);

        std::list<float> m_lstBuffer;      ///< vector containing the a-leg samples

        std::vector<kiss_fft_cpx> m_vfft_out;

        std::queue<freq_mag> m_qResult;


        SpectogramBuilder* m_pBuilder;

        uiRect m_uiSettingsAnalyse;
        uiRect m_uiSettingsWindow;
        uiRect m_uiSettingsOverlap;
        uiRect m_uiSettingsBins;
        uiRect m_uiSettingsDisplay;
        uiRect m_uiSettingsMeter;
        uiRect m_uiPeakFrequency;
        uiRect m_uiPeakLevel;

        bool m_bHold;


        double m_dBinSize;


        int m_nFFTAnalyse;

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

        uiRect m_uiClose;
        uiRect m_uiAmplitude;
        uiRect m_uiBin;
        uiRect m_uiNudgeDown;
        uiRect m_uiNudgeUp;

        unsigned long m_nSampleRate;
        unsigned long m_nChannels;


        ColourGradient m_HeatMap;
        std::list<wxBitmap> m_lstBitmaps;
        std::vector<std::pair<unsigned char, wxString>> m_vChannels;

        wxBitmap m_bmpScale;

        bool m_bLinear;
};

