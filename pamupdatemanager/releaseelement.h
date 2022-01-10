#pragma once
#include "advelement.h"
class wxDC;

class  ReleaseElement : public advElement
{
    public:

        ReleaseElement(wxDC& dc, unsigned int nWidth, wxString sMessage);
        virtual ~ReleaseElement(){}

        void Destroy();

        void SelectSubElement(size_t nSub);
        void EnableSubElement(size_t nSub, bool bEnable);

        int GetMessageLevel() const
        {
            return m_nLevel;
        }


    protected:

        bool SubElementDown(const wxPoint& pnt);
        bool SubElementUp(const wxPoint& pnt, bool bIn);
        bool SubElementMove(const wxPoint& pnt);
        int SubElementHeld();
        void Draw(wxDC& dc, bool bSelected);

        void CreateHitRect(int nHeight, const wxString& sLine);
        void ElementMoved();

        wxFont GetFont(wxDC& dc);

        unsigned int m_nHeader;
        unsigned long m_nHeight;
};



