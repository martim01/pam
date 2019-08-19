#pragma once
#include <wx/string.h>
#include "dlldefine.h"
#include <wx/dc.h>
#include <wx/colour.h>



/** @class basic ui element class for new wammacs stuff. Draws a rectangle with colours, borders, labels bitmaps etc.
**/

class PAMBASE_IMPEXPORT uiRect
{
    public:
        /** @brief Constructor
        *   @param rect the enclosing rectangle of the uiRect
        *   @param nGradient the direction of any background shading
        **/
        uiRect(const wxRect& rect = wxRect(0,0,0,0), unsigned int nGradient=wxNORTH);

        ///< @brief Destructor
        virtual ~uiRect();

        ///< @brief Copy constructor
        uiRect(const uiRect& ui);

        /** @brief Set the enclosing rectangle
        *   @param rect the enclosing rectangle
        **/
        void SetRect(const wxRect& rect);

        /** @brief Set the enclosing rectangle
        *   @param nLeft
        *   @param nTop
        *   @param nWidth
        *   @param nHeight
        **/
        void SetRect(int nLeft, int nTop, int nWidth, int nHeight);

        /** @brief Set the backround colour of the rectangle
        *   @param colour
        **/
        void SetBackgroundColour(const wxColour &colour=wxNullColour, const wxColour& clrLow=wxNullColour);

        /** @brief Set the text colour of the rectangle
        *   @param colour
        **/
        void SetForegroundColour(const wxColour &colour);

        /** @brief Set the alignment of the text in the rectangle
        *   @param nAlign, a union of one of wxALIGN_LEFT,wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT and one of wxALIGN_TOP, wxALIGN_BOTTOM, wxALIGN_CENTER_VERTICAL
        **/
        void SetTextAlign(unsigned int nAlign);

        ///< @brief Get the alignment of the text
        unsigned int GetTextAlign() const
        {   return m_nAlign;    }

        /** @brief Set the direction of the background shading
        *   @param nGradient one of wxEAST, wxNORTH, wxSOUTH, wxWEST
        **/
        void SetGradient(unsigned int nGradient)
        {
            m_nGradient = nGradient;
        }

        ///< @brief Get the direction of background shading
        unsigned int GetGradient() const
        {
            return m_nGradient;
        }

        /** @brief Returns the background colour
        *   @return <i>wxColour</i>
        **/
        virtual const wxColour& GetBackgroundColour() const
        {
            return m_clrBackgroundHigh;
        }

        /** @brief Returns the foreground colour
        *   @return <i>wxColour</i>
        **/
        virtual const wxColour& GetForegroundColour() const
        {
            return m_clrText;
        }


        /** @brief Sets the text to draw in the rectangle
        *   @param sLabel
        **/
        virtual void SetLabel(const wxString& sLabel);

        /** @brief Gets the text that is currently drawn in the rectangle
        *   @return <i>wxString<i>
        **/
        virtual const wxString& GetLabel() const;

        /** @brief Set the alignment of the bitmap in the rectangle
        *   @param nAlign, a union of one of wxALIGN_LEFT,wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT and one of wxALIGN_TOP, wxALIGN_BOTTOM, wxALIGN_CENTER_VERTICAL
        **/
        void SetBitmapAlign(unsigned int nAlign);

        ///< @brief Get the alignment of the bitmap in the rectangle
        unsigned int GetBitmapAlign() const
        {   return m_nBitmapAlign;    }

        /** @brief Sets the bitmap to draw in the rectangle
        *   @param bmp
        **/
        void SetBitmap(const wxBitmap& bmp);

        /** @brief Gets the bitmap that is currently drawn in the rectangle
        *   @return <i>wxBitmap<i>
        **/
        const wxBitmap& GetBitmap() const;

        /** @brief Draw the rectangle
        *   @param dc the wxDC context to draw to
        *   @param sLabel the text to draw (overrides any internal text)
        *   @param nState the type of the border to draw, one of uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE
        *   @param bmp the bitmap to draw (overrides any internal bitmap)
        **/
        virtual void Draw(wxDC& dc, const wxString& sLabel, unsigned short nState, const wxBitmap& bmp = wxNullBitmap);

        /** @brief Draw the rectangle using the internal label and bitmap
        *   @param dc the wxDC context to draw to
        *   @param nState the type of the border to draw, one of uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE
        **/
        virtual void Draw(wxDC& dc, unsigned short nState);

        /** @brief Checks whether the wxPoint pnt is contained within the rectangle
        *   @param pnt
        *   @return <i>bool</i> true if the point is within the rectangle
        **/
        bool Contains(const wxPoint& pnt) const;

        /** @brief Set the client data associated with the rectangle
        *   @param pData a void* pointer to some data
        **/
        void SetClientData(void* pData);

        /** @brief Get the client data associated with the rectangle
        *   @return <i>void*</i> the void* pointer set using SetClientData or 0
        **/
        void* GetClientData() const;

        /** @brief Set the integer data associated with the rectangle
        *   @param nData
        **/
        void SetIntData(int nData);

