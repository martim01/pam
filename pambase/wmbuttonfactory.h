#pragma once

#include "uirect.h"
#include "dlldefine.h"

/** @brief Class that creates buttons for wmList. If you create a new class based on this one and a new class based on uiRect then you can create different sorts of buttons in wmList
**/
class PAMBASE_IMPEXPORT wmButtonFactory
{
    public:
        ///< @brief constructor
        wmButtonFactory(){}
        virtual ~wmButtonFactory(){}

        /** @brief Function that creates button for wmList
        *   @return <i>uiRect*</i>
        **/
        virtual uiRect* CreateButton()
        {
            return new uiRect();
        }
};


