/** 
* @file guitiledmapparser.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
#ifndef	__KEN_TILEDMAPPARSER_20110127_H__
#define __KEN_TILEDMAPPARSER_20110127_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guiintsize.h"
#include "guiintvector2.h"
#include "guirect.h"
#include <vector>
#include <map>

//============================================================================//
// declare
//============================================================================//
class TiXmlElement;

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/* CGUITiledMapLayerInfo contains the information about the layers like:
	- Layer name
	- Layer size
	- Layer opacity at creation time (it can be modified at runtime)
	- Whether the layer is visible
	This information is obtained from the TMX file.
	*/
	class CGUITiledMapLayerInfo
	{
	public:
		CGUITiledMapLayerInfo();
		~CGUITiledMapLayerInfo();

		const CGUIString& GetName() const;
		const std::map<CGUIString, CGUIString>& GetProperties() const;

		bool IsVisible() const;
		void SetVisible( bool bVisible );

		real GetOpacity() const;

	protected:
		friend class CGUITiledMapInfo;
		friend class CGUITiledMapLayer;

		CGUIString m_strName;
		CGUIIntSize m_aLayerSize;
		std::vector<uint32> m_vTiles;
		bool m_bVisible;
		real m_fOpacity;
		bool m_bOwnTiles;
		std::map<CGUIString, CGUIString> m_mapProperties;
		CGUIIntVector2 m_aOffset;
	};

	class CGUITiledMapObjectInfo
	{
	public:
		const CGUIString& GetName() const;
		const CGUIString& GetType() const;
		const CGUIIntVector2& GetPosition() const;

	protected:
		friend class CGUITiledMapInfo;

		CGUIString m_strObjectName;
		CGUIString m_strObjectType;
		CGUIIntVector2 m_aPosition;
		CGUIIntSize m_aSize;
		std::map<CGUIString,CGUIString>	m_mapProperties;
	};

	class CGUITiledMapObjectGroup
	{
	public:
		CGUITiledMapObjectGroup();
		~CGUITiledMapObjectGroup();

		const CGUIString& GetName() const;
		const CGUITiledMapObjectInfo* GetObjectInfo( const CGUIString& rObjectName ) const;
		const CGUIString* GetProperty( const CGUIString& rPropertyName ) const;

	protected:
		friend class CGUITiledMapInfo;

		CGUIString m_strGroupName;
		CGUIIntVector2 m_aPositionOffset;
		std::vector<CGUITiledMapObjectInfo> m_vObjects;
		std::map<CGUIString,CGUIString>	m_mapProperties;
	};

	/* CGUITiledMapTilesetInfo contains the information about the tilesets like:
	- Tileset name
	- Tilset spacing
	- Tileset margin
	- size of the tiles
	- Image used for the tiles
	- Image size

	This information is obtained from the TMX file. 
	*/
	class CGUITiledMapTilesetInfo
	{
	public:
		CGUITiledMapTilesetInfo();
		~CGUITiledMapTilesetInfo();

		CGUIRect RectForGID( uint32 gid, const CGUIIntSize& rImageSize ) const;
		const CGUIIntSize& GetImageSize() const;
		const CGUIIntSize& GetTileSize() const;

		uint32 GetFirstGid() const;
		const CGUIString& GetSourceImage() const;

	protected:
		friend class CGUITiledMapInfo;

		CGUIString m_strName;
		uint32 m_nFirstGid;
		uint32 m_nGidNum;
		CGUIIntSize m_aTileSize;
		uint32 m_nSpacing;
		uint32 m_nMargin;
		CGUIString m_strSourceImage; // image containing the tiles
		CGUIIntSize m_aImageSize;
	};


	/* CGUITiledMapInfo contains the information about the map like:
	- Map orientation (hexagonal, isometric or orthogonal)
	- Tile size
	- Map size

	And it also contains:
	- Layers (an array of TMXLayerInfo objects)
	- Tilesets (an array of TMXTilesetInfo objects)
	- ObjectGroups (an array of TMXObjectGroupInfo objects)

	This information is obtained from the TMX file.
	*/
	class CGUITiledMapInfo
	{	
	public:	
		CGUITiledMapInfo();
		~CGUITiledMapInfo();

		int32 InitWithTMXFile( const CGUIString& tmxFile );

		std::vector<CGUITiledMapLayerInfo>& GetLayerInfos();
		CGUITiledMapLayerInfo* GetLayerInfo( const CGUIString& rLayerName );

		const CGUIIntSize& GetMapSize() const;
		const CGUIIntSize& GetTileSize() const;
		ETMXOrientation GetOrientation() const;
		std::vector<CGUITiledMapTilesetInfo>& GetTilesets();
		const std::vector<CGUITiledMapObjectGroup>& GetObjectGroups() const;
		const std::map<CGUIString, CGUIString>& GetProperties() const;
		const std::map<uint32, std::map<CGUIString, CGUIString> >& GetAllTileProperties() const;

	protected:
		int32 ParseXMLFile( const CGUIString& tmxFile );
		int32 ParseNode_map( TiXmlElement* pMapNode );
		int32 ParseNode_tileset( TiXmlElement* pTilesetNode );
		int32 ParseNode_layer( TiXmlElement* pLayerNode );
		int32 ParseNode_image( TiXmlElement* pImageNode );
		int32 ParseNode_tile( TiXmlElement* pTileNode );
		int32 ParseNode_properties( TiXmlElement* pPropertiesNode, std::map<CGUIString, CGUIString>& mapTileProperties );
		int32 ParseNode_data( TiXmlElement* pTileNode );
		int32 ParseNode_objectgroup( TiXmlElement* pObjectGroupNode );
		int32 ParseNode_object( TiXmlElement* pObjectNode );

		int32 ParseSourceFile( const CGUIString& tmxFile );

	protected:
		uint32 m_uParentGID;
		CGUIString m_strFilename; // tmx filename
		ETMXOrientation m_eOrientation; // map orientation
		CGUIIntSize m_aMapSize; // map width & height
		CGUIIntSize m_aTileSize; // tiles width & height
		std::vector<CGUITiledMapLayerInfo> m_vLayers; // Layers
		std::vector<CGUITiledMapTilesetInfo> m_vTilesets; // tilesets
		std::vector<CGUITiledMapObjectGroup> m_vObjectGroups; // ObjectGroups
		std::map<CGUIString, CGUIString> m_mapProperties; // properties
		std::map<uint32, std::map<CGUIString, CGUIString> > m_mapTileProperties; // tile properties <gid, tile property>
	};

} //namespace guiex

#endif //__KEN_TILEDMAPPARSER_20110127_H__

