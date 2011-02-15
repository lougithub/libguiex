/** 
* @file guitilemaplayer.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
#ifndef	__KEN_TILEMAPLAYER_20110127_H__
#define __KEN_TILEMAPLAYER_20110127_H__


//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guiex.h>
#include "guitilemapparser.h"

//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CCTMXTiledMap;
	class CGUITexture;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CCTMXLayer
	{
	public:
		CCTMXLayer( CCTMXTiledMap* pOwnerMap, uint32 nLayerIndex );
		~CCTMXLayer();

		const CGUIString& GetLayerName() const;

		const CGUIIntSize& GetLayerSize() const;
		const CGUIIntSize& GetMapTileSize() const;

		uint32 GetMinGID() const;
		uint32 GetMaxGID() const;

		real GetOpacity() const;

		const std::vector<uint32>& GetTileGIDs() const;
		std::vector<uint32>& GetTileGIDs();

		const std::map<CGUIString, CGUIString>& GetProperties() const;

		ETMXOrientation GetLayerOrientation() const;

		void Render( IGUIInterfaceRender* pRender, const CGUIMatrix4& rWorldMatrix );

	protected:
		struct STileData
		{
			uint32 m_uGID;
			CGUIRect m_aDestRect;
			CGUIRect m_aUV;
		};

	protected:
		CCTMXTilesetInfo* GetTilesetForLayer();
		int32 InitLayer();
		int32 AppendTileForGID( uint32 gid, const CGUIIntVector2& pos );
		CGUIVector2 PositionAt( const CGUIIntVector2& pos );
		CGUIVector2 PositionForOrthoAt( const CGUIIntVector2& pos );
		CGUIVector2 PositionForIsoAt( const CGUIIntVector2& pos );
		CGUIVector2 PositionForHexAt( const CGUIIntVector2& pos );

	protected:
		uint32 m_nLayerIndex;
		CCTMXTiledMap* m_pOwnerMap;
		CCTMXLayerInfo* m_pLayerInfo;
		CCTMXTilesetInfo* m_pTileSetInfo;

		CGUISize m_aLayerSize;
		CGUIVector2 m_aLayerOffset;

		CGUITexture* m_pTexture;

		uint32 minGID;
		uint32 maxGID;

		std::vector<STileData> m_vecTiles;
	};


} //namespace guiex


#endif //__KEN_TILEMAPLAYER_20110127_H__

