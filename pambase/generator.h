#pragma once

#include <map>
#include <wx/event.h>
#include "dlldefine.h"
#include <list>
#include <queue>

struct genfreq
{
    genfreq() : dFrequency(0.0), dAmplitude(0.0), nCycles(0), dCycleCount(0.0), nType(0), dPhase(0.0){}
    genfreq(double dF, double dA, int nC, int nT) : dFrequency(dF), dAmplitude(dA), nCycles(nC), dCycleCount(0), nType(nT){}
    double dFrequency;
    double dAmplitude;
    int nCycles;
    double dCycleCount;
    int nType;
    double dPhase;
};

class PAMBASE_IMPEXPORT Sequence
{
    public:
        Sequence(int nChannels);
        int GetChannels() const
        {   return m_nChannels; }
        void AppendGenFreq(double dFrequency, double ddBFS, int nCycles, int nType);

        std::list<genfreq>::iterator GetSequenceBegin();
        std::list<genfreq>::iterator GetSequenceEnd();
        const std::list<genfreq>::iterator& GetSequencePosition();

        void AdvanceSequence();
        enum {LEFT=1, RIGHT=2, MONO=3};

    private:
        int m_nChannels;
        std::list<genfreq> m_lstSequence;
        std::list<genfreq>::iterator m_itPosition;

};


class PAMBASE_IMPEXPORT Generator
{
    public:
        Generator(wxEvtHandler* pHandler);
        ~Generator();
        void SetSampleRate(unsigned int nSampleRate);

        void SetFrequency(double dFrequency, double ddBFS, int nType);


        void Generate(unsigned int nSize);

        void AddSequence(const wxString& sName, Sequence* pSeq);
        void DeleteSequence(const wxString& sName);
        void ClearSequences();

        enum {SINE=0, SQUARE, SAW, TRIANGLE};




    protected:


        void GenerateSequence(Sequence* pSeq, float* pBuffer, unsigned int nSize);
        void GenerateFrequency(float* pBuffer, unsigned int nSize);

        float GenerateSin(const genfreq& gfreq, double dPhase);
        float GenerateSquare(const genfreq& gfreq, double dPhase);
        float GenerateSaw(const genfreq& gfreq, double dPhase);
        float GenerateTriangle(const genfreq& gfreq, double dPhase);

        wxEvtHandler* m_pHandler;

        std::map<wxString, Sequence*> m_mSequences;

        std::queue<genfreq> m_queueFreq;
        double m_dSampleRate;
        size_t m_nPhase;


};
