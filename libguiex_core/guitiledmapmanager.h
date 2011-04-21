/** 
* @file guitiledmapmanager.h
* @brief tile map manager.
* @author ken
* @date 2011-02-15
*/

#ifndef __GUI_TILEDMAPMANAGER_20110215_H__
#define __GUI_TILEDMAPMANAGER_20110215_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guitiledmap.h"
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
	class GUIEXPORT CGUITiledMapData : public CGUIResource
	{
	public:
		const CGUIProperty& GetTiledMapData() const;

	protected:
		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
		friend class CGUITiledMapManager;
		CGUITiledMapData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty );

	protected:
		CGUIProperty m_aProperty;
	};



	/**
	* @class CGUITiledMapManager
	* @brief tiledmap manager
	* 
	*/
	class GUIEXPORT CGUITiledMapManager : public CGUIResourceManager <CGUITiledMapData, CGUITiledMap>
	{
	public:
		CGUITiledMapManager();
		virtual ~CGUITiledMapManager();

		static CGUITiledMapManager* Instance(); 

		virtual int32 RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty);
		
		CGUITiledMap* AllocateResource( const CGUIString& rResName );
		virtual void DeallocateResource( CGUIResource* pRes );

		CGUITiledMap* AllocateResource( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

		CGUITiledMap* DoCreateTiledMap( const CGUIString& rSceneName, const CGUIProperty& rProperty );

	private:
		static CGUITiledMapManager* m_pSingleton;
	};

}//namespace guiex

#endif //__GUI_TILEDMAPMANAGER_20110215_H__

