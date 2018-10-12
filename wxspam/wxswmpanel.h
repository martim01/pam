#pragma once
#include "wxwidgets/wxscontainer.h"

/** \brief Notebook container */
class wxswmPanel : public wxsContainer
{
    public:

        wxswmPanel(wxsItemResData* Data);

    private:

        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnBuildCreatingCode();
        virtual void OnEnumContainerProperties(long Flags);

};

