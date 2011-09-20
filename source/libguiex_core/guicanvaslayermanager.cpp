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
#include "guiinterfacerender.h"
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
		:m_bCacheDirtyFlag(false)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUICanvasLayerManager::CGUICanvasLayerManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayerManager::~CGUICanvasLayerManager()
	{
		//check
		GUI_ASSERT(m_vecGarbage.empty(), "[CGUICanvasLayerManager::~CGUICanvasLayerManager]: shouldn't has any layer in garbate");
		GUI_ASSERT(m_arrayCanvasLayers.empty(), "[CGUICanvasLayerManager::~CGUICanvasLayerManager]: shouldn't has any layer");
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
		if( m_bCacheDirtyFlag )
		{
			m_bCacheDirtyFlag = false;
			m_arrayCanvasLayersCache = m_arrayCanvasLayers;
		}

		for( TArrayCanvasLayer::reverse_iterator itor = m_arrayCanvasLayersCache.rbegin();
			itor != m_arrayCanvasLayersCache.rend();
			++itor )
		{
			(*itor)->Update( fDeltaTime );
		}

		RefreshGarbage();
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::Render( IGUIInterfaceRender* pRender )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			pRender->Clear( eRenderBuffer_DEPTH_BIT );
			(*itor)->BeginRender( pRender );
			(*itor)->Render( pRender );
			(*itor)->EndRender( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::RenderExtraInfo( class IGUIInterfaceRender* pRender )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			(*itor)->BeginRender( pRender );
			(*itor)->RenderExtraInfo( pRender );
			(*itor)->EndRender( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::Refresh( )
	{
		for( TArrayCanvasLayer::reverse_iterator itor = m_arrayCanvasLayers.rbegin();
			itor != m_arrayCanvasLayers.rend();
			++itor )
		{
			(*itor)->Refresh( );
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUICanvasLayerManager::GetWidgetUnderPoint(const CGUIVector2& rPos)
	{
		for( TArrayCanvasLayer::reverse_iterator itor = m_arrayCanvasLayers.rbegin();
			itor != m_arrayCanvasLayers.rend();
			++itor )
		{
			CGUIWidget* pWidget = (*itor)->GetWidgetUnderPoint( rPos );
			if( pWidget )
			{
				return pWidget;
			}
			if( (*itor)->IsInputConsumed() )
			{
				return NULL;
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::PushCanvasLayer( CGUICanvasLayer* pLayer )
	{
		m_arrayCanvasLayers.push_back( pLayer );
		m_bCacheDirtyFlag = true;

		if( !pLayer->IsTopMost() && m_arrayCanvasLayers.size() >= 2 )
		{
			for( int32 i=int32(m_arrayCanvasLayers.size())-2; i>=0; --i )
			{
				if( m_arrayCanvasLayers[i]->IsTopMost() )
				{
					std::swap( m_arrayCanvasLayers[i], m_arrayCanvasLayers[i+1] );
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer* CGUICanvasLayerManager::PopCanvasLayer( )
	{
		if( m_arrayCanvasLayers.empty() )
		{
			GUI_THROW(  "[CGUICanvasLayerManager::PopCanvasLayer]: want to pop canvaslayer but there isn't any layer" );
			return NULL;
		}
		CGUICanvasLayer* pLayer = m_arrayCanvasLayers.back();
		m_arrayCanvasLayers.pop_back();
		m_bCacheDirtyFlag = true;
		return pLayer;
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::PopAndDestroyCanvasLayer( )
	{
		CGUICanvasLayer* pCanvasLayer = PopCanvasLayer();
		pCanvasLayer->Finalize();
		pCanvasLayer->DestroySelf();
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::RemoveCanvasLayer( CGUICanvasLayer* pLayer )
	{
		TArrayCanvasLayer::iterator itor = std::find( m_arrayCanvasLayers.begin(), m_arrayCanvasLayers.end(), pLayer );
		if( itor != m_arrayCanvasLayers.end() )
		{
			m_arrayCanvasLayers.erase( itor );
			m_bCacheDirtyFlag = true;
			return;
		}
		GUI_THROW( "[CGUICanvasLayerManager::RemoveCanvasLayer]: not found given layer");
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::DestroyCanvasLayer( CGUICanvasLayer* pLayer )
	{
		TArrayCanvasLayer::iterator itor = std::find( m_arrayCanvasLayers.begin(), m_arrayCanvasLayers.end(), pLayer );
		if( itor != m_arrayCanvasLayers.end() )
		{
			m_arrayCanvasLayers.erase( itor );
			m_bCacheDirtyFlag = true;
			pLayer->Finalize();
			pLayer->DestroySelf();
			return;
		}
		GUI_THROW( "[CGUICanvasLayerManager::DestroyCanvasLayer]: not found given layer");
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::DelayedDestroyCanvasLayer( CGUICanvasLayer* pLayer )
	{
		TArrayCanvasLayer::iterator itor = std::find( m_arrayCanvasLayers.begin(), m_arrayCanvasLayers.end(), pLayer );
		if( itor != m_arrayCanvasLayers.end() )
		{
			m_arrayCanvasLayers.erase( itor );
			m_bCacheDirtyFlag = true;
			m_vecGarbage.push_back( pLayer );
			return;
		}
		GUI_THROW( "[CGUICanvasLayerManager::DelayedDestroyCanvasLayer]: not found given layer");
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::DestroyAllCanvasLayer( )
	{
		for( TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
			itor != m_arrayCanvasLayers.end();
			++itor )
		{
			(*itor)->Finalize();
			(*itor)->DestroySelf( );
		}
		m_arrayCanvasLayers.clear();
		m_bCacheDirtyFlag = true;
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayerManager::RefreshGarbage()
	{
		if( m_vecGarbage.empty() )
		{
			return;
		}

		for( uint32 i=0; i<uint32(m_vecGarbage.size()); ++i )
		{
			CGUICanvasLayer* pLayer = m_vecGarbage[i];
			if( pLayer )
			{
				pLayer->Finalize();
				pLayer->DestroySelf();
			}
		}

		m_vecGarbage.clear( );
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
			GUI_THROW( "[CGUICanvasLayerManager::GetCanvasLayer]: not finding layer by index");
			return NULL;
		}

		TArrayCanvasLayer::iterator itor = m_arrayCanvasLayers.begin();
		for( uint32 i=1; i<nIndex; ++i )
		{
			++itor;
		}
		return *itor;
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

		GUI_THROW( "[CGUICanvasLayerManager::GetCanvasLayer]: not finding layer by name");
		return NULL;	
	}
	//------------------------------------------------------------------------------
}
