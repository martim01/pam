
#pragma once
#include <wx/window.h>
#include <wx/bitmap.h>
#include "uirect.h"
#include <list>

//#include "wmscroller.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class MaxMinGraph : public wxWindow
{
    DECLARE_EVENT_TABLE()

    public:

        /** @brief default constructor
        **/
        MaxMinGraph(){}

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
        MaxMinGraph(wxWindow *parent,
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



        virtual ~MaxMinGraph();

        void SetLevels(double dMax, double dMin, double dCurrent);


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(200,50);
        }


  protected:


        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);



        double m_dMax;
        double m_dMin;
        double m_dCurrent;

        double m_dResolution;
};



