#pragma once
#include <wx/window.h>
#include "uirect.h"
#include <vector>
#include "pmcontrol.h"

class [CLASS_PREFIX]Builder;
class session;
class timedbuffer;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class [CLASS_PREFIX]Meter : public pmControl
{
    DECLARE_EVENT_TABLE()
	wxDECLARE_DYNAMIC_CLASS([CLASS_PREFIX]Meter);

    public:

        /** @brief default constructor
        **/
        [CLASS_PREFIX]Meter() : pmControl() { }

        /** @brief Constructor
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the control
        *   @param label the text to write in the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the button
        **/
        [CLASS_PREFIX]Meter(wxWindow *parent, [CLASS_PREFIX]Builder* pBuilder,
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

