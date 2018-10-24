#pragma once
#include <vector>
#include <wx/textctrl.h>
#include "uirect.h"
#include "pmcontrol.h"
/** @brief An edit control that allows easy editing of time and dates. It has a number of different time and date formats and allows the user to enter them without having to worry about validation
**/
class PAMBASE_IMPEXPORT wmTimeEdit : public pmControl
{
    public:
        wxDECLARE_DYNAMIC_CLASS(wmTimeEdit);
        DECLARE_EVENT_TABLE()
        ///< @brief Default Constructor
        wmTimeEdit();

        ///< @brief Destruction
        virtual ~wmTimeEdit();

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
        wmTimeEdit(wxWindow *parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);

        /** @brief Constructor
        *   @param parent the parent window
        *   @param id the wxWindowId of the control
        *   @param nDateFormat the date format of the control - one of @ref DateFormats
        *   @param nTimeFormat the time format of the control - one of @ref TimeFormats
        *   @param nFrames the number of frames within one second - should the wmTimeEdit::TIME_HMSFr be being used
        *   @param bTimeFirst if true then the time is shown before the date. If false the date is shown before the time
        *   @param chDate the character to show between each part of the date
        *   @param chTime the character to show between each part of the time
        *   @param pos the position of the control
        *   @param size the size of the control
        **/
        wmTimeEdit(wxWindow* pParent, wxWindowID id, unsigned int nDateFormat, unsigned int nTimeFormat, unsigned int nFrames, bool bTimeFirst = true, wxChar chDate=wxT('/'), wxChar chTime=wxT(':'), const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize);

        /** @brief Creates the control
        *   @param parent the parent window
        *   @param id the wxWindowId of the control
        *   @param nDateFormat the date format of the control - one of DateFormats
        *   @param nTimeFormat the time format of the control - one of TimeFormats
        *   @param nFrames the number of frames within one second - should the wmTimeEdit::TIME_HMSFr be being used
        *   @param bTimeFirst if true then the time is shown before the date. If false the date is shown before the time
        *   @param chDate the character to show between each part of the date
        *   @param chTime the character to show between each part of the time
        *   @param pos the position of the control
        *   @param size the size of the control
        **/
    bool Create(wxWindow* pParent, wxWindowID id, unsigned int nDateFormat, unsigned int nTimeFormat, unsigned int nFrames, bool bTimeFirst = true, wxChar chDate=wxT('/'), wxChar chTime=wxT(':'), const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long nStyle=0);


        /** @brief Returns the wxDateTime that has been entered in to the control
        *   @return <i>wxDateTime</i>
        **/
        const wxDateTime GetDateTime() const
        {   return m_dtTimestamp;   }

        /** @brief Sets the date and time to display in the control
        *   @param dtTime the date and time to display
        **/
        void SetDateTime(const wxDateTime& dtTime);

        /** @brief Enables/disables the control
        **/
        bool Enable(bool bEnable = true);

        /** @brief Gets the best size of the control for sizers
        **/
        wxSize DoGetBestSize() const;



