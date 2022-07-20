#pragma once
#include "timedbuffer.h"


extern float PearsonCorrelation(const timedbuffer* pBuffer, unsigned long nChannels, unsigned long nChannelX, unsigned long nChannelY, bool bCosineOnly=false);
