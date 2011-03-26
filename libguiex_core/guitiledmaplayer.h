/** 
* @file guitiledmaplayer.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
#ifndef	__KEN_TILEDMAPLAYER_20110127_H__
#define __KEN_TILEDMAPLAYER_20110127_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guitiledmapparser.h"

//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUITiledMap;
	class CGUITexture;
	class IGUIInterfaceRender;
	class CGUIMatrix4;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUITiledMapLayer
	{
	public:
		CGUITiledMapLayer( const CGUITiledMap* pOwnerMap, uint32 nLayerIndex );
		~CGUITiledMapLayer();

		const CGUIString& GetLayerName() const;

		const CGUIIntSize& GetLayerSize() const;
		const CGUIIntSize& GetMapTileSize() const;

		uint32 GetMinGID() const;
		uint32 GetMaxGID() const;

		real GetOpacity() const;

		const std::vector<uint32>& GetTileGIDs() const;

		const std::map<CGUIString, CGUIString>& GetProperties() const;

		ETMXOrientation GetLayerOrientation() const;

		void Render( IGUIInterfaceRender* pRender );

	protected:
		struct STileData
		{
			uint32 m_uGID;
			CGUIRect m_aDestRect;
			CGUIRect m_aUV;
		};

	protected:
		const CGUITiledMapTilesetInfo* GetTilesetForLayer() const;
		int32 InitLayer();
		int32 AppendTileForGID( uint32 gid, const CGUIIntVector2& pos );
		CGUIVector2 PositionAt( const CGUIIntVector2& pos );
		CGUIVector2 PositionForOrthoAt( const CGUIIntVector2& pos );
		CGUIVector2 PositionForIsoAt( const CGUIIntVector2& pos );
		CGUIVector2 PositionForHexAt( const CGUIIntVector2& pos );

	protected:
		uint32 m_nLayerIndex;
		const CGUITiledMap* m_pOwnerMap;
		const CGUITiledMapLayerInfo* m_pLayerInfo;
		const CGUITiledMapTilesetInfo* m_pTileSetInfo;

		CGUISize m_aLayerSize;
		CGUIVector2 m_aLayerOffset;

		CGUITexture* m_pTexture;

		uint32 minGID;
		uint32 maxGID;

		std::vector<STileData> m_vecTiles;
	};


} //namespace guiex


#endif //__KEN_TILEDMAPLAYER_20110127_H__

