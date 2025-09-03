#ifndef POPHUB_PNLR128_H
#define POPHUB_PNLR128_H

#include <array>
#include <memory>
#include <vector>

#include <wx/datetime.h>
#include <wx/panel.h>

#include "r128calculator.h"
#include "session.h"



class LevelMeter;
class timedbuffer;

class MaxMinGraph;
class R128MeterH;
class wmLabel;
class wmButton;
class pophubBuilder;
class R128Calculator;
class TruePeakCalculator;
class CorrelationBar;

class pnlR128 : public wxPanel
{
public:
	pnlR128(wxWindow *parent, pophubBuilder *pBuilder, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long nStyle=wxBORDER_RAISED);
	virtual ~pnlR128();


	void SetSession(const session &aSession);

	void SetAudioData(const timedbuffer *pBuffer);

	void SetMode(unsigned int nMode);
	void Freeze(bool bFreeze);
	void ShowPeaks(unsigned int nMode);
	void ClearMeters();
	void SetShading(bool bShaded);
	void OutputChannels(const std::vector<char> &vChannels);
	void UpdateMeters();

	void InitLabel(wmLabel *pLabel);


	void ChangeR128(const wxString &sType);



private:
    enum enumType { kMomentary=0, kShort, kIntegrated};
	void OnbtnCalculateClick(wxCommandEvent &event);
	void OnbtnResetClick(wxCommandEvent &event);

	void OnLeftUp(wxMouseEvent &event);

	void CreateMeters();

	pophubBuilder* m_pBuilder = nullptr;
	R128MeterH* m_pLevels = nullptr;
 	std::array<R128MeterH*, 3> m_aMeters;

    wmLabel* m_plblShortTitle = nullptr;
	wmLabel* m_plblShort = nullptr;
	wmLabel* m_plblMomentaryTitle = nullptr;
	wmLabel* m_plblMomentary = nullptr;

	wmLabel* m_plblShortMax = nullptr;
	wmLabel* m_plblMomentaryMax = nullptr;

	wmLabel* m_plblLufsTitle = nullptr;
	wmLabel* m_plblLufs = nullptr;
	wmLabel* m_plblTimeTitle = nullptr;
	wmLabel* m_plblTime = nullptr;

	wmLabel* m_plblRange = nullptr;
	wmLabel* m_plblRangeTitle = nullptr;

	
    wmButton* m_pbtnCalculate = nullptr;
    wmButton* m_pbtnReset = nullptr;
	wmLabel* m_plblGroup = nullptr;

	
	R128Calculator m_calc;
	
    double m_dOffset = 0.0;
	unsigned int m_nChannels = 2;

	subsession m_subsession;

	bool m_bRun = true;

	static const wxColour kColourLfs;
	static const wxColour kColourShort;
	static const wxColour kColourPeak;
	static const wxColour kColourTime;
	static const wxColour kColourPeakAlarm;
	DECLARE_EVENT_TABLE()
};

#endif
