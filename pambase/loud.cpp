/*
 *   $Id: loud.cpp,v 1.6 2008/01/31 16:31:52 andrewmason Exp $
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
 * $Log: loud.cpp,v $
 * Revision 1.6  2008/01/31 16:31:52  andrewmason
 * integration time in seconds. compiler warning avoidance
 *
 * Revision 1.5  2007/10/19 11:00:13  andrewmason
 * Return -999, not 0, for very small valuse. Updated surround channel weighting
 *
 * Revision 1.4  2007/09/28 15:09:59  andrewmason
 * Longer integration time
 *
 * Revision 1.3  2005/10/27 15:06:33  andrewmason
 * Added HRTF and multichannel features to loudness meter.  Not yet fully verified.
 *
 * Revision 1.2  2005/10/24 16:23:39  andrewmason
 * Added Gilbert Soloudre's HRTF for multichannel applications - as in the 119th AES convention paper.
 *
 * Revision 1.1.1.1  2005/08/01 16:11:57  andrewmason
 * Initial import.
 *
 */



#include "loud.h"



//! Calculate latest intermediate loudness value
/*!
 * Process incoming audio samples to produce intermediate loudness
 * value.  Do only processing required to keep up with input.
 * Leave logarithms, angle calcs until required.
 *\param nChannels Number of audio channels in input buffer
 *\param nSamples Number of samples for each channel in input buffer
 *\param sample Pointer to input buffer of samples
 */

void loud::calcIntermediate(int nChannels, int nSamples, const float* samples)
{


    int i;						// sample counter
    int ch; 						// channel counter
    double sample;  			// a sample from a channel

    int oldest_tmp;
    /* changed order of loop so that we can sum the channels as
     * we go along */
    for (oldest_tmp = this->oldest, i=0; i< nSamples; i++)
    {
        for(ch=0; ch<nChannels; ch++)
        {
            double y;
            double w_n;
            double h_n;
            double pw_n;

            /* calculate the loudness of each channel independently */
            /* In making it multichannel like this, we have lost the possibilty
             * to detect "out of phase" channels by M/S metering */

            sample = (double)samples[i*nChannels + ch];
            sample *= (double)SAMPLE_LIMIT;

            /* Apply HRTF weighting curve */
            pw_n = sample -
                   (pre_filt_buf[ch][0] * pa1 + pre_filt_buf[ch][1] * pa2);
            h_n = pw_n * pb0 + pre_filt_buf[ch][0] * pb1 + pre_filt_buf[ch][1] * pb2;
            // shuffle the samples along the delay ..
            pre_filt_buf[ch][1] = pre_filt_buf[ch][0];
            pre_filt_buf[ch][0] = pw_n;

            /* end of HRTF weighting curve application */

            /* Apply RLB weighting curve */

            w_n = 0.1 + h_n -
                  (filt_buf[ch][0] * a1 + filt_buf[ch][1] * a2);

            y =  w_n * b0 + filt_buf[ch][0] * b1 + filt_buf[ch][1] * b2;


            y = y * y; /* and square it */

            // shuffle the samples along the delay ..
            filt_buf[ch][1] = filt_buf[ch][0];
            filt_buf[ch][0] = w_n;


            /* end of RLB weighting curve application */

            intermediateValue[ch] = intermediateValue[ch] + (y - integration_buffer[ch][oldest_tmp]);
            integration_buffer[ch][oldest_tmp] = y;

            maxValue[ch] =  (maxValue[ch] > intermediateValue[ch]) ?
                            maxValue[ch] :
                            intermediateValue[ch];
            minValue[ch] =  (minValue[ch] < intermediateValue[ch]) ?
                            minValue[ch] :
                            intermediateValue[ch];
        }
        sumValue = sumMultiChannels(intermediateValue);
        sumMaxValue = (sumMaxValue > sumValue) ? sumMaxValue : sumValue;
        sumMinValue = (sumMinValue < sumValue) ? sumMinValue : sumValue;

        oldest_tmp = (oldest_tmp + 1) % integration_time;
    }
    this->oldest = (this->oldest + nSamples) % integration_time;

}

float loud::getValue()
{
    return(convert(sumValue));
}

float loud::getValue(int channelNumber)
{
    if((channelNumber < 0 ) || (channelNumber >= nChannels))
    {
        fprintf(stderr,"Channel number out of range\n");
        return (0.0f);
    }

    if (mode == MULTICHANNEL_SUM)
        return getValue();

    switch (mode )
    {
    case M6:
        switch (channelNumber)
        {
        case 0:
            return (convert(0.5*(intermediateValue[0] +
                                 intermediateValue[1])));
        case 1:
            return (convert(0.5*(intermediateValue[0] -
                                 intermediateValue[1])));
        }
        break;

    case M3:
        switch (channelNumber)
        {
        case 0:
            return (convert(0.7071*(intermediateValue[0] +
                                    intermediateValue[1])));
        case 1:
            return (convert(0.7071*(intermediateValue[0] -
                                    intermediateValue[1])));
        }
        break;

    case AB:
    default:
        return (convert(intermediateValue[channelNumber]));
    }

    /* the code cannot get to this point, but the compiler warns of control reaching end of non void function */
    return (0.0f);

}

