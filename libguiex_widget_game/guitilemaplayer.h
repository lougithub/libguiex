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

	protected:
		struct STileData
		{
			uint32 m_uGID;
			CGUIVector2 m_aPosition;
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

	//public:
	//	/** creates a CCTMXLayer with an tileset info, a layer info and a map info */
	//	+(id) layerWithTilesetInfo:(CCTMXTilesetInfo*)tilesetInfo layerInfo:(CCTMXLayerInfo*)layerInfo mapInfo:(CCTMXMapInfo*)mapInfo;
	//	/** initializes a CCTMXLayer with a tileset info, a layer info and a map info */
	//	-(id) initWithTilesetInfo:(CCTMXTilesetInfo*)tilesetInfo layerInfo:(CCTMXLayerInfo*)layerInfo mapInfo:(CCTMXMapInfo*)mapInfo;

	//	/** dealloc the map that contains the tile position from memory.
	//	Unless you want to know at runtime the tiles positions, you can safely call this method.
	//	If you are going to call [layer tileGIDAt:] then, don't release the map
	//	*/
	//	-(void) releaseMap;

	//	/** returns the tile (CCSprite) at a given a tile coordinate.
	//	The returned CCSprite will be already added to the CCTMXLayer. Don't add it again.
	//	The CCSprite can be treated like any other CCSprite: rotated, scaled, translated, opacity, color, etc.
	//	You can remove either by calling:
	//	- [layer removeChild:sprite cleanup:cleanup];
	//	- or [layer removeTileAt:ccp(x,y)];
	//	*/
	//	-(CCSprite*) tileAt:(CGPoint)tileCoordinate;

	//	/** returns the tile gid at a given tile coordinate.
	//	if it returns 0, it means that the tile is empty.
	//	This method requires the the tile map has not been previously released (eg. don't call [layer releaseMap])
	//	*/
	//	-(uint32) tileGIDAt:(CGPoint)tileCoordinate;

	//	/** sets the tile gid (gid = tile global id) at a given tile coordinate.
	//	The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
	//	If a tile is already placed at that position, then it will be removed.
	//	*/
	//	-(void) setTileGID:(uint32)gid at:(CGPoint)tileCoordinate;

	//	/** removes a tile at given tile coordinate */
	//	-(void) removeTileAt:(CGPoint)tileCoordinate;

	//	/** returns the position in pixels of a given tile coordinate */
	//	-(CGPoint) positionAt:(CGPoint)tileCoordinate;

	//	/** return the value for the specific property name */
	//	-(id) propertyNamed:(CGUIString )propertyName;

	//	/** Creates the tiles */
	//	-(void) setupTiles;

	//	/** CCTMXLayer doesn't support adding a CCSprite manually.
	//	@warning addchild:z:tag: is not supported on CCTMXLayer. Instead of setTileGID:at:/tileAt:
	//	*/
	//	-(void) addChild: (CCNode*)node z:(int)z tag:(int)tag;

	//public:
	//	CCTMXTilesetInfo* tileset;
	//	CGUIString layerName;
	//	CGUIIntSize	layerSize;
	//	CGUIIntSize mapTileSize;
	//	uint32* tiles;
	//	int layerOrientation;
	//	std::map<CGUIString, CGUIString> properties;
	//	real opacity; // TMX Layer supports opacity



	//	// Only used when vertexZ is used
	//	int vertexZvalue;
	//	bool useAutomaticVertexZ;
	//	float alphaFuncValue;

	//	// used for optimization
	//	CCSprite *reusedTile;
	//	ccCArray *atlasIndexArray;


	};


} //namespace guiex


#endif //__KEN_TILEMAPLAYER_20110127_H__

