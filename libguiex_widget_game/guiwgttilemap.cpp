/** 
* @file guiwgttilemap.cpp
* @brief used to render tile map
* @author ken
* @date 2011-01-26
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiwgttilemap.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guitilemap.h>
#include <libguiex_core/guitilemapmanager.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtTileMap);
	//------------------------------------------------------------------------------
	CGUIWgtTileMap::CGUIWgtTileMap( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitTileMap();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTileMap::CGUIWgtTileMap( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitTileMap();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTileMap::~CGUIWgtTileMap()
	{
		GUI_ASSERT( !m_pTileMap, "resource leak" );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::InitTileMap()
	{
		SetFocusable(false);
		SetActivable(false);

		m_pTileMap = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::OnDestroy()
	{
		if( m_pTileMap )
		{
			CGUITileMapManager::Instance()->DeallocateResource( m_pTileMap );
			m_pTileMap = NULL;
		}

		CGUIWidget::OnDestroy();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::RenderSelf(IGUIInterfaceRender* pRender)
	{
		if( m_pTileMap )
		{
			m_pTileMap->Render( pRender, getFullTransform() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileMap( const CGUIString& rTileMapName )
	{
		if( rTileMapName.empty() )
		{
			//clear
			SetTileMap( NULL );
		}
		else
		{
			CGUITileMap* pTileMap = CGUITileMapManager::Instance()->AllocateResource( rTileMapName );
			SetTileMap( pTileMap );
			CGUITileMapManager::Instance()->DeallocateResource( pTileMap );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileMap( class CGUITileMap* pTileMap )
	{
		if( m_pTileMap == pTileMap )
		{
			return;
		}

		if( m_pTileMap )
		{
			CGUITileMapManager::Instance()->DeallocateResource( m_pTileMap );
			m_pTileMap = NULL;
		}

		if( pTileMap )
		{
			m_pTileMap = pTileMap;
			m_pTileMap->RefRetain();
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtTileMap::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_TileMap && rProperty.GetName() == "tilemap" )
		{
			if( m_pTileMap )
			{
				rProperty.SetValue( m_pTileMap->GetName() );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::ProcessProperty( const CGUIProperty& rProperty)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		if( rProperty.GetType() == ePropertyType_TileMap && rProperty.GetName() == "tilemap")
		{
			SetTileMap( rProperty.GetValue());
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex


