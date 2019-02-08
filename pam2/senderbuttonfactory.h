#pragma once
#include "wmbuttonfactory.h"
#include "uirect.h"
#include <wx/log.h>


class uiSender : public uiRect
{
    public:
        uiSender(const wxRect& rect = wxRect(0,0,0,0), unsigned int nGradient=wxEAST);
        virtual ~uiSender(){}

        enum enumState{STATE_IDLE=0, STATE_ASKED, STATE_REPLIED, STATE_ACTIVE, STATE_ERROR};
        void SetState(enumState eState);

        virtual void Draw(wxDC& dc, const wxString& sLabel, unsigned short nState, const wxBitmap& bmp = wxNullBitmap);

        /** @brief Draw the rectangle using the internal label and bitmap
        *   @param dc the wxDC context to draw to
        *   @param nState the type of the border to draw, one of uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE
        **/
        virtual void Draw(wxDC& dc, unsigned short nState);

        virtual void RectSet();

    protected:
        uiRect m_uiState;
        uiRect m_uiSender;

        static const wxColour CLR_STATE[5];
        static const wxString STR_STATE[5];

};



class wmSenderButtonFactory : public wmButtonFactory
{
    public:
        wmSenderButtonFactory(){}
        virtual uiRect* CreateButton()
        {
            return new uiSender();
        }
};
