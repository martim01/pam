/*
 *   $Id: ppm.h,v 1.1.1.1 2005/08/01 16:11:57 andrewmason Exp $
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
 * $Log: ppm.h,v $
 * Revision 1.1.1.1  2005/08/01 16:11:57  andrewmason
 * Initial import.
 *
 */


#ifdef PPM_H

#else
#define PPM_H
#include <stdio.h>
#include <math.h>
#include "meter.h"

#define LOCK_ON_FACTOR ((float)(1.0 - 0.6))
#define LOCK_ON_TIME ((float)0.0025)
#define DROP_FACTOR ((float)(pow(10.0f, (-24.0f/20.0f))))
#define DROP_TIME (2.8f)
#define CLIP_PPM (8.5f)
#define SAMPLE_LIMIT 1
#define DB_PER_PPM (4.0f)
#define DB_CONST (20.0f)
#define SF 48000


#define MIN_INTERMEDIATE (0.001)	/*arbitrary minimum  sensible limit for preventing log going out of range */

//! A class to implement a meter movement with BBC PPM characteristics.
/*!
	This class implements a model of a BBC-type PPM. Its characteristics
	have been modelled as faithfully as possible. Methods are provided for
	audio input to the meter and for getting the meter reading.

	Characteristics that differ from the standard meter, in terms of faster
	or slower response, can also be set.
	*/

class ppm : public meter
{

private:
    float lockonfract; 	//!< rise time factor
    float drop; 			//!< fall time factor
    float samplingRate; 	//!< sampling rate of audio samples

public:
    ppm()
    {
        ;
    };
    //! The constructor
    /*!
    	\param nChans The number of channels this meter has
    */
    ppm(int nChans)
    {
        nChannels = nChans;
        mode = AB;
        intermediateValue = new float[nChans];
        maxValue = new float[nChans];
        minValue = new float[nChans];
        for (int i=0; i<nChans; i++)
        {
            intermediateValue[i] = 0.0f;
            maxValue[i] = 0.0f;
            minValue[i] = 0.0f;
        }
        samplingRate = SF;
        lockonfract = (1.0 - pow((double)LOCK_ON_FACTOR, (double)(1.0 / (samplingRate*LOCK_ON_TIME))));
        drop = pow ((double)DROP_FACTOR, (double)(1.0 / (samplingRate * DROP_TIME)));
    }

    //! Calculate an intermediate value, not on a real PPM scale
    void calcIntermediate(int nChannels, int nSamples, const float* samples);
    //! Convert an intermediate value into a reading on a real PPM scale
    float convert(float);

    float getValue(int); //!< get the current PPM value

    //! get the biggest PPM value since the last call to getMaxValue()
    float getMaxValue(int);

    //! get the smallest PPM value since the last call to getMinValue()
    float getMinValue(int);

    //! print out on the screen a text representation of a bar-graph
    void show();

    //! set the stereo mode: M3, M6, AB
    bool setMode(Mode);

    //! set a parameter that takes a double type
    bool setParam(meter::params p, double value);

    //! set a parameter that takes an int type
    bool setParam(meter::params p, int value);
    bool setSpeed(int speed);

};

#endif
