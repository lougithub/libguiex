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
#include <vector>

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
		real opacity;
		bool ownTiles;
		uint32 minGID;
		uint32 maxGID;
		std::map<CGUIString, CGUIString> properties;
		CGUIVector2 offset;
	};

	class CCTMXObjectInfo
	{
	public:
		CGUIString objectName;
		CGUIString objectType;
		CGUIVector2 position;
		CGUISize size;
		std::map<CGUIString,CGUIString>	properties;
	};

	class CCTMXObjectGroup
	{
	public:
		CCTMXObjectGroup();
		~CCTMXObjectGroup();

	public:
		CGUIString groupName;
		CGUIVector2 positionOffset;
		std::vector<CCTMXObjectInfo*> objects;
		std::map<CGUIString,CGUIString>	properties;
	}

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
		int32 ParseNode_map( class TiXmlElement* pMapNode );
		int32 ParseNode_tileset( class TiXmlElement* pTilesetNode );
		int32 ParseNode_layer( class TiXmlElement* pLayerNode );
		int32 ParseNode_image( class TiXmlElement* pImageNode );
		int32 ParseNode_tile( class TiXmlElement* pTileNode );
		int32 ParseNode_properties( class TiXmlElement* pPropertiesNode, std::map<CGUIString, CGUIString>& mapTileProperties );
		int32 ParseNode_data( class TiXmlElement* pTileNode );
		int32 ParseNode_objectgroup( class TiXmlElement* pObjectGroupNode );
		int32 ParseNode_object( class TiXmlElement* pObjectNode );

		int32 ParseSourceFile( const CGUIString& tmxFile );


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
		std::vector<CCTMXLayerInfo> layers; // Layers
		std::vector<CCTMXTilesetInfo> tilesets; // tilesets
		std::vector<CCTMXObjectGroup> objectGroups; // ObjectGroups
		std::map<CGUIString, CGUIString> properties; // properties
		std::map<uint32, std::map<CGUIString, CGUIString> > tileProperties; // tile properties <gid, tile property>
	};

} //namespace guiex

#endif //__KEN_TILEMAPPARSER_20110127_H__

