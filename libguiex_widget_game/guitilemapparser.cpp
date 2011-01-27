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

	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::InitWithTMXFile( const CGUIString& tmxFile )
	{
		filename = tmxFile;
		storingCharacters = false;
		layerAttribs = TMXLayerAttribNone;
		parentElement = TMXPropertyNone;

		ParseXMLFile( filename );		
	}
	//------------------------------------------------------------------------------
	int32 CCTMXMapInfo::ParseXMLFile( const CGUIString& tmxFile )
	{
		NSURL *url = [NSURL fileURLWithPath:[CCFileUtils fullPathFromRelativePath:xmlFilename] ];
		NSXMLParser *parser = [[NSXMLParser alloc] initWithContentsOfURL:url];

		// we'll do the parsing
		[parser setDelegate:self];
		[parser setShouldProcessNamespaces:NO];
		[parser setShouldReportNamespacePrefixes:NO];
		[parser setShouldResolveExternalEntities:NO];
		[parser parse];

		NSAssert1( ! [parser parserError], @"Error parsing file: %@.", xmlFilename );

		[parser release];

	}
	//------------------------------------------------------------------------------
	// the XML parser calls here with all the elements
	-(void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict
	{	
		if([elementName isEqualToString:@"map"]) {
			NSString *version = [attributeDict valueForKey:@"version"];
			if( ! [version isEqualToString:@"1.0"] )
				CCLOG(@"cocos2d: TMXFormat: Unsupported TMX version: %@", version);
			NSString *orientationStr = [attributeDict valueForKey:@"orientation"];
			if( [orientationStr isEqualToString:@"orthogonal"])
				orientation_ = CCTMXOrientationOrtho;
			else if ( [orientationStr isEqualToString:@"isometric"])
				orientation_ = CCTMXOrientationIso;
			else if( [orientationStr isEqualToString:@"hexagonal"])
				orientation_ = CCTMXOrientationHex;
			else
				CCLOG(@"cocos2d: TMXFomat: Unsupported orientation: %@", orientation_);

			mapSize_.width = [[attributeDict valueForKey:@"width"] intValue];
			mapSize_.height = [[attributeDict valueForKey:@"height"] intValue];
			tileSize_.width = [[attributeDict valueForKey:@"tilewidth"] intValue];
			tileSize_.height = [[attributeDict valueForKey:@"tileheight"] intValue];

			// The parent element is now "map"
			parentElement = TMXPropertyMap;
		} else if([elementName isEqualToString:@"tileset"]) {

			// If this is an external tileset then start parsing that
			NSString *externalTilesetFilename = [attributeDict valueForKey:@"source"];
			if (externalTilesetFilename) {
				// Tileset file will be relative to the map file. So we need to convert it to an absolute path
				NSString *dir = [filename_ stringByDeletingLastPathComponent];	// Directory of map file
				externalTilesetFilename = [dir stringByAppendingPathComponent:externalTilesetFilename];	// Append path to tileset file

				[self parseXMLFile:externalTilesetFilename];
			} else {

				CCTMXTilesetInfo *tileset = [CCTMXTilesetInfo new];
				tileset.name = [attributeDict valueForKey:@"name"];
				tileset.firstGid = [[attributeDict valueForKey:@"firstgid"] intValue];
				tileset.spacing = [[attributeDict valueForKey:@"spacing"] intValue];
				tileset.margin = [[attributeDict valueForKey:@"margin"] intValue];
				CGSize s;
				s.width = [[attributeDict valueForKey:@"tilewidth"] intValue];
				s.height = [[attributeDict valueForKey:@"tileheight"] intValue];
				tileset.tileSize = s;

				[tilesets_ addObject:tileset];
				[tileset release];
			}

		}else if([elementName isEqualToString:@"tile"]){
			CCTMXTilesetInfo* info = [tilesets_ lastObject];
			NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:3];
			parentGID_ =  [info firstGid] + [[attributeDict valueForKey:@"id"] intValue];
			[tileProperties_ setObject:dict forKey:[NSNumber numberWithInt:parentGID_]];

			parentElement = TMXPropertyTile;

		}else if([elementName isEqualToString:@"layer"]) {
			CCTMXLayerInfo *layer = [CCTMXLayerInfo new];
			layer.name = [attributeDict valueForKey:@"name"];

			CGSize s;
			s.width = [[attributeDict valueForKey:@"width"] intValue];
			s.height = [[attributeDict valueForKey:@"height"] intValue];
			layer.layerSize = s;

			layer.visible = ![[attributeDict valueForKey:@"visible"] isEqualToString:@"0"];

			if( [attributeDict valueForKey:@"opacity"] )
				layer.opacity = 255 * [[attributeDict valueForKey:@"opacity"] floatValue];
			else
				layer.opacity = 255;

			int x = [[attributeDict valueForKey:@"x"] intValue];
			int y = [[attributeDict valueForKey:@"y"] intValue];
			layer.offset = ccp(x,y);

			[layers_ addObject:layer];
			[layer release];

			// The parent element is now "layer"
			parentElement = TMXPropertyLayer;

		} else if([elementName isEqualToString:@"objectgroup"]) {

			CCTMXObjectGroup *objectGroup = [[CCTMXObjectGroup alloc] init];
			objectGroup.groupName = [attributeDict valueForKey:@"name"];
			CGPoint positionOffset;
			positionOffset.x = [[attributeDict valueForKey:@"x"] intValue] * tileSize_.width;
			positionOffset.y = [[attributeDict valueForKey:@"y"] intValue] * tileSize_.height;
			objectGroup.positionOffset = positionOffset;

			[objectGroups_ addObject:objectGroup];
			[objectGroup release];

			// The parent element is now "objectgroup"
			parentElement = TMXPropertyObjectGroup;

		} else if([elementName isEqualToString:@"image"]) {

			CCTMXTilesetInfo *tileset = [tilesets_ lastObject];

			// build full path
			NSString *imagename = [attributeDict valueForKey:@"source"];		
			NSString *path = [filename_ stringByDeletingLastPathComponent];		
			tileset.sourceImage = [path stringByAppendingPathComponent:imagename];

		} else if([elementName isEqualToString:@"data"]) {
			NSString *encoding = [attributeDict valueForKey:@"encoding"];
			NSString *compression = [attributeDict valueForKey:@"compression"];

			if( [encoding isEqualToString:@"base64"] ) {
				layerAttribs |= TMXLayerAttribBase64;
				storingCharacters = YES;

				if( [compression isEqualToString:@"gzip"] )
					layerAttribs |= TMXLayerAttribGzip;

				NSAssert( !compression || [compression isEqualToString:@"gzip"], @"TMX: unsupported compression method" );
			}

			NSAssert( layerAttribs != TMXLayerAttribNone, @"TMX tile map: Only base64 and/or gzip maps are supported" );

		} else if([elementName isEqualToString:@"object"]) {

			CCTMXObjectGroup *objectGroup = [objectGroups_ lastObject];

			// The value for "type" was blank or not a valid class name
			// Create an instance of TMXObjectInfo to store the object and its properties
			NSMutableDictionary *dict = [[NSMutableDictionary alloc] initWithCapacity:5];

			// Set the name of the object to the value for "name"
			[dict setValue:[attributeDict valueForKey:@"name"] forKey:@"name"];

			// Assign all the attributes as key/name pairs in the properties dictionary
			[dict setValue:[attributeDict valueForKey:@"type"] forKey:@"type"];
			int x = [[attributeDict valueForKey:@"x"] intValue] + objectGroup.positionOffset.x;
			[dict setValue:[NSNumber numberWithInt:x] forKey:@"x"];
			int y = [[attributeDict valueForKey:@"y"] intValue] + objectGroup.positionOffset.y;
			// Correct y position. (Tiled uses Flipped, cocos2d uses Standard)
			y = (mapSize_.height * tileSize_.height) - y - [[attributeDict valueForKey:@"height"] intValue];
			[dict setValue:[NSNumber numberWithInt:y] forKey:@"y"];
			[dict setValue:[attributeDict valueForKey:@"width"] forKey:@"width"];
			[dict setValue:[attributeDict valueForKey:@"height"] forKey:@"height"];

			// Add the object to the objectGroup
			[[objectGroup objects] addObject:dict];
			[dict release];

			// The parent element is now "object"
			parentElement = TMXPropertyObject;

		} else if([elementName isEqualToString:@"property"]) {

			if ( parentElement == TMXPropertyNone ) {

				CCLOG( @"TMX tile map: Parent element is unsupported. Cannot add property named '%@' with value '%@'",
					[attributeDict valueForKey:@"name"], [attributeDict valueForKey:@"value"] );

			} else if ( parentElement == TMXPropertyMap ) {

				// The parent element is the map
				[properties_ setValue:[attributeDict valueForKey:@"value"] forKey:[attributeDict valueForKey:@"name"]];

			} else if ( parentElement == TMXPropertyLayer ) {

				// The parent element is the last layer
				CCTMXLayerInfo *layer = [layers_ lastObject];
				// Add the property to the layer
				[[layer properties] setValue:[attributeDict valueForKey:@"value"] forKey:[attributeDict valueForKey:@"name"]];

			} else if ( parentElement == TMXPropertyObjectGroup ) {

				// The parent element is the last object group
				CCTMXObjectGroup *objectGroup = [objectGroups_ lastObject];
				[[objectGroup properties] setValue:[attributeDict valueForKey:@"value"] forKey:[attributeDict valueForKey:@"name"]];

			} else if ( parentElement == TMXPropertyObject ) {

				// The parent element is the last object
				CCTMXObjectGroup *objectGroup = [objectGroups_ lastObject];
				NSMutableDictionary *dict = [[objectGroup objects] lastObject];

				NSString *propertyName = [attributeDict valueForKey:@"name"];
				NSString *propertyValue = [attributeDict valueForKey:@"value"];

				[dict setValue:propertyValue forKey:propertyName];
			} else if ( parentElement == TMXPropertyTile ) {

				NSMutableDictionary* dict = [tileProperties_ objectForKey:[NSNumber numberWithInt:parentGID_]];
				NSString *propertyName = [attributeDict valueForKey:@"name"];
				NSString *propertyValue = [attributeDict valueForKey:@"value"];
				[dict setObject:propertyValue forKey:propertyName];

			}
		}
	}
	//------------------------------------------------------------------------------
	- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
	{
		int len = 0;

		if([elementName isEqualToString:@"data"] && layerAttribs&TMXLayerAttribBase64) {
			storingCharacters = NO;

			CCTMXLayerInfo *layer = [layers_ lastObject];

			unsigned char *buffer;
			len = base64Decode((unsigned char*)[currentString UTF8String], [currentString length], &buffer);
			if( ! buffer ) {
				CCLOG(@"cocos2d: TiledMap: decode data error");
				return;
			}

			if( layerAttribs & TMXLayerAttribGzip ) {
				unsigned char *deflated;
				ccInflateMemory(buffer, len, &deflated);
				free( buffer );

				if( ! deflated ) {
					CCLOG(@"cocos2d: TiledMap: inflate data error");
					return;
				}

				layer.tiles = (unsigned int*) deflated;
			} else
				layer.tiles = (unsigned int*) buffer;

			[currentString setString:@""];

		} else if ([elementName isEqualToString:@"map"]) {
			// The map element has ended
			parentElement = TMXPropertyNone;

		}	else if ([elementName isEqualToString:@"layer"]) {
			// The layer element has ended
			parentElement = TMXPropertyNone;

		} else if ([elementName isEqualToString:@"objectgroup"]) {
			// The objectgroup element has ended
			parentElement = TMXPropertyNone;

		} else if ([elementName isEqualToString:@"object"]) {
			// The object element has ended
			parentElement = TMXPropertyNone;
		}
	}
	//------------------------------------------------------------------------------
	- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
	{
		if (storingCharacters)
			[currentString appendString:string];
	}
	//------------------------------------------------------------------------------
	//
	// the level did not load, file not found, etc.
	//
	-(void)parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError{
		CCLOG(@"cocos2d: Error on XML Parse: %@", [parseError localizedDescription] );
	}
	//------------------------------------------------------------------------------
}//namespace guiex

