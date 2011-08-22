/** 
* @file tdgameobjectmanager.h
* @brief manager of game object.
* @author ken
* @date 2011-08-22
*/

#ifndef __TD_GAMEOBJECTMANAGER_20110822_H__
#define __TD_GAMEOBJECTMANAGER_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgameobject.h"
#include <libguiex_core/guiex.h>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	enum EGameObjectType
	{
		eGameObject_Monster = 0,
		eGameObject_Bullet,
	};

#define GAMEOBJECT_AUTO_EXPAND_SIZE 10
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameObjectManager
	{
	public:
		CTDGameObjectManager( );
		~CTDGameObjectManager();

		void SetGameWorld( class CTDGameWorld* pGameWorld );

		template< class T >
		void InitObjects( uint32 uObjectNum );
		void ReleaseAllObjects();

		template< class T > 
		T* AllocateObject( uint32 uObjectType );
		void FreeObject( class CTDGameObject* pObject );

	protected:
		typedef std::map< uint32, std::set<class CTDGameObject*> > TMapObjectPool;
		TMapObjectPool m_mapObjectActivePool;
		TMapObjectPool m_mapObjectDeactivePool;

	private:
		class CTDGameWorld* m_pGameWorld;
	};

	//------------------------------------------------------------------------------
	template< class T >
	inline void CTDGameObjectManager::InitObjects( uint32 uObjectNum )
	{
		uint32 uObjectType = -1;
		for( uint32 i=0; i<uObjectNum; ++i )
		{
			CTDGameObject* pObject = new T( m_pGameWorld );
			pObject->OnDeactive();
			uObjectType = pObject->GetType();
			m_mapObjectDeactivePool[uObjectType].insert( pObject );
		}
		GUI_TRACE( GUI_FORMAT( "[CTDGameObjectManager::InitObjects]: init %d game objects, type [%d]", uObjectNum, uObjectType));
	}
	//------------------------------------------------------------------------------
	template< class T > 
	inline T* CTDGameObjectManager::AllocateObject( uint32 uObjectType )
	{
		std::set<class CTDGameObject*>& rSet = m_mapObjectDeactivePool[uObjectType];
		if( rSet.empty() )
		{
			//no object, init more
			InitObjects<T>(GAMEOBJECT_AUTO_EXPAND_SIZE);
		}
		CTDGameObject* pGameObject = *rSet.begin();
		rSet.erase( rSet.begin() );
		m_mapObjectActivePool[uObjectType].insert( pGameObject );
		pGameObject->OnActive();
		return static_cast<T*>(pGameObject);
	}
	//------------------------------------------------------------------------------
}

#endif //__TD_GAMEOBJECTMANAGER_20110822_H__
