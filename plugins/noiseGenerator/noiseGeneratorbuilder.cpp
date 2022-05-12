#include "noiseGeneratorbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "noiseGeneratorpanel.h"
#include "noise.h"


using namespace std;

noiseGeneratorBuilder::noiseGeneratorBuilder() : GeneratorPluginBuilder()
{

    RegisterForSettingsUpdates(this);


    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&noiseGeneratorBuilder::OnSettingChanged);

}

void noiseGeneratorBuilder::GetAudioData(timedbuffer* pBuffer)
{
    for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
    {
        pBuffer->GetWritableBuffer()[i] = m_vGenerators[i%m_vGenerators.size()]->ProduceSample();
    }
}



wxWindow* noiseGeneratorBuilder::CreateGeneratorPanel(wxWindow* pParent)
{
	m_pMeter = new noiseGeneratorPanel(pParent, this);
	return m_pMeter;

}


void noiseGeneratorBuilder::LoadSettings()
{
	//@todo load any settings
    m_asGenerators = wxStringTokenize(ReadSetting("generators", ""), ",");

    m_vGenerators.resize(m_asGenerators.size());

    for(size_t i = 0; i < m_asGenerators.GetCount(); i++)
    {
        CreateGenerator(i, m_asGenerators[i]);
    }

    //@todo amplitude
}

void noiseGeneratorBuilder::CreateGenerator(size_t n, const wxString& sType)
{
    if(sType.CmpNoCase("pink") == 0)
    {
        m_vGenerators[n] = std::make_unique<PinkNoise>();
    }
    else if(sType.CmpNoCase("a") == 0)
    {
        m_vGenerators[n] = std::make_unique<GreyANoise>();
    }
    else if(sType.CmpNoCase("k") == 0)
    {
        m_vGenerators[n] = std::make_unique<GreyKNoise>();
    }
    else if(sType.CmpNoCase("grey") == 0)
    {
        m_vGenerators[n] = std::make_unique<GreyNoise>();
    }
    else
    {
        m_vGenerators[n] = std::make_unique<WhiteNoise>();
    }


}



void noiseGeneratorBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "generators")
    {
        wxArrayString asGenerators = wxStringTokenize(event.GetValue(), ",");
        m_vGenerators.resize(asGenerators.size());

        for(size_t i = 0; i < asGenerators.GetCount(); i++)
        {
            if(i >= m_asGenerators.GetCount() || m_asGenerators[i].CmpNoCase(asGenerators[i]) != 0) //generator doesn't exist or has changed
            {
                CreateGenerator(i, asGenerators[i]);
            }
        }
        m_asGenerators = asGenerators;
    }
    else if(event.GetKey() == "amplitude")
    {
        //@todo
    }
}


void noiseGeneratorBuilder::Init()
{

}

void noiseGeneratorBuilder::Stop()
{
}



