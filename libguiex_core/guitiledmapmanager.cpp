/** 
* @file guitiledmapmanager.cpp
* @brief tiledmap manager.
* @author ken
* @date 2011-02-15
*/

//============================================================================//
// include 
//============================================================================// 
#include "guitiledmapmanager.h"
#include "guiinterfacemanager.h"
#include "guipropertyconvertor.h"
#include "guistringconvertor.h"
#include "guiexception.h"
#include "guiproperty.h"
#include "guitiledmap.h"
#include "guisystem.h"

 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITiledMapData::CGUITiledMapData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
	:CGUIResource( rName, rSceneName, "TILEDMAPDATA", GSystem->GetTiledMapManager() )
	,m_aProperty( rProperty )
	{
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUITiledMapData::GetTiledMapData() const
	{
		return m_aProperty;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapData::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUITiledMapData::DoUnload()
	{
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	CGUITiledMapManager * CGUITiledMapManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUITiledMapManager::CGUITiledMapManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUITiledMapManager::CGUITiledMapManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUITiledMapManager::~CGUITiledMapManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUITiledMapManager* CGUITiledMapManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMapManager::RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		CGUITiledMapData* pTiledMapata = new CGUITiledMapData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResourceImp( pTiledMapata );
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUITiledMapManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUITiledMapManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	CGUITiledMap* CGUITiledMapManager::DoCreateTiledMap( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUITiledMap* pTiledMap = new CGUITiledMap( rProperty.GetName(), rSceneName );
		if( pTiledMap->LoadValueFromProperty( rProperty ) != 0 )
		{
			delete pTiledMap;
			throw CGUIException( 
				"[CGUITiledMapManager::DoCreateTiledMap]: failed to load particle <%s> from property",
				rProperty.GetName().c_str() );
			return NULL;
		}
		pTiledMap->Load();
		return pTiledMap;
	}
	//------------------------------------------------------------------------------
	CGUITiledMap* CGUITiledMapManager::AllocateResource( const CGUIString& rResName )
	{
		CGUITiledMapData* pTiledMapData = CGUIResourceManager<CGUITiledMapData, CGUITiledMap>::GetRegisterResource( rResName );
		if( !pTiledMapData )
		{
			throw CGUIException( 
				"[CGUITiledMapManager::AllocateResource]: failed to get sound by name <%s>",
				rResName.c_str());
			return NULL;
		}

		CGUITiledMap* pTiledMap = DoCreateTiledMap( pTiledMapData->GetSceneName(), pTiledMapData->GetTiledMapData() );
		pTiledMap->RefRetain();
		AddToAllocatePool( pTiledMap );

		return pTiledMap;
	}
	//------------------------------------------------------------------------------
	CGUITiledMap* CGUITiledMapManager::AllocateResource( const CGUIString& rName, const CGUIString& rSceneName )
	{
		CGUITiledMap* pTiledMap = new CGUITiledMap( rName, rSceneName );
		pTiledMap->RefRetain();
		AddToAllocatePool( pTiledMap );

		return pTiledMap;
	}
	//------------------------------------------------------------------------------
	void CGUITiledMapManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			ReleaseFromAllocatePool( pRes );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
