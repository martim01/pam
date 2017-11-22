#ifndef PNLLISSAJOU_H
#define PNLLISSAJOU_H
#include <vector>

//(*Headers(pnlLissajou)
#include "lissajoumeter.h"
#include "levelmeter.h"
#include <wx/panel.h>
//*)

class timedbuffer;
class session;
class LissajouBuilder;

class pnlLissajou: public wxPanel
{
	public:

		pnlLissajou(wxWindow* parent,LissajouBuilder* pBuilder,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLissajou();

		//(*Declarations(pnlLissajou)
		LevelMeter* m_pMeterRight;
		LissajouMeter* m_pJellyfish;
		LevelMeter* m_pMeterLeft;
		wxPanel* pnlLissajou_Meters;
		LevelMeter* m_pMeterLevels;
		//*)

		void SetAudioData(const timedbuffer* pBuffer);
		void SetSession(const session& aSession);
		void SetScale(unsigned int nScale);
		void Rotate(bool bRotate);
		void ShowLevels(bool bShow);

		void SetAxis(unsigned int nChannelX, unsigned int nChannelY);
		void SetMeterMode(unsigned int nMode);

		void OutputChannels(const std::vector<char>& vChannels);
		void Follow();

	protected:

		//(*Identifiers(pnlLissajou)
		static const long ID_CUSTOM7;
		static const long ID_CUSTOM1;
		static const long ID_CUSTOM6;
		static const long ID_CUSTOM8;
		static const long ID_PANEL19;
		//*)

	private:

		//(*Handlers(pnlLissajou)
		void OnLeftUp(wxMouseEvent& event);
		void OnResize(wxSizeEvent& event);
		//*)
        void CheckAxis();
		static LissajouBuilder* m_pBuilder;

		std::vector<char> m_vOutputs;

		unsigned int m_nChannels;

		DECLARE_EVENT_TABLE()
};

#endif
