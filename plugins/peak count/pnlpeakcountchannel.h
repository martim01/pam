#ifndef PNLPEAKCOUNTCHANNEL_H
#define PNLPEAKCOUNTCHANNEL_H

//(*Headers(pnlPeakCountChannel)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

class pnlPeakCountChannel: public wxPanel
{
	public:

		pnlPeakCountChannel(wxWindow* parent,unsigned int nChannel, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlPeakCountChannel();

        void AddPeaks(unsigned int nSamples, unsigned int nPeaks, unsigned int nTotal);
        void ResetTest();

        double GetLastAmplitude() const
        {
            return m_dLastAmplitude;
        }
        void SetLastAmplitude(double dAmplitude)
        {
            m_dLastAmplitude = dAmplitude;
        }

		//(*Declarations(pnlPeakCountChannel)
		wmLabel* m_plbLChannel;
		wmLabel* m_plblPeaks;
		wmLabel* m_plblPercent;
		wmLabel* m_plblCount;
		//*)

	protected:

		//(*Identifiers(pnlPeakCountChannel)
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL2;
		//*)

	private:

		//(*Handlers(pnlPeakCountChannel)
		//*)
        void UpdateLabels();

		unsigned int m_nPeaks;
		unsigned int m_nSamples;
		unsigned int m_nTotal;

		double m_dLastAmplitude;

		DECLARE_EVENT_TABLE()
};

#endif
