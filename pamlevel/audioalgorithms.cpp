#include "audioalgorithms.h"
#include <cmath>


float PearsonCorrelation(const timedbuffer* pBuffer, unsigned long nChannels, unsigned long nChannelX, unsigned long nChannelY, bool bCosineOnly)
{
    double dBalance[2] = {0.0, 0.0};
    double dProduct(0.0);

    //get the average value first
    float dAverage[2] = {0.0, 0.0};

    if(!bCosineOnly)
    {
        for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=nChannels)
        {
            dAverage[0] += pBuffer->GetBuffer()[i+nChannelX];
            dAverage[1] += pBuffer->GetBuffer()[i+nChannelY];
        }
        float dCount = (static_cast<float>(pBuffer->GetBufferSize())/static_cast<float>(nChannels));
        dAverage[0] /= dCount;
        dAverage[1] /= dCount;

    }

    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=nChannels)
    {
        float dX = pBuffer->GetBuffer()[i+nChannelX]-dAverage[0];
        float dY = pBuffer->GetBuffer()[i+nChannelY]-dAverage[1];

        dBalance[0] += pow(dX,2);
        dBalance[1] += pow(dY,2);
        dProduct += dX*dY;

    }
    if(dBalance[0] == 0.0 || dBalance[1] == 0.0)
    {
        return 0.0;
    }
    return dProduct/sqrt(dBalance[0]*dBalance[1]);
}
