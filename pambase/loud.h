/*
 *   $Id: loud.h,v 1.6 2008/01/31 16:33:39 andrewmason Exp $
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
 * $Log: loud.h,v $
 * Revision 1.6  2008/01/31 16:33:39  andrewmason
 * longer integration time. multichannel allowed. integration time in seconds
 *
 * Revision 1.5  2007/10/19 10:59:42  andrewmason
 * updated surround channel weighting
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


#ifdef LOUD_H
#else
#define LOUD_H
#include <stdio.h>
#include <math.h>

#include "meter.h"
#include "dlldefine.h"

#define SAMPLE_LIMIT 32768
#define SAMPLE_LIMIT_SQ (SAMPLE_LIMIT*SAMPLE_LIMIT)
#define SF 48000.0
#define FRAME_SIZE 6
#define SKIPFACTOR  (12000/FRAME_SIZE) /* or 500,  was 125 */
#define MAX_INTEGRATION_TIME (60*(int)SF) /* 60 second max  */
#define X_REF_SQ 1073741824.000000   /* reference loudness level  = 0dBFS */

#define MIN_INTERMEDIATE (0.001)	/* arbitrary minimum  sensible limit for
											 * intermediate value
									 		 * to prevent logarithm going off the scale */

#define PPM_4 4					/* a constant offset for 0( level)dB */
#define DB_PER_LOUD 4			/* dB per unit on loudness scale */
#define PLUS_FOUR_DB 2.51188634151
#define SURROUND_CHANNEL_WEIGHTING 1.41

#define TAPS_IN_FILTER 2
#define MAX_CHANS 8

//! A class to implement a loudness meter movement.
/*!
	This class implements a model of a loudness meter. Its characteristics
	are based on the meter descrived in Gilbert Soulodre's paper,
	"Evaluation of Objective Loudness Meters", AES 116th Convention, Berlin,
	May 2004, pre-print 6161.

	The meter was the one that provided the closest match to subjective
	opinion in the tests.
	*/


class PAMBASE_IMPEXPORT loud : public meter
{

private:
    double pre_filt_buf[MAX_CHANS][TAPS_IN_FILTER]; // buffers for delay taps
    double filt_buf[MAX_CHANS][TAPS_IN_FILTER]; // buffers for delay taps
    double integration_buffer[MAX_CHANS][MAX_INTEGRATION_TIME];

    /* to keep track of single value for multichannel loudness */
    float sumValue;
    float sumMaxValue;
    float sumMinValue;

    /* pre-filter coefficients */
#define pa1  (-1.69065929318241)
#define pa2 (0.73248077421585)
#define pb0 (1.53512485958679)
#define pb1 (-2.691696189403638)
#define pb2 (1.19839281085285)

    /* filter coefficients */
#define a1  (-1.99004745483398)
#define a2 (0.99007225036621)
#define b0 (1.0)
#define b1 (-2.0)
#define b2 (1.0)

    int oldest;
    int integration_time;


public:
    loud()
    {
        ;
    };
    //! The constructor
    /*!
    	\param nChans The number of channels this meter has
    */
    loud(int nChans)
    {
        int i, j;

        nChannels = nChans;
        integration_time = MAX_INTEGRATION_TIME/2;

        mode = AB;
        intermediateValue = new float[nChans];
        maxValue = new float[nChans];
        minValue = new float[nChans];
        oldest = 0;

        for (i=0; i<nChans; i++)
        {
            intermediateValue[i] = 0.0f;
            maxValue[i] = 0.0f;
            minValue[i] = 0.0f;
        }

        sumValue = 0.0f;
        sumMaxValue = 0.0f;
        sumMinValue = 0.0f;

        for (i=0; i< TAPS_IN_FILTER; i++)
        {
            for (j=0; j<nChans; j++)
            {
                filt_buf[j][i] = 0.0;
                pre_filt_buf[j][i] = 0.0;
            }
        }

        for (i=0; i< MAX_INTEGRATION_TIME; i++)
        {
            for (j=0; j<nChans; j++)
            {
                integration_buffer[j][i] = 0.0;
            }
        }
    }

    //! Calculate an intermediate value, not on the real scale.
    void calcIntermediate(int nChannels, int nSamples, const float* samples);

    //! Convert an intermediate value into a value on the real scale.
    float convert(float);

    //! Get the current loudness value of the multichannel meter.
    float getValue();

    //! Get the current loudness value of an individual channel.
    float getValue(int);

    //! Get the biggest loudness value of a channel since the last call to getMaxValue.
    float getMaxValue(int);

    //! Get the max combined loudness value since the last call to getMaxValue.
    float getMaxValue();

    //! Get the smallest loudness value of a channel since the last call to getMinValue.
    float getMinValue(int);

    //! Get the smallest combined loudness value since the last call to getMinValue.
    float getMinValue();

    //! Combine the loudness values of all channels to produce one number
    float sumMultiChannels(float *);

    void show();			//!< show a textual representation of the meter reading.
    bool setMode(Mode);	//!< set the stereo mode of the meter.

    //! Set a parameter taking a double value.
    bool setParam(meter::params p, double value);

    //! Set a parameter taking an integer value.
    bool setParam(meter::params p, int value);

    //! Set the speed of the meter.
    bool setSpeed(int s);

    //! Set the speed of the meter.
    bool setIntegrationTime(int seconds);

};

#endif
