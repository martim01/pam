/*
 *   $Id: ppm.cpp,v 1.2 2005/10/21 15:22:04 andrewmason Exp $
 *
 *   libbaptools - tools for broadcast audio production
 *
 *   Copyright (C) 2005  British Broadcasting Corporation
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.

 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * $Log: ppm.cpp,v $
 * Revision 1.2  2005/10/21 15:22:04  andrewmason
 * Fix error in taking absolute value of samples. Would have mis-read on assymetric waveforms.
 *
 * Revision 1.1.1.1  2005/08/01 16:11:57  andrewmason
 * Initial import.
 *
 */

#include "ppm.h"
#include <wx/log.h>


//! Calculate latest intermediate value
/*!
 * Process incoming audio samples to produce intermediate PPM
 * value.  Do only processing required to keep up with input.
 * Leave logarithms, angle calcs until required.
 *\param nChannels Number of audio channels in input buffer
 *\param nSamples Number of samples for each channel in input buffer
 *\param samples Pointer to input buffer of samples
 */

void ppm::calcIntermediate(int nChannels, int nSamples, const float* samples)
{

    int i;	// sample counter
    int ch; // channel counter
    for(ch=0; ch<nChannels; ch++)
    {
        double combinedSample; // an A,B, M3, or M6 sample

        float maxPpm=0.0;
        float tmpPpm = intermediateValue[ch];

        for(i=0; i<nSamples; i++)
        {
            if((nChannels == 2)&& (mode != AB))
            {
                switch (mode)
                {
                case M3:
                    combinedSample = (ch == 0 ) ?
                                     (samples[i*2+0]+samples[i*2+1])*0.7071 :
                                     (samples[i*2+0]-samples[i*2+1])*0.7071;
                    break;

                case M6:
                    combinedSample = (ch == 0 ) ?
                                     (samples[i*2+0]+samples[i*2+1])*0.5 :
                                     (samples[i*2+0]-samples[i*2+1])*0.5;
                    break;

                default:
                    fprintf(stderr,"Stereo mode not recognised\n");
                    return;
                    break;
                }
            }
            else
            {
                combinedSample = (double)samples[i*nChannels + ch];
            }


            if(fabs((float)combinedSample) > tmpPpm)
            {
                tmpPpm = tmpPpm +
                         lockonfract * (fabs(combinedSample) - tmpPpm);
            }
            tmpPpm *= drop;
            intermediateValue[ch] = tmpPpm;
            maxPpm = (tmpPpm > maxPpm) ? tmpPpm : maxPpm;

            maxValue[ch] =  (maxValue[ch] > intermediateValue[ch]) ?
                            maxValue[ch] :
                            intermediateValue[ch];
            minValue[ch] =  (minValue[ch] < intermediateValue[ch]) ?
                            minValue[ch] :
                            intermediateValue[ch];

        }
    }
}

/*!
 * Convert latest intermediate value into final PPM reading
 *\param channelNumber Which channel of a multichannel PPM to return
 *\return PPM value of requested channel
 */
float ppm::getValue(int channelNumber)
{
    if((channelNumber < 0 ) || (channelNumber >= nChannels))
    {
        fprintf(stderr,"Channel number out of range\n");
        return (0.0f);
    }

    return (convert(intermediateValue[channelNumber]));
}

//! get maxmi
float ppm::getMaxValue(int channelNumber)
{
    if((channelNumber < 0 ) || (channelNumber >= nChannels))
    {
        fprintf(stderr,"Channel number out of range\n");
        return (0.0f);
    }
    float retVal = (convert(maxValue[channelNumber]));

    maxValue[channelNumber] = intermediateValue[channelNumber];
    return retVal;
}

float ppm::getMinValue(int channelNumber)
{
    if((channelNumber < 0 ) || (channelNumber >= nChannels))
    {
        fprintf(stderr,"Channel number out of range\n");
        return (0.0f);
    }
    float retVal = (convert(minValue[channelNumber]));
    minValue[channelNumber] = intermediateValue[channelNumber];

    return retVal;
}
float ppm::convert(float intermediate)
{
    if (intermediate < MIN_INTERMEDIATE )
    {
        return 0.0;
    }
    else
    {
        float ppmVal = (CLIP_PPM - (DB_CONST/DB_PER_PPM * log10(SAMPLE_LIMIT/intermediate)));
        if (ppmVal <1.0)
        {
            /* scale to 24dB per PPM instead of 4dB per PPM */
            ppmVal = 1.0 - 1.0/(24.0/DB_PER_PPM) + (1.0/(24.0/DB_PER_PPM)) * ppmVal;
        }

        return (ppmVal < 0.0) ? 0.0 : ppmVal;
    }
}

void ppm::show()
{


    for(int i=0; i<nChannels; i++)
    {
        int k;
        for(k=0; k< 4*getValue(i); k++)
        {
            fprintf (stderr, "O");
        }
        for (; k<39; k++)
        {
            fprintf(stderr, " ");
        }
    }
    for(int l=0; l<80; l++) fprintf(stderr, "\b");

}

bool ppm::setMode(Mode m)
{
    switch (m)
    {
    case AB:
    case M3:
    case M6:
        mode = m;
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool ppm::setParam(meter::params i, double f)
{
    switch (i)
    {
    case RISETIME:
        if (f>0.0)
        {
            lockonfract = f;
            wxLogDebug(wxT("Rise %f"), f);
        }
        else
            return false;
        break;

    case FALLTIME:
        if (f >0.0)
        {
            drop = f;
            wxLogDebug(wxT("Drop %f"), f);
        }
        else
            return false;
        break;
    default:
        return false;
    }

    return true;
}

bool ppm::setParam(meter::params i, int j)
{
    switch (i)
    {
    case SAMPLING_RATE :
        samplingRate = j;
        lockonfract = (1.0 - pow((double)LOCK_ON_FACTOR, (double)(1.0 / (samplingRate*LOCK_ON_TIME))));
        drop = pow ((double)DROP_FACTOR, (double)(1.0 / (samplingRate * DROP_TIME)));
        break;

    case STEREO_MODE :
        setMode(j);
        break;

    default:
        fprintf(stderr, "Unrecognised parameter\n");
        return false;
        break;
    }
    return true;
}

bool ppm::setSpeed(int speed)
{

    double defRise = (1.0 - pow((double)LOCK_ON_FACTOR, (double)(1.0 / (samplingRate*LOCK_ON_TIME))));
    double defFall = pow((double)DROP_FACTOR,(double)(1.0/(samplingRate * DROP_TIME)));
    switch (speed)
    {

    case SLOW:
        setParam(RISETIME, defRise/2.0);
        setParam(FALLTIME, 1.0-(1.0-defFall)/2.0);
        break;

    case FAST:
        setParam(RISETIME, defRise*2.0);
        setParam(FALLTIME, 1.0-(1.0-defFall)*2.0);
        break;

    case NORMAL:

        setParam(RISETIME, defRise);
        setParam(FALLTIME, defFall);
        break;
    default:
        return false;
    }
    return true;
}

