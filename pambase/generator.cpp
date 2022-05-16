#include "generator.h"
#include "timedbuffer.h"
#include "audio.h"
#include <wx/log.h>
#include <iterator>
#include "settings.h"
#include "soundfile.h"
#include <wx/xml/xml.h>
#include "log.h"
#include "generatorpluginfactory.h"
#include "generatorpluginbuilder.h"

using namespace std;


Sequence::Sequence(int nChannel, double dSampleRate) : m_nChannel(nChannel), m_dSampleRate(dSampleRate)
{
    m_itPosition = m_lstSequence.end();
}
void Sequence::AppendGenFreq(float dFrequency, float ddBFS, int nCycles, int nType)
{

    float dAmplitude(0.0);
    if(ddBFS > -80.0)
    {
        dAmplitude = pow(10.0, ddBFS/20.0);
    }

    double dSamples = static_cast<double>(nCycles)*m_dSampleRate/dFrequency;


    m_lstSequence.push_back(genfreq(dFrequency, dAmplitude, static_cast<int>(dSamples),nType));
    if(m_itPosition == m_lstSequence.end())
    {
        m_itPosition = m_lstSequence.begin();
    }
}

list<genfreq>::iterator Sequence::GetSequenceBegin()
{
    return m_lstSequence.begin();
}

list<genfreq>::iterator Sequence::GetSequenceEnd()
{
    return m_lstSequence.end();
}

const std::list<genfreq>::iterator& Sequence::GetSequencePosition()
{
    return m_itPosition;
}

void Sequence::AdvanceSequence()
{
    (*m_itPosition).nSampleCount = 0;
    //(*m_itPosition).nPhase = 0;
    ++m_itPosition;
    if(m_itPosition == m_lstSequence.end())
    {
        m_itPosition = m_lstSequence.begin();
    }
}


Generator::Generator() :
    m_dSampleRate(48000),
    m_nPhase(0),
    m_pSoundfile(nullptr),
	m_nGenerator(FREQUENCY),
    m_pPlugin(0)
{
}

Generator::~Generator()
{
    ClearSequences();
    ClearFrequences();
    CloseFile();

}

void Generator::SetSampleRate(unsigned int nSampleRate)
{
    m_dSampleRate = nSampleRate;
    m_nPhase = 0;
    for(auto pairSeq : m_mSequences)
    {
        pairSeq.second->SetSampleRate(m_dSampleRate);
    }
    if(m_pPlugin)
    {
        m_pPlugin->SetSampleRate(m_dSampleRate);
    }
}


timedbuffer* Generator::Generate(unsigned int nSizePerChannel)
{
    unsigned int nSize = nSizePerChannel*GetNumberOfChannels();

    timedbuffer* pData = new timedbuffer(nSize, GetNumberOfChannels());
    float dSize(m_mSequences.size());

    switch(m_nGenerator)
    {
        case FILE:
            ReadSoundFile(pData);
            break;
        case SEQUENCE:
            GenerateSequences(pData);
            break;
        case FREQUENCY:
            GenerateFrequency(pData->GetWritableBuffer(), nSize);
            break;
        case PLUGIN:
            GeneratePlugin(pData);
    }

    pData->SetDuration(pData->GetBufferSize()*4);

    return pData;
}

void Generator::GenerateSequences(timedbuffer* pData)
{
    for(int i = 0; i < pData->GetBufferSize(); i++)
    {
        pData->GetWritableBuffer()[i] = 0.0;
    }
    if(m_nSequenceChannels != 0)
    {
        for(auto pairSeq : m_mSequences)
        {
            GenerateSequence(pairSeq.second, pData->GetWritableBuffer(), pData->GetBufferSize());
        }
        m_nPhase += (pData->GetBufferSize()/m_nSequenceChannels);
        if(m_nPhase >= static_cast<unsigned long>(m_dSampleRate))
        {
            m_nPhase -= static_cast<unsigned long>(m_dSampleRate);
        }
    }
}

void Generator::GenerateSequence(std::shared_ptr<Sequence> pSeq, float* pBuffer, unsigned int nSize)
{

    unsigned int nPhase = m_nPhase;

    for(int i = pSeq->GetChannel(); i < nSize; i+=m_nSequenceChannels)
    {
        float dAmplitude(0.0);
        switch(pSeq->GetSequencePosition()->nType)
        {
            case SINE:
                dAmplitude = GenerateSin((*pSeq->GetSequencePosition()),nPhase);
                break;
            case SQUARE:
                dAmplitude = GenerateSquare((*pSeq->GetSequencePosition()),nPhase);
                break;
            case SAW:
                dAmplitude = GenerateSaw((*pSeq->GetSequencePosition()),nPhase);
                break;
            case TRIANGLE:
                dAmplitude = GenerateTriangle((*pSeq->GetSequencePosition()),nPhase);
                break;
        }
        pBuffer[i] += dAmplitude;

        nPhase++;
        if(nPhase == static_cast<unsigned long>(m_dSampleRate))
        {
            nPhase = 0;
        }

        if(pSeq->GetSequencePosition()->nSamplesToPlay > 0)
        {
            pSeq->GetSequencePosition()->nSampleCount++;
            if(pSeq->GetSequencePosition()->nSampleCount == pSeq->GetSequencePosition()->nSamplesToPlay)
            {
                pSeq->AdvanceSequence();
            }
        }

    }
}

