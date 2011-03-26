/** 
* @file guitiledmaplayer.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
//============================================================================//
// include
//============================================================================//
#include "guitiledmaplayer.h"
#include "guitiledmap.h"
#include "guitiledmapparser.h"
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
	CGUITiledMapLayer::CGUITiledMapLayer( const CGUITiledMap* pOwnerMap, uint32 nLayerIndex )
		:m_pOwnerMap( pOwnerMap )
		,m_nLayerIndex( nLayerIndex )
		,m_pLayerInfo( NULL )
		,m_pTileSetInfo( NULL )
		,m_pTexture( NULL )
		,minGID( 0 )
		,maxGID( 0 )
	{
		//check layer index
		if( m_pOwnerMap->GetMapInfo()->GetLayers().size() <= nLayerIndex )
		{
			throw CGUIException("[CGUITiledMapLayer::CGUITiledMapLayer]: invalid tile map layer index");
			return;
		}

		//init layer info
		m_pLayerInfo = &m_pOwnerMap->GetMapInfo()->GetLayers()[nLayerIndex];

		//init tile set
		m_pTileSetInfo = GetTilesetForLayer();

		//init layer
		if( 0 != InitLayer() )
		{
			throw CGUIException("[CGUITiledMapLayer::CGUITiledMapLayer]: failed to init layer");
			return;
		}
	}
	//------------------------------------------------------------------------------
	CGUITiledMapLayer::~CGUITiledMapLayer()
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture(m_pTexture);
			m_pTexture = NULL;
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMapLayer::GetLayerName() const
	{
		return m_pLayerInfo->m_strName;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITiledMapLayer::GetLayerSize() const
	{
		return m_pLayerInfo->m_aLayerSize;
	}
	//------------------------------------------------------------------------------
	uint32 CGUITiledMapLayer::GetMinGID() const
	{
		return minGID;
	}
	//------------------------------------------------------------------------------
	uint32 CGUITiledMapLayer::GetMaxGID() const
	{
		return maxGID;
	}
	//------------------------------------------------------------------------------
	real CGUITiledMapLayer::GetOpacity() const
	{
		return m_pLayerInfo->m_fOpacity;
	}
	//------------------------------------------------------------------------------
	const std::vector<uint32>& CGUITiledMapLayer::GetTileGIDs() const
	{
		return m_pLayerInfo->m_vTiles;
	}
	//------------------------------------------------------------------------------
	const std::map<CGUIString, CGUIString>& CGUITiledMapLayer::GetProperties() const
	{
		return m_pLayerInfo->m_mapProperties;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITiledMapLayer::GetMapTileSize() const
	{
		return m_pOwnerMap->GetMapInfo()->GetTileSize();
	}
	//------------------------------------------------------------------------------
	ETMXOrientation CGUITiledMapLayer::GetLayerOrientation() const
	{
		return m_pOwnerMap->GetMapInfo()->GetOrientation();
	}
	//------------------------------------------------------------------------------
	const CGUITiledMapTilesetInfo* CGUITiledMapLayer::GetTilesetForLayer() const
	{		
		const CGUIIntSize& size = m_pLayerInfo->m_aLayerSize;

		for( uint32 i=0; i<m_pOwnerMap->GetMapInfo()->GetTilesets().size(); ++i )
		{
			const CGUITiledMapTilesetInfo* pTileset = &m_pOwnerMap->GetMapInfo()->GetTilesets()[i];
			for( uint32 y = 0; y < size.m_uHeight; y++ ) 
			{
				for( uint32 x = 0; x < size.m_uWidth; x++ )
				{
					uint32 pos = x + size.m_uWidth * y;
					uint32 gid = m_pLayerInfo->m_vTiles[ pos ];

					// XXX: gid == 0 --> empty tile
					if( gid != 0 ) 
					{
						// Optimization: quick return
						// if the layer is invalid (more than 1 tileset per layer) an assert will be thrown later
						if( gid >= pTileset->GetFirstGid() )
						{
							return pTileset;
						}
					}
				}
			}		
		}

		throw CGUIException("[CGUITiledMapLayer::GetTilesetForLayer]: Layer '%s' has no tiles", GetLayerName().c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapLayer::InitLayer()
	{
		//update image
		CGUIString strFullPath = m_pOwnerMap->GetWorkingDir() + m_pTileSetInfo->GetSourceImage();
		m_pTexture = CGUITextureManager::Instance()->CreateTexture(strFullPath);
		if( !m_pTexture )
		{
			throw CGUIException("[CGUITiledMapLayer::InitLayer]: failed to create texture from path <%s>",strFullPath.c_str());
			return -1;
		}

		// update offset (after layer orientation is set);
		m_aLayerOffset.x = real( m_pLayerInfo->m_aOffset.x );
		m_aLayerOffset.y = real( m_pLayerInfo->m_aOffset.y );

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

		if( maxGID < m_pTileSetInfo->GetFirstGid() || minGID > m_pTileSetInfo->GetFirstGid() )
		{
			throw CGUIException("[CGUITiledMapLayer::InitLayer]: Only 1 tilset per layer is supported");
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapLayer::AppendTileForGID( uint32 gid, const CGUIIntVector2& pos )
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
	CGUIVector2 CGUITiledMapLayer::PositionAt( const CGUIIntVector2& pos )
	{
		CGUIVector2 ret;
		switch( GetLayerOrientation() ) 
		{
		case eTMXOrientationOrtho:
			ret = PositionForOrthoAt(pos);
			break;
		case eTMXOrientationIso:
			ret = PositionForIsoAt(pos);
			break;
		case eTMXOrientationHex:
			ret = PositionForHexAt(pos);
			break;
		}
		return ret;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITiledMapLayer::PositionForOrthoAt( const CGUIIntVector2& pos )
	{
		return CGUIVector2(
			real(pos.x * GetMapTileSize().m_uWidth),
			real (pos.y * GetMapTileSize().m_uHeight));
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITiledMapLayer::PositionForIsoAt( const CGUIIntVector2& pos )
	{
		return CGUIVector2(
			real(GetMapTileSize().m_uWidth /2 * ( GetLayerSize().m_uWidth + pos.x - pos.y - 1)),
			real(GetMapTileSize().m_uHeight /2 * (pos.x + pos.y)));
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUITiledMapLayer::PositionForHexAt( const CGUIIntVector2& pos )
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
	void CGUITiledMapLayer::Render( IGUIInterfaceRender* pRender )
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
