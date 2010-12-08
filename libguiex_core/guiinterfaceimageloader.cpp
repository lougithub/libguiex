/** 
* @file guiinterfaceimageloader.cpp
* @brief interface to load image from file or memory.
* @author ken
* @date 2006-07-13
*/

//============================================================================//
// include 
//============================================================================// 

#include <libguiex_core/guiinterfaceimageloader.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceImageLoader::StaticGetModuleName()
	{ 
		return "IGUIImageLoader";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceImageLoader::IGUIInterfaceImageLoader()
		:IGUIInterface(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceImageLoader::~IGUIInterfaceImageLoader()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
