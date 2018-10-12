#pragma once
#include <wx/control.h>
#include <vector>
#include <wx/textctrl.h>
#include "uirect.h"
#include "dlldefine.h"


/** @brief An edit control that allows easy editing of time and dates. It has a number of different time and date formats and allows the user to enter them without having to worry about validation
**/
class  PAMBASE_IMPEXPORT wmIpEdit : public wxControl
{
    DECLARE_DYNAMIC_CLASS(wmIpEdit)
    DECLARE_EVENT_TABLE()

    public:
        ///< @brief Default Constructor
        wmIpEdit();

        ///< @brief Destruction
        virtual ~wmIpEdit();

        /** @brief Constructor - made to be the same as wxTextCtrl
        *   @param parent the parent window
        *   @param id the wxWindowId of the control
        *   @param value the format of the date and time in the control. This should be of the form ' @ref DateFormatStrings: @ref TimeFormatStrings'
        *   @param pos the position of the control
        *   @param size the size of the control
        *   @param style not used
        *   @param validator not used
        *   @param name not used
        **/
        wmIpEdit(wxWindow *parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);

        /** @brief Creates the control
        *   @param parent the parent window
        *   @param id the wxWindowId of the control
        *   @param nDateFormat the date format of the control - one of DateFormats
        *   @param nTimeFormat the time format of the control - one of TimeFormats
        *   @param nFrames the number of frames within one second - should the wmIpEdit::TIME_HMSFr be being used
        *   @param bTimeFirst if true then the time is shown before the date. If false the date is shown before the time
        *   @param chDate the character to show between each part of the date
        *   @param chTime the character to show between each part of the time
        *   @param pos the position of the control
        *   @param size the size of the control
        **/
    bool Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long nStyle=0);



        /** @brief Enables/disables the control
        **/
        bool Enable(bool bEnable = true);

        /** @brief Gets the best size of the control for sizers
        **/
        wxSize DoGetBestSize() const;

        void SetValue(const wxString& sIpAddress);
        const wxString& GetValue() const;

        void SetValidation(unsigned short nValidation){}


        virtual bool SetBackgroundColour(const wxColour& clr);
        virtual bool SetForegroundColour(const wxColour& clr);
        void SetNonFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder);
        void SetFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder);

        void SetBorderStyle(unsigned long nNonFocused, unsigned long nFocused);
        void SetFocusedBackground(const wxColour& clr);
        void SetFocusedForeground(const wxColour& clr);

        void SetTextAlign(unsigned long nAlign);



    protected:
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMouse(wxMouseEvent& event);
        void OnCaptureLost(wxMouseCaptureLostEvent& event);
        void OnSetFocus(wxFocusEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnChar(wxKeyEvent& event);



        void Char(wxChar chChar);

        void SetString(const wxDateTime& dt);


        void CreateCaret();
        void HideCaret();
        void SetInsertPos(int nPos);
        unsigned int GetInsertPos() const;
        bool Insert() const;
        wxPoint GetCaretPos() const;
        void ReleaseCaret();
        void SetCaretPos(wxDC& dc, const wxString& sText);
        wxSize GetCaretSize(wxDC& dc);


        unsigned int m_nFrames;
        double m_dFrameTime;

        std::vector<unsigned int> m_vType;
        wxString m_sFormat;

        unsigned int m_nTimeFormat;
        unsigned int m_nDateFormat;

        unsigned int m_nDayPos;
        unsigned int m_nMonthPos;
        unsigned int m_nYearPos;
        unsigned int m_nMilliPos;


        enum {DOT=0, HUNDREDS, TENS, UNITS};


        wxString m_sText[4];
        wxDateTime m_dtTimestamp;
        bool m_bFocus;
        wxCaret* m_pCaret;
        unsigned int m_nInsert;
        bool m_bInsert;

        uiRect m_uiText[4];
        wxColour m_clrBackground[2];
        wxColour m_clrForeground[2];
        unsigned int m_nBorderStyle[2];

};


