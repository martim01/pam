#pragma once
#include <wx/window.h>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <wx/timer.h>
#include <wx/bitmap.h>
#include "pmcontrol.h"
#include "uirect.h"

/** @brief Control that displays a touchscreen keyboard. It is possible to show a full QWERTY, Number Pad, Phone or Navigational keyaoard.
**/
class PAMBASE_IMPEXPORT wmKeyboard : public pmControl
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmKeyboard)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmKeyboard);
    #endif // WXSPAM
    DECLARE_EVENT_TABLE()

   public:
        ///< @brief Default constructor
        wmKeyboard();
        ///< @brief Destructor
        virtual ~wmKeyboard();

        /** @brief Constructor - simply calls Create
        *   @param pParent the parent window
        *   @param id the wxWindowID of the keyboard
        *   @param pos the top-left position of the keyboard in its parent window
        *   @param size the size of the keyboard
        *   @param nStyle the type of keyboard to show
        *   @param bHints whether to show a hints window showing possible words
        **/
        wmKeyboard(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, unsigned int nStyle=KBS_NORMAL, bool bHints=false);

        /** Creates the keyboard and intiliazes it
        *   @param sName the name of the keyboard
        *   @param pParent the parent window
        *   @param id the wxWindowID of the keyboard
        *   @param pos the top-left position of the keyboard in its parent window
        *   @param size the size of the keyboard
        **/
        bool Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, unsigned int nStyle=KBS_NORMAL, bool bHints=false);

        /** Returns the best size for the keyboard for wxSizer
        *   @return <i>wxSize</i>
        **/
        wxSize DoGetBestSize() const;


        /** @brief Sets the text to display on the "Return" button
        *   @param sReturn the text to show
        **/
        void SetReturnString(const wxString& sReturn);

        /** @brief Sets the text to display on the "Escape" button
        *   @param sEscape the text to show
        **/
        void SetEscapeString(const wxString& sEscape);

        /** @brief Sets the text to display on the "blank" button
        *   @param sBlank the text to show
        **/
        void SetBlankString(const wxString& sBlank);


        static const unsigned int KBS_NORMAL = 0;       ///< @brief a qwerty keyboard
        static const unsigned int KBS_NUMPAD = 1;       ///< @brief a number pad
        static const unsigned int KBS_PHONE  = 2;       ///< @brief a phone kekpad
        static const unsigned int KBS_NAVIGATE= 3;      ///< @brief navigational buttons
        static const unsigned int KBS_CAPS   = 4;       ///< @brief a QWERTY keyboard
        static const unsigned int KBS_CALC   = 5;       ///< @brief a QWERTY keyboard

    protected:
        /** Called to draw the keyboard
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the keyboard resizes
        *   @param event
        **/
        void OnSize(wxSizeEvent& event);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);

        void OnCaptureLost(wxMouseCaptureLostEvent& event);
        void OnTimer(wxTimerEvent& event);

        void Draw(wxDC& dc);

        void CreateRects();
        unsigned int CreateHintRect();
        void CreateNormal();
        void CreateCaps();
        void CreateNumpad();
        void CreatePhone();
        void CreateNavigate();
        void CreateCalc();

        void SendKeyEvent(wxEventType evType);
        void SendKeyEvent(wxEventType evType, unsigned int nKeyCode);

        void SetButtonText(unsigned short nButtonType, const wxString& sText);

        struct button
        {
            button(const wxString& sn,const wxString& ss,const wxString& sSy,
                   const wxString& sSy2, const wxRect& rect, short nt=0) : rectEnclosing(rect), nType(nt)
                   {
                        sKey[0] = sn;
                        sKey[1] = ss;
                        sKey[2] = sSy;
                        sKey[3] = sSy2;
                        for(size_t i = 0; i < 4; i++)
                        {
                            nKey[i] = sKey[i].GetChar(0);
                        }
                        rectSwipe[SWIPE_UP] = wxRect(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight()/4);
                        rectSwipe[SWIPE_DOWN] = wxRect(rect.GetLeft(), rect.GetBottom() - rect.GetHeight()/4, rect.GetWidth(), rect.GetHeight()/4);
                        rectSwipe[SWIPE_LEFT] = wxRect(rect.GetLeft(), rect.GetTop(), rect.GetWidth()/4, rect.GetHeight());
                        rectSwipe[SWIPE_RIGHT] = wxRect(rect.GetLeft()-rect.GetWidth()/4, rect.GetTop(), rect.GetWidth()/4, rect.GetHeight());
                   }
            button(const wxBitmap& bmpN, const wxBitmap& bmpS, const wxBitmap& bmpSy, const wxBitmap& bmpSy2, unsigned int nK, const wxRect& rect, short nt=0) : rectEnclosing(rect), nType(nt)
            {
                bmpKey[0] = bmpN;
                bmpKey[1] = bmpS;
                bmpKey[2] = bmpSy;
                bmpKey[3] = bmpSy2;
                for(size_t i = 0; i < 4; i++)
                {
                    nKey[i] = nK;
                }
                rectSwipe[SWIPE_UP] = wxRect(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight()/4);
                rectSwipe[SWIPE_DOWN] = wxRect(rect.GetLeft(), rect.GetBottom() - rect.GetHeight()/4, rect.GetWidth(), rect.GetHeight()/4);
                rectSwipe[SWIPE_LEFT] = wxRect(rect.GetLeft(), rect.GetTop(), rect.GetWidth()/4, rect.GetHeight());
                rectSwipe[SWIPE_RIGHT] = wxRect(rect.GetLeft()-rect.GetWidth()/4, rect.GetTop(), rect.GetWidth()/4, rect.GetHeight());
            }
            wxString sKey[4];
            wxString sSwipe[4];

            wxBitmap bmpKey[4];
            unsigned int nKey[4];
            wxRect rectEnclosing;
            wxRect rectSwipe[4];
            short nType;
        };

        static const short KEY_CHAR = 0;
        static const short KEY_RETURN = 1;
        static const short KEY_SHIFT = 2;
        static const short KEY_ABC = 3;
        static const short KEY_OTHER = 4;
        static const short KEY_BACK = 5;
        static const short KEY_ESCAPE = 6;
        static const short KEY_NAV = 7;
        static const short KEY_BLANK = 8;

        static const short MODE_NORMAL = 0;
        static const short MODE_SHIFT = 1;
        static const short MODE_SYMBOL = 2;
        static const short MODE_FN = 3;

        static const short SWIPE_UP     = 0;
        static const short SWIPE_RIGHT  = 1;
        static const short SWIPE_DOWN   = 2;
        static const short SWIPE_LEFT   = 3;


        unsigned int m_nStyle;
        std::list<button> m_lstButtons;
        std::list<button>::iterator m_itDown;

        unsigned short m_nMode;

        wxTimer m_timerRepeat;
        bool m_bHints;
        uiRect m_uiHints[3];

};



