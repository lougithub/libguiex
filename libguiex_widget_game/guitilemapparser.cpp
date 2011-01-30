/** 
* @file guitilemapparser.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
//============================================================================//
// include
//============================================================================//
#include "guitilemapparser.h"
#include "guitilemap.h"
#include <libguiex_core/base64.h>
#include <tinyxml.h>


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	// CCTMXLayerInfo
	//------------------------------------------------------------------------------
	CCTMXLayerInfo::CCTMXLayerInfo()
		:ownTiles( true ),
		,minGID( 100000 )
		,maxGID( 0 )
		,tiles( NULL );
		,offset( 0.0f,0.0f )
		,opacity( 255 )
	{

	}
	//------------------------------------------------------------------------------
	CCTMXLayerInfo::~CCTMXLayerInfo()
	{
		if( ownTiles && tiles )
		{
			free( tiles );
			tiles = NULL;
		}
	}



	//------------------------------------------------------------------------------
	// CCTMXObjectGroup
	//------------------------------------------------------------------------------
	CCTMXObjectGroup::CCTMXObjectGroup()
	{

	}
	//------------------------------------------------------------------------------
	CCTMXObjectGroup::~CCTMXObjectGroup()
	{
		for( size_t i=0; i<objects.size(); ++i )
		{
			delete objects[i];
		}
		objects.clear();
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CCTMXTilesetInfo
	//------------------------------------------------------------------------------
	CCTMXTilesetInfo::CCTMXTilesetInfo()
		:firstGid( 0 )
		,spacing( 0 )
		,margin( 0 )
		,tileSize( 0.0f,0.0f )
		,imageSize( 0.0f, 0.0f )
	{
	}
	//------------------------------------------------------------------------------
	CCTMXTilesetInfo::~CCTMXTilesetInfo()
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect CCTMXTilesetInfo::RectForGID( uint32 gid )
	{
		gid = gid - firstGid;

		real max_x = ( imageSize.m_fWidth - margin*2 + spacing ) / ( tileSize.m_fWidth + spacing );

		CGUIVector2 aTopLeft;
		aTopLeft.x = (gid % max_x) * (tileSize.m_fWidth + spacing) + margin; //column
		aTopLeft.y = (gid / max_x) * (tileSize.m_fHeight + spacing) + margin; //row
		
		return CGUIRect( aTopLeft, tileSize );
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CCTMXMapInfo
	//------------------------------------------------------------------------------
	CCTMXMapInfo::CCTMXMapInfo()
		:storingCharacters( false )
		,layerAttribs( 0 )
		,parentElement( 0 )
		,parentGID( 0 )
		,orientation( 0 ) // map orientation
	{
	}
	//------------------------------------------------------------------------------
	CCTMXMapInfo::~CCTMXMapInfo()
	{
		for( size_t i=0; i<tilesets.size(); ++i )
		{
			delete tilesets[i];
		}
		tilesets.clear();

		for( size_t i=0; i<layers.size(); ++i )
		{
			delete layers[i];
		}
		layers.clear();

		for( size_t i=0; i<objectGroups.size(); ++i )
		{
			delete objectGroups[i];
		}
		objectGroups.clear();
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::InitWithTMXFile( const CGUIString& tmxFile )
	{
		filename = tmxFile;
		storingCharacters = false;
		layerAttribs = TMXLayerAttribNone;
		parentElement = TMXPropertyNone;

		return ParseXMLFile( filename );		
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseXMLFile( const CGUIString& tmxFile )
	{
		/*
		<map version="1.0" orientation="orthogonal" width="64" height="64" tilewidth="32" tileheight="32">
			<tileset firstgid="1" name="tile 0" tilewidth="32" tileheight="32" spacing="2" margin="2">
				<image source="fixed-ortho-test2.png"/>
				<tile id="0">
					<properties>
						<property name="type" value="object"/>
					</properties>
				</tile>
			</tileset>
			<layer name="Layer 0" width="64" height="64">
				<data encoding="base64" compression="gzip">
					H4sIAAAAAAAAA+2bS2/UMBDHDSyUHpBgOZVXV0ALEqKwS8sRhLj0CILyBeAAJ7gARygPiSLBhcciwZX3Q3CCG3w0ZtS1mJixM7GdxFHyk/7aJHKS+cePOLZXqY6Ojo7/WQM9rjuIGvkN+lN3ECXTA20WqldTjB3VMGsIGRlClgwhQ0bIgFGqzDLHRsz2EpNuaNkfqLQ9U8rwrxmo9J9F28t/RwfHNCNkm6G2QP1T6P4T0NNqwqkcif8qKftZu8o/Req/aWVj2nI8z/95h5qEr//DDjWJEP8nVNb3omqmf5/3X5Py/xPoM9GXCNfUXi8Rpeq/DHzz/z7oQbmhJc1P0K+6g5jgWy/2gPaC9nncMyX/viyAjoGO56T7CvpmHHuvmu8fuSBIw40xp57/30E/VP78wBljfztoB9mndYvWq9T9a2zzA3rsSOf/YPK7H3RAcF30/yEksBLY6pA0HZeeI8X8l77LXemkfZ9noOegF4xeBvrwxeXnImhFZft3If6blv8uf2OPe6XoX6PbfgljoiKk7F/KmJGUOtv/66AbgdfgvBd5BnXmP/Y9Yz372OW/inHDt6B3ka7VxPqP/m3vXono+7lK/yHfm5QU85/Op9qQfm/mEdO/C1xPs6Wg8pB8b+ZRlf8y8PF/B3SXaBV0T3BekX5QVcTI/5jfHj59XxebGFFilv+PnqqTGP5129tG/xsM+fivem53Y46KUMSnbv9Syv9QfMt82/3rMt9W/23J/z7Z1vM2If5d85x1rJGR+r+m/o394/6IpFkscL/Y37mhYzcS/z1G+G0ypdbH9m3/EbgMugK6So6hf66v+ZA5vy+IP/R5uto17X+OaOfkVzK+fRN0C3TbES9X5vWxg4L4Y/s3hTGg33mVfQ7S8f1Hlngldf2kIH7J83Shfc6AdoF2q6x/jGGOkfZ9SGX9m/en/kckXm7dvXnuMtk2v1m1VlWc/D8COg06qrL+l+2niqD+pxi5KFr/R66EFqjXU4pv/0JA/68m20XnxfLqv7lOxWcNV55/SRuEvAa9YY7T/Ee/8ypbd1xg3VtT5f4fMc+/pA1C6LoAur5F+8dxkSL5j2so+4pfb+BTzjnwfyzU6zm13i6Z9X9guTfGqPs+NE66bea/1L/vmgLuf0omQ+H1Y2llIts8OqezAvnG8xeas1zUAEAAAA==
				</data>
			</layer>
			<objectgroup color="#001ca4" name="Object Group 1" width="0" height="0">
				<object name="platform 1" x="0" y="290" width="32" height="30"/>
				<object name="" x="0" y="3" width="31" height="32"/>
				<object name="" x="130" y="162" width="29" height="29"/>
				<object name="" x="290" y="290" width="28" height="29"/>
			</objectgroup>
		</map>
		*/
		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( tmxFile, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			throw CGUIException("[CCTMXMapInfo::ParseXMLFile]: failed to read file <%s>!", tmxFile.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[CCTMXMapInfo::ParseXMLFile]: failed to parse file <%s>!\n\n<%s>", 
				tmxFile.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[CCTMXMapInfo::ParseXMLFile], failed to get root node from file <%s>!", tmxFile.c_str());
			return -1;
		}

		//parse node attribute
		if( 0 != ParseNode_map( pRootNode ) )
		{
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_map( class TiXmlElement* pMapNode )
	{
		//version
		CGUIString strVersion = pMapNode->Attribute("version");
		if( strVersion != "1.0" )
		{
			throw CGUIException("[CCTMXMapInfo::ParseNode_map]: Unsupported TMX version %s", strVersion->c_str());
			return -1;
		}

		//orientation
		CGUIString strOrientation = pMapNode->Attribute("orientation");
		if( strOrientation == "orthogonal" )
		{
			orientation = CCTMXOrientationOrtho;
		}
		else if( strOrientation == "isometric")
		{
			orientation = CCTMXOrientationIso;
		}
		else if( strOrientation == "hexagonal")
		{
			orientation = CCTMXOrientationHex;
		}
		else
		{
			throw CGUIException( "[CCTMXMapInfo::ParseNode_map]: Unsupported orientation: %s", strOrientation.c_str() );
			return -1;
		}

		//width and height
		CGUIString strWidth = pMapNode->Attribute("width");
		StringToValue( strWidth, mapSize.m_fWidth );
		CGUIString strHeight = pMapNode->Attribute("height");
		StringToValue( strHeight, mapSize.m_fHeight );

		//tile width and height
		CGUIString strTileWidth = pMapNode->Attribute("tilewidth");
		StringToValue( strTileWidth, tileSize.m_fWidth );
		CGUIString strTileHeight = pMapNode->Attribute("tileheight");
		StringToValue( strTileHeight, tileSize.m_fHeight );


		//parse child node
		TiXmlElement* pChildNode = pMapNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("tileset") == pChildNode->Value())
			{
				if( 0 != ParseNode_tileset( pChildNode ) )
				{
					return -1;
				}
			}
			else if( CGUIString("layer") == pChildNode->Value())
			{
				if( 0 != ParseNode_layer( pChildNode ) )
				{
					return -1;
				}
			}
			else if( CGUIString("objectgroup") == pChildNode->Value())
			{
				if( 0 != ParseNode_objectgroup( pChildNode ) )
				{
					return -1;
				}
			}
			else if( CGUIString("properties" ) == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseXMLFile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseSourceFile( const CGUIString& tmxFile )
	{
		throw CGUIException( "[CCTMXMapInfo::ParseSourceFile]: unsupport to parse source file now");
		return -1;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_tileset( class TiXmlElement* pTilesetNode )
	{
		//tile width and height
		const char* szSource = pTilesetNode->Attribute("source");
		if( szSource )
		{
			if( 0 != ParseSourceFile( szSource ) )
			{
				return -1;
			}
			return 0;
		}

		CCTMXTilesetInfo *pTileset = new CCTMXTilesetInfo;
		tilesets.push_back( pTileset );

		//name
		CGUIString strName = pTilesetNode->Attribute("name");
		pTileset->name = strName;

		//first gid
		CGUIString strGid = pTilesetNode->Attribute( "firstgid");
		StringToValue( strGid, pTileset.firstGid );

		//spacing
		CGUIString strSpacing = pTilesetNode->Attribute( "spacing");
		StringToValue( strSpacing, pTileset.spacing );

		//margin
		CGUIString strMargin = pTilesetNode->Attribute( "margin");
		StringToValue( strMargin, pTileset.margin );

		//size
		CGUIString strWidth = pTilesetNode->Attribute( "tilewidth");
		StringToValue( strWidth, pTileset->tileSize.m_fWidth );
		CGUIString strHeight = pTilesetNode->Attribute( "tileheight");
		StringToValue( strHeight, pTileset->tileSize.m_fHeight );

		//parse child node
		TiXmlElement* pChildNode = pTilesetNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("image") == pChildNode->Value())
			{
				if( 0 != ParseNode_image( pChildNode ) )
				{
					return -1;
				}
			}
			else if( CGUIString("tile") == pChildNode->Value())
			{
				if( 0 != ParseNode_tile( pChildNode ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseXMLFile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_image( class TiXmlElement* pImageNode )
	{
		if( tilesets.empty() )
		{
			throw CGUIException("[CCTMXMapInfo::ParseNode_tile]: not find tileset when process tile node" );
			return -1;
		}
		CCTMXTilesetInfo* info = tilesets.back();

		CGUIString strSource = pImageNode->Attribute("source");
		info->sourceImage = strSource;

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_tile( class TiXmlElement* pTileNode )
	{
		if( tilesets.empty() )
		{
			throw CGUIException("[CCTMXMapInfo::ParseNode_tile]: not find tileset when process tile node" );
			return -1;
		}
		CCTMXTilesetInfo* info = tilesets.back();

		uint32 uID = 0;
		CGUIString strID = pTileNode->Attribute( "id" );
		StringToValue( strID, uID );
		uint32 uParentID = info->firstGid + uID;

		std::map<CGUIString, CGUIString> mapTileProperties;

		//parse child node
		TiXmlElement* pChildNode = pTileNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("properties") == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, mapTileProperties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseNode_tile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}
			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		tileProperties.insert( std::make_pair( uParentID, mapTileProperties ) );

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_properties( class TiXmlElement* pPropertiesNode, std::map<CGUIString, CGUIString>& mapTileProperties )
	{
		//parse child node
		TiXmlElement* pChildNode = pPropertiesNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("property") == pChildNode->Value())
			{
				CGUIString strName = pChildNode->Attribute("name");
				CGUIString strValue = pChildNode->Attribute("value");
				mapTileProperties.insert( std::make_pair( strName, strValue ) );
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseNode_tile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_layer( class TiXmlElement* pLayerNode )
	{
		CCTMXLayerInfo *layer = new CCTMXLayerInfo;
		layers.push_back( layer );

		//name
		layer->name = pLayerNode->Attribute( "layer" );

		//size
		CGUIString strWidth = pLayerNode->Attribute("width");
		StringToValue( strWidth, layer->layerSize.m_fWidth );
		CGUIString strHeight = pLayerNode->Attribute("height");
		StringToValue( strHeight, layer->layerSize.m_fHeight );

		//visible
		CGUIString strVisible = pLayerNode->Attribute("visible");
		StringToValue( strVisible, layer->visible );

		//opacity
		const char* szOpacity = pLayerNode->Attribute("opacity");
		if( !szOpacity )
		{
			layer->opacity = 1.0f;
			layer->visible = false;
		}
		else
		{
			StringToValue( szOpacity, layer->opacity );
			if( szOpacity == "0" )
			{
				layer->visible = false;
			}
			else
			{
				layer->visible = true;
			}
		}

		//offset
		const char* szX = pLayerNode->Attribute("x");
		const char* szY = pLayerNode->Attribute("y");
		layer->offset.x = layer->offset.y = 0;
		if( szX )
		{
			StringToValue( szX, layer->offset.x );
		}
		if( szY )
		{
			StringToValue( szY, layer->offset.y );
		}

		//parse child node
		TiXmlElement* pChildNode = pLayerNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("data") == pChildNode->Value())
			{
				if( 0 != ParseNode_data( pChildNode ) )
				{
					return -1;
				}
			}
			if( CGUIString("properties") == pChildNode->Value())
			{
				if( 0 != ParseNode_data( pChildNode, layer->properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseNode_layer]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_data( class TiXmlElement* pDataNode )
	{
		CGUIString strEncoding = pDataNode->Attribute("encoding");
		CGUIString strCompression = pDataNode->Attribute("compression");

		if( strEncoding == "base64" )
		{
			layerAttribs |= TMXLayerAttribBase64;
		}
		if( strCompression == "gzip" )
		{
			layerAttribs |= TMXLayerAttribGzip;
		}
		GUI_ASSERT( layerAttribs != TMXLayerAttribNone, "[CCTMXMapInfo::ParseNode_data]: Only base64 and/or gzip maps are supported" );


		const char* data = pDataNode->GetText();

		if( data && layerAttribs & TMXLayerAttribBase64 )
		{
			if( layers.empty() )
			{
				throw CGUIException("[CCTMXMapInfo::ParseNode_data]: not find layers when process data node" );
				return -1;
			}
			CCTMXLayerInfo* layer = layerAttribs.back();

			unsigned char *buffer = NULL;
			int len = 0;
			len = base64Decode( data, strlen(data), &buffer);
			if( !buffer )
			{
				throw CGUIException("[CCTMXMapInfo::ParseNode_data]: TiledMap decode data error");
				return -1;
			}

		}

		return 0;




			//int len = 0;

		if([elementName isEqualToString:@"data"] && layerAttribs&TMXLayerAttribBase64) 
		{
			storingCharacters = NO;




			if( layerAttribs & TMXLayerAttribGzip ) 
			{
				unsigned char *deflated;
				ccInflateMemory(buffer, len, &deflated);
				free( buffer );

				if( ! deflated )
				{
					CCLOG(@"cocos2d: TiledMap: inflate data error");
					return;
				}

				layer.tiles = (unsigned int*) deflated;
			} else
				layer.tiles = (unsigned int*) buffer;

			//	[currentString setString:@""];
	} 



	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_objectgroup( class TiXmlElement* pObjectGroupNode )
	{
		CCTMXObjectGroup *objectGroup = new CCTMXObjectGroup;
		objectGroups->push_back( objectGroup );

		//name
		objectGroup->groupName = pObjectGroupNode->Attribute( "name" );

		//offset
		CGUIString strX = pObjectGroupNode->Attribute( "x" );
		StringToValue( strX, objectGroup->positionOffset.x );
		CGUIString strY = pObjectGroupNode->Attribute( "y" );
		StringToValue( strY, objectGroup->positionOffset.y );

		//parse child node
		TiXmlElement* pChildNode = pObjectGroupNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("object") == pChildNode->Value())
			{
				if( 0 != ParseNode_object( pChildNode ))
				{
					return -1;
				}
			}
			else if( CGUIString("properties" ) == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, objectGroup->properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseNode_objectgroup]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseNode_object( class TiXmlElement* pObjectNode )
	{
		if( objectGroups.empty() )
		{
			throw CGUIException("[CCTMXMapInfo::ParseNode_object]: not find object group when process objectgroup node" );
			return -1;
		}
		CCTMXObjectGroup* objectGroup = objectGroups.back();

		CCTMXObjectInfo* pInfo = new CCTMXObjectInfo;
		objectGroup->objects.push_back( pInfo );

		//name
		pInfo->objectName = pObjectNode->Attribute("name");

		//type
		const char* szType = pObjectNode->Attribute("type");
		if( szType )
		{
			pInfo->objectType = szType;
		}

		//size
		CGUIString strWidth = pObjectNode->Attribute( "width");
		StringToValue( strWidth, pInfo->size.m_fWidth );
		CGUIString strHeight = pObjectNode->Attribute( "height");
		StringToValue( strHeight, pInfo->size.m_fHeight );

		//position
		CGUIString strX = pObjectNode->Attribute("x");
		StringToValue( strX, pInfo->position.x );
		pInfo->position.x += objectGroup->positionOffset.x;
		CGUIString strY = pObjectNode->Attribute("y");
		StringToValue( strY, pInfo->position.y );
		pInfo->position.y += objectGroup->positionOffset.y;
		// Correct y position. (Tiled uses Flipped, we uses Standard)
		pInfo->position.y = (mapSize.m_fHeight * tileSize.m_fHeight) - pInfo->position.y - pInfo->size.m_fHeight;

		//parse child node
		TiXmlElement* pChildNode = pObjectNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("properties" ) == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, pInfo->properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CCTMXMapInfo::ParseNode_object]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex

