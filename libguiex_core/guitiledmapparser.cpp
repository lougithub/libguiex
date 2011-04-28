/** 
* @file guitiledmapparser.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
//============================================================================//
// include
//============================================================================//
#include "guitiledmapparser.h"
#include "guitiledmap.h"
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
					CGUIException::ThrowException("[inflateMemory_]: realloc failed");
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
				CGUIException::ThrowException("[InflateMemory]: Out of memory while decompressing map data!");
				break;
			case Z_VERSION_ERROR:
				CGUIException::ThrowException("[InflateMemory]: Incompatible zlib version!");
				break;
			case Z_DATA_ERROR:
				CGUIException::ThrowException("[InflateMemory]: Incorrect zlib compressed data!");
				break;
			default:
				CGUIException::ThrowException("[InflateMemory]: Unknown error while decompressing map data!");
				break;
			}
		}

		return outLength;
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CGUITiledMapLayerInfo
	//------------------------------------------------------------------------------
	CGUITiledMapLayerInfo::CGUITiledMapLayerInfo()
		:m_bOwnTiles( true )
		,m_fOpacity( 1.0f )
	{

	}
	//------------------------------------------------------------------------------
	CGUITiledMapLayerInfo::~CGUITiledMapLayerInfo()
	{
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMapLayerInfo::GetName() const
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------
	bool CGUITiledMapLayerInfo::IsVisible() const
	{
		return m_bVisible;
	}
	//------------------------------------------------------------------------------
	void CGUITiledMapLayerInfo::SetVisible( bool bVisible )
	{
		m_bVisible = bVisible;
	}
	//------------------------------------------------------------------------------
	real CGUITiledMapLayerInfo::GetOpacity() const
	{
		return m_fOpacity;
	}
	//------------------------------------------------------------------------------
	const std::map<CGUIString, CGUIString>& CGUITiledMapLayerInfo::GetProperties() const
	{
		return m_mapProperties;
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CGUITiledMapObjectInfo
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMapObjectInfo::GetName() const
	{
		return m_strObjectName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMapObjectInfo::GetType() const
	{
		return m_strObjectType;
	}
	//------------------------------------------------------------------------------
	const CGUIIntVector2& CGUITiledMapObjectInfo::GetPosition() const
	{
		return m_aPosition;
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CGUITiledMapObjectGroup
	//------------------------------------------------------------------------------
	CGUITiledMapObjectGroup::CGUITiledMapObjectGroup()
	{

	}
	//------------------------------------------------------------------------------
	CGUITiledMapObjectGroup::~CGUITiledMapObjectGroup()
	{
	}
	//------------------------------------------------------------------------------
	const CGUITiledMapObjectInfo* CGUITiledMapObjectGroup::GetObjectInfo( const CGUIString& rObjectName ) const
	{
		for( std::vector<CGUITiledMapObjectInfo>::const_iterator itor = m_vObjects.begin();
			itor != m_vObjects.end();
			++itor )
		{
			if((*itor).GetName() == rObjectName )
			{
				return &(*itor);
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIString* CGUITiledMapObjectGroup::GetProperty( const CGUIString& rPropertyName ) const
	{
		std::map<CGUIString, CGUIString>::const_iterator itorFind = m_mapProperties.find( rPropertyName );
		if( itorFind == m_mapProperties.end() )
		{
			return NULL;
		}
		else
		{
			return &itorFind->second;
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMapObjectGroup::GetName() const
	{
		return m_strGroupName;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CGUITiledMapTilesetInfo
	//------------------------------------------------------------------------------
	CGUITiledMapTilesetInfo::CGUITiledMapTilesetInfo()
		:m_nFirstGid( 0 )
		,m_nGidNum( 0 )
		,m_nSpacing( 0 )
		,m_nMargin( 0 )
	{
	}
	//------------------------------------------------------------------------------
	CGUITiledMapTilesetInfo::~CGUITiledMapTilesetInfo()
	{
	}
	//------------------------------------------------------------------------------
	uint32 CGUITiledMapTilesetInfo::GetFirstGid() const
	{
		return m_nFirstGid;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMapTilesetInfo::GetSourceImage() const
	{
		return m_strSourceImage;
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUITiledMapTilesetInfo::RectForGID( uint32 gid, const CGUIIntSize& rImageSize ) const
	{
		gid = gid - m_nFirstGid;
		uint32 max_x = ( rImageSize.m_uWidth - m_nMargin*2 + m_nSpacing ) / ( m_aTileSize.m_uWidth + m_nSpacing );

		CGUIVector2 aTopLeft;
		aTopLeft.x = real((gid % max_x) * (m_aTileSize.m_uWidth + m_nSpacing) + m_nMargin) / rImageSize.m_uWidth; //column
		aTopLeft.y = real((gid / max_x) * (m_aTileSize.m_uHeight + m_nSpacing) + m_nMargin)/ rImageSize.m_uHeight; //row
		CGUISize aSize(
			real(m_aTileSize.m_uWidth) / rImageSize.m_uWidth,
			real(m_aTileSize.m_uHeight) / rImageSize.m_uHeight
			);
		return CGUIRect( aTopLeft, aSize );
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITiledMapTilesetInfo::GetImageSize() const
	{
		return m_aImageSize;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITiledMapTilesetInfo::GetTileSize() const
	{
		return m_aTileSize;
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CGUITiledMapInfo
	//------------------------------------------------------------------------------
	CGUITiledMapInfo::CGUITiledMapInfo()
		:m_uParentGID( 0 )
		,m_eOrientation( eTMXOrientationOrtho ) // map orientation
	{
	}
	//------------------------------------------------------------------------------
	CGUITiledMapInfo::~CGUITiledMapInfo()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::InitWithTMXFile( const CGUIString& tmxFile )
	{
		m_strFilename = tmxFile;

		return ParseXMLFile( m_strFilename );		
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseXMLFile( const CGUIString& tmxFile )
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
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseXMLFile]: failed to read file <%s>!", tmxFile.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			CGUIException::ThrowException(
				"[CGUITiledMapInfo::ParseXMLFile]: failed to parse file <%s>!\n\n<%s>", 
				tmxFile.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseXMLFile], failed to get root node from file <%s>!", tmxFile.c_str());
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
	int32 CGUITiledMapInfo::ParseNode_map( TiXmlElement* pMapNode )
	{
		//version
		CGUIString strVersion = pMapNode->Attribute("version");
		if( strVersion != "1.0" )
		{
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_map]: Unsupported TMX version %s", strVersion.c_str());
			return -1;
		}

		//orientation
		CGUIString strOrientation = pMapNode->Attribute("orientation");
		if( strOrientation == "orthogonal" )
		{
			m_eOrientation = eTMXOrientationOrtho;
		}
		else if( strOrientation == "isometric")
		{
			m_eOrientation = eTMXOrientationIso;
		}
		else if( strOrientation == "hexagonal")
		{
			m_eOrientation = eTMXOrientationHex;
		}
		else
		{
			CGUIException::ThrowException( "[CGUITiledMapInfo::ParseNode_map]: Unsupported orientation: %s", strOrientation.c_str() );
			return -1;
		}

		//width and height
		CGUIString strWidth = pMapNode->Attribute("width");
		StringToValue( strWidth, m_aMapSize.m_uWidth );
		CGUIString strHeight = pMapNode->Attribute("height");
		StringToValue( strHeight, m_aMapSize.m_uHeight );

		//tile width and height
		CGUIString strTileWidth = pMapNode->Attribute("tilewidth");
		StringToValue( strTileWidth, m_aTileSize.m_uWidth );
		CGUIString strTileHeight = pMapNode->Attribute("tileheight");
		StringToValue( strTileHeight, m_aTileSize.m_uHeight );


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
				if( 0 != ParseNode_properties( pChildNode, m_mapProperties ) )
				{
					return -1;
				}
			}
			else
			{
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseXMLFile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseSourceFile( const CGUIString& tmxFile )
	{
		CGUIException::ThrowException( "[CGUITiledMapInfo::ParseSourceFile]: unsupport to parse source file now");
		return -1;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_tileset( TiXmlElement* pTilesetNode )
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

		m_vTilesets.push_back( CGUITiledMapTilesetInfo() );
		CGUITiledMapTilesetInfo& tileset = m_vTilesets.back();

		//name
		CGUIString strName = pTilesetNode->Attribute("name");
		tileset.m_strName = strName;

		//first gid
		CGUIString strGid = pTilesetNode->Attribute( "firstgid");
		StringToValue( strGid, tileset.m_nFirstGid );

		//spacing
		const char* spacing = pTilesetNode->Attribute( "spacing");
		tileset.m_nSpacing = 0;
		if( spacing )
		{
			StringToValue(CGUIString(spacing), tileset.m_nSpacing );
		}

		//margin
		const char* margin = pTilesetNode->Attribute( "margin");
		tileset.m_nMargin = 0;
		if( margin )
		{
			StringToValue(CGUIString(margin), tileset.m_nMargin );
		}

		//size
		CGUIString strWidth = pTilesetNode->Attribute( "tilewidth");
		StringToValue( strWidth, tileset.m_aTileSize.m_uWidth );
		CGUIString strHeight = pTilesetNode->Attribute( "tileheight");
		StringToValue( strHeight, tileset.m_aTileSize.m_uHeight );

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
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseXMLFile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_image( TiXmlElement* pImageNode )
	{
		if( m_vTilesets.empty() )
		{
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_image]: not find tileset when process tile node" );
			return -1;
		}
		CGUITiledMapTilesetInfo& info = m_vTilesets.back();

		CGUIString strSource = pImageNode->Attribute("source");
		info.m_strSourceImage = strSource;

		//image width and height
		CGUIString strImageWidth = pImageNode->Attribute("width");
		StringToValue( strImageWidth, info.m_aImageSize.m_uWidth );
		CGUIString strImageHeight = pImageNode->Attribute("height");
		StringToValue( strImageHeight, info.m_aImageSize.m_uHeight );

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_tile( TiXmlElement* pTileNode )
	{
		if( m_vTilesets.empty() )
		{
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_tile]: not find tileset when process tile node" );
			return -1;
		}
		CGUITiledMapTilesetInfo& info = m_vTilesets.back();

		uint32 uID = 0;
		CGUIString strID = pTileNode->Attribute( "id" );
		StringToValue( strID, uID );
		uint32 uParentID = info.m_nFirstGid + uID;

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
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseNode_tile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}
			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		m_mapTileProperties.insert( std::make_pair( uParentID, mapTileProperties ) );

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_properties( TiXmlElement* pPropertiesNode, std::map<CGUIString, CGUIString>& mapTileProperties )
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
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseNode_tile]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_layer( TiXmlElement* pLayerNode )
	{
		m_vLayers.push_back( CGUITiledMapLayerInfo() );
		CGUITiledMapLayerInfo &layer = m_vLayers.back();

		//name
		layer.m_strName = pLayerNode->Attribute( "name" );

		//size
		CGUIString strWidth = pLayerNode->Attribute("width");
		StringToValue( strWidth, layer.m_aLayerSize.m_uWidth );
		CGUIString strHeight = pLayerNode->Attribute("height");
		StringToValue( strHeight, layer.m_aLayerSize.m_uHeight );

		//visible
		const char* szVisible = pLayerNode->Attribute("visible");
		if( szVisible )
		{
			StringToValue( szVisible, layer.m_bVisible );
		}
		else
		{
			layer.m_bVisible = true;
		}

		//opacity
		const char* szOpacity = pLayerNode->Attribute("opacity");
		if( !szOpacity )
		{
			layer.m_fOpacity = 1.0f;
			layer.m_bVisible = true;
		}
		else
		{
			StringToValue( szOpacity, layer.m_fOpacity );
			if( GUI_REAL_EQUAL(layer.m_fOpacity, 0.0f ))
			{
				layer.m_bVisible = false;
			}
			else
			{
				layer.m_bVisible = true;
			}
		}

		//offset
		const char* szX = pLayerNode->Attribute("x");
		const char* szY = pLayerNode->Attribute("y");
		layer.m_aOffset.x = layer.m_aOffset.y = 0;
		if( szX )
		{
			StringToValue( szX, layer.m_aOffset.x );
		}
		if( szY )
		{
			StringToValue( szY, layer.m_aOffset.y );
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
				if( 0 != ParseNode_properties( pChildNode, layer.m_mapProperties ) )
				{
					return -1;
				}
			}
			else
			{
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseNode_layer]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_data( TiXmlElement* pDataNode )
	{
		CGUIString strEncoding = pDataNode->Attribute("encoding");
		CGUIString strCompression = pDataNode->Attribute("compression");

		if( strEncoding != "base64" )
		{
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_data]: date only support base64 now");
			return -1;
		}
		const char* data = pDataNode->GetText();

		if( data )
		{
			if( m_vLayers.empty() )
			{
				CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_data]: not find layers when process data node" );
				return -1;
			}
			CGUITiledMapLayerInfo& layer = m_vLayers.back();

			unsigned char *buffer = NULL;
			int len = 0;
			len = base64Decode( (const unsigned char*)data, strlen(data), &buffer);
			if( !buffer )
			{
				CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_data]: TiledMap decode data error");
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
					CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_data]: inflate data error");
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
				CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_data]: invalid tile num");
				return -1;
			}
			layer.m_vTiles.resize( nTileNum, 0 );
			for( int i=0; i<nTileNum; ++i )
			{
				// gid are stored in little endian.
				// if host is big endian, then swap
#if GUI_ENDIAN == GUI_ENDIAN_BIG
				pTiles[i] = ((pTiles[i] & 0xFF) << 24) | ((pTiles[i] & 0xFF00) << 8) | ((pTiles[i] >> 8) & 0xFF00) | ((pTiles[i] >> 24) & 0xFF);
#endif	//#if GUI_ENDIAN == GUI_ENDIAN_BIG
				layer.m_vTiles[i] = pTiles[i];
			}
			free( pTiles );
		}

		return 0;
	} 
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_objectgroup( TiXmlElement* pObjectGroupNode )
	{
		m_vObjectGroups.push_back( CGUITiledMapObjectGroup() );
		CGUITiledMapObjectGroup& objectGroup = m_vObjectGroups.back();

		//name
		objectGroup.m_strGroupName = pObjectGroupNode->Attribute( "name" );

		//offset
		const char* x = pObjectGroupNode->Attribute( "x" );
		objectGroup.m_aPositionOffset.x = 0;
		if( x )
		{
			StringToValue( CGUIString(x), objectGroup.m_aPositionOffset.x );
		}
		const char* y = pObjectGroupNode->Attribute( "y" );
		objectGroup.m_aPositionOffset.y = 0;
		if( y )
		{
			StringToValue( CGUIString(y), objectGroup.m_aPositionOffset.y );
		}

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
				if( 0 != ParseNode_properties( pChildNode, objectGroup.m_mapProperties ) )
				{
					return -1;
				}
			}
			else
			{
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseNode_objectgroup]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapInfo::ParseNode_object( TiXmlElement* pObjectNode )
	{
		if( m_vObjectGroups.empty() )
		{
			CGUIException::ThrowException("[CGUITiledMapInfo::ParseNode_object]: not find object group when process objectgroup node" );
			return -1;
		}
		CGUITiledMapObjectGroup& objectGroup = m_vObjectGroups.back();

		objectGroup.m_vObjects.push_back( CGUITiledMapObjectInfo() );
		CGUITiledMapObjectInfo& object = objectGroup.m_vObjects.back();

		//name
		object.m_strObjectName = pObjectNode->Attribute("name");

		//type
		const char* szType = pObjectNode->Attribute("type");
		if( szType )
		{
			object.m_strObjectType = szType;
		}

		//size
		CGUIString strWidth = pObjectNode->Attribute( "width");
		StringToValue( strWidth, object.m_aSize.m_uWidth );
		CGUIString strHeight = pObjectNode->Attribute( "height");
		StringToValue( strHeight, object.m_aSize.m_uHeight );

		//position
		CGUIString strX = pObjectNode->Attribute("x");
		StringToValue( strX, object.m_aPosition.x );
		object.m_aPosition.x += objectGroup.m_aPositionOffset.x;
		CGUIString strY = pObjectNode->Attribute("y");
		StringToValue( strY, object.m_aPosition.y );
		object.m_aPosition.y += objectGroup.m_aPositionOffset.y;
		// Correct y position. (Tiled uses Flipped, we uses Standard)
		//object.m_aPosition.y = (m_aMapSize.m_uHeight * m_aTileSize.m_uHeight) - object.m_aPosition.y - object.m_aSize.m_uHeight;

		//parse child node
		TiXmlElement* pChildNode = pObjectNode->FirstChildElement();
		while( pChildNode )
		{
			if( CGUIString("properties" ) == pChildNode->Value())
			{
				if( 0 != ParseNode_properties( pChildNode, object.m_mapProperties ) )
				{
					return -1;
				}
			}
			else
			{
				CGUIException::ThrowException( "[CGUITiledMapInfo::ParseNode_object]: unknown tmx file node <%s>", pChildNode->Value());
				return -1;
			}

			//for next node
			pChildNode = pChildNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	std::vector<CGUITiledMapLayerInfo>& CGUITiledMapInfo::GetLayerInfos()
	{
		return m_vLayers;
	}
	//------------------------------------------------------------------------------
	CGUITiledMapLayerInfo* CGUITiledMapInfo::GetLayerInfo( const CGUIString& rLayerName )
	{
		for( std::vector<CGUITiledMapLayerInfo>::iterator itor = m_vLayers.begin();
			itor != m_vLayers.end();
			++itor )
		{
			if( rLayerName == (*itor).GetName() )
			{
				return &(*itor);
			}
		}
		CGUIException::ThrowException("[CGUITiledMapInfo::GetLayerInfo]: failed to find layer named <%s>", rLayerName.c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITiledMapInfo::GetMapSize() const
	{
		return m_aMapSize;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUITiledMapInfo::GetTileSize() const
	{
		return m_aTileSize;
	}
	//------------------------------------------------------------------------------
	ETMXOrientation CGUITiledMapInfo::GetOrientation() const
	{
		return m_eOrientation;
	}
	//------------------------------------------------------------------------------
	std::vector<CGUITiledMapTilesetInfo>& CGUITiledMapInfo::GetTilesets()
	{
		return m_vTilesets;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUITiledMapObjectGroup>& CGUITiledMapInfo::GetObjectGroups() const
	{
		return m_vObjectGroups;
	}
	//------------------------------------------------------------------------------
	const std::map<CGUIString, CGUIString>& CGUITiledMapInfo::GetProperties() const
	{
		return m_mapProperties;
	}
	//------------------------------------------------------------------------------
	const std::map<uint32, std::map<CGUIString, CGUIString> >& CGUITiledMapInfo::GetAllTileProperties() const
	{
		return m_mapTileProperties;
	}
	//------------------------------------------------------------------------------

}//namespace guiex

