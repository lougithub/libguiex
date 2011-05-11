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
#include "guiresource.h"
#include <map>
#include <set>
#include <vector>


//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIResource;
	class CGUIProperty;
}

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUIResourceManagerBase
	{
	public:
		CGUIResourceManagerBase( const char* szResMgrType );
		virtual ~CGUIResourceManagerBase();

		virtual int32 RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty ) = 0;
		virtual void UnloadAllResources( ) = 0;
		virtual void ReleaseAllResources( ) = 0;
		virtual void ReleaseResourceByScene( const CGUIString& rSceneName ) = 0;

		virtual void DeallocateResource( CGUIResource* pRes ) = 0;

		const CGUIString& GetManagerType() const;

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ) = 0; 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ) = 0; 
		virtual void ReleaseAllRegisterResources( ) = 0;

		void DoRefRelease( CGUIResource* pRes );

	protected:
		CGUIString m_strMgrType;
	};

	/**
	* @class CGUIResourceManager
	*/
	template< class TRegisterResType, class TAllocateResType >
	class GUIEXPORT CGUIResourceManager : public CGUIResourceManagerBase
	{
	public:
		typedef TRegisterResType TRegisterResourceType;
		typedef TAllocateResType TAllocateResourceType;

		CGUIResourceManager(const char* szResMgrType);
		virtual ~CGUIResourceManager();

	public:
		virtual void UnloadAllResources( );
		virtual void ReleaseAllResources( );
		virtual void ReleaseResourceByScene( const CGUIString& rSceneName );

		const std::map< CGUIString, TRegisterResType* >& GetRegisterResourceMap() const;

	protected:
		int32 RegisterResourceImp( TRegisterResType* pRes );

		TRegisterResType* GetRegisterResource( const CGUIString& rResName ) const;
		void LoadAllResources( );

		//allocate resource
		void AddToAllocatePool( CGUIResource* pRes );
		int32 ReleaseFromAllocatePool( CGUIResource* pRes );
		void ReleaseAllocateResources( );

		//register resource
		void ReleaseRegisterResource( const CGUIString& rResName );
		virtual void ReleaseAllRegisterResources( );

		void CheckResourceReference( CGUIResource* pRes );

		void DestroyRegisterResource( TRegisterResType* pRes );
		void DestroyAllocateResource( CGUIResource* pRes );

	private:
		typedef std::map< CGUIString, TRegisterResType* > TMapResource;
		TMapResource m_mapRegisterResource;

		typedef std::set<CGUIResource*> TSetResource;
		TSetResource m_setAllocateResource;
	};
}



