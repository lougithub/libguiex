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
		ClearTileImageCache();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::InitTileMap()
	{
		SetFocusable(false);
		SetActivable(false);

		m_nRows = 0;
		m_nColumns = 0;
		m_aTileImageSize.SetWidth( 32 );
		m_aTileImageSize.SetHeight( 32 );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIVector2 aStartPos = GetBoundArea( ).GetPosition();
		for( uint32 nRow = 0; nRow < m_nRows; ++nRow )
		{
			for( uint32 nColumn=0; nColumn<m_nColumns; ++nColumn )
			{
				const CGUIImage* pImage = GetTileImageCache( GetTileID( nRow, nColumn ) );
				if( pImage )
				{
					CGUIRect aRenderArea( 
						aStartPos + CGUIVector2( nColumn*m_aTileImageSize.m_fWidth, nRow*m_aTileImageSize.m_fHeight),
						m_aTileImageSize );
					DrawImage( pRender, pImage, aRenderArea);
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::UpdateTileMapSize( )
	{
		CGUISize aTileMapPixelSize = m_aTileImageSize;
		if( m_nRows > 0 && m_nColumns > 0 )
		{
			aTileMapPixelSize.SetWidth( m_aTileImageSize.GetWidth() * m_nColumns );
			aTileMapPixelSize.SetHeight( m_aTileImageSize.GetHeight() * m_nRows );
		}
		SetSizeType( eScreenValue_Pixel );
		SetPixelSize( aTileMapPixelSize );
		Refresh();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileImageSize( const CGUISize& rTileImageSize )
	{
		m_aTileImageSize = rTileImageSize;
		UpdateTileMapSize();
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIWgtTileMap::GetTileImageSize( ) const
	{
		return m_aTileImageSize;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::ClearTileImageCache()
	{
		for( TTileImageCache::iterator itor = m_vecTileImageCache.begin();
				itor != m_vecTileImageCache.end();
				++itor )
		{
			if( *itor )
			{
				CGUIImageManager::Instance()->DeallocateResource( *itor );
			}
		}
		m_vecTileImageCache.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileImageCacheCapacity( uint32 nCapacity )
	{
		ClearTileImageCache();
		m_vecTileImageCache.resize( nCapacity );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtTileMap::GetTileImageCacheCapacity( ) const
	{
		return m_vecTileImageCache.size();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileImageCache( uint32 nTileID, CGUIImage* pImage )
	{
		if( nTileID>= m_vecTileImageCache.size() )
		{
			throw CGUIException( "[CGUIWgtTileMap::SetTileImageCache]: tile index is out of range");
			return;
		}
		if( m_vecTileImageCache[nTileID] )
		{
			CGUIImageManager::Instance()->DeallocateResource( m_vecTileImageCache[nTileID] );
		}
		m_vecTileImageCache[nTileID] = pImage;
		if( pImage )
		{
			pImage->RefRetain();
		}
	}
	//------------------------------------------------------------------------------
	const CGUIImage* CGUIWgtTileMap::GetTileImageCache( uint32 nTileID) const
	{
		if( nTileID>= m_vecTileImageCache.size() )
		{
			throw CGUIException( "[CGUIWgtTileMap::GetTileImageCache]: tile index is out of range");
			return NULL;
		}
		return m_vecTileImageCache[nTileID];
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::ClearTileMapData()
	{
		m_aTileMapData.clear();
		m_nRows = 0;
		m_nColumns = 0;
		UpdateTileMapSize();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileMapData( const TTileMapData& rTileMapData )
	{
		m_aTileMapData = rTileMapData;
		m_nRows = rTileMapData.size();
		if( m_nRows > 0 )
		{
			m_nColumns = rTileMapData[0].size();
		}
		else
		{
			m_nColumns = 0;
		}
		UpdateTileMapSize();
	}
	//------------------------------------------------------------------------------
	const CGUIWgtTileMap::TTileMapData& CGUIWgtTileMap::GetTileMapData() const
	{
		return m_aTileMapData;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileMapSize( uint32 nRow, uint32 nColumn )
	{
		ClearTileMapData();
		m_aTileMapData.resize( nRow );
		for( TTileMapData::iterator itor = m_aTileMapData.begin(); 
				itor != m_aTileMapData.end();
				++itor )
		{
			(*itor).resize( nColumn, 0 );
		}
		m_nColumns = nColumn;
		m_nRows = nRow;
		UpdateTileMapSize();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTileMap::SetTileID( uint32 nRow, uint32 nColumn, uint32 nTileID )
	{
		if( nRow >= m_nRows || nColumn >= m_nColumns )
		{
			throw CGUIException("[CGUIWgtTileMap::SetTileID]: the position of inserted tile is out of range.");
			return;
		}
		m_aTileMapData[nRow][nColumn] = nTileID;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtTileMap::GetTileID( uint32 nRow, uint32 nColumn )
	{
		if( nRow >= m_nRows || nColumn >= m_nColumns )
		{
			throw CGUIException("[CGUIWgtTileMap::GetTileID]: the position of tile is out of range.");
			return 0;
		}
		return m_aTileMapData[nRow][nColumn];
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


