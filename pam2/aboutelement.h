#pragma once
#include "advelement.h"
class wxDC;

class  AboutElement : public advElement
{
    public:

        AboutElement(unsigned int nWidth, const wxString& sName, const wxString& sVersion, const wxString& sDate, const wxString& sTag, const wxString& sBranch);
        virtual ~AboutElement(){}

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

        enum{ID_TITLE, ID_VERSION, ID_DATE_LABEL, ID_DATE, ID_TAG_LABEL, ID_TAG, ID_BRANCH_LABEL, ID_BRANCH, ID_ENCLOSING};

        static const wxColour CLR_LABEL;
        static const wxColour CLR_DATA;
        static const wxColour CLR_BACK;

        unsigned long HEIGHT_TITLE = 40;
        unsigned long HEIGHT_REST = 20;
        unsigned long WIDTH_LABEL = 52;
        unsigned long WIDTH_DATA = 144;
};



