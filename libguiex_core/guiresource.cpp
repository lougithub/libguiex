/** 
* @file guiresource.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-26
*/


//============================================================================//
// include
//============================================================================//
#include "guiresource.h"
#include "guiexception.h"
#include "guiresourcemanager.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIResource::CGUIResource( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rResourceType, CGUIResourceManagerBase* pResourceManager )
		:m_strName( rName )
		,m_strSceneName( rSceneName )
		,m_eIsLoaded( LOADSTATE_Unloaded )
		,m_strResourceType( rResourceType )
		,m_pResourceManager( pResourceManager )
	{
		GUI_ASSERT( m_pResourceManager, "invalid parameter" );
	}
	//------------------------------------------------------------------------------
	CGUIResource::~CGUIResource()
	{
		if( m_eIsLoaded != LOADSTATE_Unloaded )
		{
			throw CGUIException( 
				"[CGUIResource::~CGUIResource]:resource <%s:%s:%s> hasn't been unloaded", 
				GetName().c_str(), 
				GetResourceType().c_str(),
				GetSceneName().c_str() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIResource::DoRefReleaseByManager()
	{
		DoDecreaseReference();
	}
	//------------------------------------------------------------------------------
	void CGUIResource::RefRelease()
	{
		m_pResourceManager->DeallocateResource( this );
	}
	//------------------------------------------------------------------------------
	int32 CGUIResource::Load() const
	{
		if( m_eIsLoaded ==  LOADSTATE_Loaded)
		{
			return 0;
		}
		int32 ret = DoLoad();
		if( ret != 0 )
		{
			throw CGUIException( "[CGUIResource::Load]: failed to load resource <%s: %s>",
				m_strSceneName.c_str(),
				m_strName.c_str());
			return ret;
		}
		m_eIsLoaded = LOADSTATE_Loaded;
		return ret;
	}
	//------------------------------------------------------------------------------
	void CGUIResource::Unload()
	{
		if( m_eIsLoaded == LOADSTATE_Unloaded )
		{
			return;
		}

		DoUnload();
		m_eIsLoaded = LOADSTATE_Unloaded;
	}
	//------------------------------------------------------------------------------
	bool CGUIResource::IsLoaded() const
	{
		return m_eIsLoaded == LOADSTATE_Loaded;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIResource::GetName() const
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIResource::GetSceneName() const
	{
		return m_strSceneName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIResource::GetResourceType() const
	{
		return m_strResourceType;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------


}
