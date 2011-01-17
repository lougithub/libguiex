/** 
* @file guipropertyconvertor.cpp
* @brief convert property from other value or
* convert other value to property
* @author ken
* @date 2007-09-21
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guipropertyconvertor.h>


//============================================================================//
// define
//============================================================================// 
namespace guiex
{

#if GUI_DEBUG
#	define GUI_PROPERTY_TEST( property, type )	\
	if( (property).GetType() != type )	\
	{									\
		throw CGUIException(			\
		"[GUI_PROPERTY_TEST: invalid property type, name=[%s] type=[%s] value=[%s] ",\
		(property).GetName().c_str(), (property).GetTypeAsString().c_str(), (property).GetValue().c_str());\
		return;\
	}
#else	//#if GUI_DEBUG
#	define GUI_PROPERTY_TEST( property, type )
#endif	//#if GUI_DEBUG

}//namespace guiex
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	static void AddOrReplaceProperty( CGUIProperty& rParentProperty, const CGUIProperty& rNewProperty )
	{
		CGUIProperty* pOldProp = rParentProperty.GetProperty( rNewProperty.GetName(), rNewProperty.GetType());
		if( pOldProp )
		{
			*pOldProp = rNewProperty;
		}
		else
		{
			rParentProperty.AddProperty( rNewProperty );
		}
	}

	//------------------------------------------------------------------------------
	//CGUIWidgetPosition
	template<  >
	void PropertyToValue<CGUIWidgetPosition>( const CGUIProperty& rProperty, CGUIWidgetPosition& rValue)
	{
		/*
		<property "position" type="CGUIWidgetPosition" category="Default">
			<property name="type" type="EScreenValue" value="eScreenValue_Pixel" />
			<property name="value" type="CGUIVector2" value="0,0" />
		</property>
		*/
		GUI_PROPERTY_TEST(rProperty, ePropertyType_WidgetPosition);

		const CGUIProperty* pValueProperty = rProperty.GetProperty( "value", "CGUIVector2" );
		if( !pValueProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIWidgetPosition]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST(*pValueProperty, ePropertyType_Vector2);
			PropertyToValue( *pValueProperty, rValue.m_aValue );
		}

		const CGUIProperty* pTypeProperty = rProperty.GetProperty( "type", "EScreenValue" );
		if( !pTypeProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIWidgetPosition]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST(*pTypeProperty, ePropertyType_ScreenValue);
			PropertyToValue( *pTypeProperty, rValue.m_eType);
		}
	}

	template<  >
	void ValueToProperty<CGUIWidgetPosition>( const CGUIWidgetPosition& rValue, CGUIProperty& rProperty )
	{
		CGUIProperty aValueProperty("value", "CGUIVector2");
		ValueToProperty( rValue.m_aValue, aValueProperty );
		AddOrReplaceProperty( rProperty, aValueProperty );

		CGUIProperty aTypeProperty( "type", "EScreenValue" );
		ValueToProperty( rValue.m_eType, aTypeProperty );
		AddOrReplaceProperty( rProperty, aTypeProperty );
	}
	//------------------------------------------------------------------------------
	//CGUIWidgetSize
	template<  >
	void PropertyToValue<CGUIWidgetSize>( const CGUIProperty& rProperty, CGUIWidgetSize& rValue)
	{
		/*
		<property "size" type="CGUIWidgetSize">
			<property name="type" type="EScreenValue" value="eScreenValue_Pixel" />
			<property name="value" type="CGUISize" value="0,0" />
		</property>
		*/
		GUI_PROPERTY_TEST( rProperty, ePropertyType_WidgetSize);
		const CGUIProperty* pValueProperty = rProperty.GetProperty( "value", "CGUISize" );
		if( !pValueProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIWidgetSize]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pValueProperty, ePropertyType_Size);
			PropertyToValue( *pValueProperty, rValue.m_aValue );
		}

		const CGUIProperty* pTypeProperty = rProperty.GetProperty( "type", "EScreenValue");
		if( !pTypeProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIWidgetSize]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pTypeProperty, ePropertyType_ScreenValue);
			PropertyToValue( *pTypeProperty, rValue.m_eType);
		}
	}

	template<  >
	void ValueToProperty<CGUIWidgetSize>( const CGUIWidgetSize& rValue, CGUIProperty& rProperty )
	{
		CGUIProperty aValueProperty( "value", "CGUISize" );
		ValueToProperty( rValue.m_aValue, aValueProperty );
		AddOrReplaceProperty( rProperty, aValueProperty );

		CGUIProperty aTypeProperty( "type", "EScreenValue" );
		ValueToProperty( rValue.m_eType, aTypeProperty );
		AddOrReplaceProperty( rProperty, aTypeProperty );
	}
	//------------------------------------------------------------------------------
	//CGUIStringInfo
	template<  >
	void PropertyToValue<CGUIStringInfo>( const CGUIProperty& rProperty, CGUIStringInfo& rValue)
	{
		/*
		<property name="text_info" type="CGUIStringInfo">
			<property name="size"		type="uint16" value="16"/>
			<property name="index"		type="int16" value="0"/>
			<property name="color" 		type="CGUIColor" value="1.0,1.0,1.0,1.0"/>
		</property>
		*/
		GUI_PROPERTY_TEST( rProperty, ePropertyType_StringInfo);
		const CGUIProperty* pSizeProperty = rProperty.GetProperty( "size", "uint16" );
		if( !pSizeProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIStringInfo]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pSizeProperty, ePropertyType_UInt16);
			PropertyToValue( *pSizeProperty, rValue.m_nFontSize );
		}

		const CGUIProperty* pIndexProperty = rProperty.GetProperty( "index", "int16" );
		if( !pIndexProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIStringInfo]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pIndexProperty, ePropertyType_Int16);
			PropertyToValue( *pIndexProperty, rValue.m_nFontIdx );
		}

		const CGUIProperty* pColorProperty = rProperty.GetProperty( "color", "CGUIColor" );
		if( !pColorProperty )
		{
			throw CGUIException(
				"[PropertyToValue[CGUIStringInfo]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pColorProperty, ePropertyType_Color );
			PropertyToValue( *pColorProperty, rValue.m_aColor );
		}
	}

	template<  >
	void ValueToProperty<CGUIStringInfo>( const CGUIStringInfo& rValue, CGUIProperty& rProperty )
	{
		CGUIProperty aSizeProperty ( "size", "uint16" );
		ValueToProperty( rValue.m_nFontSize, aSizeProperty );
		AddOrReplaceProperty( rProperty, aSizeProperty );

		CGUIProperty aIndexProperty ( "index", "int16" );
		ValueToProperty( rValue.m_nFontIdx, aIndexProperty );
		AddOrReplaceProperty( rProperty, aIndexProperty );

		CGUIProperty aColorProperty ( "color", "CGUIColor" );
		ValueToProperty( rValue.m_aColor, aColorProperty );
		AddOrReplaceProperty( rProperty, aColorProperty );
	}
	//------------------------------------------------------------------------------
	//SGUIBlendFunc
	template<  >
	void PropertyToValue<SGUIBlendFunc>( const CGUIProperty& rProperty, SGUIBlendFunc& rValue)
	{
		/*
		<property name="blendFunc" type="SGUIBlendFunc"/>
			<property name="src" type="EBlendFunc" value="eBlendFunc_ONE"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
		</property>
		*/
		GUI_PROPERTY_TEST( rProperty, ePropertyType_SBlendFunc);

		const CGUIProperty* pSrcProperty = rProperty.GetProperty( "src", "EBlendFunc" );
		if( !pSrcProperty )
		{
			throw CGUIException(
				"[PropertyToValue[SGUIBlendFunc]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pSrcProperty, ePropertyType_BlendFunc);
			PropertyToValue( *pSrcProperty, rValue.src );
		}

		const CGUIProperty* pDstProperty = rProperty.GetProperty( "dst", "EBlendFunc" );
		if( !pDstProperty )
		{
			throw CGUIException(
				"[PropertyToValue[SGUIBlendFunc]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pDstProperty, ePropertyType_BlendFunc);
			PropertyToValue( *pDstProperty, rValue.dst );
		}
	}

	template<  >
	void ValueToProperty<SGUIBlendFunc>( const SGUIBlendFunc& rValue, CGUIProperty& rProperty )
	{
		CGUIProperty aSrcProperty ( "src", "EBlendFunc" );
		ValueToProperty( rValue.src, aSrcProperty );
		AddOrReplaceProperty( rProperty, aSrcProperty );

		CGUIProperty aDstProperty ( "dst", "EBlendFunc" );
		ValueToProperty( rValue.dst, aDstProperty );
		AddOrReplaceProperty( rProperty, aDstProperty );
	}
	//------------------------------------------------------------------------------
}
