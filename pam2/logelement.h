#pragma once
#include "advelement.h"
class wxDC;

class  LogElement : public advElement
{
    public:

        LogElement(wxDC& dc, unsigned int nWidth, const wxString& sMessage, int nLevel);
        virtual ~LogElement(){}

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

        void CreateHitRect(size_t nId, int nHeight, const wxString& sLine);
        void ElementMoved();
        static const unsigned int COLUMN_TIME = 100;
        unsigned long m_nHeight;
        int m_nLevel;
};


