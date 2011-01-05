/** 
 * @file guiresourcemanager.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-26
 */


#ifndef __KEN_GUIRESOURCEMANAGER_20091026_H__
#define	__KEN_GUIRESOURCEMANAGER_20091026_H__

//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guiexception.h"
#include <map>
#include <set>
#include <vector>


//============================================================================//
// declare
//============================================================================//


//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUIResourceManagerBase
	{
	public:
		CGUIResourceManagerBase();
		virtual ~CGUIResourceManagerBase();

		virtual void UnloadAllResources( ) = 0;
		virtual void ReleaseAllResources( ) = 0;
		virtual void ReleaseResourceByScene( const CGUIString& rSceneName ) = 0;

	protected:
		virtual	void DestroyResourceImp( void* pRes ) = 0; 
	};

	/**
	* @class CGUIResourceManager
	*/
	template< class TResType >
	class GUIEXPORT CGUIResourceManager : public CGUIResourceManagerBase
	{
	public:
		typedef TResType TResourceType;

		CGUIResourceManager();
		virtual ~CGUIResourceManager();

	public:
		int32 RegisterResource( TResType* pRes );

		virtual void UnloadAllResources( );
		virtual void ReleaseAllResources( );
		virtual void ReleaseResourceByScene( const CGUIString& rSceneName );

		const std::map< CGUIString, TResType* >& GetRegisterResourceMap() const;

	protected:
		TResType* GetResource( const CGUIString& rResName ) const;
		void LoadAllResources( );

		//allocate resource
		void AddToAllocatePool( TResType* pRes );
		int32 ReleaseFromAllocatePool( TResType* pRes );
		void ReleaseAllocateResources( );

		//register resource
		void ReleaseRegisterResource( const CGUIString& rResName );
		void ReleaseAllRegisterResources( );

		void CheckResourceReference( TResType* pRes );

	protected:
		typedef std::map< CGUIString, TResType* > TMapResource;
		TMapResource m_mapResource;

		typedef std::set<TResType*> TSetResource;
		TSetResource m_setAllocatePool;
	};
}



//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	template< class TResType >
	inline CGUIResourceManager<TResType>::CGUIResourceManager()
	{
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline CGUIResourceManager<TResType>::~CGUIResourceManager()
	{
		ReleaseAllResources();
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	const std::map< CGUIString, TResType* >& CGUIResourceManager<TResType>::GetRegisterResourceMap() const
	{
		return m_mapResource;
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline int32 CGUIResourceManager<TResType>::RegisterResource( TResType* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		if( m_mapResource.find( pRes->GetName()) != m_mapResource.end())
		{
			throw CGUIException( "the resource <%s> has existed", pRes->GetName().c_str());
			return -1;
		}

		pRes->RefRetain();
		m_mapResource.insert( std::make_pair( pRes->GetName(), pRes));
		return 0;
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline TResType* CGUIResourceManager<TResType>::GetResource( const CGUIString& rResName ) const
	{
		typename TMapResource::const_iterator itor = m_mapResource.find( rResName );
		if( itor != m_mapResource.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseAllResources( )
	{
		ReleaseAllRegisterResources();
		
		ReleaseAllocateResources();
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseResourceByScene( const CGUIString& rSceneName )
	{
		std::vector< CGUIString > vecReleaseRes;
		for( typename TMapResource::iterator itor = m_mapResource.begin();
			itor != m_mapResource.end();
			++itor )
		{
			if( itor->second->GetSceneName() == rSceneName )
			{
				vecReleaseRes.push_back( itor->first );
			}
		}

		for( std::vector< CGUIString >::iterator itor = vecReleaseRes.begin();
			itor != vecReleaseRes.end();
			++itor )
		{
			ReleaseRegisterResource( *itor );
		}
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseAllRegisterResources( )
	{
		for( typename TMapResource::iterator itor = m_mapResource.begin();
			itor != m_mapResource.end();
			++itor )
		{
			TResType* pRes = itor->second;
			pRes->RefRelease();

			CheckResourceReference( pRes );
			DestroyResourceImp( pRes );
		}
		m_mapResource.clear();
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseAllocateResources( )
	{
		for( typename TSetResource::iterator itor = m_setAllocatePool.begin();
			itor != m_setAllocatePool.end();
			++itor )
		{
			TResType* pRes = *itor;
			CheckResourceReference( pRes );
			DestroyResourceImp( pRes );
		}
		m_setAllocatePool.clear();
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::UnloadAllResources( )
	{
		for( typename TMapResource::iterator itor = m_mapResource.begin();
			itor != m_mapResource.end();
			++itor )
		{
			itor->second->Unload();
		}

		for( typename TSetResource::iterator itor = m_setAllocatePool.begin();
			itor != m_setAllocatePool.end();
			++itor )
		{
			(*itor)->Unload();
		}
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::LoadAllResources( )
	{
		for( typename TMapResource::iterator itor = m_mapResource.begin();
			itor != m_mapResource.end();
			++itor )
		{
			itor->second->Load();
		}

		for( typename TSetResource::iterator itor = m_setAllocatePool.begin();
			itor != m_setAllocatePool.end();
			++itor )
		{
			(*itor)->Load();
		}
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseRegisterResource( const CGUIString& rResName )
	{
		typename TMapResource::iterator itor = m_mapResource.find( rResName );
		if( itor != m_mapResource.end())
		{
			TResType* pRes = itor->second;
			pRes->RefRelease();
			CheckResourceReference( pRes );
			DestroyResourceImp( pRes );
			m_mapResource.erase( itor );
		}
		else
		{
			throw CGUIException("[CGUIResourceManager::ReleaseRegisterResource]: failed to find resource by name <%s>", rResName.c_str());
		}
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::AddToAllocatePool( TResType* pRes )
	{
		typename TSetResource::const_iterator itorFind = m_setAllocatePool.find( pRes );
		if( itorFind != m_setAllocatePool.end() )
		{
			//not found
			throw CGUIException("[CGUIResourceManager::AddToAllocatePool]: failed to add resource <%s:%s> to allocate pool", 
				pRes->GetName().c_str(),
				pRes->GetResourceType().c_str());
			return;
		}
		m_setAllocatePool.insert( pRes );
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline int32 CGUIResourceManager<TResType>::ReleaseFromAllocatePool( TResType* pRes )
	{
		typename TSetResource::const_iterator itorFind = m_setAllocatePool.find( pRes );
		if( itorFind == m_setAllocatePool.end() )
		{
			//not found
			throw CGUIException("[CGUIResourceManager::ReleaseFromAllocatePool]: failed to remove resource <%s:%s> from allocate pool" );
			return -1;
		}
		CheckResourceReference( *itorFind );
		DestroyResourceImp( *itorFind );
		m_setAllocatePool.erase( itorFind );
		return 0;
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::CheckResourceReference( TResType* pRes )
	{
		if( pRes->GetRefCount() != 0 )
		{
			throw CGUIException( "[CGUIResourceManager::CheckResourceReference]:resource reference is still in using[%d]: <%s:%s:%s>", 
				pRes->GetRefCount(),
				pRes->GetName().c_str(), 
				pRes->GetResourceType().c_str(),
				pRes->GetSceneName().c_str() );
		}
	}
	//------------------------------------------------------------------------------
}


#endif	//__KEN_GUIRESOURCEMANAGER_20091026_H__

