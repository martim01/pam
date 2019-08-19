#pragma once
#include <wx/window.h>
#include <map>
#include <wx/pen.h>
#include "pmcontrol.h"
/** @class a class that draws a button on the screen, derives from wxWindow
**/


struct PAMBASE_IMPEXPORT wmLine
{
    wmLine() : pntStart(0,0), pntEnd(0,0), clr(*wxBLACK), nWidth(1), nPenStyle(wxSOLID), nLineStyle(LINESTYLE_FLAT), nStartArrow(LINEEND_NONE),nEndArrow(LINEEND_NONE), nArrowSize(8), bDraw(true){}

    wmLine(const wxPoint& pnt1, const wxPoint& pnt2, const wxColour& colour = *wxBLACK, int nw = 1, int nPen = wxSOLID, unsigned int nLS = LINESTYLE_FLAT, unsigned int nSA = LINEEND_NONE, unsigned int nEA = LINEEND_NONE,int nASize = 7, bool bD=true) :
        pntStart(pnt1), pntEnd(pnt2), clr(colour), nWidth(nw), nPenStyle(nPen), nLineStyle(nLS), nStartArrow(nSA),nEndArrow(nEA), nArrowSize(8), bDraw(bD){}

    wmLine(const wmLine& ln) : pntStart(ln.pntStart), pntEnd(ln.pntEnd), clr(ln.clr), nWidth(ln.nWidth), nPenStyle(ln.nPenStyle), nLineStyle(ln.nLineStyle), nStartArrow(ln.nStartArrow),nEndArrow(ln.nEndArrow), nArrowSize(ln.nArrowSize), bDraw(ln.bDraw){}

    wxPoint pntStart;
    wxPoint pntEnd;
    wxColour clr;
    int nWidth;
    int nPenStyle;


    unsigned int nLineStyle;
    unsigned int nStartArrow;
    unsigned int nEndArrow;
    unsigned int nArrowSize;
    bool bDraw;

    static const unsigned int LINEEND_NONE  = 0;
    static const unsigned int LINEEND_ARROW = 1;
    static const unsigned int LINEEND_SARROW = 2;
    static const unsigned int LINEEND_CIRCLE =3;
    static const unsigned int LINEEND_SQUARE =4;

    static const unsigned int LINESTYLE_FLAT =0;
    static const unsigned int LINESTYLE_ETCH =1;
    static const unsigned int LINESTYLE_RIDGE=2;

};

typedef std::map<wxString, wmLine> mLines_t;


class PAMBASE_IMPEXPORT wmLineWnd : public pmControl
{
    DECLARE_EVENT_TABLE()
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmLineWnd)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmLineWnd);
    #endif // WXSPAM
    public:

        /** @brief default constructor
        **/
        wmLineWnd() : pmControl(), m_nStyle(0) { }

        /** @brief Constructor - made to be the same as a wxButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param name not used
        **/

        wmLineWnd(wxWindow *parent,
                 wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0);


        /** @brief Creates the button
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param name not used
        **/
        virtual bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0);


        virtual ~wmLineWnd();


        /** @brief Sets the background colour of the button for all button states
        *   @param colour
        *   @return <i>bool</i> true
        **/
        bool SetBackgroundColour(const wxColour &colour);

        /** @brief Sets the text colour of the button for all button states
        *   @param colour
        *   @return <i>bool</i> true
        **/


        bool AddPercentageLine(const wxString& sName, const wxPoint& pntStart, const wxPoint& pntEnd, const wxColour& colour = *wxBLACK, int nWidth = 1, int nPen = wxSOLID, unsigned int nLineStyle = wmLine::LINESTYLE_FLAT, unsigned int nStartArrow = wmLine::LINEEND_NONE, unsigned int nEndArrow = wmLine::LINEEND_NONE, bool bDraw=true, unsigned int nArrowSize=7);

        bool AddPercentageLine(const wxString& sName, const wmLine& ln);

        bool UpdatePercentageLine(const wxString& sName, const wxPoint& pntStart, const wxPoint& pntEnd, const wxColour& colour = *wxBLACK, int nWidth = 1, int nPen = wxSOLID, unsigned int nLineStyle = wmLine::LINESTYLE_FLAT, unsigned int nStartArrow = wmLine::LINEEND_NONE, unsigned int nEndArrow = wmLine::LINEEND_NONE, bool bDraw=true, unsigned int nArrowSize=7);

        bool DeletePercentageLine(const wxString& sName);

        bool HideLine(const wxString& sName, bool bHide);
        bool ChangeStartPercentage(const wxString& sName, const wxPoint& pntStart);
        bool ChangeEndPercentage(const wxString& sName, const wxPoint& pntEnd);
        bool ChangeColour(const wxString& sName, const wxColour& clr);
        bool ChangeWidth(const wxString& sName, int nWidth);
        bool ChangePenStyle(const wxString& sName, int nStyle);

        bool LineExists(const wxString& sName);
        wxPoint GetStartPercentage(const wxString& sName);
        wxPoint GetEndPercentage(const wxString& sName);
        wxColour GetColour(const wxString& sName);
        int GetWidth(const wxString& sName);
        int GetPenStyle(const wxString& sName);
        int GetStartArrow(const wxString& sName);
        int GetEndArrow(const wxString& sName);
        bool IsDrawn(const wxString& sName);
        int GetArrowSize(const wxString& sName);



        wxSize DoGetBestSize() const
        {
            return wxSize(100,100);
        }

        mLines_t::iterator GetLinesBegin();
        mLines_t::iterator GetLinesEnd();




  protected:



        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        virtual void Draw(wxDC& dc);

        void DrawArrow(wxDC& dc, const wxPoint& ptPoint, const wxPoint& ptBase, int nSize, bool bFill);

        void DrawSquare(wxDC& dc, const wxPoint& ptPoint, const wxPoint& ptBase, int nSize);
        void DrawCircle(wxDC& dc, const wxPoint& ptPoint, const wxPoint& ptBase,int nSize);

        void Rotate(const wxPoint& ptPoint, const wxPoint& ptBase, unsigned int nPoints, wxPoint pnts[]);

        void CalculatePoints(double dWidth, double dHeight, const wmLine& line, wxPoint& pntStart, wxPoint& pntEnd);
        unsigned int m_nStyle;

        wxColour m_clrBackground;


        std::map<wxString, wmLine> m_mLinePercentages;



};



