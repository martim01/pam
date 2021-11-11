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


iniSection::iniSection(const wxString& sSection, unsigned long nLine)
: m_sSectionName(sSection),
m_nLine(nLine)
{

}


iniSection::~iniSection()
{
}



/*!
    \fn iniSection::GetDataBegin()
 */
mapIniData::const_iterator iniSection::GetDataBegin() const
{
    return m_mSectionData.begin();
}

/*!
    \fn iniSection::GetDataEnd()
 */
mapIniData::const_iterator iniSection::GetDataEnd() const
{
    return m_mSectionData.end();
}


/*!
    \fn iniSection::GetString(const wxString& sKey, const wxString& sDefault)
 */
const wxString& iniSection::GetString(const wxString& sKey, const wxString& sDefault) const
{
  	//does the key exist
  	auto it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
		return sDefault;
	return it->second;
}

/*!
    \fn iniSection::GetInt(const wxString& sKey, int nDefault)
 */
int iniSection::GetInt(const wxString& sKey, int nDefault) const
{
  	//does the key exist
  	auto it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
		return nDefault;
    long n = nDefault;

	it->second.ToLong(&n);
	return n;
}

/*!
    \fn iniSection::GetString(const wxString& sKey, double dDefault)
 */
double iniSection::GetDouble(const wxString& sKey, double dDefault) const
{
  	//does the key exist
	auto it = m_mSectionData.find(sKey);
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

void iniSection::SetValue(const wxString& sKey, wxString sValue)
{
    sValue.Trim();
    auto insData = m_mSectionData.insert(std::make_pair(sKey, sValue));
    if(insData.second == false)
    {
        insData.first->second = sValue;
        //m_mSectionData[sKey] = sValue;
    }

}

size_t iniSection::GetNumberOfEntries() const
{
    return m_mSectionData.size();
}

void iniSection::RemoveKey(const wxString& sKey)
{
    m_mSectionData.erase(sKey);
}
