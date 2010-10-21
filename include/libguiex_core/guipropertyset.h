/** 
 * @file guipropertyset.h
 * @brief the property set, contain the config of the widget
 * @author ken
 * @date 2006-09-12
 */

#ifndef __GUI_PROPERTYSET_20060912_H__
#define __GUI_PROPERTYSET_20060912_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include <vector>

namespace guiex
{
//============================================================================//
// declare
//============================================================================// 
class CGUIProperty;
}

namespace guiex
{
//============================================================================//
// class
//============================================================================// 

/**
 * @class CGUIPropertySet
 * @brief a property set
  * @exception CGUIException
 */
class GUIEXPORT CGUIPropertySet
{
public:
	/**
	 * @brief constructor
	 */
	CGUIPropertySet();

	/**
	 * @brief destructor
	 */
	~CGUIPropertySet();

	/// copy constructor
	CGUIPropertySet(const CGUIPropertySet& rPropertySet );

	/// assign operator
	void operator=(const CGUIPropertySet& rPropertySet );

	/// get property size
	uint32	GetSize() const;

	/// clear all property
	void			Clear();

	/// get property by index
	const CGUIProperty*	GetProperty(uint32 nIdx) const;

	/// get property by index
	CGUIProperty*	GetProperty(uint32 nIdx);

	/// get property by name
	const CGUIProperty*	GetProperty(const CGUIString& rName) const;

	/// get property by name
	CGUIProperty*	GetProperty(const CGUIString& rName);


	/// whether this set contains the property which has this name
	bool		HasProperty(const CGUIString& rName) const;

	/// add a property to list
	void		AddProperty(CGUIProperty* pProperty);

	/// add a property set to list
	void		AddPropertySet(const CGUIPropertySet& rPropertySet);

	/// remove a property from list
	void		RemoveProperty(const CGUIProperty* pProperty);

protected:

protected:
	typedef std::vector<CGUIProperty*>	TSetProperty;
	TSetProperty	m_setProperty;	
};

}//namespace guiex

#endif //__GUI_PROPERTYSET_20060912_H__
