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
	* @brief base class of "propertyable" object, such as size, rect and so on.
	*/
	class GUIEXPORT CGUIPropertyable
	{
	public:
		/** 
		* @brief default constructor
		* @param uPropertyType type of this "propertyable" object
		*/
		CGUIPropertyable( );
		virtual ~CGUIPropertyable();

		virtual void SaveToProperty( CGUIProperty& rProperty ) const = 0;
		virtual void LoadFromProperty( const CGUIProperty& rProperty ) = 0;
	};
	
}	//namespace guiex

#endif //__GUI_PROPERTYABLE_H_20101022__
