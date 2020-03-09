/***************************************************************************
 *   Copyright (C) 2005 by Matthew Martin   *
 *   matthew@nabiezzi.plus.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "inimanager.h"
#include "inisection.h"

#include <wx/log.h>


using namespace std;

iniManager::iniManager()
{
}


iniManager::~iniManager()
{

	DeleteSections();
}

void iniManager::DeleteSections()
{
    map<wxString,iniSection*>::iterator it= m_mSections.begin();
	while(it != m_mSections.end())
	{
		delete it->second;
		++it;
	}
	m_mSections.clear();
}

/*!
    \fn iniManager::ReadIniFile(const wxString& sFilename)
 */
bool iniManager::ReadIniFile(const wxString& sFilename)
{
	//unsigned int errno;
	DeleteSections();

    //Close file if open
	if(m_if.is_open())
		m_if.close();

	//attempt to open the file
	m_if.open(sFilename.mb_str(),ios::in);
	if(!m_if.is_open())
	{
	    wxLogDebug(wxT("IniManager file '%s' does not exist"), sFilename.c_str());
		return false;
	}

	m_if.clear();

	string sLine;
	string sTag;
	string sData;

	//read in each line
	while(!m_if.eof())
	{
		// if the line starts with a [ then its the start of a section
		if(sLine[0] == '[')
		{
			//get the name of the section
			size_t nClosePos = sLine.find("]");
			if(nClosePos == std::string::npos)	//this is an error
            {
                wxLogDebug(wxT("IniManager file '%s' invalid section"), sFilename.c_str());
                return false;
            }
			wxString sSection = wxString::FromAscii(sLine.substr(1,nClosePos-1).c_str());
			m_mSections[sSection] = new iniSection(sSection);
			sLine = m_mSections[sSection]->ReadSection(&m_if).char_str();
		}
		else
		{
			getline(m_if,sLine,'\n');
			//if the line starts with a # we ignore
			if(sLine[0] == '#')
				continue;
		}
	}
		//Close the file again
	if(m_if.is_open())
		m_if.close();
    return true;
}


/*!
    \fn iniManager::GetSectionBegin()
 */
std::map<wxString,iniSection*>::iterator iniManager::GetSectionBegin()
{
    return m_mSections.begin();
}

/*!
    \fn iniManager::GetSectionEnd()
 */
std::map<wxString,iniSection*>::iterator iniManager::GetSectionEnd()
{
    return m_mSections.end();
}

const iniSection* iniManager::GetSection(const wxString& sSectionName) const
{
    std::map<wxString,iniSection*>::const_iterator it = m_mSections.find(sSectionName);
    if(it == m_mSections.end())
        return NULL;
    return it->second;
}

/*!
    \fn iniManager::GetIniString(const wxString& sSection, const wxString& sKey, const wxString& sDefault)
 */
const wxString& iniManager::GetIniString(const wxString& sSection, const wxString& sKey, const wxString& sDefault) const
{
    //does the section exist?
    itConstSection it = m_mSections.find(sSection);
	if(it==m_mSections.end())
		return sDefault;


	return it->second->GetString(sKey,sDefault);
}

/*!
    \fn iniManager::GetIniInt(const wxString& sSection, const wxString& sKey, int nDefault)
 */
int iniManager::GetIniInt(const wxString& sSection, const wxString& sKey, int nDefault) const
{
    //does the section exist?
	itConstSection it = m_mSections.find(sSection);
	if(it==m_mSections.end())
		return nDefault;

	return it->second->GetInt(sKey,nDefault);
}

/*!
    \fn iniManager::GetIniDouble(const wxString& sSection, const wxString& sKey, double dDefault)
 */
double iniManager::GetIniDouble(const wxString& sSection, const wxString& sKey, double dDefault) const
{
    //does the section exist?
	itConstSection it = m_mSections.find(sSection);
	if(it==m_mSections.end())
		return dDefault;

	return it->second->GetDouble(sKey,dDefault);
}

bool iniManager::WriteIniFile(const wxString& sFilename)
{
	//unsigned int errno;

    //Close file if open
	if(m_of.is_open())
		m_of.close();

	//attempt to open the file

	m_of.open(sFilename.char_str());
	if(!m_of.is_open())
	{
		return false;
	}

	m_of.clear();

	std::map<wxString,iniSection*>::iterator it = m_mSections.begin();
	while(it != m_mSections.end())
	{
		it->second->WriteSection(&m_of);
		++it;
	}

	//m_of.flush();
		//Close the file again
	if(m_of.is_open())
		m_of.close();

    return true;

}

iniSection* iniManager::CreateSection(const wxString& sSectionName)
{
    map<wxString, iniSection*>::iterator itSection =m_mSections.find(sSectionName);
    if(itSection == m_mSections.end())
    {
        itSection  = m_mSections.insert(make_pair(sSectionName,new iniSection(sSectionName))).first;
    }
    return itSection->second;
}

void iniManager::SetSectionValue(const wxString& sSectionName, const wxString& sKey, const wxString& sValue)
{
    iniSection* pSection = CreateSection(sSectionName);
    pSection->SetValue(sKey,sValue);
}

void iniManager::RemoveSectionValue(const wxString& sSectionName, const wxString& sKey)
{
    iniSection* pSection = CreateSection(sSectionName);
    pSection->RemoveKey(sKey);
}

void iniManager::RemoveSection(const wxString& sSectionName)
{
    map<wxString, iniSection*>::iterator itSection = m_mSections.find(sSectionName);
    if(itSection != m_mSections.end())
    {
        delete itSection->second;
        m_mSections.erase(itSection);
    }
}

void iniManager::RemoveAllSections()
{
    for(auto itSection : m_mSections)
    {
        delete itSection.second;
    }
    m_mSections.clear();
}

size_t iniManager::GetNumberOfSectionEntries(const wxString& sSectionName)
{
	itSection it = m_mSections.find(sSectionName);
	if(it== m_mSections.end())
        return 0;
    return it->second->GetNumberOfEntries();
}
