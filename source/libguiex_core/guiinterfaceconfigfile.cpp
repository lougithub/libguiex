/** 
* @file guiinterfaceconfigfile.cpp
* @brief file config file for widget
* @author ken
* @date 2006-09-13
*/

//============================================================================//
// include
//============================================================================// 
#include "guiinterfaceconfigfile.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceConfigFile::StaticGetModuleType()
	{ 
		return "IGUIConfigFile";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceConfigFile::IGUIInterfaceConfigFile( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceConfigFile::~IGUIInterfaceConfigFile()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex
