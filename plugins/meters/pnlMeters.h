#ifndef PNLMETERS_H
#define PNLMETERS_H

#include <vector>


class session;
class LevelMeter;
class timedbuffer;
struct ppmtype;

//(*Headers(pnlMeters)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

class LevelMeter;
class wmButton;
class MetersBuilder;
class LevelCalculator;
struct ppmtype;

class pnlMeters: public wxPanel
{
	public:

		pnlMeters(wxWindow* parent,MetersBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMeters();

		//(*Declarations(pnlMeters)
		wxPanel* pnlInfo;
		wmLabel* m_pLbl23;
		wmLabel* m_pLbl25;
		wmLabel* m_pLbl24;
		wmLabel* m_plblSessionChannels;
		wmLabel* m_plblSessionBits;
		wmLabel* m_plblSessionFrequency;
		wmLabel* m_pLbl20;
		wmLabel* m_pLbl27;
		wmLabel* m_pLbl26;
		wmLabel* m_plblSessionType;
		wmLabel* m_plblSessionSource;
		wmLabel* m_pLbl33;
		wmLabel* m_plblSessionName;
		wmLabel* m_plblInput;
		//*)

		void SetSession(const session& aSession);

		void SetAudioData(const timedbuffer* pBuffer);

		void SetMode(const wxString& sMode);
        void Freeze(bool bFreeze);
        void ShowPeaks(unsigned int nMode);
        void ClearMeters();
        void SetSpeed(unsigned long nSpeed);
        void SetM3M6(unsigned long nMode);
        void SetShading(bool bShaded);
        void OutputChannels(const std::vector<char>& vChannels);


        void SetScale(const wxString& sTitle, const ppmtype& aType);

	protected:

		//(*Identifiers(pnlMeters)
		static const long ID_M_PLBL59;
		static const long ID_M_PLBL38;
		static const long ID_M_PLBL40;
		static const long ID_M_PLBL39;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL41;
		static const long ID_M_PLBL43;
		static const long ID_M_PLBL42;
		static const long ID_M_PLBL44;
		static const long ID_M_PLBL47;
		static const long ID_M_PLBL45;
		static const long ID_M_PLBL48;
		static const long ID_M_PLBL46;
		static const long ID_M_PLBL49;
		static const long ID_PANEL1;
		//*)

	private:


		//(*Handlers(pnlMeters)
		void OnLeftUp(wxMouseEvent& event);
		void OnInfoLeftUp(wxMouseEvent& event);
		//*)

		void OnMonitorClicked(wxCommandEvent& event);

		void SetLightColours();

        LevelMeter* m_pLevels;
		std::vector<LevelMeter*> m_vMeters;
        std::vector<wmButton*> m_vMonitor;
		MetersBuilder* m_pBuilder;

		LevelCalculator* m_pCalculator;
		unsigned int m_nInputChannels;

        double m_dOffset;

        double m_dOvermod;

        std::vector<char> m_vOutputChannels;

		DECLARE_EVENT_TABLE()
};

#endif
