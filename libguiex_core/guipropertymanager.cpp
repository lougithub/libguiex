/** 
* @file guipropertymanager.cpp
* @brief used to manage property
* @author ken
* @date 2006-06-21
*/

//============================================================================//
// include
//============================================================================// 
#include "guipropertymanager.h"
#include "guiproperty.h"
#include "guistring.h"
#include "guiexception.h"


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIPropertyManager * CGUIPropertyManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIPropertyManager::CGUIPropertyManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIPropertyManager::CGUIPropertyManager]:instance has been created" ); 
		m_pSingleton = this; 

		RegisterPropertyType( ePropertyType_Bool, "bool" );
		RegisterPropertyType( ePropertyType_Int32, "int32" );
		RegisterPropertyType( ePropertyType_UInt32, "uint32" );
		RegisterPropertyType( ePropertyType_Int16, "int16" );
		RegisterPropertyType( ePropertyType_UInt16, "uint16" );
		RegisterPropertyType( ePropertyType_Real, "real" );

		RegisterPropertyType( ePropertyType_Array, "array" );
		RegisterPropertyType( ePropertyType_Group, "group" );
		
		RegisterPropertyType( ePropertyType_Size, "CGUISize" );
		RegisterPropertyType( ePropertyType_IntSize, "CGUIIntSize" );
		RegisterPropertyType( ePropertyType_Rect, "CGUIRect" );
		RegisterPropertyType( ePropertyType_String, "CGUIString" );
		RegisterPropertyType( ePropertyType_LocalizedString, "CGUILocalizedString" );
		RegisterPropertyType( ePropertyType_Vector2, "CGUIVector2" );
		RegisterPropertyType( ePropertyType_Vector3, "CGUIVector3" );
		RegisterPropertyType( ePropertyType_Color, "CGUIColor" );
		RegisterPropertyType( ePropertyType_StringRenderInfo, "CGUIStringRenderInfo" );
		RegisterPropertyType( ePropertyType_SBlendFunc, "SGUIBlendFunc" );
		RegisterPropertyType( ePropertyType_Event, "CGUIEvent" );
		RegisterPropertyType( ePropertyType_WidgetPosition, "CGUIWidgetPosition" );		
		RegisterPropertyType( ePropertyType_WidgetSize, "CGUIWidgetSize" );		
		RegisterPropertyType( ePropertyType_Particle2D, "CGUIParticle2D" );		
		RegisterPropertyType( ePropertyType_TiledMap, "CGUITiledMap" );		
		

		RegisterPropertyType( ePropertyType_TextAlignmentHorz, "ETextAlignmentHorz" );
		RegisterPropertyType( ePropertyType_TextAlignmentVert, "ETextAlignmentVert" );
		RegisterPropertyType( ePropertyType_ImageOrientation, "EImageOrientation" );
		RegisterPropertyType( ePropertyType_ScreenValue, "EScreenValue" );
		RegisterPropertyType( ePropertyType_Interpolation, "EInterpolationType" );
		RegisterPropertyType( ePropertyType_Particle2DSystemMode, "EParticle2DSystemMode" );
		RegisterPropertyType( ePropertyType_BlendFunc, "EBlendFunc" );
		RegisterPropertyType( ePropertyType_Orientation, "EOrientation" );
		

		RegisterPropertyType( ePropertyType_Font, "CGUIFont" );		
		RegisterPropertyType( ePropertyType_Image, "CGUIImage" );
		RegisterPropertyType( ePropertyType_Sound, "CGUISound" );
		RegisterPropertyType( ePropertyType_As, "CGUIAs" );
		RegisterPropertyType( ePropertyType_Animation, "CGUIAnimation" );

		RegisterPropertyType( ePropertyType_AsWidgetColor, "CGUIAsWidgetColor" );
		RegisterPropertyType( ePropertyType_AsWidgetScale, "CGUIAsWidgetScale" );
		RegisterPropertyType( ePropertyType_AsWidgetPosition, "CGUIAsWidgetPosition" );
		RegisterPropertyType( ePropertyType_AsWidgetRotation, "CGUIAsWidgetRotation" );
		RegisterPropertyType( ePropertyType_AsWidgetAlpha, "CGUIAsWidgetAlpha" );
		RegisterPropertyType( ePropertyType_AsWidgetSize, "CGUIAsWidgetSize" );
		RegisterPropertyType( ePropertyType_AsContainer, "CGUIAsContainer" );

		RegisterPropertyType( ePropertyType_Widget, "CGUIWidget" );
		RegisterPropertyType( ePropertyType_Set, "set" );

		RegisterPropertyType( ePropertyType_FontDefine, "CGUIFontDefine" );
		RegisterPropertyType( ePropertyType_SoundDefine, "CGUISoundDefine" );
		RegisterPropertyType( ePropertyType_MusicDefine, "CGUIMusicDefine" );
		RegisterPropertyType( ePropertyType_ImageDefine, "CGUIImageDefine" );
		RegisterPropertyType( ePropertyType_AnimationDefine, "CGUIAnimationDefine" );
		RegisterPropertyType( ePropertyType_AsDefine, "CGUIAsDefine" );
		RegisterPropertyType( ePropertyType_WidgetDefine, "CGUIWidgetDefine" );
		RegisterPropertyType( ePropertyType_Particle2DDefine, "CGUIParticle2DDefine" );
		RegisterPropertyType( ePropertyType_TiledMapDefine, "CGUITiledMapDefine" );
		
		//scene related property
		RegisterPropertyType( ePropertyType_Scene_Name, "SceneName" );	
		RegisterPropertyType( ePropertyType_Scene_Dependency, "Dependency" );
		RegisterPropertyType( ePropertyType_Scene_WidgetFile, "WidgetFile" );
		RegisterPropertyType( ePropertyType_Scene_ScriptFile, "ScriptFile" );
		RegisterPropertyType( ePropertyType_Scene_ResourceFile, "ResourceFile" );
		RegisterPropertyType( ePropertyType_Scene_LocalizationFile, "LocalizationFile" );
	}
	//------------------------------------------------------------------------------
	CGUIPropertyManager::~CGUIPropertyManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIPropertyManager* CGUIPropertyManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	void CGUIPropertyManager::RegisterPropertyType( uint32 uPropertyType, const CGUIString& rStringValue )
	{
		TMapPropertyType2String::iterator itorFindType2String = m_mapPropertyType2String.find( uPropertyType );
		if( itorFindType2String  != m_mapPropertyType2String.end() )
		{
			GUI_THROW( GUI_FORMAT( "[CGUIPropertyManager::RegisterPropertyType]: the property <%s:%d> has existed!",
				rStringValue.c_str(), 
				uPropertyType));
		}
		TMapString2PropertyType::iterator itorFindString2Type = m_mapString2PropertyType.find( rStringValue );
		if( itorFindString2Type  != m_mapString2PropertyType.end() )
		{
			GUI_THROW( GUI_FORMAT( "[CGUIPropertyManager::RegisterPropertyType]: the property <%s:%d> has existed!",
				rStringValue.c_str(), 
				uPropertyType));
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
			GUI_THROW( GUI_FORMAT( "[CGUIPropertyManager::StringToPropertyType]: failed to find property type by string value <%s>",
				rStringValue.c_str()));
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
			GUI_THROW( GUI_FORMAT( 
				"[CGUIPropertyManager::PropertyTypeToString]: failed to find string value by property type <%d>",
				uPropertyType));
		}
		return itorFindType2String->second;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register a property set
	*/
	void CGUIPropertyManager::RegisterSet( const CGUIString& rSetName, const CGUIProperty& rPropertySet )
	{
		if( m_mapPropertySet.find(rSetName) != m_mapPropertySet.end())
		{
			GUI_THROW( GUI_FORMAT( "[CGUIPropertyManager::RegisterSet]: the property <%s> has existed!",rSetName.c_str()));
		}
		m_mapPropertySet.insert( std::make_pair(rSetName,rPropertySet) );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister a property set
	*/
	void CGUIPropertyManager::UnregisterSet( const CGUIString& rSetName )
	{
		TMapPropertySet::iterator itor = m_mapPropertySet.find(rSetName);
		if( itor == m_mapPropertySet.end())
		{
			GUI_THROW( GUI_FORMAT( "[CGUIPropertyManager::UnregisterSet]: the property <%s> doesn't existed!",rSetName.c_str()));
		}
		m_mapPropertySet.erase( itor );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get a property set
	*/
	const CGUIProperty& CGUIPropertyManager::GetSet(const CGUIString& rSetName ) const
	{
		TMapPropertySet::const_iterator itor = m_mapPropertySet.find(rSetName);
		if( itor == m_mapPropertySet.end())
		{
			GUI_THROW( GUI_FORMAT( "[CGUIPropertyManager::GetSet]: the property <%s> doesn't existed!",rSetName.c_str()));
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
