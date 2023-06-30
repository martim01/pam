#pragma once
#include <vector>
#include <map>
#include "dlldefine.h"
#include <wx/string.h>
#include <set>
#include <wx/colour.h>

struct PAMBASE_IMPEXPORT r128type
{
    r128type() {}
    std::vector<double> vLevels;
    double dTarget = -38.0;
    double dMax = 0.0;
    double dMin = -59.0;
    double dOk = -26.0;
    double dHigh = -20.0;
    wxColour clrLow;
    wxColour clrOk;
    wxColour clrHigh;
};

class PAMBASE_IMPEXPORT R128TypeManager
{
    public:
        static R128TypeManager& Get();

        std::map<wxString, r128type>::const_iterator GetTypeBegin();
        std::map<wxString, r128type>::const_iterator GetTypeEnd();
        std::map<wxString, r128type>::const_iterator FindType(const wxString& sType);

        std::set<wxString> GetTypes();


    private:
        R128TypeManager();
        void LoadTypes();

        std::map<wxString, r128type> m_mTypes;

};

