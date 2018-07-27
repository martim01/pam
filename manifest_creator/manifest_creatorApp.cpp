/***************************************************************
 * Name:      manifest_creatorApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-05
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "manifest_creatorApp.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/log.h>
//(*AppHeaders
#include "manifest_creatorMain.h"
#include <wx/image.h>
//*)

using namespace std;

int main()
{

    manifest_creatorApp anApp;
    return 0;
}

manifest_creatorApp::manifest_creatorApp()
{
    ReadProjectFile();

    m_sVersion = ReadVersionFile(wxT("version.h"));

//    for(list<wxString>::iterator itDependency = m_lstDependencies.begin(); itDependency != m_lstDependencies.end(); ++itDependency)
//    {
//        //attempt to find the dependency folder
//
//    }


    ReadChangeLogFile();
    WriteManifest();


}
void manifest_creatorApp::ReadProjectFile()
{
    wxArrayString asFiles;
    wxDir::GetAllFiles(wxT("."), &asFiles, wxT("*.cbp"), wxDIR_FILES);
    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxXmlDocument doc;
        if(doc.Load(asFiles[i]) && doc.GetRoot())
        {
            wxFileName filename(asFiles[i]);
            //m_sName = filename.GetName();

            for(wxXmlNode* pProjectNode = doc.GetRoot()->GetChildren(); pProjectNode; pProjectNode = pProjectNode->GetNext())
            {
                if(pProjectNode->GetName().CmpNoCase(wxT("Project")) == 0)
                {
                    for(wxXmlNode* pBuildNode = pProjectNode->GetChildren(); pBuildNode; pBuildNode = pBuildNode->GetNext())
                    {
                        if(pBuildNode->GetName().CmpNoCase(wxT("Build")) == 0)
                        {
                            for(wxXmlNode* pTargetNode = pBuildNode->GetChildren(); pTargetNode; pTargetNode = pTargetNode->GetNext())
                            {
                                #ifdef __WXMSW__
                                if(pTargetNode->GetName().CmpNoCase(wxT("Target")) == 0 && pTargetNode->GetAttribute(wxT("title"), wxEmptyString).CmpNoCase(wxT("Windows Release")) == 0)
                                #else
                                if(pTargetNode->GetName().CmpNoCase(wxT("Target")) == 0 && pTargetNode->GetAttribute(wxT("title"), wxEmptyString).CmpNoCase(wxT("Linux Release")) == 0)
                                #endif // __WXMSW__
                                {
                                    for(wxXmlNode* pLinkerNode = pTargetNode->GetChildren(); pLinkerNode; pLinkerNode = pLinkerNode->GetNext())
                                    {
                                        if(pLinkerNode->GetName().CmpNoCase(wxT("option")) == 0 && pLinkerNode->HasAttribute(wxT("output")))
                                        {
                                            m_sName = pLinkerNode->GetAttribute(wxT("output"), wxEmptyString).AfterLast(wxT('/'));
                                            if(m_sName.Find(wxT(".")) != wxNOT_FOUND)
                                            {
                                                m_sName = m_sName.BeforeLast(wxT('.'));
                                            }
                                            #ifdef __WXGTK__
                                            if(m_sName.Left(3) == wxT("lib"))
                                            {
                                                m_sName = m_sName.Mid(3);
                                            }
                                            #endif // __WXGTK__
                                        }
                                        if(pLinkerNode->GetName().CmpNoCase(wxT("Linker")) == 0)
                                        {
                                            for(wxXmlNode* pAddNode = pLinkerNode->GetChildren(); pAddNode; pAddNode = pAddNode->GetNext())
                                            {
                                                if(pAddNode->GetName().CmpNoCase(wxT("Add")) == 0 && pAddNode->HasAttribute(wxT("library")))
                                                {
                                                    if(pAddNode->GetAttribute(wxT("library"), wxEmptyString).Find(wxT("pam"))!=wxNOT_FOUND)
                                                    {
                                                        m_lstDependencies.push_back(pAddNode->GetAttribute(wxT("library"), wxEmptyString));
                                                    }
                                                }
                                            }
                                        }
                                        else if(pLinkerNode->GetName().CmpNoCase(wxT("Option")) == 0)
                                        {
                                            if(pLinkerNode->HasAttribute(wxT("type")))
                                            {
                                                unsigned long nType;
                                                pLinkerNode->GetAttribute(wxT("type"), wxT("0")).ToULong(&nType);
                                                if(nType == 0 || nType == 1)
                                                {
                                                    m_sType = wxT("app");
                                                }
                                                else
                                                {
                                                    m_sType = wxT("lib");
                                                }
                                            }
                                            else if(pLinkerNode->HasAttribute(wxT("output")))
                                            {
                                                m_sPath = pLinkerNode->GetAttribute(wxT("output"), wxEmptyString).BeforeLast(wxT('/'));
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        else if(pBuildNode->GetName().CmpNoCase(wxT("Compiler")) == 0)
                        {
                            for(wxXmlNode* pComNode = pBuildNode->GetChildren(); pComNode; pComNode = pComNode->GetNext())
                            {
                                if(pComNode->GetName().CmpNoCase(wxT("Add")) == 0 && pComNode->HasAttribute(wxT("option")))
                                {
                                    wxString sOption = pComNode->GetAttribute(wxT("option"), wxEmptyString);
                                    if(sOption == wxT("-D_core_"))
                                    {
                                        m_sDefinedType = wxT("core");
                                    }
                                    else if(sOption == wxT("-D_app_"))
                                    {
                                        m_sDefinedType = wxT("app");
                                    }
                                    else if(sOption == wxT("-D_monitor_"))
                                    {
                                        m_sDefinedType = wxT("monitor");
                                    }
                                    else if(sOption == wxT("-D_test_"))
                                    {
                                        m_sDefinedType = wxT("test");
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void manifest_creatorApp::ReadChangeLogFile()
{
    wxLogNull ln;
    wxTextFile txtFile;
    if(txtFile.Open(wxT("ChangesLog.txt")))
    {
        wxString sVersion;
        for(size_t i = 0; i < txtFile.GetLineCount(); i++)
        {
            if(txtFile.GetLine(i).Find(wxT("released version")) != wxNOT_FOUND)
            {
                sVersion = txtFile.GetLine(i).AfterFirst(wxT('n'));
                sVersion.Trim();
            }
            else if(txtFile.GetLine(i).Find(wxT("        -")) != wxNOT_FOUND)
            {
                m_lstChangelog.push_back(make_pair(sVersion, txtFile.GetLine(i).AfterFirst(wxT('-'))));
            }
        }
    }
}


void manifest_creatorApp::WriteManifest()
{
    list<pair<wxString,wxString> >::iterator itVersion = m_lstChangelog.begin();
    wxString sVersion(m_sVersion);

    wxXmlDocument xmlDoc;
    xmlDoc.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pam")));

    wxXmlNode* pUpdateNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("update"));

    wxXmlNode* pNameNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("name"));
    pNameNode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, m_sName));


    wxXmlNode* pTypeNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("type"));
    pTypeNode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, m_sDefinedType));

    wxXmlNode* pVersionNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("version"));
    pVersionNode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, m_sVersion));


    wxXmlNode* pChangeNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("changelog"));

    wxString sContent;
    if(itVersion != m_lstChangelog.end())
    {
        sContent << wxT("<ul>");
    }

    for(; itVersion != m_lstChangelog.end(); ++itVersion)
    {
        if(sVersion != (*itVersion).first)
        {
            sVersion = (*itVersion).first;
            sVersion.Trim();
            sContent << wxT("</ul>");
            sContent << wxT("<b><u>") << sVersion << wxT("</u></b>");
            sContent << wxT("<ul>");
        }
        sContent << wxT("<li>") << (*itVersion).second << wxT("</li>");
    }
    sContent << wxT("</ul>");
    pChangeNode->AddChild(new wxXmlNode(wxXML_CDATA_SECTION_NODE, wxEmptyString, sContent));

    pUpdateNode->AddChild(pNameNode);
    pUpdateNode->AddChild(pTypeNode);
    pUpdateNode->AddChild(pVersionNode);
    pUpdateNode->AddChild(pChangeNode);


    for(list<wxString>::iterator itDepends = m_lstDependencies.begin(); itDepends != m_lstDependencies.end(); ++itDepends)
    {
        wxXmlNode* pDependsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("depends_on"));
        wxXmlNode* pDependsNameNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("name"));
        pDependsNameNode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, (*itDepends)));
        //@todo add version
        pDependsNode->AddChild(pDependsNameNode);
        pUpdateNode->AddChild(pDependsNode);
    }





    xmlDoc.GetRoot()->AddChild(pUpdateNode);

    xmlDoc.Save(wxString::Format(wxT("%s/%s_manifest.xml"), m_sPath.c_str(), m_sName.c_str()));
}


wxString manifest_creatorApp::ReadVersionFile(wxString sFile)
{
    wxLogNull ln;
    wxTextFile txtFile(sFile);
    if(txtFile.Open())
    {
        for(size_t i = 0; i < txtFile.GetLineCount(); i++)
        {
            if(txtFile.GetLine(i).Find(wxT("static const char FULLVERSION_STRING []")) != wxNOT_FOUND)
            {
                return txtFile.GetLine(i).AfterFirst(wxT('"')).BeforeFirst(wxT('"'));
            }
        }
    }
    return wxEmptyString;
}
