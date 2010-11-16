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
#include <vector>



//============================================================================//
// declare
//============================================================================//





//============================================================================//
// class
//============================================================================//
namespace guiex
{
	/**
	* @class CGUIResourceManager
	* 
	*/
	template< class TResType >
	class GUIEXPORT CGUIResourceManager
	{
	public:
		CGUIResourceManager();
		virtual ~CGUIResourceManager();

	public:
		int32		AddResource( TResType* pRes );
		TResType*	FindResource( const CGUIString& rResName );

		const std::map< CGUIString, TResType* >&	GetResourceMap() const;

		void		ReleaseAllResources( );
		void		ReleaseSceneResources( const CGUIString& rSceneName );
		void		ReleaseResource( const CGUIString& rResName );

		void		LoadAllResources( );
		void		UnloadAllResources( );


	protected:
		typedef std::map< CGUIString, TResType* >	TMapResource;
		TMapResource								m_mapResource;
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
	const std::map< CGUIString, TResType* >&	CGUIResourceManager<TResType>::GetResourceMap() const
	{
		return m_mapResource;
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline int32 CGUIResourceManager<TResType>::AddResource( TResType* pRes )
	{
		if( m_mapResource.find( pRes->GetName()) != m_mapResource.end())
		{
			throw CGUIException( "the resource <%s> has existed", pRes->GetName().c_str());
			return -1;
		}

		m_mapResource.insert( std::make_pair( pRes->GetName(), pRes));
		return 0;
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline TResType* CGUIResourceManager<TResType>::FindResource( const CGUIString& rResName )
	{
		typename TMapResource::iterator itor = m_mapResource.find( rResName );
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
	inline void		CGUIResourceManager<TResType>::ReleaseResource( const CGUIString& rResName )
	{
		typename TMapResource::iterator itor = m_mapResource.find( rResName );
		if( itor != m_mapResource.end())
		{
			delete itor->second;
			m_mapResource.erase( itor );
		}
		else
		{
			throw CGUIException("[CGUIResourceManager::ReleaseResource]: failed to find resource by name <%s>", rResName.c_str());
		}
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseAllResources( )
	{
		for( typename TMapResource::iterator itor = m_mapResource.begin();
			itor != m_mapResource.end();
			++itor )
		{
			delete itor->second;
		}
		m_mapResource.clear();
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
	}
	//------------------------------------------------------------------------------
	template< class TResType >
	inline void CGUIResourceManager<TResType>::ReleaseSceneResources( const CGUIString& rSceneName )
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
			typename TMapResource::iterator itorMap = m_mapResource.find( *itor );
			delete itorMap->second;
			m_mapResource.erase( itorMap );
		}
	}
	//------------------------------------------------------------------------------
}


#endif	//__KEN_GUIRESOURCEMANAGER_20091026_H__