float loud::getMaxValue(int channelNumber)
{
    if((channelNumber < 0 ) || (channelNumber >= nChannels))
    {
        fprintf(stderr,"Channel number out of range\n");
        return (0.0f);
    }

    if (mode == MULTICHANNEL_SUM)
        return getMaxValue();

    float retVal = (convert(maxValue[channelNumber]));
    maxValue[channelNumber] = intermediateValue[channelNumber];


    return retVal;
}

float loud::getMaxValue()
{

    float retVal = (convert(sumMaxValue));
    sumMaxValue = sumValue;

    return retVal;
}

float loud::getMinValue(int channelNumber)
{
    if((channelNumber < 0 ) || (channelNumber >= nChannels))
    {
        fprintf(stderr,"Channel number out of range\n");
        return (0.0f);
    }

    if (mode == MULTICHANNEL_SUM)
        return getMinValue();

    float retVal =  (convert(minValue[channelNumber]));
    minValue[channelNumber] = intermediateValue[channelNumber];

    return retVal;
}

float loud::getMinValue()
{
    float retVal = convert(sumMinValue);
    sumMinValue = sumValue;

    return retVal;
}

/**
 * Convert intermediate value into final loudness reading
 *@param channelNumber Which channel of a multichannel loudness meter to return
 *@return loudness value of requested channel
 */
float loud::convert(float intermediate)
{

    // avoid -infinity in the logarithm
    if (intermediate < MIN_INTERMEDIATE )
    {
        return -999.0;
    }
    /*float loudVal = PPM_4 + 10*log10((intermediate)/(X_REF_SQ*(integration_time)))/DB_PER_LOUD; */
    float loudVal = -0.691 + 10*log10((intermediate)/(X_REF_SQ*(integration_time)));


    /*return (loudVal < 0.0) ? 0.0 : loudVal;*/
    return (loudVal );
}

/**
 * Combine the individual channel loudnesses to produce a single value
 * @param values pointer to array containing loudness of all channels
 * @return loudness value of all channels combined
 */
float loud::sumMultiChannels(float *values)
{

    int ch;							// channel iterator
    float sum_intermediate;		// sum of channels' loudness

    /* we need to weight, +1.5dB, channels at the back */
    /* lets assume some presentation arrangements */
    /* 1/0 (mono), 2/0 (stereo), 2/2 (quadrophonic), 3/2 (5.1) */
    /* not sure about 2/1 or 3/0  - I'll go for 3/0 */

    switch (nChannels)
    {
    default:
    case 1:
    case 2:
    case 3:
        for (ch = 0, sum_intermediate=0.0; ch <nChannels; ch++)
        {
            sum_intermediate += values[ch];
        }
        break;

    case 4:
        sum_intermediate = values[0] + values[1]
                           + SURROUND_CHANNEL_WEIGHTING *(values[2] + values[3]) ;
        break;

    case 5:
        sum_intermediate = values[0] + values[1] + values[2]
                           + SURROUND_CHANNEL_WEIGHTING *(values[3] + values[4]) ;
        break;

    }
    return (sum_intermediate);
}
void loud::show()
{


    for(int i=0; i<nChannels; i++)
    {
        int k;
        fprintf (stderr, "|");
        for(k=0; k< 4*getValue(i); k++)
        {
            fprintf (stderr, "O");
        }
        for (; k<38; k++)
        {
            fprintf(stderr, " ");
        }
    }
    for(int l=0; l<80; l++) fprintf(stderr, "\b");

    fflush(stderr);
}

bool loud::setMode(Mode m)
{
    switch (m)
    {
    case AB:
    case M3:
    case M6:
    case MULTICHANNEL_SUM:
        mode = m;
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool loud::setParam(meter::params p, double f)
{
    double theValue;
    theValue = f;

    switch (p)
    {
    default:
        fprintf(stderr, "Unrecognised parameter\n");
        return false;
    }
    return true;
}

bool loud::setParam(meter::params p, int j)
{
    switch (p)
    {
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

bool loud::setSpeed(int s)
{

    int i;  /* general purpose loop counter */

    for (i=0; i< MAX_INTEGRATION_TIME; i++)
    {
        for (int j=0; j<nChannels; j++)
        {
            integration_buffer[j][i] = 0.0;
        }
    }

    for (i=0; i<nChannels; i++)
    {
        intermediateValue[i] = 0.0f;
        maxValue[i] = 0.0f;
        minValue[i] = 0.0f;
    }

    switch (s)
    {
    case SLOW:
        integration_time = MAX_INTEGRATION_TIME;
        break;

    case FAST:
        integration_time = MAX_INTEGRATION_TIME/4;
        break;

    case NORMAL:
        integration_time = MAX_INTEGRATION_TIME/2;
        break;

    default:
        return false;
        break;
    }
    return true;
}

bool loud::setIntegrationTime(int seconds)
{

    int i;  /* general purpose loop counter */

    for (i=0; i< MAX_INTEGRATION_TIME; i++)
    {
        for (int j=0; j<nChannels; j++)
        {
            integration_buffer[j][i] = 0.0;
        }
    }

    for (i=0; i<nChannels; i++)
    {
        intermediateValue[i] = 0.0f;
        maxValue[i] = 0.0f;
        minValue[i] = 0.0f;
    }

    if ((seconds * SF) > MAX_INTEGRATION_TIME)
    {
        return false;
    }

    integration_time = (int)(seconds * SF);
    return true;
}
