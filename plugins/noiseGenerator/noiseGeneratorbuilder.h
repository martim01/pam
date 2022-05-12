#pragma once
#include "generatorpluginbuilder.h"
#include <wx/string.h>
#include <vector>
#include <memory>

class Noise;
class SettingEvent;
class noiseGeneratorPanel;

class WXEXPORT noiseGeneratorBuilder : public GeneratorPluginBuilder
{
    public:
        noiseGeneratorBuilder();

        void GetAudioData(timedbuffer* pBuffer) override;

        wxString GetName() const
        {
            return wxT("Noise");
        }

        void Init() override;
        void Stop() override;

        void LoadSettings();

        unsigned long GetNumberOfChannels() override { return m_vGenerators.size(); }

    protected:

	friend class noiseGeneratorPanel;

    void OnSettingChanged(SettingEvent& event);
    virtual wxWindow* CreateGeneratorPanel(wxWindow* pParent);

    void CreateGenerator(size_t n, const wxString& sType);

	noiseGeneratorPanel* m_pMeter;

    wxArrayString m_asGenerators;

	std::vector<std::unique_ptr<Noise> > m_vGenerators;//std:unique_ptr<Noise>> m_vGenerators;
};



