#pragma once
#include "testpluginbuilder.h"

class pnlLevels;

class LevelsBuilder : public TestPluginBuilder
{
    public:
        LevelsBuilder();
        virtual ~LevelsBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        friend class pnlLevels;
        friend class MaxMinGraph;
        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlLevels* m_ppnlLevels;

};





