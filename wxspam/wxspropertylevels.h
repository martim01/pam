#pragma once

#include <map>
#include "properties/wxscustomeditorproperty.h"
#include "dlgaddlines.h"

class wxsPropertyLevels: public wxsCustomEditorProperty
{
	public:

        /** \brief Ctor
         *  \param PGName       name of property in Property Grid
         *  \param DataName     name of property in data stuctures
         *  \param DataSubName  name of name applied for each array element
         *  \param Offset       offset of value (returned from wxsOFFSET macro)
         */
		wxsPropertyLevels(long nLevelOffset, const wxString& PGName,const wxString& DataName, int Priority=100);

		/** \brief Returning type name */
		virtual const wxString GetTypeName() { return wxT("wxArrayString"); }

        /** \brief Showing editor for this property */
        virtual bool ShowEditor(wxsPropertyContainer* Object);

    protected:

        virtual bool XmlRead(wxsPropertyContainer* pObject,TiXmlElement* pElement);
        virtual bool XmlWrite(wxsPropertyContainer* pObject,TiXmlElement* pElement);
        virtual bool PropStreamRead(wxsPropertyContainer* pObject,wxsPropertyStream* pStream);
        virtual bool PropStreamWrite(wxsPropertyContainer* pObject,wxsPropertyStream* pStream);
        virtual wxString GetStr(wxsPropertyContainer* pObject);

        void AppendProperty(TiXmlElement* pParent, wxString sProperty, wxString sValue);
	private:
        long m_nLevelOffset;
};

/** \addtogroup properties_macros
 *  \{ */

/** \brief Macro automatically declaring wxArrayString property
 *  \param ClassName name of class holding this property
 *  \param VarName name of variable inside class
 *  \param PGName name used in property grid
 *  \param DataName name used in Xml / Data Streams
 *  \param DataSubName name for subelement used in Xml / Data Streams
 */
 #define WXS_LEVELS(ClassName,Levels, PGName,DataName) \
    { static wxsPropertyLevels _Property(wxsOFFSET(ClassName,Levels), PGName, DataName, 100); \
      Property(_Property); }


/** \brief Macro automatically declaring wxArrayString property with custom priority
 *  \param ClassName name of class holding this property
 *  \param VarName name of variable inside class
 *  \param PGName name used in property grid
 *  \param DataName name used in Xml / Data Streams
 *  \param DataSubName name for subelement used in Xml / Data Streams
 *  \param Priority priority of property
 */
#define WXS_LEVELS_P(ClassName,Levels,PGName,DataName, Priority) \
    { static wxsPropertyLevels _Property(wxsOFFSET(ClassName,Levels), PGName,DataName, Priority); \
      Property(_Property); }

/** \} */




