#ifndef PNLDISTORTIONCHANNEL_H
#define PNLDISTORTIONCHANNEL_H
#include <list>

//(*Headers(pnlDistortionChannel)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

class timedbuffer;
class DistortionBuilder;

class pnlDistortionChannel: public wxPanel
{
	public:

		pnlDistortionChannel(wxWindow* parent,unsigned int nChannel, unsigned int nSampleRate, DistortionBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDistortionChannel();

		void AddAudioData(float dValue);
		void Reset();

		//(*Declarations(pnlDistortionChannel)
		wmLabel* m_plblDistortion;
		wmLabel* m_plblMax;
		wmLabel* m_plblTime;
		wmLabel* m_plblAmplitude;
		wmLabel* m_plbLChannel;
		wmLabel* m_plblPeaks;
		wmLabel* m_plblFrequency;
		//*)

		void RunTest();

	protected:

		//(*Identifiers(pnlDistortionChannel)
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		//*)

	private:

		//(*Handlers(pnlDistortionChannel)
		//*)
		unsigned int m_nChannel;
		unsigned int m_nSampleRate;
		DistortionBuilder* m_pBuilder;
        double m_dMax;
        std::list<float> m_lstBuffer;
		DECLARE_EVENT_TABLE()
};

#endif
