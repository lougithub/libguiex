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
#include <libguiex_core/guisystem.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIResourceManagerBase::CGUIResourceManagerBase()
	{
		GSystem->RegisterResourceManager( this );
	}
	//------------------------------------------------------------------------------
	CGUIResourceManagerBase::~CGUIResourceManagerBase()
	{
		GSystem->UnregisterResourceManager( this );
	}
	//------------------------------------------------------------------------------
}
