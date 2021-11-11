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
#pragma once

#include "dlldefine.h"
#include <map>
#include <wx/string.h>
#include <sstream>

//#include <wx\dlimpexp.h>


struct ci_less
{
    bool operator() (const wxString& s1, const wxString& s2) const
    {
        return (s1.Upper() < s2.Upper());
    }
};


/**
*	@brief class that contains data loaded from a section of an ini file
*	@author Matthew Martin
*/

typedef std::map<wxString,wxString, ci_less> mapIniData;
typedef std::map<wxString,wxString, ci_less>::iterator itIniData;

class PAMBASE_IMPEXPORT iniSection{

public:
        /** Constructor
        *	@param sSection the section name
        **/
        iniSection(const wxString& sSection, unsigned long nLine);

        /** Destructor
        **/
        ~iniSection();


	/** @brief Get and iterator pointing to the beginning of the data
	*	@return <i>itIniData</i> the iterator
	**/
        mapIniData::const_iterator GetDataBegin() const;

        /** @brief Get and iterator pointing to the end of the data
        *	@return <i>itIniData</i> the iterator
        **/
        mapIniData::const_iterator GetDataEnd() const;

        const mapIniData& GetData() const { return m_mSectionData; }

        /** @brief Get the name of the section
        *	@return <i>wxString</i> the section name
        **/
        const wxString& GetSectionName() const
        {	return m_sSectionName;	}

        /** @brief Get the string value of data with key sKey
        *	@param sKey the key name of the data
        *	@param sDefault the value to return of the data does not exist
        *	@return <i>wxString</i> the value of the data
        **/
        const wxString& GetString(const wxString& sKey, const wxString& sDefault) const;

        /** @brief Get the int value of data with key sKey
        *	@param sKey the key name of the data
        *	@param nDefault the value to return of the data does not exist
        *	@return <i>int</i> the value of the data
        **/
        int GetInt(const wxString& sKey, int nDefault) const;

        /** @brief Get the double value of data with key sKey
        *	@param sKey the key name of the data
        *	@param dDefault the value to return of the data does not exist
        *	@return <i>double</i> the value of the data
        **/
        double GetDouble(const wxString& sKey, double dDefault) const;

        /** @brief Get a count of the number of bits of data
        *	@return <i>size_t</> thee count
        **/
        size_t GetNumberOfEntries() const;

        /** @brief Sets the key,value pair in m_mData
        *	@param sKey the key
        *	@param sValue the value
        **/
        void SetValue(const wxString& sKey, wxString sValue);

        /** @brief Returns a const_iterator pointing to the key,data pair of the given key
        *   @param sKey the key to search fro
        *   @return <i>map<wxString,wxString>::const_iterator
        **/
        mapIniData::const_iterator FindData(const wxString& sKey) const;

        unsigned long GetLine() const { return m_nLine; }

        void RemoveKey(const wxString& sKey);

        void WriteSection(std::ofstream* pof);

	protected:
        mapIniData m_mSectionData;	///< map containing key, value pairs */
        wxString m_sSectionName;					///< the section name */
        unsigned long m_nLine;







};