void Generator::GenerateFrequency(float* pBuffer, unsigned int nSize)
{

    for(int i = 0; i < nSize; i+=2)
    {
        float dAmplitude(0.0);
        switch(m_queueFreq.front().nType)
        {
            case SINE:
                dAmplitude = GenerateSin(m_queueFreq.front(), m_nPhase);
                break;
            case SQUARE:
                dAmplitude = GenerateSquare(m_queueFreq.front(), m_nPhase);
                break;
            case SAW:
                dAmplitude = GenerateSaw(m_queueFreq.front(), m_nPhase);
                break;
            case TRIANGLE:
                dAmplitude = GenerateTriangle(m_queueFreq.front(), m_nPhase);
                break;
        }

        pBuffer[i] = dAmplitude;
        pBuffer[i+1] = dAmplitude;


        m_nPhase++;
        if(m_nPhase == static_cast<unsigned long>(m_dSampleRate))
        {
            m_nPhase = 0;
        }

        m_queueFreq.front().nPhase++;
        if(m_queueFreq.front().nPhase == static_cast<unsigned long>(m_dSampleRate))
        {
            m_queueFreq.front().nPhase = 0;
        }

        m_queueFreq.front().nSampleCount++;
        if(m_queueFreq.front().nSampleCount == m_queueFreq.front().nSamplesToPlay)
        {
           m_queueFreq.front().nSampleCount = 0;
            if(m_queueFreq.size() > 1)
            {
                m_queueFreq.pop();
            }
        }
    }
}

float Generator::GenerateSin(const genfreq& gfreq, float dPhase)
{
    float dIn(2.0 * M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate));
    float dSin = sin(dIn) * gfreq.dAmplitude;

    return dSin;
}

float Generator::GenerateSquare(const genfreq& gfreq, float dPhase)
{
    float dAmp(sin(2.0 * M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate)));
    if(dAmp >0 )
    {
        return gfreq.dAmplitude ;
    }
    else if(dAmp < 0)
    {
        return -gfreq.dAmplitude ;
    }
    return 0;
}

float Generator::GenerateSaw(const genfreq& gfreq, float dPhase)
{
    return - (2.0*gfreq.dAmplitude*atan(tan((M_PI/2.0) - (M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate)) )))/M_PI;
}

float Generator::GenerateTriangle(const genfreq& gfreq, float dPhase)
{
    return (2.0*gfreq.dAmplitude*asin(sin(2 * M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate))))/M_PI;
}


void Generator::AddSequence(const wxString& sName, std::shared_ptr<Sequence> pSeq)
{
    m_mSequences.insert(make_pair(sName, pSeq));
}

void Generator::ClearSequences()
{
    m_mSequences.clear();
}

void Generator::DeleteSequence(const wxString& sName)
{
    m_mSequences.erase(sName);
}


void Generator::SetFrequency(float dFrequency, float ddBFS, int nType)
{
    ClearSequences();
    CloseFile();

    m_nGenerator = FREQUENCY;

    float dAmplitude = 0.0;
    if(ddBFS > -80.0)
    {
        dAmplitude = pow(10.0, ddBFS/20.0);
    }

    m_queueFreq.push(genfreq(dFrequency, dAmplitude, 48000.0/dFrequency, nType));


}

void Generator::ClearFrequences()
{
    while(m_queueFreq.empty() == false)
    {
        m_queueFreq.pop();
    }
}

void Generator::CloseFile()
{
    m_pSoundfile = nullptr;
}

bool Generator::SetPlugin(const wxString& sPlugin)
{
    Stop();
    m_nGenerator = PLUGIN;

    map<wxString, GeneratorPluginBuilder*>::iterator itPlugin = GeneratorPluginFactory::Get()->FindPlugin(sPlugin);
    if(itPlugin != GeneratorPluginFactory::Get()->GetPluginEnd())
    {
        m_pPlugin = itPlugin->second;
        m_pPlugin->Init();
        m_pPlugin->SetSampleRate(m_dSampleRate);
        return true;
    }
    m_pPlugin = NULL;
    return false;
}


bool Generator::SetFile()
{
    Stop();

    m_nGenerator = FILE;

    wxString sFilePath;
    sFilePath << Settings::Get().GetWavDirectory() << wxT("/") << Settings::Get().Read(wxT("Output"), wxT("File"), wxEmptyString) << wxT(".wav");

    bool bOk(false);

    //Does the file exist
    if(wxFileExists(sFilePath))
    {
        m_pSoundfile = std::make_unique<SoundFile>();
        bOk = m_pSoundfile->OpenToRead(sFilePath);
        if(bOk == false)
        {
            CloseFile();
            pmlLog(pml::LOG_ERROR) << "Generator\tFailed to open file '" << sFilePath << "'";
        }
        else
        {
            pmlLog() << "Generator\tOpened file '" << sFilePath << "'";
            pmlLog() << "Generator\tSampleRate =" << GetSampleRate();
            pmlLog() << "Generator\tChannels =" << GetNumberOfChannels();

            Generate(8192);
        }
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "Generator\tFile '" << sFilePath << "' does not exist or generator not established";
    }

    return bOk;

}

unsigned int Generator::GetSampleRate()
{
    if(m_pSoundfile)
    {
        return m_pSoundfile->GetSampleRate();
    }
    return m_dSampleRate;
}


void Generator::ReadSoundFile(timedbuffer* pData)
{
    if(m_pSoundfile)
    {
        if(m_pSoundfile->ReadAudio(pData->GetWritableBuffer(), pData->GetBufferSize(), 1))
        {
            pData->SetDuration(pData->GetBufferSize()*(m_pSoundfile->GetFormat()&0x0F));
        }
    }
}


bool Generator::LoadSequence(const wxString& sFile)
{
    ClearSequences();
    ClearFrequences();
    CloseFile();

    m_nGenerator = SEQUENCE;

    wxXmlDocument doc;
    if(doc.Load(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), sFile.c_str())) && doc.GetRoot())
    {
        doc.GetRoot()->GetAttribute("channels", "2").ToULong(&m_nSequenceChannels);
        for(wxXmlNode* pSequenceNode = doc.GetRoot()->GetChildren(); pSequenceNode; pSequenceNode = pSequenceNode->GetNext())
        {

            if(pSequenceNode->GetName().CmpNoCase(wxT("sequence")) == 0)
            {
                unsigned long nChannel(0);
                pSequenceNode->GetAttribute(wxT("channels"), wxT("0")).ToULong(&nChannel);
                std::shared_ptr<Sequence> pSequence = std::make_shared<Sequence>(nChannel, m_dSampleRate);

                for(wxXmlNode* pFreqGenNode = pSequenceNode->GetChildren(); pFreqGenNode; pFreqGenNode = pFreqGenNode->GetNext())
                {
                    if(pFreqGenNode->GetName().CmpNoCase(wxT("genfreq")) == 0)
                    {
                        double dFrequency, ddBFS;
                        long nCycles(0), nType(0);
                        if(pFreqGenNode->GetAttribute(wxT("frequency"), wxEmptyString).ToDouble(&dFrequency) && pFreqGenNode->GetAttribute(wxT("dBFS"), wxEmptyString).ToDouble(&ddBFS) && pFreqGenNode->GetAttribute(wxT("cycles"), wxEmptyString).ToLong(&nCycles) && pFreqGenNode->GetAttribute(wxT("type"), wxT("0")).ToLong(&nType))
                        {
                            pSequence->AppendGenFreq(dFrequency, ddBFS, nCycles, nType);
                        }
                    }
                }
                AddSequence(pSequenceNode->GetAttribute(wxT("name"), wxEmptyString), pSequence);
            }
        }
        Generate(8192);
        return true;
    }
    return false;

}

void Generator::Stop()
{
    ClearFrequences();
    ClearSequences();
    CloseFile();
    if(m_pPlugin)
    {
        m_pPlugin->Stop();
    }
    m_pPlugin = NULL;
}


void Generator::GeneratePlugin(timedbuffer* pData)
{
    if(m_pPlugin)
    {
        m_pPlugin->GetAudioData(pData);
    }
}


int Generator::GetNumberOfChannels()
{
    switch(m_nGenerator)
    {
        case FILE:
            if(m_pSoundfile)
            {
                return m_pSoundfile->GetChannels();
            }
            return 0;
            break;
        case SEQUENCE:
            return m_nSequenceChannels;
            break;
        case FREQUENCY:
            return 2;
            break;
        case PLUGIN:
            if(m_pPlugin)
            {
                return m_pPlugin->GetNumberOfChannels();
            }
            return 0;
    }

    return 2;
}
