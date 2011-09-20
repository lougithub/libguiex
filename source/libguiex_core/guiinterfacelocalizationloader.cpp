/** 
 * @file guiinterfacelocalizationloader.cpp
 * @brief load localization file
 * @author ken
 * @date 2011-04-28
 */

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacelocalizationloader.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceLocalizationLoader::StaticGetModuleType()
	{ 
		return "IGUILocalizationLoader";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceLocalizationLoader::IGUIInterfaceLocalizationLoader( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceLocalizationLoader::~IGUIInterfaceLocalizationLoader()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex
