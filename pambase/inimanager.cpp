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
#include <iostream>
#include <fstream>
#include <wx/log.h>
#include <wx/stopwatch.h>
#include "log.h"


iniManager::iniManager()
{
}


iniManager::~iniManager()
{

	DeleteSections();
}

void iniManager::DeleteSections()
{
    m_mSections.clear();
}

/*!
    \fn iniManager::ReadIniFile(const wxString& sFilename)
 */
bool iniManager::ReadIniFile(const wxString& sFilename)
{
    	//unsigned int errno;
	DeleteSections();
    m_sFilepath = sFilename;

    std::ifstream ifFile;

	//attempt to open the file
	ifFile.open(m_sFilepath.mb_str(),std::ios::in);
	if(!ifFile.is_open())
	{
	    pmlLog(pml::LOG_CRITICAL) << "IniManager file '" << m_sFilepath.ToStdString() << "' does not exist";
		return false;
	}

	ifFile.seekg(0, std::ios::end);
    size_t nSize = ifFile.tellg();
    ifFile.seekg(0,std::ios::beg);

    std::string text(nSize,0);
    ifFile.read(&text[0], text.size());
    std::istringstream isstr(text);
    ifFile.close();


	std::string sLine;
	std::string sTag;
	std::string sData;


	unsigned long nLine(0);
	//read in each line
	auto itSection = m_mSections.end();

	while(getline(isstr, sLine))
	{
	    ++nLine;
		// if the line starts with a [ then its the start of a section
		if(sLine[0] == '[')
		{
		    std::string sSection;
		    sSection.reserve(sLine.length());

		    for(size_t i = 1; i < sLine.length(); i++)
            {
                if(sLine[i] == ']')
                    break;

                sSection+=sLine[i];
            }
			//get the name of the section
            itSection = m_mSections.insert(std::make_pair(sSection, std::make_shared<iniSection>(sSection, nLine))).first;
		}
		else if(itSection != m_mSections.end() && sLine[0] != '#' && sLine[0] != ';' && sLine.size() >= 2)
        {
            size_t nEqualPos = std::string::npos;
            size_t nCommentPos = 0;
            std::string sKey;
            std::string sData;
            sKey.reserve(sLine.length());
            sData.reserve(sLine.length());

            for(;nCommentPos < sLine.length(); nCommentPos++)
            {
                if(sLine[nCommentPos] == '#' || sLine[nCommentPos] == ';')
                {
                    break;
                }
                else if(sLine[nCommentPos] == '=' && nEqualPos == std::string::npos)
                {
                    nEqualPos = nCommentPos;
                }
                else if(nEqualPos == std::string::npos)
                {
                    sKey += sLine[nCommentPos];
                }
                else
                {
                    sData += sLine[nCommentPos];
                }
            }
            if(nEqualPos != std::string::npos)
            {
                itSection->second->SetValue(sKey, sData);
            }
        }
	}
    return true;
}


/*!
    \fn iniManager::GetSectionBegin()
 */
mapSection::iterator iniManager::GetSectionBegin()
{
    return m_mSections.begin();
}

/*!
    \fn iniManager::GetSectionEnd()
 */
mapSection::iterator iniManager::GetSectionEnd()
{
    return m_mSections.end();
}

std::shared_ptr<iniSection> iniManager::GetSection(const wxString& sSectionName)
{
    auto it = m_mSections.find(sSectionName);
    if(it == m_mSections.end())
        return nullptr;
    return it->second;
}

/*!
    \fn iniManager::GetIniString(const wxString& sSection, const wxString& sKey, const wxString& sDefault)
 */
const wxString& iniManager::GetIniString(const wxString& sSection, const wxString& sKey, const wxString& sDefault) const
{
    //does the section exist?

    auto it = m_mSections.find(sSection);
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
	auto it = m_mSections.find(sSection);
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
	auto it = m_mSections.find(sSection);
	if(it==m_mSections.end())
		return dDefault;

	return it->second->GetDouble(sKey,dDefault);
}

bool iniManager::WriteIniFile(const wxString& sFilename)
{
	//unsigned int errno;
	if(sFilename.empty() == false)
    {
        m_sFilepath = sFilename;
    }

	std::ofstream ofFile;
    //Close file if open
	if(ofFile.is_open())
		ofFile.close();

	//attempt to open the file

	ofFile.open(m_sFilepath.char_str());
	if(!ofFile.is_open())
	{
		return false;
	}

	ofFile.clear();

	for(const auto& pairSection : m_mSections)
    {
        pairSection.second->WriteSection(&ofFile);
	}

		//Close the file again
	if(ofFile.is_open())
		ofFile.close();

    return true;

}

std::shared_ptr<iniSection> iniManager::CreateSection(const wxString& sSectionName)
{
    auto itSection =m_mSections.find(sSectionName);
    if(itSection == m_mSections.end())
    {
        itSection  = m_mSections.insert(std::make_pair(sSectionName,std::make_shared<iniSection>(sSectionName,0))).first;
    }
    return itSection->second;
}

void iniManager::SetSectionValue(const wxString& sSectionName, const wxString& sKey, const wxString& sValue)
{
    auto pSection = CreateSection(sSectionName);
    pSection->SetValue(sKey,sValue);

}
size_t iniManager::GetNumberOfSectionEntries(const wxString& sSectionName)
{

	itSection it = m_mSections.find(sSectionName);
	if(it== m_mSections.end())
        return 0;
    return it->second->GetNumberOfEntries();
}

bool iniManager::RemoveSectionValue(const wxString& sSection, const wxString& sKey)
{
    auto itSection = m_mSections.find(sSection);

    if(itSection != m_mSections.end())
    {
        itSection->second->RemoveKey(sKey);
        return true;
    }
    return false;

}

void iniManager::RemoveSection(const wxString& sSection)
{
    m_mSections.erase(sSection);
}
