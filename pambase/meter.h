/*
 *   $Id: meter.h,v 1.2 2005/10/27 15:06:33 andrewmason Exp $
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
 * $Log: meter.h,v $
 * Revision 1.2  2005/10/27 15:06:33  andrewmason
 * Added HRTF and multichannel features to loudness meter.  Not yet fully verified.
 *
 * Revision 1.1.1.1  2005/08/01 16:11:57  andrewmason
 * Initial import.
 *
 */


#pragma once

typedef int Mode;


//! Base class for a number of types of meter.
/*! The class is used by other types of meter. The methods
 here are pure virtual.
 */
class meter
{


private:

protected:
    float *maxValue; //!< keeps track of maximum value since last reading
    float *minValue;	//!< keeps track of minimum value since last reading

public:

    /*! stereo modes: AB, (A+B)-3dB, (A+B)-6dB */
    enum modes { AB, M3, M6, MULTICHANNEL_SUM };
    enum params { SAMPLING_RATE, STEREO_MODE, RISETIME, FALLTIME };
    enum speeds { SLOW, NORMAL, FAST };

    float *intermediateValue; //!< keeps track of the lastest value, before logs and bounds checking.
    Mode mode;	//! the stereo mode
    int nChannels;	//! how many channels

    virtual ~meter(){}

    meter()
    {
        ;
    };

    //! Method to take samples and update the meter.
    /*!
    	This can produce a computationally less intensive representation,
    	for example without taking logarithms.
    */
    virtual void calcIntermediate(int nSamples, int nChannels, const float* samples) = 0;

    //! Return the latest intermediate value
    virtual float getValue(int)= 0;

    //! Return the maximum value since last reading of maximum  value
    virtual float getMaxValue(int)= 0;

    //! Return the minimum value since last reading of minimum value
    virtual float getMinValue(int)= 0;

    //! Possibly rudimentary way of representing the meter value
    virtual void show()= 0;

    //! Set the stereo mode
    virtual bool setMode(Mode)= 0;

    //! Set a parameter of double type
    virtual bool setParam(meter::params p, double value)= 0;

    //! Set a parameter of int type
    virtual bool setParam(meter::params p, int value)= 0;

    //! Set the meter response, typically fast, normal, or slow.
    virtual bool setSpeed(int speed)=0;



};


