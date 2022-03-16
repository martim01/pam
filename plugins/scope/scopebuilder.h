#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class Scope;
class pnlRouting;
class SettingEvent;
class pnlTrigger;
class pnlOffset;

class WXEXPORT ScopeBuilder : public MonitorPluginBuilder
{
    public:
        ScopeBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Scope");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

        friend class Scope;
        friend class pnlRouting;
        friend class pnlOptions;
        friend class pnlTimeframe;
        friend class pnlTrigger;
        friend class pnlVertical;
        friend class pnlOffset;

        void OnSettingChanged(SettingEvent& event);
        void ResetSlide();
        void ClearMemory();
        void SetMemory(bool bSet);


        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        Scope* m_pScope;
        pnlRouting* m_pRouting;
        pnlTrigger* m_pTrigger;
        pnlOffset* m_pOffset;
};

