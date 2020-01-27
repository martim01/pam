#pragma once
#include <map>
#include <wx/string.h>

class MacDb
{
    public:
        MacDb(){}

        bool LoadXml();
        wxString GetVendor(const wxString& sMac) const;
    private:
        std::map<wxString, wxString> m_mVendors;
};
