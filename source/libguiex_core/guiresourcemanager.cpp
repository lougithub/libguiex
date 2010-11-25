/** 
 * @file guiresourcemanager.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-26
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiresourcemanager.h>
#include <libguiex_core/guiwidgetsystem.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIResourceManagerBase::CGUIResourceManagerBase()
	{
		CGUISystem::Instance()->RegisterResourceManager( this );
	}
	//------------------------------------------------------------------------------
	CGUIResourceManagerBase::~CGUIResourceManagerBase()
	{
		CGUISystem::Instance()->UnregisterResourceManager( this );
	}
	//------------------------------------------------------------------------------
}
