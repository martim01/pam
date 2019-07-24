#pragma once

#include <wx/control.h>
#include <vector>
#include <wx/textctrl.h>
#include <set>
#include "uirect.h"
#include "pmcontrol.h"

/** @brief Class that provides an alternative text edit control
**/
class PAMBASE_IMPEXPORT wmEdit : public pmControl
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmEdit)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmEdit);
    #endif // WXSPAM

    DECLARE_EVENT_TABLE()

    public:
        ///< @brief Default Constructor
        wmEdit();

        ///< @brief Destructor
        virtual ~wmEdit();

        /** @brief Constructor
        *   @param parent the parent window
        *   @param id the wxWindowId of the control
        *   @param value the text to display in the edit control
        *   @param pos the position of the control
        *   @param size the size of the control
        *   @param validator not used
        *   @param name not used
        **/
        wmEdit(wxWindow *parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);



        /** @brief Creates the control
        *   @param parent the parent window
        *   @param id the wxWindowId of the control
        *   @param value the text to display in the edit control
        *   @param pos the position of the control
        *   @param size the size of the control
        **/
        bool Create(wxWindow* pParent, wxWindowID id, const wxString& value, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style=0);

        /** @brief Sets the text in the control
        *   @param sValue the text
        **/
        void SetValue(const wxString& sValue);

        /** @brief Returns the text that is in the control
        *   @return <i>wxString</i>
        **/
        const wxString& GetValue() const
        {
            return m_sText;
        }

        /** @brief Enables/disables the control
        *   @param bEnable
        **/
        bool Enable(bool bEnable = true);

        /** Sets the validation type of the control. Depending on the validation type different letters/numbers may be entered in to the control. Possible values areh:
        <ul>
            <li>wmEdit::ASCII (any ASCII character may be entered)</li>
            <li>wmEdit::ALPHA (any letter may be entered)</li>
            <li>wmEdit::ALPHANUMERIC (any letter or number may be entered)</li>
            <li>wmEdit::INTEGER (any integer number may be entered)</li>
            <li>wmEdit::REAL_NUMBER (any real number may be entered)</li>
        </ul>
        *   @param nValidation
        **/
        void SetValidation(unsigned short nValidation)
        {
            m_nValidation = nValidation;
        }

        /** @brief Sets whether letters entered in to the control should be automatically capitalized.
        *   @param bCapitalize
        **/
        void Capitalize(bool bCapitalize=true);

        /** @brief Returns the best size of the control for sizes
        *   @return <i>wxSize</i>
        **/
        wxSize DoGetBestSize() const;

        /** @brief If bExclude is true then any character in the string sExclude cannot be entered in to the control. If bExclude is false than only characters that are in the string can be entered in to the control. Any call to this function will override previous calls
        *   @param sExclude a string containing characters to include/exclude
        *   @param bExclude
        **/
        void SetExclude(const wxString& sExclude, bool bExclude);

        /** @brief Sets the maximum number of characters that you can enter in to the control
        *   @param nLength the maximum number of characters
        **/
        void SetMaxLength(long nLength)
        {
            m_nMaxCharacters = nLength;
        }

        void SetInsertPos(int nPos);
        unsigned int GetInsertPos() const;
        void DeleteChar();
        void ChangeInsertMode(bool bInsert);

        virtual bool SetBackgroundColour(const wxColour& clr);
        virtual bool SetForegroundColour(const wxColour& clr);
        void SetNonFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder);
        void SetFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder);

        void SetBorderStyle(unsigned long nNonFocused, unsigned long nFocused);
        void SetFocusedBackground(const wxColour& clr);
        void SetFocusedForeground(const wxColour& clr);

        static const unsigned short ASCII           = 1;    ///< @brief any ASCII character may be entered
        static const unsigned short ALPHA           = 2;    ///< @brief any letter may be entered
        static const unsigned short ALPHANUMERIC    = 3;    ///< @brief any letter or number may be entered
        static const unsigned short INTEGER         = 4;    ///< @brief any integer may be entered
        static const unsigned short REAL_NUMBER     = 5;    ///< @brief any real number may be entered-
        static const unsigned short DIGITS          = 6;    ///< @brief only digits
        static const unsigned short IP              = 7;    ///< @brief . and number



    protected:
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMouse(wxMouseEvent& event);
        void OnCaptureLost(wxMouseCaptureLostEvent& event);
        void OnSetFocus(wxFocusEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnChar(wxKeyEvent& event);
        //void OnKeyDown(wxKeyEvent& event);
        //void OnKeyUp(wxKeyEvent& event);


        void Char(wxChar chChar);
        bool Validate(const wxChar& ch);
        bool AllowChar(const wxChar& ch);

        wxString AddChar(wxChar ch);
        void CreatePassword();

        void UpdateText(const wxString& sUpdate);

        void CreateCaret();
        void HideCaret();
        void ChangeInsertMode(bool bInsert, wxDC& dc);


        bool Insert() const;
        wxPoint GetCaretPos() const;
        void ReleaseCaret();
        void SetCaretPos(wxDC& dc, const wxString& sText);
        wxSize GetCaretSize(wxDC& dc);


        wxString m_sText;
        wxString m_sPassword;
        long m_nStyle;



        bool m_bFocus;
        wxCaret* m_pCaret;
        unsigned int m_nInsert;
        bool m_bInsert;

        short m_nValidation;
        size_t m_nMaxCharacters;

        uiRect m_uiText;


        std::set<wxChar> m_setCheck;
        bool m_bExclude;

        bool m_bCapitalize;

//        wmScroller* m_pScroller;
        wxColour m_clrBackground[2];
        wxColour m_clrForeground[2];
        unsigned int m_nBorderStyle[2];
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_TEXT_ESCAPE,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_TEXT_TAB,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_TEXT_BACKSPACE,-1)

