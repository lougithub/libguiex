/** 
* @file guitilemapmanager.h
* @brief tile map manager.
* @author ken
* @date 2011-02-15
*/

#ifndef __GUI_TILEMAPMANAGER_20110215_H__
#define __GUI_TILEMAPMANAGER_20110215_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guitilemap.h"
#include "guiresourcemanager.h"
#include "guiproperty.h"
#include <set>
#include <map>


//============================================================================//
// declare
//============================================================================// 

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUITileMapData : public CGUIResource
	{
	public:
		const CGUIProperty& GetTileMapData() const;

	protected:
		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		friend class CGUITileMapManager;
		CGUITileMapData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty );

	protected:
		CGUIProperty m_aProperty;
	};



	/**
	* @class CGUITileMapManager
	* @brief tilemap manager
	* 
	*/
	class GUIEXPORT CGUITileMapManager : public CGUIResourceManager <CGUITileMapData, CGUITileMap>
	{
	public:
		CGUITileMapManager();
		virtual ~CGUITileMapManager();

		static CGUITileMapManager* Instance(); 

		int32 RegisterTileMap( const CGUIString& rSceneName, const CGUIProperty& rProperty);
		
		CGUITileMap* AllocateResource( const CGUIString& rResName );
		virtual void DeallocateResource( CGUIResource* pRes );

		CGUITileMap* AllocateResource( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

		CGUITileMap* DoCreateTileMap( const CGUIString& rSceneName, const CGUIProperty& rProperty );

	private:
		static CGUITileMapManager* m_pSingleton;
	};

}//namespace guiex

#endif //__GUI_TILEMAPMANAGER_20110215_H__

