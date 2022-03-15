#ifndef PNLLISSAJOU_H
#define PNLLISSAJOU_H
#include <vector>

//(*Headers(pnlLissajou)
#include "lissajoumeter.h"
#include "levelmeter.h"
#include <wx/panel.h>
//*)
#include "session.h"

class timedbuffer;

class LissajouBuilder;
class LevelCalculator;
struct ppmtype;


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
		void SetMeterMode(const wxString& sMode);

		void OutputChannels(const std::vector<char>& vChannels);
		void Follow();
		void SetDisplayType(unsigned int nType);

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

		void SetScale(const wxString& sTitle, const ppmtype& aType);

		static LissajouBuilder* m_pBuilder;



		LevelCalculator* m_pCalculator;

		std::vector<char> m_vOutputs;

		unsigned int m_nChannels;
		unsigned int m_nChannelX;
		unsigned int m_nChannelY;

		subsession m_subsession;

		DECLARE_EVENT_TABLE()
};

#endif
