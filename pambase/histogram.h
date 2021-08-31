#pragma once
#include <wx/window.h>

#include <map>
#include <list>
#include "pmcontrol.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class PAMBASE_IMPEXPORT Histogram : public pmControl
{
    DECLARE_EVENT_TABLE()

    wxDECLARE_DYNAMIC_CLASS(Histogram);

    public:

        /** @brief default constructor
        **/
        Histogram() : pmControl(){ }

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
        Histogram(wxWindow *parent,wxWindowID id, const wxPoint& pos, const wxSize& size);

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
        bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);



        virtual ~Histogram();


        void OnLeftDown(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(100,200);
        }

        void AddGraph(const wxString& sName, const wxColour& clr, double dInterval, size_t nMaxEntries=10000);
        void AddPeak(const wxString& sGraph, double dPeak);

        void ShowGraph(const wxString& sGraph);
        void ClearGraphs();
        void DeleteAllGraphs();
        void HideAllGraphs();

        void ChangeGranularity(const wxString& sGraph, int nWhich);
        void ChangeResolution(const wxString& sGraph, int nWhich);

        void ClearGraph(const wxString& sGraph);

  protected:
        enum {DECREASE, RESET, INCREASE};




        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        wxColour RGBA_Blend(wxColour clrFore, wxColour clrBack, double dAlpa);

        struct graph
        {
            graph(const wxColour& clr, double dInt, size_t nMax) : clrLine(clr), dIntervalDefault(dInt), dInterval(dInt), nMaxEntries(nMax),
            dResolution(1.0), dFirstDisplay(-0.0), dResolutionX(1.0), bScrolled(false){}
            wxColour clrLine;
            std::map<int, unsigned int> mColumns;
            std::list<double> lstPeaks;
            double dIntervalDefault;
            double dInterval;
            size_t nMaxEntries;
            double dResolution;
            double dFirstDisplay;
            double dResolutionX;
            bool bScrolled;
        };

        void WorkoutColumns(graph& aGraph);
        void ClearGraph(graph& aGraph);

        std::map<wxString, graph> m_mGraphs;
        wxString m_sGraph;

        wxRect m_rectGraph;

        wxPoint m_pntMouse;
        bool m_bMouseDown;
        double m_dFirstColumn;
};


