#include "monitorevent.h"
using namespace std;

DEFINE_EVENT_TYPE(wxEVT_MONITOR_REQUEST)

//wxDECLARE_DYNAMIC_CLASS(MonitorEvent, wxCommandEvent)

MonitorEvent::MonitorEvent() : wxCommandEvent(wxEVT_MONITOR_REQUEST, wxID_ANY)
{

}

MonitorEvent::MonitorEvent(const vector<char>& vChannels) : wxCommandEvent(wxEVT_MONITOR_REQUEST,wxID_ANY),
m_vChannels(vChannels)
{

}


MonitorEvent::MonitorEvent(const MonitorEvent& event) : wxCommandEvent(event), m_vChannels(event.GetChannels())
{

}


const std::vector<char>& MonitorEvent::GetChannels() const
{
    return m_vChannels;
}

