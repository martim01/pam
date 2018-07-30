#ifndef DLGSEQUENCE_H
#define DLGSEQUENCE_H

//(*Headers(dlgSequence)
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/dialog.h>
#include "wmedit.h"
#include "wmlist.h"
//*)
#include <wx/xml/xml.h>

class dlgSequence: public wxDialog
{
	public:

		dlgSequence(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgSequence();

		//(*Declarations(dlgSequence)
		wmLabel* m_pLbl4;
		wmButton* m_pbtnFileCopy;
		wxPanel* pnlSequences;
		wmLabel* m_pLbl3;
		wmButton* m_pbtnFileRename;
		wmList* m_plstSequences;
		wmEdit* m_pedtAmplitude;
		wmButton* m_pbtnSequenceCopy;
		wmButton* m_ptbnFileCreate;
		wmList* m_plstFrequency;
		wmButton* m_pbtnFrequencyDown;
		wmButton* m_pbtnFrequencyAppend;
		wmKeyboard* m_pkbd;
		wmList* m_plstFiles;
		wmLabel* m_pLbl1;
		wmButton* m_pbtnSequenceRename;
		wxStaticBox* StaticBox1;
		wxStaticBox* StaticBox2;
		wxPanel* pnlFrequencies;
		wmEdit* m_pedtFrequency;
		wmButton* m_pbtnSequenceCreate;
		wmButton* m_pbtnFileDelete;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnSequenceDelete;
		wxPanel* pnlFile;
		wmButton* m_pbtnFinished;
		wxStaticBox* StaticBox3;
		wmEdit* m_pedtCycles;
		wmList* m_plstFrequencyShape;
		wmButton* m_pbtnFrequencyUpdate;
		wmButton* m_pbtnFrequencyDelete;
		wmButton* m_pbtnFrequencyUp;
		//*)

	protected:

		//(*Identifiers(dlgSequence)
		static const long ID_STATICBOX1;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN13;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_PANEL1;
		static const long ID_STATICBOX2;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN14;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN6;
		static const long ID_PANEL2;
		static const long ID_STATICBOX3;
		static const long ID_M_PLST3;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN10;
		static const long ID_M_PBTN11;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PEDT1;
		static const long ID_M_PEDT2;
		static const long ID_M_PEDT3;
		static const long ID_M_PLST4;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN9;
		static const long ID_M_PKBD1;
		static const long ID_M_PBTN12;
		static const long ID_PANEL3;
		//*)

	private:

		//(*Handlers(dlgSequence)
		void OnlstFilesSelected(wxCommandEvent& event);
		void OnbtnFileRenameClick(wxCommandEvent& event);
		void OntbnFileDeleteClick(wxCommandEvent& event);
		void OntbnFileCreateClick(wxCommandEvent& event);
		void OnbtnSequenceRenameClick(wxCommandEvent& event);
		void OnbtnSequenceDeleteClick(wxCommandEvent& event);
		void OnbtnSequenceCreateClick(wxCommandEvent& event);
		void OnlstSequencesSelected(wxCommandEvent& event);
		void OnlstFrequencySelected(wxCommandEvent& event);
		void OnbtnFrequencyDeleteClick(wxCommandEvent& event);
		void OnlstFrequencyShapeSelected(wxCommandEvent& event);
		void OnbtnFrequencyUpClick(wxCommandEvent& event);
		void OnbtnFrequencyDownClick(wxCommandEvent& event);
		void OnbtnFrequencyUpdateClick(wxCommandEvent& event);
		void OnbtnFrequencyAppendClick(wxCommandEvent& event);
		void OnedtFrequencyTextEnter(wxCommandEvent& event);
		void OnedtAmplitudeTextEnter(wxCommandEvent& event);
		void OnedtCyclesTextEnter(wxCommandEvent& event);
		void OnbtnFinishedClick(wxCommandEvent& event);
		void OnCheckFrequency(wxCommandEvent& event);
		void OnbtnFileCopyClick(wxCommandEvent& event);
		void OnbtnSequencyCopyClick(wxCommandEvent& event);
		//*)

		void LoadFile();
		void LoadSequence();
		void LoadFrequency();
        void SaveDoc();

        void ClearFrequencyPanel();
        void ClearSequencePanel();

        void FillInFreqGen(wxXmlNode* pNode);

		wxString m_sFilename;
		wxXmlDocument* m_pDoc;
		wxXmlNode* m_pSequenceNode;
		wxXmlNode* m_pFrequencyNode;

        int m_nFileButton;
        int m_nSequenceButton;
        int m_nFrequencyButton;
        int m_nWaveShape;

		static const wxString STR_CHANNELS[4];
		static const wxString STR_SHAPE[4];
		DECLARE_EVENT_TABLE()
};

#endif
