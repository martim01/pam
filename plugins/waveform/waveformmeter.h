#pragma once
#include <wx/window.h>
#include "uirect.h"
#include <vector>
#include <list>
#include "pmcontrol.h"
class waveformBuilder;
class session;
class timedbuffer;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class waveformMeter : public pmControl
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(waveformMeter);

    public:

        /** @brief default constructor
        **/
        waveformMeter() : pmControl(){ }

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
        waveformMeter(wxWindow *parent, waveformBuilder* pBuilder,
                 wxWindowID id = wxNewId(),
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



        virtual ~waveformMeter();


        void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);


        wxSize DoGetBestSize() const
        {
            return wxSize(800,480);
        }

  protected:


        void OnPaint(wxPaintEvent& event);
        virtual void OnSize(wxSizeEvent& event);
        void WorkoutResolution();

        void OnLeftUp(wxMouseEvent& event);

        waveformBuilder* m_pBuilder;

//        std::vector<std::list<std::pair<float,float> >> m_vWaveform;
        unsigned int m_nChannels;
        std::list<wxBitmap> m_lstWaveform;
        double m_dResolution;

};


