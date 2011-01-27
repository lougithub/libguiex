/** 
* @file guitilemapparser.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
#ifndef	__KEN_TILEMAPPARSER_20110127_H__
#define __KEN_TILEMAPPARSER_20110127_H__


//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guibase.h>

//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	enum 
	{
		TMXLayerAttribNone = 1 << 0,
		TMXLayerAttribBase64 = 1 << 1,
		TMXLayerAttribGzip = 1 << 2,
	};

	enum 
	{
		TMXPropertyNone,
		TMXPropertyMap,
		TMXPropertyLayer,
		TMXPropertyObjectGroup,
		TMXPropertyObject,
		TMXPropertyTile
	};
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/* CCTMXLayerInfo contains the information about the layers like:
	- Layer name
	- Layer size
	- Layer opacity at creation time (it can be modified at runtime)
	- Whether the layer is visible
	This information is obtained from the TMX file.
	*/
	class CCTMXLayerInfo
	{
	public:
		CCTMXLayerInfo();
		~CCTMXLayerInfo();

	public:
		CGUIString name;
		CGUISize layerSize;
		uint32 *tiles;
		bool visible;
		uint8 opacity;
		bool ownTiles;
		uint32 minGID;
		uint32 maxGID;
		std::map<CGUIString, CGUIString> properties;
		CGUIVector2 offset;
	};


	/* CCTMXTilesetInfo contains the information about the tilesets like:
	- Tileset name
	- Tilset spacing
	- Tileset margin
	- size of the tiles
	- Image used for the tiles
	- Image size

	This information is obtained from the TMX file. 
	*/
	class CCTMXTilesetInfo
	{
	public:
		CCTMXTilesetInfo();
		~CCTMXTilesetInfo();

		CGUIRect RectForGID( uint32 gid );

	public:
		CGUIString name;
		uint32 firstGid;
		CGUISize tileSize;
		uint32 spacing;
		uint32 margin;
		CGUIString sourceImage; // image containing the tiles
		CGUISize imageSize; // size in pixels of the image
	};


	/* CCTMXMapInfo contains the information about the map like:
	- Map orientation (hexagonal, isometric or orthogonal)
	- Tile size
	- Map size

	And it also contains:
	- Layers (an array of TMXLayerInfo objects)
	- Tilesets (an array of TMXTilesetInfo objects)
	- ObjectGroups (an array of TMXObjectGroupInfo objects)

	This information is obtained from the TMX file.
	*/
	class CCTMXMapInfo
	{	
	public:	
		CCTMXMapInfo();
		~CCTMXMapInfo();

		int32 InitWithTMXFile( const CGUIString& tmxFile );

	protected:
		int32 ParseXMLFile( const CGUIString& tmxFile );

	public:
		CGUIString currentString;
		bool storingCharacters;	
		int32 layerAttribs;
		int32 parentElement;
		uint32 parentGID;
		CGUIString filename; // tmx filename
		int32 orientation;	// map orientation
		CGUISize mapSize; // map width & height
		CGUISize tileSize; // tiles width & height
		NSMutableArray layers; // Layers
		NSMutableArray tilesets; // tilesets
		NSMutableArray objectGroups; // ObjectGroups
		std::map<CGUIString, CGUIString> properties; // properties
		std::map<CGUIString, CGUIString> tileProperties; // tile properties
	};

} //namespace guiex

#endif //__KEN_TILEMAPPARSER_20110127_H__

