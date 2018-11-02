#ifndef PNLSETTINGSGENERATORS_H
#define PNLSETTINGSGENERATORS_H

//(*Headers(pnlSettingsGenerators)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/statbox.h>
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmslider.h"
#include "wmlist.h"
//*)

class pnlSettingsGenerators: public wxPanel
{
	public:

		pnlSettingsGenerators(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, wxString sNotused=wxEmptyString);
		virtual ~pnlSettingsGenerators();

		void InputSessionChanged();

		//(*Declarations(pnlSettingsGenerators)
		wxPanel* Panel5;
		wmLabel* m_pLbl7;
		wxPanel* pnlGenerator;
		wmButton* m_pbtn900;
		wmList* m_plstAogFiles;
		wxPanel* Panel4;
		wmLabel* m_plbldB;
		wmButton* m_pbtnAogHome;
		wmLabel* m_pLbl1;
		wmButton* m_pbtnAogEnd;
		wmButton* m_pbtnPlus1;
		wmButton* m_pbtn10000;
		wmSwitcherPanel* m_pswpAog;
		wxStaticBox* StaticBox1;
		wmButton* m_pbtn1000;
		wmButton* m_pbtnNoise0dBu;
		wxPanel* Panel3;
		wmSlider* m_pNoiseAmplitude;
		wmButton* m_pbtnAogNext;
		wxStaticBox* StaticBox2;
		wmLabel* m_plblNoisedB;
		wmList* m_plstAudioSources;
		wmButton* m_pbtnMinus10;
		wmSlider* m_pAmplitude;
		wmLabel* m_pLbl6;
		wmButton* m_pbtn450;
		wmButton* m_pbtnAogPrev;
		wmButton* m_pbtnMinus1;
		wmSlider* m_pSlider;
		wxStaticBox* StaticBox3;
		wmList* m_plstShape;
		wxPanel* Panel2;
		wmButton* m_pbtn5000;
		wmButton* m_pbtnPlus10;
		wmList* m_plstOutputRight;
		wmButton* m_pbtn0dbu;
		wmList* m_plstOutputLeft;
		wmLabel* m_plblFrequency;
		wmList* m_plstColour;
		wmButton* m_pbtnSequences;
		wmLabel* m_plblInput;
		//*)



	protected:

		//(*Identifiers(pnlSettingsGenerators)
		static const long ID_M_PLST8;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL8;
		static const long ID_M_PLST3;
		static const long ID_M_PLST10;
		static const long ID_M_PLBL10;
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
		//*)

		void PopulateChannelList(wmList* pList, int nSelected);
		void OnSettingChanged(SettingEvent& event);

        void ShowFiles();
		void ShowSequences();

        void ShowAogPagingButtons();
        void OnSliderMove(wxCommandEvent& event);
        void OnAmplitudeMove(wxCommandEvent& event);
        void OnNoiseAmplitudeMove(wxCommandEvent& event);

        void LoadPlugins();
		DECLARE_EVENT_TABLE()
};

#endif
