#pragma once
#include <vector>
#include <map>
#include "dlldefine.h"
#include <wx/string.h>

struct PAMBASE_IMPEXPORT ppmtype
{
    ppmtype() : dOffset(0), dRiseTime(0), dRisedB(90), dFallTime(0), dFalldB(20), dScaling(1), nType(0), dOverMod(0.0){}
    std::vector<double> vLevels;
    double dOffset;
    double dRiseTime;
    double dRisedB;
    double dFallTime;
    double dFalldB;
    double dScaling;
    long nType;
    wxString sUnit;
    wxString sReference;
    double dOverMod;
};

class PAMBASE_IMPEXPORT PPMTypeManager
{
    public:
        static PPMTypeManager& Get();

        std::map<wxString, ppmtype>::const_iterator GetTypeBegin();
        std::map<wxString, ppmtype>::const_iterator GetTypeEnd();
        std::map<wxString, ppmtype>::const_iterator FindType(const wxString& sType);


    private:
        PPMTypeManager();
        void LoadTypes();

        std::map<wxString, ppmtype> m_mTypes;

};




//const double SCALE_BRITISH[9]      = {-34,-30,-26,-22,-18,-14,-10,-6,-2};
//const double SCALE_DIN[9]      = {-62,-52,-42,-32,-22,-17,-12,-7,-2};
//const double SCALE_NORDIC[10]   = {-54,-48,-42,-36,-30,-24,-18,-12,-9,-6};
//const double SCALE_DIGITAL[11] = {-60,-50,-40,-35,-30,-25,-20,-15,-10,-5,0};
//const double SCALE_VU[9]       = {-34, -24, -21, -19, -17, -14, -11,-6,-3};
//const double SCALE_EBU[9]       = {-34,-30,-26,-22,-18,-14,-10,-6,-2};
//const double SCALE_DBFS[15]       = {-60,-54,-48,-42,-36,-30,-24,-21,-18,-15,-12,-9,-6,-3,0};
//const double SCALE_DBU_NICAM[15]       = {-67, -61, -55, -49, -43, -37, -31, -28, -25, -22, -19, -16, -13, -6, 0,};
//
//const double OFFSET_BRITISH    = -34;
//const double OFFSET_DIN    = -12;
//const double OFFSET_NORDIC = -18;
//const double OFFSET_DIGITAL = 0;
//const double OFFSET_VU = -14;
//const double OFFSET_EBU = -18;
//const double OFFSET_DBU_NICAM = -13;
//const double OFFSET_DBU = -18;
//
//
//const double RISETIME_BRITISH   =
//const double RISEAMOUNT_BRITISH
