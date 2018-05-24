#ifndef PNLIDENTIFY_H
#define PNLIDENTIFY_H

//(*Headers(pnlIdentify)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)

#include <vector>

class SoundFile;
class timedbuffer;
struct session;

class MyProcess;
class IdentifyBuilder;

class pnlIdentify: public wxPanel
{
	public:

		pnlIdentify(wxWindow* parent,IdentifyBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlIdentify();

		//(*Declarations(pnlIdentify)
		wmLabel* m_pLbl4;
		wmButton* m_pbtnIdentify;
		wmLabel* m_pLbl3;
		wmLabel* m_plblDuration;
		wmLabel* m_pLbl7;
		wmLabel* m_plblTrack;
		wmLabel* m_plblTitle;
		wmLabel* m_pLbl12;
		wmLabel* m_plblStatus;
		wmLabel* m_pLbl5;
		wmLabel* m_plblArtist;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl2;
		wxPanel* Panel2;
		wmLabel* m_plblMatch;
		wmLabel* m_plblAlbum;
		//*)

		void InputSession(const session& aSession);
        void SetAudioData(const timedbuffer* pBuffer);
        void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(pnlIdentify)
		static const long ID_M_PLBL37;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL12;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(pnlIdentify)
		void OnbtnIdentifyClick(wxCommandEvent& event);
		//*)

		void LaunchGracenote();
		void OnGracenoteFinished(wxCommandEvent& event);

        void SendBusy(bool bBusy);

        IdentifyBuilder* m_pBuilder;

		MyProcess *m_pProcess;
		SoundFile* m_pSf;

		bool m_bWriting;
        unsigned int m_nInputChannels;
        unsigned int m_nSampleRate;
		unsigned int m_nChannels[2];

		DECLARE_EVENT_TABLE()
};

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_GRACENOTE_BUSY, -1)
#endif
