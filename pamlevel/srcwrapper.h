#pragma once
#include "samplerate.h"

struct SrcWrapper
{
    SrcWrapper() : pState(0){}
    SRC_STATE* pState;
    SRC_DATA data;
};
