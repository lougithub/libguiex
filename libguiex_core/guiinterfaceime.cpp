/** 
 * @file guiime.cpp
 * @brief ime, used by edit
 * @author ken
 * @date 2006-08-25
 */


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfaceime.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceIme::StaticGetModuleType()
	{ 
		return "IGUIIme";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceIme::IGUIInterfaceIme( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName)
	{

	}
//------------------------------------------------------------------------------
	IGUIInterfaceIme::~IGUIInterfaceIme()
	{
	}
//------------------------------------------------------------------------------

}//namespace guiex
