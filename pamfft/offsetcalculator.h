#pragma once
#include <wx/event.h>
#include "pamfftdll.h"
#include <math.h>
#include <vector>


extern int CalculateOffset(std::vector<float>& vInLeft, std::vector<float>& vInRight);
extern void CalculateOffset(wxEvtHandler* pHandler, const std::vector<float>& vInLeft, const std::vector<float>& vInRight);

wxDECLARE_EXPORTED_EVENT(PAMFFT_IMPEXPORT, wxEVT_OFFSET_DONE, wxCommandEvent);
