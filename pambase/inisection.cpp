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
#include "inisection.h"
#include <iostream>
#include <fstream>

#include <wx/log.h>

using namespace std;

iniSection::iniSection(const wxString& sSection)
: m_sSectionName(sSection)
{

}


iniSection::~iniSection()
{
}




/*!
    \fn iniSection::ReadSection(std::ifstream* pif)
	reads lines from ifstream* pif until the end of the ini section or end of file
	returns the last line it read
 */
wxString iniSection::ReadSection(std::ifstream* pif)
{
   //we loop through the ini file until we hit the end, or a new section
	string sLine;
	while(!pif->eof())
	{
		getline(*pif,sLine,'\n');


		if(sLine[0] == '#' || sLine.size() < 2 || sLine[0] == ';')	//comment
			continue;

		if(sLine[0] == '[')	// new section
			return wxString::FromUTF8(sLine.c_str());

		size_t nEqualPos = sLine.find("=");
		if(nEqualPos == std::string::npos)
		{
			return wxT("");
		}

		size_t nCommentPos = sLine.find("#",nEqualPos);
		if(nCommentPos == std::string::npos)
			nCommentPos = sLine.size();

		string sKey = sLine.substr(0,nEqualPos);
		m_mSectionData[wxString::FromUTF8(sKey.c_str())] = wxString::FromUTF8(sLine.substr(nEqualPos+1, nCommentPos-(nEqualPos+1)).c_str());
	}
	return wxString::FromUTF8(sLine.c_str());
    //return NULL_STRING;
}


/*!
    \fn iniSection::GetDataBegin()
 */
std::map<wxString,wxString>::const_iterator iniSection::GetDataBegin() const
{
    return m_mSectionData.begin();
}

/*!
    \fn iniSection::GetDataEnd()
 */
std::map<wxString,wxString>::const_iterator iniSection::GetDataEnd() const
{
    return m_mSectionData.end();
}


/*!
    \fn iniSection::GetString(const wxString& sKey, const wxString& sDefault)
 */
const wxString& iniSection::GetString(const wxString& sKey, const wxString& sDefault)
{
  	//does the key exist
  	itIniData it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
		return sDefault;
	return it->second;
}

/*!
    \fn iniSection::GetInt(const wxString& sKey, int nDefault)
 */
int iniSection::GetInt(const wxString& sKey, int nDefault)
{
  	//does the key exist
  	itIniData it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
		return nDefault;
    long n = nDefault;

	it->second.ToLong(&n);
	return n;
}

/*!
    \fn iniSection::GetString(const wxString& sKey, double dDefault)
 */
double iniSection::GetDouble(const wxString& sKey, double dDefault)
{
  	//does the key exist
	itIniData it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
		return dDefault;
    double d = dDefault;
	it->second.ToDouble(&d);
    return d;
}

void iniSection::WriteSection(std::ofstream* pof)
{
	//write the section name
	*pof	<<	"["
			<<	m_sSectionName.mb_str()
			<<	"]\n";
	//now write the data
	std::map<wxString,wxString>::iterator it = m_mSectionData.begin();
	while(it != m_mSectionData.end())
	{
		*pof	<<	it->first.mb_str()
				<<	"="
				<<	it->second.mb_str()
				<<	"\n";
		++it;
	}
}

void iniSection::SetValue(const wxString& sKey, const wxString& sValue)
{
	m_mSectionData[sKey] = sValue;
}

size_t iniSection::GetNumberOfEntries()
{
    return m_mSectionData.size();
}


void iniSection::RemoveKey(const wxString& sKey)
{
    m_mSectionData.erase(sKey);
}