        /** @brief Get the integer data associated with the rectangle
        *   @return <i>int</i> the integer set using SetIntData or 0
        **/
        int GetIntData() const;

        /** @brief Set the index of the rectangle
        *   @param nIndex
        **/
        void SetIndex(size_t nIndex);

        /** @brief Get the index of the rectangle
        *   @return <i>size_t</i>
        **/
        size_t GetIndex() const;

        /** @brief Get the left point of the rectangle
        *   @return <i>int</i>
        **/
        int GetLeft() const;

        /** @brief Get the right point of the rectangle
        *   @return <i>int</i>
        **/
        int GetRight() const;

        /** @brief Get the height of the rectangle
        *   @return <i>int</i>
        **/
        int GetHeight() const;

        /** @brief Get the width of the rectangle
        *   @return <i>int</i>
        **/
        int GetWidth() const;

        /** @brief Get the top point of the rectangle
        *   @return <i>int</i>
        **/
        int GetTop() const;

        /** @brief Get the bottom point of the rectangle
        *   @return <i>int</i>
        **/
        int GetBottom() const;

        /** @brief Get the center wxPoint of the rectangle
        *   @return <i>wxPoint</i>
        **/
        wxPoint GetCenter() const;

        /** @brief Offset the rectangle
        *   @param x the amount to offset horizontally
        *   @param y the amount of offset vertically
        **/
        void Offset(int x, int y);

        /** @brief Set the left point of the rectangle
        *   @param n the point
        **/
        void SetLeft(int n);

        /** @brief Set the right point of the rectangle
        *   @param n the point
        **/
        void SetRight(int n);

        /** @brief Set the top point of the rectangle
        *   @param n the point
        **/
        void SetTop(int n);

        /** @brief Set the bottom point of the rectangle
        *   @param n the point
        **/
        void SetBottom(int n);

        /** @brief Set the width of the rectangle
        *   @param n the width
        **/
        void SetWidth(int n);

        /** @brief Set the height of the rectangle
        *   @param n the height
        **/
        void SetHeight(int n);

        /** @brief Get the enclosing rectangle
        *   @return <i>wxRect</i>
        **/
        const wxRect& GetRect() const
        { return m_rectEnclosing; }

        /** @brief Get the text rectangle (this is slightly smaller than the enclosing rectangle
        *   @return <i>wxRect</i>
        **/
        const wxRect& GetTextRect() const
        { return m_rectText; }

        void Pad(unsigned long nHorizontal, unsigned long nVertical);

        static wxSize GetSizeOfText(wxDC& dc, const wxString& sText, const wxRect& rect);

        wxSize GetSize() const;

        void ClipText(bool bClip);

        enum {BORDER_UP=0, BORDER_DOWN, BORDER_FLAT, BORDER_NONE};

    protected:
        virtual void RectSet(){}

        ///< @brief Draws an "up" border
        void DrawUp(wxDC& dc);

        ///< @brief Draws a "down" border
        void DrawDown(wxDC& dc);

        ///< @brief Draws a "flat" border
        void DrawFlat(wxDC& dc);

        /** Draws the text returning a wxSize which contains the width and height of the text
        *   @param dc
        *   @param sText the text the draw
        *   @param rect the rectangle to draw the text in
        *   @param nAlign the alignment of the text in the rectangle
        *   @param bWrap whether to word wrap the text or not
        *   @param bDraw whether to draw the text or simply calculate the height
        *   @param bClip whether to clip the text if it does not fix
        *   @return <i>wxSize</i> returns the height (and width) in pixels that the text will take up
        **/
        static wxSize DrawText(wxDC& dc,const wxString& sText, const wxRect& rect, int nAlign, bool bWrap=true, bool bDraw=true, bool bClip=true, bool bClippingRegion=true);

        ///< @brief Creates the text rect (which is slightly smaller than the enclosing rect
        void SetTextRect();

        wxRect m_rectEnclosing;         ///< @brief the rectangle
        wxRect m_rectText;              ///< @brief the rectangle that text will be drawn in
        wxColour m_clrBackgroundHigh;   ///< @brief the lighter background colour
        wxColour m_clrBackgroundLow;    ///< @brief the darker background colour
        wxColour m_clrBorderHigh;       ///< @brief the lighter border colour
        wxColour m_clrBorderLow;        ///< @brief the darker background colour

        wxColour m_clrText;             ///< @brief the colour of the text
        unsigned int m_nGradient;       ///< @brief direction of the background shading
        unsigned int m_nAlign;          ///< @brief the text alignment
        unsigned int m_nBitmapAlign;    ///< @brief the bitmap alignment

        void* m_pData;                  ///< @brief associated client data
        int m_nData;                    ///< @brief assoicated integer data

        wxString m_sLabel;              ///< @brief the text to draw
        wxBitmap m_bmp;                 ///< @brief the bitmap to draw

        size_t m_nIndex;                ///< @brief the index of the uirect

        bool m_bClip;

};



