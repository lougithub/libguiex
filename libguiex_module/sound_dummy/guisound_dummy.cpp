/** 
 * @file guisound_dummy.cpp
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_dummy.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_dummy);
	//------------------------------------------------------------------------------
	const char* IGUISound_dummy::StaticGetModuleName()
	{
		return "IGUISound_dummy";
	}
	//------------------------------------------------------------------------------
	IGUISound_dummy::IGUISound_dummy()
	:IGUIInterfaceSound( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUISound_dummy::~IGUISound_dummy()
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_dummy::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int32 IGUISound_dummy::LoadEffect( int32 /*nID*/,  const CGUIString& /*rFileName*/)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::UnloadEffect( int32 /*nID*/ )
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUISound_dummy::PlayEffect( int32 /*nID*/ )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	
}//namespace guiex
