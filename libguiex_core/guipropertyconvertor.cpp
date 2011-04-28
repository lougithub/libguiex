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
#include "guipropertyconvertor.h"


//============================================================================//
// define
//============================================================================// 
namespace guiex
{

#if GUI_DEBUG
#	define GUI_PROPERTY_TEST( property, type )	\
	if( (property).GetType() != type )	\
	{									\
		CGUIException::ThrowException(			\
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
			CGUIException::ThrowException(
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
			CGUIException::ThrowException(
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
			CGUIException::ThrowException(
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
			CGUIException::ThrowException(
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
	//CGUIStringRenderInfo
	template<  >
	void PropertyToValue<CGUIStringRenderInfo>( const CGUIProperty& rProperty, CGUIStringRenderInfo& rValue)
	{
		/*
		<property name="text_info" type="CGUIStringRenderInfo">
			<property name="scale" type="real" value="1"/>
			<property name="font" type="CGUIFont" value="0"/>
			<property name="color" type="CGUIColor" value="1.0,1.0,1.0,1.0"/>
		</property>
		*/
		GUI_PROPERTY_TEST( rProperty, ePropertyType_StringRenderInfo);
		const CGUIProperty* pScaleProperty = rProperty.GetProperty( "scale", "real" );
		if( !pScaleProperty )
		{
			CGUIException::ThrowException(
				"[PropertyToValue[CGUIStringRenderInfo]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pScaleProperty, ePropertyType_Real);
			PropertyToValue( *pScaleProperty, rValue.m_fFontScale );
		}

		const CGUIProperty* pFontProperty = rProperty.GetProperty( "font", "CGUIFont" );
		if( !pFontProperty )
		{
			CGUIException::ThrowException(
				"[PropertyToValue[CGUIStringRenderInfo]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pFontProperty, ePropertyType_Font);
			PropertyToValue( *pFontProperty, rValue.m_uFontID );
		}

		const CGUIProperty* pColorProperty = rProperty.GetProperty( "color", "CGUIColor" );
		if( !pColorProperty )
		{
			CGUIException::ThrowException(
				"[PropertyToValue[CGUIStringRenderInfo]]: invalid property format" );
			return;
		}
		else
		{
			GUI_PROPERTY_TEST( *pColorProperty, ePropertyType_Color );
			PropertyToValue( *pColorProperty, rValue.m_aColor );
		}
	}

	template<  >
	void ValueToProperty<CGUIStringRenderInfo>( const CGUIStringRenderInfo& rValue, CGUIProperty& rProperty )
	{
		CGUIProperty aScaleProperty ( "scale", "real" );
		ValueToProperty( rValue.m_fFontScale, aScaleProperty );
		AddOrReplaceProperty( rProperty, aScaleProperty );

		CGUIProperty aIndexProperty ( "font", "CGUIFont" );
		ValueToProperty( rValue.m_uFontID, aIndexProperty );
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
			CGUIException::ThrowException(
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
			CGUIException::ThrowException(
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
