/** 
* @file guitilemap.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
#ifndef	__KEN_TILEMAP_20110127_H__
#define __KEN_TILEMAP_20110127_H__


//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guiex.h>
#include <vector>
#include <map>

//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CCTMXLayer;
	class CCTMXMapInfo;
	class CCTMXObjectGroup;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CCTMXTiledMap
	{
	public:
		CCTMXTiledMap();
		~CCTMXTiledMap();

		void Reset();

		int32 ParseTMXFile( const CGUIString& rFileName );

		CCTMXLayer* GetLayer( const CGUIString& rLayerName );
		const CGUIString* GetProperty( const CGUIString& rPropertyName ) const;
		const std::map<CGUIString, CGUIString>* GetTileProperties( uint32 gid );
		const CCTMXObjectGroup* GetObjectGroup( const CGUIString& rGroupName ) const;


	public:
		CCTMXMapInfo* m_pMapInfo;

		typedef std::vector<CCTMXLayer*> TLayerArray;
		TLayerArray m_arrayLayer;
	};

} //namespace guiex


#endif //__KEN_TILEMAP_20110127_H__

