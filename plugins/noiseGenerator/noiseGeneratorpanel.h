#pragma once

//(*Headers(noiseGeneratorPanel)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmslider.h"
#include <wx/panel.h>
#include <wx/statbox.h>
//*)

#include <vector>
#include "pmpanel.h"

class noiseGeneratorBuilder;
class timedbuffer;
class session;
class SettingEvent;

class noiseGeneratorPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(noiseGeneratorPanel);

		noiseGeneratorPanel(){}
		noiseGeneratorPanel(wxWindow* parent,noiseGeneratorBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~noiseGeneratorPanel();

		//(*Declarations(noiseGeneratorPanel)
		wmButton* m_pbtnGenerators;
		wmButton* m_pbtnNoise0dBu;
		wmLabel* m_pLbl1;
		wmLabel* m_plblNoisedB;
		wmLabel* m_plblSelected;
		wmList* m_plstColour;
		wmList* m_plstGenerators;
		wmSlider* m_pNoiseAmplitude;
		wxPanel* m_ppnlSettings;
		wxStaticBox* StaticBox3;

		//*)

		void OnSettingEvent(SettingEvent& event);

	protected:

		//(*Identifiers(noiseGeneratorPanel)
		static const long ID_M_PLBL2;
		static const long ID_M_PBTN1;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST5;
		static const long ID_STATICBOX3;
		static const long ID_CUSTOM2;
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN21;
		static const long ID_PANEL12;
		//*)


	private:

		//(*Handlers(noiseGeneratorPanel)
		void OnlstGeneratorsSelected(wxCommandEvent& event);
		void OnpbtnGeneratorsClick(wxCommandEvent& event);
		//*)

		void OnNoiseAmplitudeMove(wxCommandEvent& event);
		void OnlstColourSelected(wxCommandEvent& event);
		void OnbtnNoise0dBuClicked(wxCommandEvent& event);

		void LoadSettings();


		void WriteGenerators();
		void WriteAmplitudes();
		void PopulateGeneratorList();
		void ExtractAmplitudes(const wxString& sValue);


		wxArrayString m_asGenerators;
		wxArrayDouble m_adAmplitude;

        noiseGeneratorBuilder* m_pBuilder;

        int m_nSelected;

		DECLARE_EVENT_TABLE()
};

