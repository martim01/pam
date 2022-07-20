#pragma once
#include <wx/window.h>

#include <map>
#include <list>
#include "pmcontrol.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class PAMBASE_IMPEXPORT LevelGraph : public pmControl
{
    DECLARE_EVENT_TABLE()

    wxDECLARE_DYNAMIC_CLASS(LevelGraph);

    public:

        /** @brief default constructor
        **/
        LevelGraph() : pmControl(), m_nDataSize(0), m_dMax(-80),  m_dMin(0), m_dResolution(1)
		{ }

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
        LevelGraph(wxWindow *parent,wxWindowID id, const wxPoint& pos, const wxSize& size, size_t nDataSize, double dMax, double dMin);

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



        virtual ~LevelGraph();



        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(100,200);
        }

        void SetLimit(const wxString& sGraph, double dMax, double dMin);
        void SetZoneLimit(double dMax, double dMin)
        {
            m_dMax = dMax;
            m_dMin = dMin;
            m_dResolution = static_cast<double>(GetClientSize().GetHeight())/(m_dMax-m_dMin);
        }

        enum enumMode {DS_MIN = 0, DS_MAX, DS_AV};

        void AddGraph(const wxString& sName, const wxColour& clr, bool bAutoRange=false, enumMode eMode = DS_MAX);
        std::pair<bool, double> AddPeak(const wxString& sGraph, double dPeak);

        void AddZone(double dMin, double dMax, const wxColour& clr);
        void ClearZones();

        void AddLine(double dPosition, const wxPen& penStyle);
        void ClearLines();

        void ShowGraph(const wxString& sGraph, bool bShow=true);
        void ClearGraphs();
        void DeleteAllGraphs();
        void HideAllGraphs();
        void ShowRange(const wxString& sGraph, bool bShow=true);
        void SetAutoRange(const wxString& sGraph, bool bAuto);

        std::pair<double,double> GetRange(const wxString& sGraph);

  protected:


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
            graph(const wxColour& clr, bool bAuto, enumMode eM) : clrLine(clr), dDataSetTotal(0.0), dDataSetMax(-120.0), nDataSize(0), bShow(true),
            dMax(std::numeric_limits<double>::lowest()),
            dMin(std::numeric_limits<double>::max()), dResolution(1),
            bShowRange(false), bAutoRange(bAuto), eMode(eM) {}

            wxColour clrLine;
            std::list<double> lstPeaks;
            double dDataSetTotal;
            double dDataSetMax;
            double dDataSetMin;
            size_t nDataSize;
            bool bShow;
            double dMax;
            double dMin;
            double dResolution;
            bool bShowRange;
            bool bAutoRange;

            enumMode eMode;
        };


        double ProcessDataSet(graph& aGraph);
        double GetDataSetMax(graph& aGraph);

        void AutoRange(graph& aGraph);

        std::map<wxString, graph> m_mGraphs;

        size_t m_nDataSize;
        double m_dMax;
        double m_dMin;
        double m_dResolution;
        bool m_bAutoRange;

        struct zone
        {
            zone(double dMi, double dMa, const wxColour& clr) : dMin(dMi), dMax(dMa), clrZone(clr){}
            double dMin;
            double dMax;
            wxColour clrZone;
        };

        std::list<zone> m_lstZones;

        std::map<double, wxPen> m_mLines;
};

