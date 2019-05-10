#pragma once
#include <wx/xml/xml.h>
#include "curl/curl.h"
#include <map>
#include <list>


struct MemoryStruct
{
    ///< @brief Constructor
    MemoryStruct()
    {
        pMemory = reinterpret_cast<char*>(malloc(1));
        nSize = 0;
    }

    ///< @brief Destructor
    ~MemoryStruct()
    {
        free(pMemory);
    }

  char *pMemory;    ///< @brief Pointer to some memory
  size_t nSize;     ///< @brief Size of the allocated memory
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
static size_t WriteFileCallback(void *contents, size_t size, size_t nmemb, void *userp);
static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr);



struct UpdateObject
{
    UpdateObject() : nType(UNKNOWN){}
    wxString sName;
    wxString sVersion;
    wxString sChangelog;
    wxString sFullPath;
    wxString sFilename;
    int nType;

    std::map<wxString, wxString> mDependsOn;

    enum {UNKNOWN, APP, CORE_DLL, PLUGIN_MONITOR, PLUGIN_TEST, DOCUMENTATION, PROFILES, CONFIG};
};

class wxXmlNode;

class UpdateManager
{
    public:
        static UpdateManager& Get();

        bool GetUpdateList();

        bool Update(wxString sName);

        std::map<wxString, UpdateObject>::const_iterator GetUpdateListBegin() const;
        std::map<wxString, UpdateObject>::const_iterator GetUpdateListEnd() const;

        wxString GetChangelog(const wxString& sName) const;
        wxString GetVersion(const wxString& sName) const;
        std::map<wxString, wxString> GetDependencies(const wxString& sName);

        bool UpdateIsNewer(const wxString& sUpdate, const wxString& sLocalVersion);
        bool UpdateVersionIsNewer(const wxString& sUpdateVersion, const wxString& sLocalVersion);

    private:
        UpdateManager();
        ~UpdateManager();



        bool GetUpdateListFromWebServer();
        bool GetUpdateListFromShare();
        bool GetUpdateListFromUSB();
        bool GetUpdateListFromLocal(const wxString& sPath);
        bool GetUpdateListFromFTP();

        bool UpdateFromWebServer(const wxString& sName, const wxString& sTempFile);
        bool UpdateFromShare(const wxString& sName, const wxString& sTempFile, int nType);
        bool UpdateFromLocal(const wxString& sName, const wxString& sTempFile, int nType);
        bool UpdateFromFTP(const wxString& sName, const wxString& sTempFile);
        bool UpdateFromUSB(const wxString& sName, const wxString& sTempFile, int nType);

        bool DecodeUpdateList(const wxXmlDocument& xmlDoc);
        void AddUpdateToList(wxXmlNode* pUpdateNode);
        void AddDependencyToList(UpdateObject& anObject, wxXmlNode* pDepNode);


        std::map<wxString, UpdateObject> m_mUpdates;

        wxString m_sCurlError;
        wxString m_sCurlErrorBuffer;
        long m_nCurlResponseCode;

};
