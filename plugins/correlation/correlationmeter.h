#pragma once
#include <wx/window.h>
#include "uirect.h"
#include <vector>
#include "pmcontrol.h"

class correlationBuilder;
class session;
class timedbuffer;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class correlationMeter : public pmControl
{
    DECLARE_EVENT_TABLE()
	wxDECLARE_DYNAMIC_CLASS(correlationMeter);

    public:

        /** @brief default constructor
        **/
        correlationMeter() : pmControl() { }

        /** @brief Constructor
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the control
        *   @param label the text to write in the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the button
        **/
        correlationMeter(wxWindow *parent, correlationBuilder* pBuilder,
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



        virtual ~correlationMeter();

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
        

        wxSize DoGetBestSize() const
        {
            return wxSize(800,480);
        }

  protected:


        void OnPaint(wxPaintEvent& event);
        virtual void OnSize(wxSizeEvent& event);


        void OnLeftUp(wxMouseEvent& event);

        correlationBuilder* m_pBuilder;


};

