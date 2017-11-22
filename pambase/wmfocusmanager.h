#pragma once
#include <set>
#include <wx/window.h>
#include "dlldefine.h"


/** @brief Small class that controls which wm windows may have keyboard focus. You shouldn't need to use this class - controls which are allowed keyboard focus, register themselves with this class and once they gain focus they will only give it up to another control that is also registered with this class.
**/
class PAMBASE_IMPEXPORT wmFocusManager
{
    public:
        /** @brief Gets a pointer to the singleton
        *   @return <i>wmFocusManager*</i>
        **/
        static wmFocusManager* Get();

        /** @brief Adds a window to those that are allowed to have keyboard focus
        *   @param pWindow the window to add
        **/
        void AddControl(wxWindow* pWindow);

        /** @brief Checks whether the window is allowed to have focus
        *   @param pWindow the window to check
        *   @return <i>bool</i> true if the window can have focus
        **/
        bool AllowedFocus(wxWindow* pWindow);

    private:
        wmFocusManager(){}
        std::set<wxWindow*> m_setWindows;


};
