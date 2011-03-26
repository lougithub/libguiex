/** 
* @file guiwgttiledmap.cpp
* @brief used to render tile map
* @author ken
* @date 2011-01-26
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiwgttiledmap.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guitiledmap.h>
#include <libguiex_core/guitiledmapmanager.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtTiledMap);
	//------------------------------------------------------------------------------
	CGUIWgtTiledMap::CGUIWgtTiledMap( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitTiledMap();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTiledMap::CGUIWgtTiledMap( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitTiledMap();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTiledMap::~CGUIWgtTiledMap()
	{
		GUI_ASSERT( !m_pTiledMap, "resource leak" );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTiledMap::InitTiledMap()
	{
		SetFocusable(false);
		SetActivable(false);

		m_pTiledMap = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTiledMap::OnDestroy()
	{
		if( m_pTiledMap )
		{
			m_pTiledMap->RefRelease();
			m_pTiledMap = NULL;
		}

		CGUIWidget::OnDestroy();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTiledMap::RenderSelf(IGUIInterfaceRender* pRender)
	{
		if( m_pTiledMap )
		{
			m_pTiledMap->Render( pRender, getFullTransform() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTiledMap::SetTiledMap( const CGUIString& rTiledMapName )
	{
		if( rTiledMapName.empty() )
		{
			//clear
			SetTiledMap( NULL );
		}
		else
		{
			CGUITiledMap* pTiledMap = CGUITiledMapManager::Instance()->AllocateResource( rTiledMapName );
			SetTiledMap( pTiledMap );
			pTiledMap->RefRelease();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTiledMap::SetTiledMap( class CGUITiledMap* pTiledMap )
	{
		if( m_pTiledMap == pTiledMap )
		{
			return;
		}

		if( m_pTiledMap )
		{
			m_pTiledMap->RefRelease();
			m_pTiledMap = NULL;
		}

		if( pTiledMap )
		{
			m_pTiledMap = pTiledMap;
			m_pTiledMap->RefRetain();
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtTiledMap::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_TiledMap && rProperty.GetName() == "tiledmap" )
		{
			if( m_pTiledMap )
			{
				rProperty.SetValue( m_pTiledMap->GetName() );
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
	void CGUIWgtTiledMap::ProcessProperty( const CGUIProperty& rProperty)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		if( rProperty.GetType() == ePropertyType_TiledMap && rProperty.GetName() == "tiledmap")
		{
			SetTiledMap( rProperty.GetValue());
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex


