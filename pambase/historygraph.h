#pragma once
#include <wx/window.h>
#include <chrono>
#include <map>
#include <list>
#include "pmcontrol.h"

using graphPoint = std::pair<double, std::chrono::time_point<std::chrono::system_clock>>;

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class PAMBASE_IMPEXPORT HistoryGraph : public pmControl
{
    DECLARE_EVENT_TABLE()

    wxDECLARE_DYNAMIC_CLASS(HistoryGraph);

    public:

        /** @brief default constructor
        **/
        HistoryGraph() : pmControl(){ }

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
        HistoryGraph(wxWindow *parent,wxWindowID id, const wxPoint& pos, const wxSize& size);

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



        virtual ~HistoryGraph();

        void SetRightAxisWidth(unsigned int nWidth);

        //void OnLeftDown(wxMouseEvent& event);
        //void OnMouseMove(wxMouseEvent& event);
        //void OnLeftUp(wxMouseEvent& event);


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(100,200);
        }

        void AddGraph(const wxString& sName, const wxColour& clr, unsigned int nIntervalMicroSeconds, bool bShow=true, bool bConnect=true);
        void SetGraphUnits(const wxString& sGraph, const wxString& sUnits);
        void AddPeak(const wxString& sGraph, double dPeak);
        void AddPeak(const wxString& sGraph, double dPeak, const std::chrono::time_point<std::chrono::system_clock>& tp);
        void SetLine(const wxString& sName, double dStart, const std::chrono::time_point<std::chrono::system_clock>& tpStart, double dEnd, const std::chrono::time_point<std::chrono::system_clock>& tpEnd);
        void RecalculateRange(const wxString& sGraph);
        void ShowGraph(const wxString& sGraph, bool bShow);
        void ClearGraphs();
        void DeleteAllGraphs();
        void HideAllGraphs();
        void ShowBarGraph(bool bShow);

        void ChangeInterval(const wxString& sGraph, unsigned int nIntervalMicroSeconds);
        void ChangeResolution(const wxString& sGraph, unsigned int nPixels);

        void ClearGraph(const wxString& sGraph);

        void SetMasterGraph(const wxString& sGraph);

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


        struct graph
        {
            graph(const wxColour& clr, unsigned int nInt, bool bS, bool bC) : clrLine(clr), nIntervalDefault(nInt), nInterval(nInt), bShow(bS),
            dMin(std::numeric_limits<double>::max()), dMax(std::numeric_limits<double>::lowest()), dResolution(1.0), nPixels(5), bConnect(bC){}
            wxColour clrLine;
            std::list<graphPoint> lstPeaks;
            unsigned int nIntervalDefault;
            unsigned int nInterval;
            bool bShow;
            double dMin;
            double dMax;
            double dResolution;
            unsigned int nPixels;
            bool bConnect;
            wxString sUnits;
        };

        void DrawLineGraph(wxDC& dc, const graph& aGraph);
        void DrawBarChart(wxDC& dc, const graph& aGraph);
        void DrawAxis(wxDC& dc, const graph& aGraph);

        void ClearGraph(graph& aGraph);
        void WorkoutMinMax(graph& aGraph);

        wxRect m_rectGraph;
        void AutoRange(graph& aGraph);
        std::map<wxString, graph> m_mGraphs;
        wxString m_sGraph;

        wxPoint m_pntMouse;
        bool m_bMouseDown;
        double m_dFirstColumn;

        bool m_bBarChart;
        int m_nResolution;

        std::map<wxString, graph>::const_iterator m_itMaster;
};



