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
#include "guiexception.h"
#include "guiinterfacefilesys.h"
#include "guistringconvertor.h"
#include "guiinterfacemanager.h"
#include "base64.h"

#include <zlib.h>
#include <tinyxml.h>


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	#define BUFFER_INC_FACTOR (2)
	static int inflateMemory_(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength)
	{
		/* ret value */
		int err = Z_OK;

		/* 256k initial decompress buffer */
		int bufferSize = 256 * 1024;
		*out = (unsigned char*) malloc(bufferSize);

		z_stream d_stream; /* decompression stream */	
		d_stream.zalloc = (alloc_func)0;
		d_stream.zfree = (free_func)0;
		d_stream.opaque = (voidpf)0;

		d_stream.next_in  = in;
		d_stream.avail_in = inLength;
		d_stream.next_out = *out;
		d_stream.avail_out = bufferSize;

		/* window size to hold 256k */
		if( (err = inflateInit2(&d_stream, 15 + 32)) != Z_OK )
		{
			return err;
		}

		for (;;) 
		{
			err = inflate(&d_stream, Z_NO_FLUSH);

			if (err == Z_STREAM_END)
			{
				break;
			}

			switch (err) 
			{
			case Z_NEED_DICT:
				err = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&d_stream);
				return err;
			}

			// not enough memory ?
			if (err != Z_STREAM_END) 
			{

				unsigned char *tmp = (unsigned char *)realloc(*out, bufferSize * BUFFER_INC_FACTOR);

				/* not enough memory, ouch */
				if (!tmp )
				{
					inflateEnd(&d_stream);
					throw CGUIException("[inflateMemory_]: realloc failed");
					return Z_MEM_ERROR;
				}
				/* only assign to *out if tmp is valid. it's not guaranteed that realloc will reuse the memory */
				*out = tmp;

				d_stream.next_out = *out + bufferSize;
				d_stream.avail_out = bufferSize;
				bufferSize *= BUFFER_INC_FACTOR;
			}
		}

		*outLength = bufferSize - d_stream.avail_out;
		err = inflateEnd(&d_stream);
		return err;
	}
	//------------------------------------------------------------------------------
	static int InflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out)
	{
		unsigned int outLength = 0;
		int err = inflateMemory_(in, inLength, out, &outLength);

		if (err != Z_OK || *out == NULL)
		{
			free(*out);
			*out = NULL;
			outLength = 0;
			
			switch(err)
			{
			case Z_MEM_ERROR:
				throw CGUIException("[InflateMemory]: Out of memory while decompressing map data!");
				break;
			case Z_VERSION_ERROR:
				throw CGUIException("[InflateMemory]: Incompatible zlib version!");
				break;
			case Z_DATA_ERROR:
				throw CGUIException("[InflateMemory]: Incorrect zlib compressed data!");
				break;
			default:
				throw CGUIException("[InflateMemory]: Unknown error while decompressing map data!");
				break;
			}
		}

		return outLength;
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CGUITileMapLayerInfo
	//------------------------------------------------------------------------------
	CGUITileMapLayerInfo::CGUITileMapLayerInfo()
		:ownTiles( true )
		,opacity( 1.0f )
	{

	}
	//------------------------------------------------------------------------------
	CGUITileMapLayerInfo::~CGUITileMapLayerInfo()
	{
	}

	//------------------------------------------------------------------------------
	// CGUITileMapObjectGroup
	//------------------------------------------------------------------------------
	CGUITileMapObjectGroup::CGUITileMapObjectGroup()
	{

	}
	//------------------------------------------------------------------------------
	CGUITileMapObjectGroup::~CGUITileMapObjectGroup()
	{
	}
	//------------------------------------------------------------------------------
	const CGUITileMapObjectInfo* CGUITileMapObjectGroup::GetObjectInfo( const CGUIString& rObjectName ) const
	{
		for( std::vector<CGUITileMapObjectInfo>::const_iterator itor = objects.begin();
			itor != objects.end();
			++itor )
		{
			if((*itor).objectName == rObjectName )
			{
				return &(*itor);
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIString* CGUITileMapObjectGroup::GetProperty( const CGUIString& rPropertyName ) const
	{
		std::map<CGUIString, CGUIString>::const_iterator itorFind = properties.find( rPropertyName );
		if( itorFind == properties.end() )
		{
			return NULL;
		}
		else
		{
			return &itorFind->second;
		}
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CGUITileMapTilesetInfo
	//------------------------------------------------------------------------------
	CGUITileMapTilesetInfo::CGUITileMapTilesetInfo()
		:firstGid( 0 )
		,spacing( 0 )
		,margin( 0 )
	{
	}
	//------------------------------------------------------------------------------
	CGUITileMapTilesetInfo::~CGUITileMapTilesetInfo()
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUITileMapTilesetInfo::RectForGID( uint32 gid, const CGUIIntSize& rImageSize ) const
	{
		gid = gid - firstGid;
		uint32 max_x = ( rImageSize.m_uWidth - margin*2 + spacing ) / ( tileSize.m_uWidth + spacing );

		CGUIVector2 aTopLeft;
		aTopLeft.x = real((gid % max_x) * (tileSize.m_uWidth + spacing) + margin) / rImageSize.m_uWidth; //column
		aTopLeft.y = real((gid / max_x) * (tileSize.m_uHeight + spacing) + margin)/ rImageSize.m_uHeight; //row
		CGUISize aSize(
			real(tileSize.m_uWidth) / rImageSize.m_uWidth,
			real(tileSize.m_uHeight) / rImageSize.m_uHeight
			);
		return CGUIRect( aTopLeft, aSize );
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CGUITileMapInfo
	//------------------------------------------------------------------------------
	CGUITileMapInfo::CGUITileMapInfo()
		:parentGID( 0 )
		,orientation( CCTMXOrientationOrtho ) // map orientation
	{
	}
	//------------------------------------------------------------------------------
	CGUITileMapInfo::~CGUITileMapInfo()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::InitWithTMXFile( const CGUIString& tmxFile )
	{
		filename = tmxFile;

		return ParseXMLFile( filename );		
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseXMLFile( const CGUIString& tmxFile )
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
			throw CGUIException("[CGUITileMapInfo::ParseXMLFile]: failed to read file <%s>!", tmxFile.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[CGUITileMapInfo::ParseXMLFile]: failed to parse file <%s>!\n\n<%s>", 
				tmxFile.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[CGUITileMapInfo::ParseXMLFile], failed to get root node from file <%s>!", tmxFile.c_str());
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
	int32 CGUITileMapInfo::ParseNode_map( TiXmlElement* pMapNode )
	{
		//version
		CGUIString strVersion = pMapNode->Attribute("version");
		if( strVersion != "1.0" )
		{
			throw CGUIException("[CGUITileMapInfo::ParseNode_map]: Unsupported TMX version %s", strVersion.c_str());
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
			throw CGUIException( "[CGUITileMapInfo::ParseNode_map]: Unsupported orientation: %s", strOrientation.c_str() );
			return -1;
		}

		//width and height
		CGUIString strWidth = pMapNode->Attribute("width");
		StringToValue( strWidth, mapSize.m_uWidth );
		CGUIString strHeight = pMapNode->Attribute("height");
		StringToValue( strHeight, mapSize.m_uHeight );

		//tile width and height
		CGUIString strTileWidth = pMapNode->Attribute("tilewidth");
		StringToValue( strTileWidth, tileSize.m_uWidth );
		CGUIString strTileHeight = pMapNode->Attribute("tileheight");
		StringToValue( strTileHeight, tileSize.m_uHeight );


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
				throw CGUIException( "[CGUITileMapInfo::ParseXMLFile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseSourceFile( const CGUIString& tmxFile )
	{
		throw CGUIException( "[CGUITileMapInfo::ParseSourceFile]: unsupport to parse source file now");
		return -1;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_tileset( TiXmlElement* pTilesetNode )
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

		tilesets.push_back( CGUITileMapTilesetInfo() );
		CGUITileMapTilesetInfo& tileset = tilesets.back();

		//name
		CGUIString strName = pTilesetNode->Attribute("name");
		tileset.name = strName;

		//first gid
		CGUIString strGid = pTilesetNode->Attribute( "firstgid");
		StringToValue( strGid, tileset.firstGid );

		//spacing
		CGUIString strSpacing = pTilesetNode->Attribute( "spacing");
		StringToValue( strSpacing, tileset.spacing );

		//margin
		CGUIString strMargin = pTilesetNode->Attribute( "margin");
		StringToValue( strMargin, tileset.margin );

		//size
		CGUIString strWidth = pTilesetNode->Attribute( "tilewidth");
		StringToValue( strWidth, tileset.tileSize.m_uWidth );
		CGUIString strHeight = pTilesetNode->Attribute( "tileheight");
		StringToValue( strHeight, tileset.tileSize.m_uHeight );

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
				throw CGUIException( "[CGUITileMapInfo::ParseXMLFile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_image( TiXmlElement* pImageNode )
	{
		if( tilesets.empty() )
		{
			throw CGUIException("[CGUITileMapInfo::ParseNode_tile]: not find tileset when process tile node" );
			return -1;
		}
		CGUITileMapTilesetInfo& info = tilesets.back();

		CGUIString strSource = pImageNode->Attribute("source");
		info.sourceImage = strSource;

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_tile( TiXmlElement* pTileNode )
	{
		if( tilesets.empty() )
		{
			throw CGUIException("[CGUITileMapInfo::ParseNode_tile]: not find tileset when process tile node" );
			return -1;
		}
		CGUITileMapTilesetInfo& info = tilesets.back();

		uint32 uID = 0;
		CGUIString strID = pTileNode->Attribute( "id" );
		StringToValue( strID, uID );
		uint32 uParentID = info.firstGid + uID;

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
				throw CGUIException( "[CGUITileMapInfo::ParseNode_tile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}
			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		tileProperties.insert( std::make_pair( uParentID, mapTileProperties ) );

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_properties( TiXmlElement* pPropertiesNode, std::map<CGUIString, CGUIString>& mapTileProperties )
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
				throw CGUIException( "[CGUITileMapInfo::ParseNode_tile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_layer( TiXmlElement* pLayerNode )
	{
		layers.push_back( CGUITileMapLayerInfo() );
		CGUITileMapLayerInfo &layer = layers.back();

		//name
		layer.name = pLayerNode->Attribute( "name" );

		//size
		CGUIString strWidth = pLayerNode->Attribute("width");
		StringToValue( strWidth, layer.layerSize.m_uWidth );
		CGUIString strHeight = pLayerNode->Attribute("height");
		StringToValue( strHeight, layer.layerSize.m_uHeight );

		//visible
		const char* szVisible = pLayerNode->Attribute("visible");
		if( szVisible )
		{
			StringToValue( szVisible, layer.visible );
		}
		else
		{
			layer.visible = true;
		}

		//opacity
		const char* szOpacity = pLayerNode->Attribute("opacity");
		if( !szOpacity )
		{
			layer.opacity = 1.0f;
			layer.visible = false;
		}
		else
		{
			StringToValue( szOpacity, layer.opacity );
			if( layer.opacity == 0.0f )
			{
				layer.visible = false;
			}
			else
			{
				layer.visible = true;
			}
		}

		//offset
		const char* szX = pLayerNode->Attribute("x");
		const char* szY = pLayerNode->Attribute("y");
		layer.offset.x = layer.offset.y = 0;
		if( szX )
		{
			StringToValue( szX, layer.offset.x );
		}
		if( szY )
		{
			StringToValue( szY, layer.offset.y );
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
			else if( CGUIString("properties") == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, layer.properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CGUITileMapInfo::ParseNode_layer]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_data( TiXmlElement* pDataNode )
	{
		CGUIString strEncoding = pDataNode->Attribute("encoding");
		CGUIString strCompression = pDataNode->Attribute("compression");

		if( strEncoding != "base64" )
		{
			throw CGUIException("[CGUITileMapInfo::ParseNode_data]: date only support base64 now");
			return -1;
		}
		const char* data = pDataNode->GetText();

		if( data )
		{
			if( layers.empty() )
			{
				throw CGUIException("[CGUITileMapInfo::ParseNode_data]: not find layers when process data node" );
				return -1;
			}
			CGUITileMapLayerInfo& layer = layers.back();

			unsigned char *buffer = NULL;
			int len = 0;
			len = base64Decode( (const unsigned char*)data, strlen(data), &buffer);
			if( !buffer )
			{
				throw CGUIException("[CGUITileMapInfo::ParseNode_data]: TiledMap decode data error");
				return -1;
			}

			uint32* pTiles = NULL;
			int nTileNum = 0;
			if( strCompression == "gzip" || strCompression == "zlib" )
			{
				unsigned char *deflated;
				int outlength = InflateMemory(buffer, len, &deflated);
				free( buffer );

				if( !deflated )
				{
					throw CGUIException("[CGUITileMapInfo::ParseNode_data]: inflate data error");
					return -1;
				}

				pTiles = (uint32*) deflated;
				nTileNum = outlength / 4;
			} 
			else
			{
				pTiles = (uint32*) buffer;
				nTileNum = len / 4;
			}
			if( nTileNum <= 0 )
			{
				free( pTiles );
				throw CGUIException("[CGUITileMapInfo::ParseNode_data]: invalid tile num");
				return -1;
			}
			layer.tiles.resize( nTileNum, 0 );
			for( int i=0; i<nTileNum; ++i )
			{
				// gid are stored in little endian.
				// if host is big endian, then swap
#if GUI_ENDIAN == GUI_ENDIAN_BIG
				pTiles[i] = ((pTiles[i] & 0xFF) << 24) | ((pTiles[i] & 0xFF00) << 8) | ((pTiles[i] >> 8) & 0xFF00) | ((pTiles[i] >> 24) & 0xFF);
#endif	//#if GUI_ENDIAN == GUI_ENDIAN_BIG
				layer.tiles[i] = pTiles[i];
			}
			free( pTiles );
		}

		return 0;
	} 
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_objectgroup( TiXmlElement* pObjectGroupNode )
	{
		objectGroups.push_back( CGUITileMapObjectGroup() );
		CGUITileMapObjectGroup& objectGroup = objectGroups.back();

		//name
		objectGroup.groupName = pObjectGroupNode->Attribute( "name" );

		//offset
		CGUIString strX = pObjectGroupNode->Attribute( "x" );
		StringToValue( strX, objectGroup.positionOffset.x );
		CGUIString strY = pObjectGroupNode->Attribute( "y" );
		StringToValue( strY, objectGroup.positionOffset.y );

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
				if( 0 != ParseNode_properties( pChildNode, objectGroup.properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CGUITileMapInfo::ParseNode_objectgroup]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapInfo::ParseNode_object( TiXmlElement* pObjectNode )
	{
		if( objectGroups.empty() )
		{
			throw CGUIException("[CGUITileMapInfo::ParseNode_object]: not find object group when process objectgroup node" );
			return -1;
		}
		CGUITileMapObjectGroup& objectGroup = objectGroups.back();

		objectGroup.objects.push_back( CGUITileMapObjectInfo() );
		CGUITileMapObjectInfo& object = objectGroup.objects.back();

		//name
		object.objectName = pObjectNode->Attribute("name");

		//type
		const char* szType = pObjectNode->Attribute("type");
		if( szType )
		{
			object.objectType = szType;
		}

		//size
		CGUIString strWidth = pObjectNode->Attribute( "width");
		StringToValue( strWidth, object.size.m_uWidth );
		CGUIString strHeight = pObjectNode->Attribute( "height");
		StringToValue( strHeight, object.size.m_uHeight );

		//position
		CGUIString strX = pObjectNode->Attribute("x");
		StringToValue( strX, object.position.x );
		object.position.x += objectGroup.positionOffset.x;
		CGUIString strY = pObjectNode->Attribute("y");
		StringToValue( strY, object.position.y );
		object.position.y += objectGroup.positionOffset.y;
		// Correct y position. (Tiled uses Flipped, we uses Standard)
		object.position.y = (mapSize.m_uHeight * tileSize.m_uHeight) - object.position.y - object.size.m_uHeight;

		//parse child node
		TiXmlElement* pChildNode = pObjectNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("properties" ) == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, object.properties ) )
				{
					return -1;
				}
			}
			else
			{
				throw CGUIException( "[CGUITileMapInfo::ParseNode_object]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex

