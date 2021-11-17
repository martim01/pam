#pragma once
#include "wmbuttonfactory.h"
#include "uirect.h"
#include <wx/log.h>
#include "nmosapiversion.h"

class uiNode: public uiRect
{
    public:
        uiNode(const wxRect& rect = wxRect(0,0,0,0), unsigned int nGradient=wxEAST);
        virtual ~uiNode(){}


        virtual void Draw(wxDC& dc, const wxString& sLabel, unsigned short nState, const wxBitmap& bmp = wxNullBitmap);

        /** @brief Draw the rectangle using the internal label and bitmap
        *   @param dc the wxDC context to draw to
        *   @param nState the type of the border to draw, one of uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE
        **/
        virtual void Draw(wxDC& dc, unsigned short nState);

        virtual void RectSet();

        void SetLabel(const wxString& sLabel);
        const wxString& GetLabel() const;

        void SetOK(bool bOK);
        void Select(bool bSelect);

        void SetPriority(unsigned short nPriority);
        void SetVersion(const pml::nmos::ApiVersion& version);

    protected:
        void SetColour();

        uiRect m_uiUrl;
        uiRect m_uiPriority;
        uiRect m_uiVersion;

        bool m_bOK;
        bool m_bSelected;

        static const wxColour CLR_OK;
        static const wxColour CLR_BAD;
        static const wxColour CLR_SELECTED;

};



class wmNodeButtonFactory : public wmButtonFactory
{
    public:
        wmNodeButtonFactory(){}
        virtual uiRect* CreateButton()
        {
            return new uiNode();
        }
};

