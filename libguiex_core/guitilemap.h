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
#include "guibase.h"
#include "guiresource.h"
#include <vector>
#include <map>

//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUITileMapLayer;
	class CGUITileMapInfo;
	class CGUITileMapObjectGroup;

	class IGUIInterfaceRender;
	class CGUIMatrix4;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUITileMap : public CGUIResource
	{
	public:
		virtual ~CGUITileMap();

		int32 LoadValueFromProperty( const class CGUIProperty& rProperty );


		CGUITileMapLayer* GetLayer( const CGUIString& rLayerName );
		const CGUIString* GetProperty( const CGUIString& rPropertyName ) const;
		const std::map<CGUIString, CGUIString>* GetTileProperties( uint32 gid );
		const CGUITileMapObjectGroup* GetObjectGroup( const CGUIString& rGroupName ) const;

		void Render( IGUIInterfaceRender* pRender, const CGUIMatrix4& rWorldMatrix );

	protected:
		void Reset() const;
		int32 ParseTMXFile( const CGUIString& rFileName ) const;

	protected:
		friend class CGUITileMapManager;
		CGUITileMap( const CGUIString& rName, const CGUIString& rSceneName );
		virtual int32 DoLoad() const;
		virtual void DoUnload();

	public:
		mutable CGUITileMapInfo* m_pMapInfo;

		typedef std::vector<CGUITileMapLayer*> TLayerArray;
		mutable TLayerArray m_arrayLayer;

		CGUIString m_strFullWorkingDir;
		CGUIString m_strConfigFile;
	};

} //namespace guiex


#endif //__KEN_TILEMAP_20110127_H__

