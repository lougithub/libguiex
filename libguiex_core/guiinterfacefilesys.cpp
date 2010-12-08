/** 
 * @file guiinterfacefilesys.cpp
 * @brief file system's interface
 * @author ken
 * @date 2006-07-06
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacefilesys.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceFileSys::StaticGetModuleName()
	{ 
		return "IGUIFileSys";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceFileSys::IGUIInterfaceFileSys( )
		:IGUIInterface(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceFileSys::~IGUIInterfaceFileSys()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
