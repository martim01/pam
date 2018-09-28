#ifndef PNLANGLEMETERS_H
#define PNLANGLEMETERS_H

#include <vector>

//(*Headers(pnlAngleMeters)
#include <wx/panel.h>
//*)
class AngleMetersBuilder;
class AngleMeter;
class wmButton;

struct session;
class timedbuffer;
class LevelCalculator;

class pnlAngleMeters: public wxPanel
{
	public:

		pnlAngleMeters(wxWindow* parent,AngleMetersBuilder* pBuilder,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlAngleMeters();

		//(*Declarations(pnlAngleMeters)
		//*)

        void SetSession(const session& aSession);

		void SetAudioData(const timedbuffer* pBuffer);

		void SetMode(const wxString& sMode);
        void Freeze(bool bFreeze);
        void ShowPeaks(unsigned int nMode);
        void ClearMeters();
        void SetSpeed(unsigned long nSpeed);
        void SetM3M6(unsigned long nMode);
        void OutputChannels(const std::vector<char>& vChannels);

        void UpdateMeterStereo();

	protected:

		//(*Identifiers(pnlAngleMeters)
		//*)

	private:

		//(*Handlers(pnlAngleMeters)
		void OnLeftUp(wxMouseEvent& event);
		//*)

		void OnMonitorClicked(wxCommandEvent& event);
        void ColourMonitorButtons();
		void CreateMeters();

		std::vector<AngleMeter*> m_vMeters;
        std::vector<wmButton*> m_vMonitor;
        std::vector<char> m_vOutputChannels;
		AngleMetersBuilder* m_pBuilder;

        unsigned int m_nInputChannels;
        bool m_bStereo;

        LevelCalculator* m_pCalculator;
		DECLARE_EVENT_TABLE()
};

#endif
