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


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIProperty;

	/**
	 * @brief property type used in gui system
	 */
	enum EPropertyType
	{
		ePropertyType_Size = 0,
		ePropertyType_Rect,


		ePropertyType_UserDefine = 1000,
	};
}//namespace guiex


//============================================================================//
// class
//============================================================================// 

namespace guiex
{

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

		void RegisterPropertyType( uint32 uPropertyType, const CGUIString& rStringValue );
		uint32 StringToPropertyType( const CGUIString& rStringValue );
		const CGUIString& PropertyTypeToString( uint32 uPropertyType );

	protected:
		CGUIPropertyManager();
		~CGUIPropertyManager();

	protected:
		typedef std::map<CGUIString, CGUIPropertySet>	TMapPropertySet;
		TMapPropertySet		m_mapPropertySet;			/// set of property

		typedef std::map<uint32, CGUIString>			TMapPropertyType2String;
		typedef std::map<CGUIString, uint32>			TMapString2PropertyType;
		TMapPropertyType2String	m_mapPropertyType2String;
		TMapString2PropertyType	m_mapString2PropertyType;

		//for use singleton
		GUI_SINGLETON_DECLARE_EX(CGUIPropertyManager);
	};


	//for use singleton
	//GUI_SINGLETON_IMPLEMENT(CGUIPropertyManager, GUI_PROPERTY_MANAGER);


}//namespace guiex

#endif //__GUI_PROPERTYMANAGER_20060621_H__
