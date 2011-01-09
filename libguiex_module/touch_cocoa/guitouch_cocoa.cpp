/** 
* @file guitouch_cocoa.cpp
* @brief mouse interface, use winapi
* @author bxliu
* @date 2011-01-03
*/

//============================================================================//
// include
//============================================================================// 
#include "guitouch_cocoa.h"



//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUITouch_cocoa);
	//------------------------------------------------------------------------------
	const char* IGUITouch_cocoa::StaticGetModuleName()
	{
		return "IGUITouch_cocoa";
	}
	//------------------------------------------------------------------------------
	IGUITouch_cocoa::IGUITouch_cocoa()
		:IGUIInterfaceMouse( StaticGetModuleName() )
	{
	
	}
	IGUITouch_cocoa::~IGUITouch_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int IGUITouch_cocoa::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUITouch_cocoa::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUITouch_cocoa::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------


}//namespace guiex
