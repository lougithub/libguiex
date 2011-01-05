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
#include "guiexception.h"
#include "guicanvaslayer.h"
#include <algorithm>


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
			throw CGUIException( "[CGUICanvasLayerManager::~CGUICanvasLayerManager]: still has some canvaslayers");
		}
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayerManager* CGUICanvasLayerManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::Update( real fDeltaTime )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			(*itor)->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::Render( IGUIInterfaceRender* pRender )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			(*itor)->Render( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::PushCanvasLayer( CGUICanvasLayer* pLayer )
	{
		m_arrayCanvasLayers.push_back( pLayer );
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer* CGUICanvasLayerManager::PopCanvasLayer( )
	{
		if( m_arrayCanvasLayers.empty() )
		{
			throw CGUIException( "[CGUICanvasLayerManager::PopCanvasLayer]: want to pop canvaslayer but there isn't any layer" );
			return NULL;
		}
		CGUICanvasLayer* pLayer = m_arrayCanvasLayers.back();
		m_arrayCanvasLayers.pop_back();
		return pLayer;
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::PopAndDestroyCanvasLayer( )
	{
		CGUICanvasLayer* pCanvasLayer = PopCanvasLayer();
		pCanvasLayer->DestroySelf();
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer* CGUICanvasLayerManager::RemoveCanvasLayer( CGUICanvasLayer* pLayer )
	{
		TArrayCanvasLayer::iterator itor = std::find( m_arrayCanvasLayers.begin(), m_arrayCanvasLayers.end(), pLayer );
		if( itor != m_arrayCanvasLayers.end() )
		{
			m_arrayCanvasLayers.erase( itor );
			return pLayer;
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::DestroyAllCanvasLayer( )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			(*itor)->DestroySelf( );
		}
		m_arrayCanvasLayers.clear();
	}
	//------------------------------------------------------------------------------
	uint32 CGUICanvasLayerManager::GetCanvasLayerNum() const
	{
		return uint32( m_arrayCanvasLayers.size() );
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer* CGUICanvasLayerManager::GetCanvasLayer( uint32 nIndex )
	{
		if( nIndex >= uint32( m_arrayCanvasLayers.size()))
		{
			throw CGUIException("[CGUICanvasLayerManager::GetCanvasLayer]: not finding layer by index");
			return NULL;
		}
		return m_arrayCanvasLayers[nIndex];
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer* CGUICanvasLayerManager::GetCanvasLayer( const char* szLayerName )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			if( (*itor)->GetLayerName() == szLayerName )
			{
				return *itor;
			}
		}

		throw CGUIException("[CGUICanvasLayerManager::GetCanvasLayer]: not finding layer by name");
		return NULL;	
	}
	//------------------------------------------------------------------------------
}