        virtual bool SetBackgroundColour(const wxColour& clr);
        virtual bool SetForegroundColour(const wxColour& clr);
        void SetNonFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder);
        void SetFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder);

        void SetBorderStyle(unsigned long nNonFocused, unsigned long nFocused);
        void SetFocusedBackground(const wxColour& clr);
        void SetFocusedForeground(const wxColour& clr);

        void SetTextAlign(unsigned long nAlign);


        //@{
        /** @name DateFormats
        *   @anchor DateFormats
        *   @brief The possible date formats the control can display
        **/
        static const unsigned int DATE_NONE;        ///< @brief No date displayed
        static const unsigned int DATE_DDMMYYYY;    ///< @brief Display 'day, month, year'
        static const unsigned int DATE_MMDDYYYY;    ///< @brief Display 'month, day, year'
        static const unsigned int DATE_YYYYMMDD;    ///< @brief Display 'year, month, day'
        static const unsigned int DATE_DDMMYY;      ///< @brief Display 'day, month, year (not including century)'
        static const unsigned int DATE_MMDDYY;      ///< @brief Display 'month, day, year (not including century)'
        static const unsigned int DATE_YYMMDD;      ///< @brief Display 'year (not including century), month, day'
        //@}

        //@{
        /** @name TimeFormats
        *   @anchor TimeFormats
        *   @brief The possible time formats the control can display
        **/
        static const unsigned int TIME_NONE;    ///< @brief No time displayed
        static const unsigned int TIME_HM;      ///< @brief Display 'Hour, Minute'
        static const unsigned int TIME_HMS;     ///< @brief Display 'Hour, Minute, Second'
        static const unsigned int TIME_HMSmS;   ///< @brief Display 'Hour, Minute, Second, Millisecond'
        static const unsigned int TIME_HMSFr;   ///< @brief Display 'Hour, Minute, Second, Frame'
        //@}

        //@{
        /** @name DateFormatStrings
        *   @anchor DateFormatStrings
        *   @brief The possible date formats the control can display in string format
        **/
        static const wxString DDMMYYYY; ///< @brief Display 'day, month, year'.
        static const wxString MMDDYYYY; ///< @brief Display 'month, day, year'.
        static const wxString YYYYMMDD; ///< @brief Display 'year, month, day'.
        static const wxString DDMMYY;   ///< @brief Display 'day, month, year (not including century)'.
        static const wxString MMDDYY;   ///< @brief Display 'month, day, year (not including century)'.
        static const wxString YYMMDD;   ///< @brief Display 'year (not including century), month, day'.
        //@}

        //@{
        /** @name TimeFormatStrings
        *   @anchor TimeFormatStrings
        *   @brief The possible time formats the control can display in string format
        **/
        static const wxString HM;       ///< @brief Display 'Hour, Minute'.
        static const wxString HMS;      ///< @brief Display 'Hour, Minute, Second'.
        static const wxString HMSmS;    ///< @brief Display 'Hour, Minute, Second, Millisecond'.
        static const wxString HMSFr;    ///< @brief Display 'Hour, Minute, Second, Frame'.
        //@}

        static const unsigned int EVENT_TIME_ENTERED;

    protected:
        void OnPaint(wxPaintEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnMouse(wxMouseEvent& event);
        void OnCaptureLost(wxMouseCaptureLostEvent& event);
        void OnSetFocus(wxFocusEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnChar(wxKeyEvent& event);

        void SetTimeFormat(wxChar chTime);
        void SetDateFormat(wxChar chDate);


        void Char(wxChar chChar);
        void CheckDays();
        void MoveTime(int nWay);

        void SetString(const wxDateTime& dt);
        bool ConvertToDateTime();
        wxChar MaxDayTen();
        wxChar MaxDayUnit();


        void CreateCaret();
        void HideCaret();
        void ChangeInsertMode(bool bInsert, wxDC& dc);
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


        static const unsigned int SKIP;
        static const unsigned int DAY_UNIT;
        static const unsigned int DAY_TEN;
        static const unsigned int MONTH_UNIT;
        static const unsigned int MONTH_TEN;
        static const unsigned int YEAR_UNIT;
        static const unsigned int YEAR_TEN;
        static const unsigned int YEAR_CEN;
        static const unsigned int YEAR_MIL;
        static const unsigned int HOUR_TEN;
        static const unsigned int HOUR_UNIT;
        static const unsigned int MINUTE_TEN;
        static const unsigned int MINUTE_UNIT;
        static const unsigned int SECOND_TEN;
        static const unsigned int SECOND_UNIT;
        static const unsigned int MILLI_HUND;
        static const unsigned int MILLI_TEN;
        static const unsigned int MILLI_UNIT;
        static const unsigned int FRAME_TEN;
        static const unsigned int FRAME_UNIT;

        static const wxString TIME;

        wxString m_sText;
        wxDateTime m_dtTimestamp;
        bool m_bFocus;
        wxCaret* m_pCaret;
        unsigned int m_nInsert;
        bool m_bInsert;

        uiRect m_uiText;
        wxColour m_clrBackground[2];
        wxColour m_clrForeground[2];
        unsigned int m_nBorderStyle[2];

};


