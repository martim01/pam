#pragma once
#include <wx/window.h>
#include <wx/bitmap.h>
#include "audio.h"
#include "uirect.h"
#include <wx/timer.h>
#include <vector>

//#include "wmscroller.h"
class ScopeBuilder;
class timedbuffer;
/** @class a class that draws a button on the screen, derives from wxWindow
**/
class Scope : public wxWindow
{
    DECLARE_EVENT_TABLE()

    public:

        /** @brief default constructor
        **/
        Scope(){}

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
        Scope(wxWindow *parent,ScopeBuilder* pBuilder,
                 wxWindowID id = wxID_ANY,
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



        virtual ~Scope();

        void SetData(const timedbuffer* pBuffer);


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(800,480);
        }

        void ResetSlide()
        {
            m_pntSlide = wxPoint(0,0);
            Refresh();
        }

        void TurnoffNudge();

        void SetDisplayType(short nType);
        void SetRouting(short nRouting);

        int GetTimeFrame()
        {
            return m_nFrameRefresh;
        }

        double GetTrigger();
        void SetTrigger(double dTrigger);

        double GetVerticalZoom();
        void SetVerticalZoom(double dMax);

        void SetNumberOfChannels(unsigned int nChannels);
        void SetMode(unsigned int nMode);
        unsigned int GetMode()
        {   return m_nMode; }

        void SetMemory(bool bMemory)
        {
            m_bMemory = bMemory;
        }
        void SetAutotrigger(bool bAutotrigger)
        {
            m_bAutotrigger = bAutotrigger;
        }
        bool GetAutotrigger()
        {
            return m_bAutotrigger;
        }

        void Autotrigger();

        void ResetMemory();


        void SetTimeFrame(int nFrames);

        enum{LEFT, RIGHT, MIDDLE=8, SIDE=9, LEFT_RIGHT=10};
        enum {MODE_NORMAL, MODE_SLIDE, MODE_CURSOR};

  protected:


        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        void OnTimerNudge(wxTimerEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnMotion(wxMouseEvent& event);

        wxPoint m_pntMouse;

        wxRect m_rectGrid;

        uiRect m_uiExit;
        uiRect m_uiTime;
        uiRect m_uiValue;
        uiRect m_uiNudgeUp;
        uiRect m_uiNudgeDown;

        short m_nType;
        short m_nRouting;

        size_t m_nFrameRefresh;
        float m_dResolution;

        float m_dVerticalZoom;
        float m_dTrigger;
        float m_dMaxY;
        std::list<float> m_lstBuffer;
        wxBitmap m_bmpScreen;

        wxPoint m_pntSlide;
        unsigned int m_nMode;
        double m_dSampleX;
        double m_dStep;

        int m_nNudge;
        enum {NONE, UP, DOWN};
        wxTimer m_timerNudge;


        bool m_bMemory;
        bool m_bAutotrigger;

        ScopeBuilder* m_pBuilder;
        std::vector<float> m_vChannels;
        //std::vector<float> m_vChannels:

};



