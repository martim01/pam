#pragma once

//(*Headers([CLASS_PREFIX]Panel)
#include <wx/panel.h>
//*)

#include <vector>

class [CLASS_PREFIX]Builder;
class timedbuffer;
class session;

class [CLASS_PREFIX]Panel: public wxPanel
{
	public:

		[CLASS_PREFIX]Panel(wxWindow* parent,[CLASS_PREFIX]Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~[CLASS_PREFIX]Panel();

		//(*Declarations([CLASS_PREFIX]Panel)
		//*)
		
		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers([CLASS_PREFIX]Panel)
		//*)
		
		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers([CLASS_PREFIX]Panel)
		//*)
		
		[CLASS_PREFIX]Builder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};
