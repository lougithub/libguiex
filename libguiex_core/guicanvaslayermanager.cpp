/** 
 * @file guicanvaslayermanager.cpp
 * @brief canvas layer used in system
 * @author ken
 * @date 2010-12-22
 */

//============================================================================//
// include 
//============================================================================// 
#include "guicanvaslayermanager.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUICanvasLayerManager * CGUICanvasLayerManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUICanvasLayerManager::CGUICanvasLayerManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUICanvasLayerManager::CGUICanvasLayerManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayerManager::~CGUICanvasLayerManager()
	{
		if( !m_arrayCanvasLayers.empty() )
		{
			//throw CGUIException();
		}
	}
	////------------------------------------------------------------------------------
	//CGUICanvasLayerManager* CGUICanvasLayerManager::Instance()
	//{
	//	return m_pSingleton; 
	//}
	////------------------------------------------------------------------------------
	//void CGUICanvasLayerManager::Update( real fDeltaTime )
	//{

	//}
	////------------------------------------------------------------------------------
	//void CGUICanvasLayerManager::Render( )
	//{

	//}
	////------------------------------------------------------------------------------
	//void CGUICanvasLayerManager::PushCanvasLayer( CGUICanvasLayer* pLayer )
	//{

	//}
	////------------------------------------------------------------------------------
	//CGUICanvasLayer* CGUICanvasLayerManager::PopCanvasLayer( )
	//{

	//}
	////------------------------------------------------------------------------------
	//CGUICanvasLayer* CGUICanvasLayerManager::PopAndDestroyCanvasLayer( )
	//{

	//}
	////------------------------------------------------------------------------------
	//CGUICanvasLayer* CGUICanvasLayerManager::RemoveCanvasLayer( CGUICanvasLayer* pLayer )
	//{

	//}
	////------------------------------------------------------------------------------
	//CGUICanvasLayer* CGUICanvasLayerManager::DestroyAllCanvasLayer( )
	//{

	//}
	////------------------------------------------------------------------------------
	//uint32 CGUICanvasLayerManager::GetCanvasLayerNum() const
	//{

	//}
	////------------------------------------------------------------------------------
	//CGUICanvasLayer* CGUICanvasLayerManager::GetCanvasLayer( uint32 nIndex )
	//{

	//}
	////------------------------------------------------------------------------------
	//CGUICanvasLayer* CGUICanvasLayerManager::GetCanvasLayer( const char* szLayerName )
	//{

	//}
	////------------------------------------------------------------------------------
}
