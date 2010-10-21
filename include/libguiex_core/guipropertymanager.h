/** 
 * @file guipropertymanager.h
 * @brief used to manage property
 * @author ken
 * @date 2006-06-21
 */

#ifndef __GUI_PROPERTYMANAGER_20060621_H__
#define __GUI_PROPERTYMANAGER_20060621_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisingleton.h"
#include "guistring.h"
#include "guipropertyset.h"
#include <map>

namespace guiex
{
//============================================================================//
// declare
//============================================================================// 
class CGUIProperty;
}//namespace guiex

namespace guiex
{
//============================================================================//
// class
//============================================================================// 

/**
 * @class CGUIPropertyManager
 * @brief manager of property,it is a singleton class
 * @exception CGUIException
 */
class GUIEXPORT CGUIPropertyManager
{
public:
	/**
	 * @brief create a property
	 */
	CGUIProperty*	CreateProperty();

	/**
	 * @brief create a property
	 */
	CGUIProperty*	CGUIPropertyManager::CreateProperty(	
	const CGUIString& rName, 
	const CGUIString& rType);

	/**
	 * @brief create a property
	 */
	CGUIProperty*	CreateProperty(
		const CGUIString& rName, 
		const CGUIString& rType,
		const CGUIString& rValue);

	/**
	 * @brief destroy a property
	 */
	void						DestroyProperty(CGUIProperty*);

	/**
	 * @brief register a property set
	 */
	void						RegisterSet( const CGUIString& rSetName, const CGUIPropertySet& rPropertySet );

	/**
	 * @brief unregister a property set
	 */
	void						UnregisterSet( const CGUIString& rSetName );

	/**
	 * @brief get a property set
	 */
	const CGUIPropertySet&	GetSet(const CGUIString& rSetName ) const;

protected:
	CGUIPropertyManager();
	~CGUIPropertyManager();

protected:
	typedef std::map<CGUIString, CGUIPropertySet>	TMapPropertySet;
	TMapPropertySet		m_mapPropertySet;			/// set of property

	//for use singleton
	GUI_SINGLETON_DECLARE_EX(CGUIPropertyManager);
};


//for use singleton
//GUI_SINGLETON_IMPLEMENT(CGUIPropertyManager, GUI_PROPERTY_MANAGER);


}//namespace guiex

#endif //__GUI_PROPERTYMANAGER_20060621_H__
