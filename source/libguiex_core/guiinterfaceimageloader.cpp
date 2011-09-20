/** 
* @file guiinterfaceimageloader.cpp
* @brief interface to load image from file or memory.
* @author ken
* @date 2006-07-13
*/

//============================================================================//
// include 
//============================================================================// 

#include "guiinterfaceimageloader.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceImageLoader::StaticGetModuleType()
	{ 
		return "IGUIImageLoader";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceImageLoader::IGUIInterfaceImageLoader( const char* szModuleName )
		:IGUIInterface(StaticGetModuleType(), szModuleName)
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceImageLoader::~IGUIInterfaceImageLoader()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
