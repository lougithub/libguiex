/** 
* @file guitiledmap.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
#ifndef	__KEN_TILEDMAP_20110127_H__
#define __KEN_TILEDMAP_20110127_H__


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
	class CGUITiledMapLayer;
	class CGUITiledMapInfo;
	class CGUITiledMapObjectGroup;

	class IGUIInterfaceRender;
	class CGUIMatrix4;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUITiledMap : public CGUIResource
	{
	public:
		virtual ~CGUITiledMap();

		int32 LoadValueFromProperty( const class CGUIProperty& rProperty );

		const CGUITiledMapInfo* GetMapInfo() const;
		const CGUIString& GetWorkingDir() const;

		CGUITiledMapLayer* GetLayer( const CGUIString& rLayerName );
		const CGUIString* GetProperty( const CGUIString& rPropertyName ) const;
		const std::map<CGUIString, CGUIString>* GetTileProperties( uint32 gid );
		const CGUITiledMapObjectGroup* GetObjectGroup( const CGUIString& rGroupName ) const;

		void Render( IGUIInterfaceRender* pRender );

	protected:
		void Reset();
		int32 ParseTMXFile( const CGUIString& rFileName );

	protected:
		friend class CGUITiledMapManager;
		CGUITiledMap( const CGUIString& rName, const CGUIString& rSceneName );
		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
		CGUITiledMapInfo* m_pMapInfo;

		typedef std::vector<CGUITiledMapLayer*> TLayerArray;
		TLayerArray m_arrayLayer;

		CGUIString m_strFullWorkingDir;
		CGUIString m_strConfigFile;
	};

} //namespace guiex


#endif //__KEN_TILEDMAP_20110127_H__

