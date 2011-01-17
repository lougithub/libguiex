/** 
* @file guipropertyconvertor.h
* @brief convert property from other value or
* convert other value to property
* @author ken
* @date 2007-09-21
*/

#ifndef __GUI_PROPERTY_CONVERTOR_20070921_H__
#define __GUI_PROPERTY_CONVERTOR_20070921_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiexception.h"
#include "guiproperty.h"
#include "guipropertymanager.h"
#include "guistringconvertor.h"
#include "guivector2.h"
#include "guisize.h"
#include "guiwidget.h"
#include "guirendertype.h"

//============================================================================//
// declare
//============================================================================// 



//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	template< class ValueType >
	void PropertyToValue( const CGUIProperty& rProperty, ValueType& rValue)
	{
		StringToValue( rProperty.GetValue(), rValue );
	}

	template< class ValueType >
	void ValueToProperty( const ValueType& rValue, CGUIProperty& rProperty )
	{
		CGUIString strValue;
		ValueToString( rValue, strValue );
		rProperty.SetValue( strValue );
	}



	//------------------------------------------------------------------------------
	//CGUIWidgetPosition
	template<  >
	void PropertyToValue<CGUIWidgetPosition>( const CGUIProperty& rProperty, CGUIWidgetPosition& rValue);
	template<  >
	void ValueToProperty<CGUIWidgetPosition>( const CGUIWidgetPosition& rValue, CGUIProperty& rProperty );
	//------------------------------------------------------------------------------
	//CGUIWidgetSize
	template<  >
	void PropertyToValue<CGUIWidgetSize>( const CGUIProperty& rProperty, CGUIWidgetSize& rValue);
	template<  >
	void ValueToProperty<CGUIWidgetSize>( const CGUIWidgetSize& rValue, CGUIProperty& rProperty );
	//------------------------------------------------------------------------------
	//CGUIStringInfo
	template<  >
	void PropertyToValue<CGUIStringInfo>( const CGUIProperty& rProperty, CGUIStringInfo& rValue);
	template<  >
	void ValueToProperty<CGUIStringInfo>( const CGUIStringInfo& rValue, CGUIProperty& rProperty );
	//------------------------------------------------------------------------------
	//SGUIBlendFunc
	template<  >
	void PropertyToValue<SGUIBlendFunc>( const CGUIProperty& rProperty, SGUIBlendFunc& rValue);
	template<  >
	void ValueToProperty<SGUIBlendFunc>( const SGUIBlendFunc& rValue, CGUIProperty& rProperty );
	//------------------------------------------------------------------------------
}


#endif //__GUI_PROPERTY_CONVERTOR_20070921_H__
