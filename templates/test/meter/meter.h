#pragma once
#include <wx/window.h>
#include "uirect.h"
#include <vector>

class [CLASS_PREFIX]Builder;
class session;
class timedbuffer;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class [CLASS_PREFIX]Meter : public wxWindow
{
    DECLARE_EVENT_TABLE()

    public:

        /** @brief default constructor
        **/
        [CLASS_PREFIX]Meter() { }

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
        [CLASS_PREFIX]Meter(wxWindow *parent, [CLASS_PREFIX]Builder* pBuilder,
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



        virtual ~[CLASS_PREFIX]Meter();


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


        void OnLeftUp(wxMouseEvent& event);

        [CLASS_PREFIX]Builder* m_pBuilder;


};

