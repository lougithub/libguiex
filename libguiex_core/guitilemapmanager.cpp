/** 
* @file guitilemapmanager.cpp
* @brief tilemap manager.
* @author ken
* @date 2011-02-15
*/

//============================================================================//
// include 
//============================================================================// 
#include "guitilemapmanager.h"
#include "guiinterfacemanager.h"
#include "guipropertyconvertor.h"
#include "guistringconvertor.h"
#include "guiexception.h"
#include "guiproperty.h"
#include "guitilemap.h"
#include "guisystem.h"

 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITileMapData::CGUITileMapData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
	:CGUIResource( rName, rSceneName, "TILEMAPDATA", GSystem->GetTileMapManager() )
	,m_aProperty( rProperty )
	{
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUITileMapData::GetTileMapData() const
	{
		return m_aProperty;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapData::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUITileMapData::DoUnload()
	{
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	CGUITileMapManager * CGUITileMapManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUITileMapManager::CGUITileMapManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUITileMapManager::CGUITileMapManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUITileMapManager::~CGUITileMapManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUITileMapManager* CGUITileMapManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMapManager::RegisterTileMap( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		CGUITileMapData* pTileMapata = new CGUITileMapData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResource( pTileMapata );
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUITileMapManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUITileMapManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	CGUITileMap* CGUITileMapManager::DoCreateTileMap( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUITileMap* pTileMap = new CGUITileMap( rProperty.GetName(), rSceneName );
		if( pTileMap->LoadValueFromProperty( rProperty ) != 0 )
		{
			delete pTileMap;
			throw CGUIException( 
				"[CGUITileMapManager::DoCreateTileMap]: failed to load particle <%s> from property",
				rProperty.GetName().c_str() );
			return NULL;
		}
		pTileMap->Load();
		return pTileMap;
	}
	//------------------------------------------------------------------------------
	CGUITileMap* CGUITileMapManager::AllocateResource( const CGUIString& rResName )
	{
		CGUITileMapData* pTileMapData = CGUIResourceManager<CGUITileMapData, CGUITileMap>::GetRegisterResource( rResName );
		if( !pTileMapData )
		{
			throw CGUIException( 
				"[CGUITileMapManager::AllocateResource]: failed to get sound by name <%s>",
				rResName.c_str());
			return NULL;
		}

		CGUITileMap* pTileMap = DoCreateTileMap( pTileMapData->GetSceneName(), pTileMapData->GetTileMapData() );
		pTileMap->RefRetain();
		AddToAllocatePool( pTileMap );

		return pTileMap;
	}
	//------------------------------------------------------------------------------
	CGUITileMap* CGUITileMapManager::AllocateResource( const CGUIString& rName, const CGUIString& rSceneName )
	{
		CGUITileMap* pTileMap = new CGUITileMap( rName, rSceneName );
		pTileMap->RefRetain();
		AddToAllocatePool( pTileMap );

		return pTileMap;
	}
	//------------------------------------------------------------------------------
	void CGUITileMapManager::DeallocateResource( CGUIResource* pRes )
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
