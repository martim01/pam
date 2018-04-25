#pragma once
#include "testpluginbuilder.h"

class pnlRecord;

class RecordBuilder : public TestPluginBuilder
{
    public:
        RecordBuilder();
        virtual ~RecordBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        friend class pnlRecord;
        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlRecord* m_ppnlRecord;

};




