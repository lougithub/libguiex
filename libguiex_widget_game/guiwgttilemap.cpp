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
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiimagemanager.h>
#include "guitilemap.h"


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
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::InitTileMap()
	{
		SetFocusable(false);
		SetActivable(false);

		m_pTiledMap = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::RenderSelf(IGUIInterfaceRender* pRender)
	{
		if( m_pTiledMap )
		{
			m_pTiledMap->Render( pRender, getFullTransform() );
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtTileMap::GenerateProperty( CGUIProperty& rProperty )
	{
		return CGUIWidget::GenerateProperty( rProperty );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::ProcessProperty( const CGUIProperty& rProperty)
	{
		CGUIWidget::ProcessProperty( rProperty );
	}
	//------------------------------------------------------------------------------
}//namespace guiex


