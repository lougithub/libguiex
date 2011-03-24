/** 
* @file guitilemaplayer.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
//============================================================================//
// include
//============================================================================//
#include "guitilemaplayer.h"
#include "guitilemap.h"
#include "guitilemapparser.h"
#include "guiexception.h"
#include "guimath.h"
#include "guitexturemanager.h"
#include "guicolor.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guiexception.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITileMapLayer::CGUITileMapLayer( const CGUITileMap* pOwnerMap, uint32 nLayerIndex )
		:m_pOwnerMap( pOwnerMap )
		,m_nLayerIndex( nLayerIndex )
		,m_pLayerInfo( NULL )
		,m_pTileSetInfo( NULL )
		,m_pTexture( NULL )
		,minGID( 0 )
		,maxGID( 0 )
	{
		//check layer index
		if( m_pOwnerMap->m_pMapInfo->layers.size() <= nLayerIndex )
		{
			throw CGUIException("[CGUITileMapLayer::CGUITileMapLayer]: invalid tile map layer index");
			return;
		}

		//init layer info
		m_pLayerInfo = &m_pOwnerMap->m_pMapInfo->layers[nLayerIndex];

		//init tile set
		m_pTileSetInfo = GetTilesetForLayer();

		//init layer
		if( 0 != InitLayer() )
		{
			throw CGUIException("[CGUITileMapLayer::CGUITileMapLayer]: failed to init layer");
			return;
		}
	}
	//------------------------------------------------------------------------------
	CGUITileMapLayer::~CGUITileMapLayer()
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture(m_pTexture);
			m_pTexture = NULL;
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITileMapLayer::GetLayerName() const
	{
		return m_pLayerInfo->name;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITileMapLayer::GetLayerSize() const
	{
		return m_pLayerInfo->layerSize;
	}
	//------------------------------------------------------------------------------
	uint32 CGUITileMapLayer::GetMinGID() const
	{
		return minGID;
	}
	//------------------------------------------------------------------------------
	uint32 CGUITileMapLayer::GetMaxGID() const
	{
		return maxGID;
	}
	//------------------------------------------------------------------------------
	real CGUITileMapLayer::GetOpacity() const
	{
		return m_pLayerInfo->opacity;
	}
	//------------------------------------------------------------------------------
	const std::vector<uint32>& CGUITileMapLayer::GetTileGIDs() const
	{
		return m_pLayerInfo->tiles;
	}
	//------------------------------------------------------------------------------
	const std::map<CGUIString, CGUIString>& CGUITileMapLayer::GetProperties() const
	{
		return m_pLayerInfo->properties;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITileMapLayer::GetMapTileSize() const
	{
		return m_pOwnerMap->m_pMapInfo->tileSize;
	}
	//------------------------------------------------------------------------------
	ETMXOrientation CGUITileMapLayer::GetLayerOrientation() const
	{
		return m_pOwnerMap->m_pMapInfo->orientation;
	}
	//------------------------------------------------------------------------------
	CGUITileMapTilesetInfo* CGUITileMapLayer::GetTilesetForLayer()
	{		
		const CGUIIntSize& size = m_pLayerInfo->layerSize;

		for( uint32 i=0; i<m_pOwnerMap->m_pMapInfo->tilesets.size(); ++i )
		{
			CGUITileMapTilesetInfo* pTileset = &m_pOwnerMap->m_pMapInfo->tilesets[i];
			for( uint32 y = 0; y < size.m_uHeight; y++ ) 
			{
				for( uint32 x = 0; x < size.m_uWidth; x++ )
				{
					uint32 pos = x + size.m_uWidth * y;
					uint32 gid = m_pLayerInfo->tiles[ pos ];

					// XXX: gid == 0 --> empty tile
					if( gid != 0 ) 
					{
						// Optimization: quick return
						// if the layer is invalid (more than 1 tileset per layer) an assert will be thrown later
						if( gid >= pTileset->firstGid )
						{
							return pTileset;
						}
					}
				}
			}		
		}

		throw CGUIException("[CGUITileMapLayer::GetTilesetForLayer]: Layer '%s' has no tiles", GetLayerName().c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapLayer::InitLayer()
	{
		//update image
		CGUIString strFullPath = m_pOwnerMap->m_strFullWorkingDir + m_pTileSetInfo->sourceImage;
		m_pTexture = CGUITextureManager::Instance()->CreateTexture(strFullPath);
		if( !m_pTexture )
		{
			throw CGUIException("[CGUITileMapLayer::InitLayer]: failed to create texture from path <%s>",strFullPath.c_str());
			return -1;
		}

		// update offset (after layer orientation is set);
		m_aLayerOffset.x = real( m_pLayerInfo->offset.x );
		m_aLayerOffset.y = real( m_pLayerInfo->offset.y );

		//update layer size
		m_aLayerSize.m_fWidth = real( GetLayerSize().m_uWidth * GetMapTileSize().m_uWidth );
		m_aLayerSize.m_fHeight = real( GetLayerSize().m_uHeight * GetMapTileSize().m_uHeight );

		//update tilesets
		// By default all the tiles are aliased
		for( uint32 y=0; y < GetLayerSize().m_uHeight; y++ )
		{
			for( uint32 x=0; x < GetLayerSize().m_uWidth; x++ ) 
			{

				uint32 pos = x + GetLayerSize().m_uWidth * y;
				uint32 gid = GetTileGIDs()[ pos ];

				// XXX: gid == 0 --> empty tile
				if( gid != 0 )
				{
					AppendTileForGID( gid, CGUIIntVector2(x,y));

					// Optimization: update min and max GID rendered by the layer
					minGID = GUIMin(gid, minGID);
					maxGID = GUIMax(gid, maxGID);
				}
			}
		}

		if( maxGID < m_pTileSetInfo->firstGid || minGID > m_pTileSetInfo->firstGid )
		{
			throw CGUIException("[CGUITileMapLayer::InitLayer]: Only 1 tilset per layer is supported");
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapLayer::AppendTileForGID( uint32 gid, const CGUIIntVector2& pos )
	{
		STileData aTileData;
		aTileData.m_uGID = gid;
		CGUISize aTileSize( real( GetMapTileSize().m_uWidth),real( GetMapTileSize().m_uHeight));
		aTileData.m_aDestRect = CGUIRect( PositionAt( pos ), aTileSize );
		aTileData.m_aUV = m_pTileSetInfo->RectForGID( gid, CGUIIntSize( m_pTexture->GetWidth(), m_pTexture->GetHeight()) );
		m_vecTiles.push_back( aTileData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITileMapLayer::PositionAt( const CGUIIntVector2& pos )
	{
		CGUIVector2 ret;
		switch( GetLayerOrientation() ) 
		{
		case CCTMXOrientationOrtho:
			ret = PositionForOrthoAt(pos);
			break;
		case CCTMXOrientationIso:
			ret = PositionForIsoAt(pos);
			break;
		case CCTMXOrientationHex:
			ret = PositionForHexAt(pos);
			break;
		}
		return ret;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITileMapLayer::PositionForOrthoAt( const CGUIIntVector2& pos )
	{
		return CGUIVector2(
			real(pos.x * GetMapTileSize().m_uWidth),
			real (pos.y * GetMapTileSize().m_uHeight));
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITileMapLayer::PositionForIsoAt( const CGUIIntVector2& pos )
	{
		return CGUIVector2(
			real(GetMapTileSize().m_uWidth /2 * ( GetLayerSize().m_uWidth + pos.x - pos.y - 1)),
			real(GetMapTileSize().m_uHeight /2 * (pos.x + pos.y)));
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITileMapLayer::PositionForHexAt( const CGUIIntVector2& pos )
	{
		float diffY = 0;
		if( pos.x % 2 == 1 )
		{
			diffY = -real(GetMapTileSize().m_uHeight) / 2.0f ;
		}

		return CGUIVector2(
			real(pos.x * GetMapTileSize().m_uWidth*3/4),
			real(pos.y * GetMapTileSize().m_uHeight + diffY));
	}
	//------------------------------------------------------------------------------
	void CGUITileMapLayer::Render( IGUIInterfaceRender* pRender )
	{
		for( uint32 i = 0;
			i < m_vecTiles.size();
			++i )
		{
			const STileData& rData = m_vecTiles[i];

			CGUIColor aColor(1.0f,1.0f,1.0f,1.0f);
			aColor.SetAlpha(GetOpacity());
			pRender->DrawTile( rData.m_aDestRect, 0, m_pTexture,rData.m_aUV, eImageOrientation_Normal,aColor);
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
