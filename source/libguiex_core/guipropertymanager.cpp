/** 
* @file guipropertymanager.cpp
* @brief used to manage property
* @author ken
* @date 2006-06-21
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guistring.h>
#include <libguiex_core/guiexception.h>


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_SINGLETON_IMPLEMENT_EX(CGUIPropertyManager);
	//------------------------------------------------------------------------------
	CGUIPropertyManager::CGUIPropertyManager()
	{
		RegisterPropertyType( ePropertyType_Bool, "bool" );
		RegisterPropertyType( ePropertyType_Int32, "int32" );
		RegisterPropertyType( ePropertyType_UInt32, "uint32" );
		RegisterPropertyType( ePropertyType_Int16, "int16" );
		RegisterPropertyType( ePropertyType_UInt16, "uint16" );
		RegisterPropertyType( ePropertyType_Real, "real" );

		RegisterPropertyType( ePropertyType_Size, "CGUISize" );
		RegisterPropertyType( ePropertyType_Rect, "CGUIRect" );
		RegisterPropertyType( ePropertyType_String, "CGUIString" );
		RegisterPropertyType( ePropertyType_Vector2, "CGUIVector2" );
		RegisterPropertyType( ePropertyType_Vector3, "CGUIVector3" );
		RegisterPropertyType( ePropertyType_Color, "CGUIColor" );
		RegisterPropertyType( ePropertyType_StringInfo, "CGUIStringInfo" );
		RegisterPropertyType( ePropertyType_Event, "CGUIEvent" );
		RegisterPropertyType( ePropertyType_Sound, "CGUISound" );
		RegisterPropertyType( ePropertyType_WidgetPosition, "CGUIWidgetPosition" );		
		RegisterPropertyType( ePropertyType_WidgetSize, "CGUIWidgetSize" );		

		RegisterPropertyType( ePropertyType_TextAlignmentHorz, "ETextAlignmentHorz" );
		RegisterPropertyType( ePropertyType_TextAlignmentVert, "ETextAlignmentVert" );
		RegisterPropertyType( ePropertyType_ImageOrientation, "EImageOrientation" );
		RegisterPropertyType( ePropertyType_ScreenValue, "EScreenValue" );
		RegisterPropertyType( ePropertyType_Interpolation, "EInterpolationType" );
		

		RegisterPropertyType( ePropertyType_Font, "CGUIFont" );		
		RegisterPropertyType( ePropertyType_Image, "CGUIImage" );
		RegisterPropertyType( ePropertyType_As, "CGUIAs" );
		RegisterPropertyType( ePropertyType_Animation, "CGUIAnimation" );

		RegisterPropertyType( ePropertyType_AsColor, "CGUIAsColor" );
		RegisterPropertyType( ePropertyType_AsScale, "CGUIAsScale" );
		RegisterPropertyType( ePropertyType_AsPosition, "CGUIAsPosition" );
		RegisterPropertyType( ePropertyType_AsRotation, "CGUIAsRotation" );
		RegisterPropertyType( ePropertyType_AsAlpha, "CGUIAsAlpha" );
		RegisterPropertyType( ePropertyType_AsContainer, "CGUIAsContainer" );
		RegisterPropertyType( ePropertyType_AsContainerItemInfo, "CGUIAsContainerItemInfo" );

		RegisterPropertyType( ePropertyType_Widget, "CGUIWidget" );
		RegisterPropertyType( ePropertyType_Set, "set" );
		RegisterPropertyType( ePropertyType_Script, "CGUIScript" );
		
		RegisterPropertyType( ePropertyType_FontDefine, "CGUIFontDefine" );
		RegisterPropertyType( ePropertyType_ImageDefine, "CGUIImageDefine" );
		RegisterPropertyType( ePropertyType_AnimationDefine, "CGUIAnimationDefine" );
		RegisterPropertyType( ePropertyType_AsDefine, "CGUIAsDefine" );
	}
	//------------------------------------------------------------------------------
	CGUIPropertyManager::~CGUIPropertyManager()
	{

	}
	//------------------------------------------------------------------------------
	void CGUIPropertyManager::RegisterPropertyType( uint32 uPropertyType, const CGUIString& rStringValue )
	{
		TMapPropertyType2String::iterator itorFindType2String = m_mapPropertyType2String.find( uPropertyType );
		if( itorFindType2String  != m_mapPropertyType2String.end() )
		{
			throw CGUIException( "[CGUIPropertyManager::RegisterPropertyType]: the property <%s:%d> has existed!",
				rStringValue.c_str(), 
				uPropertyType);
		}
		TMapString2PropertyType::iterator itorFindString2Type = m_mapString2PropertyType.find( rStringValue );
		if( itorFindString2Type  != m_mapString2PropertyType.end() )
		{
			throw CGUIException( "[CGUIPropertyManager::RegisterPropertyType]: the property <%s:%d> has existed!",
				rStringValue.c_str(), 
				uPropertyType);
		}
		m_mapPropertyType2String.insert( std::make_pair( uPropertyType, rStringValue ));
		m_mapString2PropertyType.insert( std::make_pair( rStringValue, uPropertyType ));
	}
	//------------------------------------------------------------------------------
	uint32 CGUIPropertyManager::StringToPropertyType( const CGUIString& rStringValue )
	{
		TMapString2PropertyType::iterator itorFindString2Type = m_mapString2PropertyType.find( rStringValue );
		if( itorFindString2Type == m_mapString2PropertyType.end() )
		{
			throw CGUIException( "[CGUIPropertyManager::StringToPropertyType]: failed to find property type by string value <%s>",
				rStringValue.c_str());
			return ePropertyType_Unknown;
		}
		return itorFindString2Type->second;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIPropertyManager::PropertyTypeToString( uint32 uPropertyType )
	{
		TMapPropertyType2String::iterator itorFindType2String = m_mapPropertyType2String.find( uPropertyType );
		if( itorFindType2String  != m_mapPropertyType2String.end() )
		{
			throw CGUIException( "[CGUIPropertyManager::PropertyTypeToString]: failed to find string value by property type <%d>",
				uPropertyType);
		}
		return itorFindType2String->second;
	}
	//------------------------------------------------------------------------------
	void CGUIPropertyManager::RegisterSet( const CGUIString& rSetName, const CGUIProperty& rPropertySet )
	{
		if( m_mapPropertySet.find(rSetName) != m_mapPropertySet.end())
		{
			throw CGUIException( "[CGUIPropertyManager::RegisterSet]: the property <%s> has existed!",rSetName.c_str());
		}
		m_mapPropertySet.insert( std::make_pair(rSetName,rPropertySet) );
	}
	//------------------------------------------------------------------------------
	void CGUIPropertyManager::UnregisterSet( const CGUIString& rSetName )
	{
		TMapPropertySet::iterator itor = m_mapPropertySet.find(rSetName);
		if( itor == m_mapPropertySet.end())
		{
			throw CGUIException( "[CGUIPropertyManager::UnregisterSet]: the property <%s> doesn't existed!",rSetName.c_str());
		}
		m_mapPropertySet.erase( itor );
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUIPropertyManager::GetSet(const CGUIString& rSetName ) const
	{
		TMapPropertySet::const_iterator itor = m_mapPropertySet.find(rSetName);
		if( itor == m_mapPropertySet.end())
		{
			throw CGUIException( "[CGUIPropertyManager::GetSet]: the property <%s> doesn't existed!",rSetName.c_str());
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
