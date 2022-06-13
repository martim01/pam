#pragma once
#include <wx/window.h>
#include <wx/bitmap.h>
#include "uirect.h"
#include <list>
#include "pmcontrol.h"
//#include "wmscroller.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class OffsetGraph : public pmControl
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(OffsetGraph);

    public:

        /** @brief default constructor
        **/
        OffsetGraph():pmControl(){}

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
        OffsetGraph(wxWindow *parent,
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



        virtual ~OffsetGraph();

        void SetChannel1Name(const wxString& sChannel1);
        void SetChannel2Name(const wxString& sChannel2);


        void AddOffset(int nSamples);

        void SetSampleRate(unsigned long nSampleRate);


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(801,481);
        }

        void SetTimeFrame(int nMaxSamples);

        void ShowSamplesLeftToGet(unsigned int nSamples);

        void SetCalculating(bool bCalc);
        bool GetCalculating() const { return m_bCalculating;}

  protected:


        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);


        wxRect m_rectGrid;

        unsigned int m_nMaxSamples;
        double m_dResolution;
        double m_dTimeFrame;

        wxRect m_rectSamples;
        wxRect m_rectSamplesToGet;

        bool m_bCalculating;
        int m_nOffset;
        int m_nLastOffset;
        wxString m_sSamples;
        wxString m_sTimeframe;

        double m_dSampleRate;

        wxString m_sChannel[2];
};



