/** 
 * @file guiinterfacefilesys.cpp
 * @brief file system's interface
 * @author ken
 * @date 2006-07-06
 */

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacefilesys.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceFileSys::StaticGetModuleType()
	{ 
		return "IGUIFileSys";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceFileSys::IGUIInterfaceFileSys( const char* szModuleName )
		:IGUIInterface(StaticGetModuleType(), szModuleName)
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceFileSys::~IGUIInterfaceFileSys()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
