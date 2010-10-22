/** 
* @file guipropertyable.h
* @brief create a base class for all object which want to be propertyable
* @author ken
* @date 2010-10-22
*/


#ifndef __GUI_PROPERTYABLE_H_20101022__
#define __GUI_PROPERTYABLE_H_20101022__

//============================================================================//
// include
//============================================================================// 
#include "guiconfig.h"
#include "guibase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIProperty;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIPropertyable
	* @brief base class of propertyable object, such as size, rect and so on.
	*/
	class GUIEXPORT CGUIPropertyable
	{
	public:
		CGUIPropertyable( uint32 uPropertyType );
		virtual ~CGUIPropertyable();

		virtual CGUIProperty* SaveToProperty( const CGUIString& rPropertyName ) const = 0;
		virtual void LoadFromProperty( const CGUIProperty& rProperty ) = 0;

		uint32 GetPropertyType();

	private:
		uint32 uPropertyType;
	};
}

#endif //__GUI_PROPERTYABLE_H_20101022__
