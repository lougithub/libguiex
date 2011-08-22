/** 
* @file tdgameobjectmanager.cpp
* @brief manager of game object.
* @author ken
* @date 2011-08-22
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameobjectmanager.h"
#include "tdgameobject.h"
#include "tdgameworld.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameObjectManager::CTDGameObjectManager( )
	{
	}
	//------------------------------------------------------------------------------
	CTDGameObjectManager::~CTDGameObjectManager()
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameObjectManager::SetGameWorld( class CTDGameWorld* pGameWorld )
	{
		m_pGameWorld = pGameWorld;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectManager::ReleaseAllObjects()
	{
		GUI_TRACE( "[CTDGameObjectManager::ReleaseAllObjects]: release all game objects");
		
		//check any active objects
		for( TMapObjectPool::iterator itor = m_mapObjectActivePool.begin();
			itor != m_mapObjectActivePool.end();
			++itor )
		{
			if( !itor->second.empty() )
			{
				GUI_THROW( "[CTDGameObjectManager::ReleaseAllObjects]: active object exists in pool" );
			}
		}

		//release all object here
		for( TMapObjectPool::iterator itor = m_mapObjectDeactivePool.begin();
			itor != m_mapObjectDeactivePool.end();
			++itor )
		{
			std::set<CTDGameObject*>& rSet = itor->second;
			for( std::set<CTDGameObject*>::iterator itorSet = rSet.begin();
				itorSet != rSet.end();
				++itorSet )
			{
				delete *itorSet;
			}
			rSet.empty();
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectManager::FreeObject( CTDGameObject* pObject )
	{
		GUI_ASSERT( pObject, "invalid object" );

		TMapObjectPool::iterator itorMap = m_mapObjectActivePool.find( pObject->GetType() );
		if( itorMap == m_mapObjectActivePool.end() )
		{
			GUI_THROW( "[CTDGameObjectManager::FreeObject]: object doesn't belong to a active pool" );
			return;
		}
		std::set<CTDGameObject*>& rSet = itorMap->second;
		std::set<CTDGameObject*>::iterator itorSet= rSet.find( pObject );
		if( itorSet == rSet.end() )
		{
			GUI_THROW( "[CTDGameObjectManager::FreeObject]: object is not in pool" );
			return;
		}
		rSet.erase( itorSet );
		pObject->OnDeactive();
		m_mapObjectDeactivePool[pObject->GetType()].insert( pObject );
	}
	//------------------------------------------------------------------------------
}

