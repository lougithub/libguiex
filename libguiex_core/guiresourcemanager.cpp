/** 
 * @file guiresourcemanager.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-26
 */

//============================================================================//
// include
//============================================================================// 
#include "guiresourcemanager.h"
#include "guisystem.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIResourceManagerBase::CGUIResourceManagerBase( const char* szResMgrType )
		:m_strMgrType( szResMgrType )
	{
		GSystem->RegisterResourceManager( this );
	}
	//------------------------------------------------------------------------------
	CGUIResourceManagerBase::~CGUIResourceManagerBase()
	{
		GSystem->UnregisterResourceManager( this );
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIResourceManagerBase::GetManagerType() const
	{
		return m_strMgrType;
	}
	//------------------------------------------------------------------------------
	void CGUIResourceManagerBase::DoRefRelease( CGUIResource* pRes )
	{
		pRes->DoRefReleaseByManager();
	}
	//------------------------------------------------------------------------------
}
