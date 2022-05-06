#pragma once

#include <map>
#include <wx/event.h>
#include "dlldefine.h"
#include <list>
#include <queue>
#include <memory>
#ifdef __WXGTK__
#include <random>
#endif

/** @file paex_pink.c
	@ingroup examples_src
	@brief Generate Pink Noise using Gardner method.

	Optimization suggested by James McCartney uses a tree
	to select which random value to replace.
<pre>
	x x x x x x x x x x x x x x x x
	x   x   x   x   x   x   x   x
	x       x       x       x
	 x               x
	   x
</pre>
	Tree is generated by counting trailing zeros in an increasing index.
	When the index is zero, no random number is selected.

	@author Phil Burk  http://www.softsynth.com
*/
/*
 * $Id: paex_pink.c 1752 2011-09-08 03:21:55Z philburk $
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however,
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also
 * requested that these non-binding requests be included along with the
 * license above.
 */

class timedbuffer;
class GeneratorPluginBuilder;

struct genfreq
{
    genfreq() : dFrequency(0.0), dAmplitude(0.0), nSamplesToPlay(0), nSampleCount(0), nType(0), nPhase(0){}
    genfreq(float dF, float dA, int nC, int nT) : dFrequency(dF), dAmplitude(dA), nSamplesToPlay(nC), nSampleCount(0), nType(nT), nPhase(0){}
    float dFrequency;
    float dAmplitude;
    int nSamplesToPlay;
    int nSampleCount;
    int nType;
    int nPhase;
};

class SoundFile;
class IIR;
class KFilter;

class PAMBASE_IMPEXPORT Sequence
{
    public:
        Sequence(int nChannels, double dSampleRate);
        int GetChannels() const
        {   return m_nChannels; }
        void AppendGenFreq(float dFrequency, float ddBFS, int nCycles, int nType);

        std::list<genfreq>::iterator GetSequenceBegin();
        std::list<genfreq>::iterator GetSequenceEnd();
        const std::list<genfreq>::iterator& GetSequencePosition();

        void SetSampleRate(double dSampleRate)  {   m_dSampleRate = dSampleRate; }
        void AdvanceSequence();
        enum {LEFT=1, RIGHT=2, MONO=3};

    private:
        int m_nChannels;
        double m_dSampleRate;
        std::list<genfreq> m_lstSequence;
        std::list<genfreq>::iterator m_itPosition;




};


class PAMBASE_IMPEXPORT Generator
{
    public:
        Generator();
        ~Generator();


        void SetSampleRate(unsigned int nSampleRate);
        void SetFrequency(float dFrequency, float ddBFS, int nType);
        bool SetFile();
        bool SetPlugin(const wxString& sPlugin);

        bool LoadSequence(const wxString& sFile);

        void SetNoise(int nColour, float ddBFS);
        void SetNoiseAmplitude(float ddBFS);

        timedbuffer* Generate(unsigned int nSize);

        void Stop();

        int GetNumberOfChannels();

        void ClearSequences();
        void ClearFrequences();
        void CloseFile();



        unsigned int GetSampleRate();
        unsigned int GetChannels();

        enum {SINE=0, SQUARE, SAW, TRIANGLE};

        enum {WHITE=0, PINK, GREY, GREY_A, GREY_K, BROWN};


    protected:

        void AddSequence(const wxString& sName, std::shared_ptr<Sequence> pSeq);
        void DeleteSequence(const wxString& sName);

        void InitPinkNoise();

        void ReadSoundFile(timedbuffer* pData);
        void GenerateSequences(timedbuffer* pData);
        void GenerateSequence(std::shared_ptr<Sequence> pSeq, float* pBuffer, unsigned int nSize);
        void GenerateFrequency(float* pBuffer, unsigned int nSize);

        float GenerateSin(const genfreq& gfreq, float dPhase);
        float GenerateSquare(const genfreq& gfreq, float dPhase);
        float GenerateSaw(const genfreq& gfreq, float dPhase);
        float GenerateTriangle(const genfreq& gfreq, float dPhase);

        double randn(double mu=0.0, double sigma=1.0);


        void ClosePink();

        std::map<wxString, std::shared_ptr<Sequence>> m_mSequences;

        std::queue<genfreq> m_queueFreq;
        float m_dSampleRate;
        size_t m_nPhase;

        std::unique_ptr<SoundFile> m_pSoundfile;

        static const unsigned int PINK_RANDOM_BITS       = 24;
        static const unsigned int PINK_RANDOM_SHIFT      = 8;


        struct PinkNoise
        {
            long      pink_Rows[30];
            long      pink_RunningSum;   /* Used to optimize summing of generators. */
            int       pink_Index;        /* Incremented each sample. */
            int       pink_IndexMask;    /* Index wrapped by ANDing with this mask. */
            float     pink_Scalar;       /* Used to scale within range of -1.0 to +1.0 */
        } ;

        PinkNoise* m_pPink[2];
        static unsigned long GenerateRandomNumber( void );
        void InitializePinkNoise(int numRows );
        void GeneratePinkNoise(float* pBuffer, unsigned int nSize);
        void GenerateWhiteNoise(float* pBuffer, unsigned int nSize);
        void GenerateGreyNoise(float* pBuffer, unsigned int nSize);
        void GenerateGreyANoise(float* pBuffer, unsigned int nSize);
        void GenerateGreyKNoise(float* pBuffer, unsigned int nSize);
        void GenerateBrownNoise(float* pBuffer, unsigned int nSize);
        void GeneratePlugin(timedbuffer* pBuffer);
        float m_dNoiseAmplitude;
        int m_nGenerator;


        GeneratorPluginBuilder* m_pPlugin;

        KFilter* m_pKFilter[2];
        IIR* m_pAFilter[2];
        IIR* m_pGreyFilter[2];

        enum {FILE, FREQUENCY, SEQUENCE, NOISE_WHITE, NOISE_PINK, NOISE_GREY, NOISE_A, NOISE_K, NOISE_BROWN,PLUGIN};


        static const double AFILTER_B[7];
        static const double AFILTER_A[6];

        static const double ANFILTER_B[3];
        static const double ANFILTER_A[2];

};


