#include "pnlgroups.h"

//(*InternalHeaders(pnlGroups)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "r128builder.h"

//(*IdInit(pnlGroups)
const long pnlGroups::ID_M_PLST3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlGroups,wxPanel)
	//(*EventTable(pnlGroups)
	//*)
END_EVENT_TABLE()

pnlGroups::pnlGroups(wxWindow* parent,R128Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstGroups = new wmList(this, ID_M_PLST3, wxPoint(0,0), wxSize(190,200), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(2,2));
	m_plstGroups->SetBackgroundColour(wxColour(0,0,0));

}

pnlGroups::~pnlGroups()
{
	//(*Destroy(pnlGroups)
	//*)
}

void pnlGroups::SetChannels(const std::vector<subsession::channelGrouping>& vChannels)
{

    m_plstGroups->Freeze();
    m_plstGroups->Clear();

    unsigned char nGroup(0xFF);
    for(const auto& channel : vChannels)
    {
        if(channel.nId != nGroup)
        {
            m_plstGroups->AddButton(GetChannelGroupName(channel), wxNullBitmap, reinterpret_cast<void*>(channel.nId));
            nGroup = channel.nId;
        }
    }


    m_plstGroups->ConnectToSetting(m_pBuilder->GetSection(), "Group", reinterpret_cast<void*>(0));

    m_plstGroups->Thaw();
}

