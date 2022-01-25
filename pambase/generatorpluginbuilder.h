#pragma once
#include "dlldefine.h"
#include <wx/event.h>
#include <map>
#include <list>
#include <vector>
#include "session.h"

class wmSwitcherPanel;
class timedbuffer;

typedef std::pair<wxString, wxWindow*> pairOptionPanel_t;


class PAMBASE_IMPEXPORT GeneratorPluginBuilder : public wxEvtHandler
{
    public:
        GeneratorPluginBuilder();
        virtual ~GeneratorPluginBuilder(){}

        virtual void GetAudioData(timedbuffer* pBuffer)=0;
        virtual wxString GetName() const=0;
        virtual void Init()=0;
        virtual void Stop()=0;
        void SetSampleRate(double dSampleRate)  {   m_dSampleRate = dSampleRate; }

    protected:

        friend class GeneratorPluginFactory;

        bool IsLogActive();

        void SetHandler(wxEvtHandler* pHandler);
        void CreatePanels(wmSwitcherPanel* pswpGenerators);

        void WriteSetting(const wxString& sSetting, const wxString& sValue);
        void WriteSetting(const wxString& sSetting, int nValue);
        void WriteSetting(const wxString& sSetting, double dValue);

        wxString ReadSetting(const wxString& sSetting, const wxString& sDefault);
        int ReadSetting(const wxString& sSetting, int nDefault);
        double ReadSetting(const wxString& sSetting, double dDefault);

        void RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler);

        virtual wxWindow* CreateGeneratorPanel(wxWindow* pParent)=0;
        virtual void LoadSettings()=0;
        wxEvtHandler* m_pHandler;
        double m_dSampleRate;

    private:
        wmSwitcherPanel* m_pswpGenerators;
};