//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline CGUIResourceManager<TRegisterResType, TAllocateResType>::CGUIResourceManager( const char* szResMgrType )
		:CGUIResourceManagerBase( szResMgrType )
	{
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline CGUIResourceManager<TRegisterResType, TAllocateResType>::~CGUIResourceManager()
	{
		ReleaseAllResources();
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	const std::map< CGUIString, TRegisterResType* >& CGUIResourceManager<TRegisterResType, TAllocateResType>::GetRegisterResourceMap() const
	{
		return m_mapRegisterResource;
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline int32 CGUIResourceManager<TRegisterResType, TAllocateResType>::RegisterResourceImp( TRegisterResType* pRes )
	{
		GUI_ASSERT( pRes, GUI_FORMAT("[ResourceManager] type:%s, invalid parameter", GetManagerType().c_str()));

		if( m_mapRegisterResource.find( pRes->GetName()) != m_mapRegisterResource.end())
		{
			CGUIException::ThrowException( "the resource <%s> has existed", pRes->GetName().c_str());
			return -1;
		}

		pRes->RefRetain();
		m_mapRegisterResource.insert( std::make_pair( pRes->GetName(), pRes));
		return 0;
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline TRegisterResType* CGUIResourceManager<TRegisterResType, TAllocateResType>::GetRegisterResource( const CGUIString& rResName ) const
	{
		typename TMapResource::const_iterator itor = m_mapRegisterResource.find( rResName );
		if( itor != m_mapRegisterResource.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::ReleaseAllResources( )
	{
		ReleaseAllRegisterResources();
		
		ReleaseAllocateResources();
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::ReleaseResourceByScene( const CGUIString& rSceneName )
	{
		std::vector< CGUIString > vecReleaseRes;
		for( typename TMapResource::iterator itor = m_mapRegisterResource.begin();
			itor != m_mapRegisterResource.end();
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
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::ReleaseAllRegisterResources( )
	{
		for( typename TMapResource::iterator itor = m_mapRegisterResource.begin();
			itor != m_mapRegisterResource.end();
			++itor )
		{
			TRegisterResType* pRes = itor->second;
			DoRefRelease( pRes );

			CheckResourceReference( pRes );
			DestroyRegisterResource( pRes );
		}
		m_mapRegisterResource.clear();
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::ReleaseAllocateResources( )
	{
		for( typename TSetResource::iterator itor = m_setAllocateResource.begin();
			itor != m_setAllocateResource.end();
			++itor )
		{
			CGUIResource* pRes = *itor;
			CheckResourceReference( pRes );
			DestroyAllocateResource( pRes );
		}
		m_setAllocateResource.clear();
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::UnloadAllResources( )
	{
		for( typename TMapResource::iterator itor = m_mapRegisterResource.begin();
			itor != m_mapRegisterResource.end();
			++itor )
		{
			itor->second->Unload();
		}

		for( typename TSetResource::iterator itor = m_setAllocateResource.begin();
			itor != m_setAllocateResource.end();
			++itor )
		{
			(*itor)->Unload();
		}
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::LoadAllResources( )
	{
		for( typename TMapResource::iterator itor = m_mapRegisterResource.begin();
			itor != m_mapRegisterResource.end();
			++itor )
		{
			itor->second->Load();
		}

		for( typename TSetResource::iterator itor = m_setAllocateResource.begin();
			itor != m_setAllocateResource.end();
			++itor )
		{
			(*itor)->Load();
		}
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::ReleaseRegisterResource( const CGUIString& rResName )
	{
		typename TMapResource::iterator itor = m_mapRegisterResource.find( rResName );
		if( itor != m_mapRegisterResource.end())
		{
			TRegisterResType* pRes = itor->second;
			DoRefRelease( pRes );
			CheckResourceReference( pRes );
			DestroyRegisterResource( pRes );
			m_mapRegisterResource.erase( itor );
		}
		else
		{
			CGUIException::ThrowException("[CGUIResourceManager::ReleaseRegisterResource]: failed to find resource by name <%s>", rResName.c_str());
		}
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::AddToAllocatePool( CGUIResource* pRes )
	{
		typename TSetResource::const_iterator itorFind = m_setAllocateResource.find( pRes );
		if( itorFind != m_setAllocateResource.end() )
		{
			//not found
			CGUIException::ThrowException("[CGUIResourceManager::AddToAllocatePool]: failed to add resource <%s:%s> to allocate pool", 
				pRes->GetName().c_str(),
				pRes->GetResourceType().c_str());
			return;
		}
		m_setAllocateResource.insert( pRes );
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline int32 CGUIResourceManager<TRegisterResType, TAllocateResType>::ReleaseFromAllocatePool( CGUIResource* pRes )
	{
		typename TSetResource::iterator itorFind = m_setAllocateResource.find( pRes );
		if( itorFind == m_setAllocateResource.end() )
		{
			//not found
			CGUIException::ThrowException("[CGUIResourceManager::ReleaseFromAllocatePool]: failed to remove resource <%s:%s> from allocate pool" );
			return -1;
		}
		CheckResourceReference( *itorFind );
		DestroyAllocateResource( *itorFind );
		m_setAllocateResource.erase( itorFind );
		return 0;
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::DestroyRegisterResource( TRegisterResType* pRes )
	{
		pRes->Unload();
		DestroyRegisterResourceImp( pRes );
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::DestroyAllocateResource( CGUIResource* pRes )
	{
		pRes->Unload();
		DestroyAllocateResourceImp( pRes );
	}
	//------------------------------------------------------------------------------
	template< class TRegisterResType, class TAllocateResType >
	inline void CGUIResourceManager<TRegisterResType, TAllocateResType>::CheckResourceReference( CGUIResource* pRes )
	{
		if( pRes->GetRefCount() != 0 )
		{
			CGUIException::ThrowException( "[CGUIResourceManager::CheckResourceReference]:resource reference is still in using[%d]: <%s:%s:%s>", 
				pRes->GetRefCount(),
				pRes->GetName().c_str(), 
				pRes->GetResourceType().c_str(),
				pRes->GetSceneName().c_str() );
		}
	}
	//------------------------------------------------------------------------------
}


#endif	//__KEN_GUIRESOURCEMANAGER_20091026_H__

