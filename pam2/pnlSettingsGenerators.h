#ifndef PNLSETTINGSGENERATORS_H
#define PNLSETTINGSGENERATORS_H

//(*Headers(pnlSettingsGenerators)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmslider.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/statbox.h>
//*)

class pnlSettingsGenerators: public wxPanel
{
	public:

		pnlSettingsGenerators(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, wxString sNotused=wxEmptyString);
		virtual ~pnlSettingsGenerators();

		void SessionChanged();

		//(*Declarations(pnlSettingsGenerators)
		wmButton* m_pbtn0dbu;
		wmButton* m_pbtn10000;
		wmButton* m_pbtn1000;
		wmButton* m_pbtn450;
		wmButton* m_pbtn5000;
		wmButton* m_pbtn900;
		wmButton* m_pbtnAogEnd;
		wmButton* m_pbtnAogHome;
		wmButton* m_pbtnAogNext;
		wmButton* m_pbtnAogPrev;
		wmButton* m_pbtnMinus10;
		wmButton* m_pbtnMinus1;
		wmButton* m_pbtnMixer;
		wmButton* m_pbtnNext;
		wmButton* m_pbtnNoise0dBu;
		wmButton* m_pbtnPlus10;
		wmButton* m_pbtnPlus1;
		wmButton* m_pbtnSequences;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl7;
		wmLabel* m_plblFrequency;
		wmLabel* m_plblNoisedB;
		wmLabel* m_plblOutput;
		wmLabel* m_plblOutput_1;
		wmLabel* m_plblOutput_2;
		wmLabel* m_plblOutput_3;
		wmLabel* m_plblOutput_4;
		wmLabel* m_plblOutput_5;
		wmLabel* m_plblOutput_6;
		wmLabel* m_plblOutput_7;
		wmLabel* m_plblOutput_8;
		wmLabel* m_plbldB;
		wmList* m_plstAogFiles;
		wmList* m_plstAudioSources;
		wmList* m_plstColour;
		wmList* m_plstOutput_1;
		wmList* m_plstOutput_2;
		wmList* m_plstOutput_3;
		wmList* m_plstOutput_4;
		wmList* m_plstOutput_5;
		wmList* m_plstOutput_6;
		wmList* m_plstOutput_7;
		wmList* m_plstOutput_8;
		wmList* m_plstShape;
		wmSlider* m_pAmplitude;
		wmSlider* m_pNoiseAmplitude;
		wmSlider* m_pSlider;
		wmSwitcherPanel* m_pswpAog;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* Panel4;
		wxPanel* Panel5;
		wxPanel* pnlGenerator;
		wxStaticBox* StaticBox1;
		wxStaticBox* StaticBox2;
		wxStaticBox* StaticBox3;
		//*)



	protected:

		//(*Identifiers(pnlSettingsGenerators)
		static const long ID_M_PLST8;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL8;
		static const long ID_M_PLST3;
		static const long ID_M_PLST1;
		static const long ID_M_PLST2;
		static const long ID_M_PLST4;
		static const long ID_M_PLST6;
		static const long ID_M_PLST10;
		static const long ID_M_PLST11;
		static const long ID_M_PLST12;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL11;
		static const long ID_PANEL11;
		static const long ID_M_PLST7;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN9;
		static const long ID_M_PBTN20;
		static const long ID_PANEL9;
		static const long ID_STATICBOX2;
		static const long ID_STATICBOX1;
		static const long ID_SLIDER;
		static const long ID_M_PLBL7;
		static const long ID_M_PBTN12;
		static const long ID_M_PBTN11;
		static const long ID_M_PBTN10;
		static const long ID_M_PBTN13;
		static const long ID_M_PBTN14;
		static const long ID_M_PBTN15;
		static const long ID_M_PBTN16;
		static const long ID_M_PBTN17;
		static const long ID_M_PBTN18;
		static const long ID_CUSTOM1;
		static const long ID_M_PLBL9;
		static const long ID_M_PBTN19;
		static const long ID_M_PLST9;
		static const long ID_PANEL10;
		static const long ID_M_PLST5;
		static const long ID_STATICBOX3;
		static const long ID_CUSTOM2;
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN21;
		static const long ID_PANEL12;
		static const long ID_M_PSWP2;
		static const long ID_PANEL8;
		//*)

	private:

		//(*Handlers(pnlSettingsGenerators)
		void OnlstAudioSourcesSelected(wxCommandEvent& event);
		void OnlstOutputLeftSelected(wxCommandEvent& event);
		void OnlstOutputRightSelected(wxCommandEvent& event);
		void OnlstAogFilesSelected(wxCommandEvent& event);
		void OnbtnAogHomeClick(wxCommandEvent& event);
		void OnbtnAogPrevClick(wxCommandEvent& event);
		void OnbtnAogNextClick(wxCommandEvent& event);
		void OnbtnAogEndClick(wxCommandEvent& event);
		void OnbtnMinus10Click(wxCommandEvent& event);
		void OnbtnMinus1Click(wxCommandEvent& event);
		void OnbtnPlus1Click(wxCommandEvent& event);
		void OnbtnPlus10Click(wxCommandEvent& event);
		void Onbtn450Click(wxCommandEvent& event);
		void Onbtn900Click(wxCommandEvent& event);
		void Onbtn1000Click(wxCommandEvent& event);
		void Onbtn5000Click(wxCommandEvent& event);
		void Onbtn10000Click(wxCommandEvent& event);
		void Onbtn0dbuClick(wxCommandEvent& event);
		void OnlstShapeSelected(wxCommandEvent& event);
		void OnlstColourSelected(wxCommandEvent& event);
		void OnbtnNoise0dBuClick(wxCommandEvent& event);
		void OnbtnSequencesClick(wxCommandEvent& event);
		void OnbtnNextClick(wxCommandEvent& event);
		void OnbtnMixerClick(wxCommandEvent& event);
		//*)

		void ShowChannelMapping(wmList* pList, wmLabel* pLabel, int nChannels, int nOutput);
		void PopulateChannelList(wmList* pList);
		void OnSettingChanged(SettingEvent& event);

        void ShowFiles();
		void ShowSequences();

        void ShowAogPagingButtons();
        void OnSliderMove(wxCommandEvent& event);
        void OnAmplitudeMove(wxCommandEvent& event);
        void OnNoiseAmplitudeMove(wxCommandEvent& event);

        void DestinationChanged(const wxString& sDestination);
        void SourceChanged(const wxString& sSource);

        void LoadPlugins();
		DECLARE_EVENT_TABLE()
};

#endif
