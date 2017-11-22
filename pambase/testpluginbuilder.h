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


class PAMBASE_IMPEXPORT TestPluginBuilder : public wxEvtHandler
{
    public:
        TestPluginBuilder();
        virtual ~TestPluginBuilder(){}

        virtual void InputSession(const session& aSession)=0;
        virtual void OutputChannels(const std::vector<char>& vChannels)=0;

        virtual void SetAudioData(const timedbuffer* pBuffer)=0;
        virtual wxString GetName() const=0;

    protected:

        friend class TestPluginFactory;


        void SetHandler(wxEvtHandler* pHandler);
        void CreatePanels(wmSwitcherPanel* pswpTestsTest);

        void WriteSetting(const wxString& sSetting, const wxString& sValue);
        void WriteSetting(const wxString& sSetting, int nValue);
        void WriteSetting(const wxString& sSetting, double dValue);

        wxString ReadSetting(const wxString& sSetting, const wxString& sDefault);
        int ReadSetting(const wxString& sSetting, int nDefault);
        double ReadSetting(const wxString& sSetting, double dDefault);

        void RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler);

        virtual wxWindow* CreateTestPanel(wxWindow* pParent)=0;
        virtual void LoadSettings()=0;
        wxEvtHandler* m_pHandler;

    private:
        wmSwitcherPanel* m_pswpTests;
};


