#ifndef PNLSETTINGSINPUTNMOS_H
#define PNLSETTINGSINPUTNMOS_H

//(*Headers(pnlSettingsInputNmos)
#include "wmlist.h"
#include <wx/panel.h>
//*)
#include <memory>
#include <set>
#include <list>

class SettingEvent;
class Sender;

class pnlSettingsInputNmos: public wxPanel
{
	public:

		pnlSettingsInputNmos(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int nStyle=0, const wxString& sName="");
		virtual ~pnlSettingsInputNmos();

		//(*Declarations(pnlSettingsInputNmos)
		wmList* m_plstSenders;
		//*)
        void SetReceiverId(const wxString& sReceiverId){ m_sReceiverId = sReceiverId; }
        const wxString& GetReceiverId() const { return m_sReceiverId; }
        void SetSender(const wxString& sSenderId);
#ifdef __NMOS__
		void AddSender(std::shared_ptr<Sender> pSender);
		void UpdateSender(std::shared_ptr<Sender> pSender);
		void RemoveSenders(const std::list<std::shared_ptr<Sender>>& lstRemove);
#endif // __NMOS__
		void SubscriptionRequest(const wxString& sReceiverId, const wxString& sResponse, unsigned long nResult);



	protected:

		//(*Identifiers(pnlSettingsInputNmos)
		static const long ID_M_PLST1;
		//*)

	private:

		//(*Handlers(pnlSettingsInputNmos)
		void OnlstSendersSelected(wxCommandEvent& event);
		//*)
        void OnSettingChanged(SettingEvent& event);

		bool ConnectionIS04(size_t nSenderButton);
		bool ConnectionIS05(size_t nSenderButton);
        wxString m_sReceiverId;


		DECLARE_EVENT_TABLE()
};

#endif
