#include "wmfocusmanager.h"

using namespace std;


wmFocusManager* wmFocusManager::Get()
{
    static wmFocusManager fm;
    return &fm;
}

void wmFocusManager::AddControl(wxWindow* pWindow)
{
    m_setWindows.insert(pWindow);
}

bool wmFocusManager::AllowedFocus(wxWindow* pWindow)
{
    return true;//(m_setWindows.find(pWindow) != m_setWindows.end());
}

