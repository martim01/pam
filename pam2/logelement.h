#pragma once
#include "advelement.h"
class wxDC;

class  LogElement : public advElement
{
    public:

        LogElement(wxDC& dc, unsigned int nWidth, const wxString& sMessage, int nTest);
        virtual ~LogElement(){}

        virtual void Destroy();

        virtual void SelectSubElement(size_t nSub);
        void EnableSubElement(size_t nSub, bool bEnable);

        int GetMessageType() const
        {
            return m_nType;
        }

        void Filter(int nFilter) const;

    protected:

        virtual bool SubElementDown(const wxPoint& pnt);
        virtual bool SubElementUp(const wxPoint& pnt, bool bIn);
        virtual bool SubElementMove(const wxPoint& pnt);
        virtual int SubElementHeld();
        virtual void Draw(wxDC& dc, bool bSelected);

        virtual void CreateHitRects();
        virtual void ElementMoved();
        static const unsigned int COLUMN_TIME = 100;
        unsigned long m_nHeight;
        int m_nType;
};


