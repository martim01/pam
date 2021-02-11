#pragma once
#include "advelement.h"
class wxDC;

class  SdpElement : public advElement
{
    public:

        SdpElement(wxDC& dc, unsigned int nWidth, wxString sMessage);
        virtual ~SdpElement(){}

        void Destroy();

        void SelectSubElement(size_t nSub);
        void EnableSubElement(size_t nSub, bool bEnable);


    protected:

        bool SubElementDown(const wxPoint& pnt);
        bool SubElementUp(const wxPoint& pnt, bool bIn);
        bool SubElementMove(const wxPoint& pnt);
        int SubElementHeld();
        void Draw(wxDC& dc, bool bSelected);

        void CreateHitRect(size_t nId, int nHeight, const wxString& sLine);
        void ElementMoved();
        static const unsigned int COLUMN_TIME = 100;
        unsigned long m_nHeight;
};



