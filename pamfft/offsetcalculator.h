#pragma once
#include <wx/event.h>
#include <wx/thread.h>
#include "pamfftdll.h"
#include <math.h>
#include <vector>



/** Class that attempts to calculate the offset in milliseconds between the a and b leg of a stereo sample. Based on BBC R&D code
**/
class PAMFFT_IMPEXPORT OffsetCalculator : public wxThread
{
    public:
        ///< constructor
        OffsetCalculator(wxEvtHandler* pHandler, const std::vector<float>& vInLeft, const std::vector<float>& vInRight);

        void* Entry();


        //static const unsigned long DEFAULT_CORR_BLOCK_SIZE  = 65535;
    protected:

        /** The actual code
        **/
        void WindowData(std::vector<float>& vData);

        wxEvtHandler* m_pHandler;

        std::vector<float> m_vfft_inL;
        std::vector<float> m_vfft_inR;





};

wxDECLARE_EXPORTED_EVENT(PAMFFT_IMPEXPORT, wxEVT_OFFSET_DONE, wxCommandEvent);
