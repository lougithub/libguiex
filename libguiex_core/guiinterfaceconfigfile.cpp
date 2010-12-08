/** 
* @file guiinterfaceconfigfile.cpp
* @brief file config file for widget
* @author ken
* @date 2006-09-13
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfaceconfigfile.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceConfigFile::StaticGetModuleName()
	{ 
		return "IGUIConfigFile";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceConfigFile::IGUIInterfaceConfigFile( )
		:IGUIInterface(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceConfigFile::~IGUIInterfaceConfigFile()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex
