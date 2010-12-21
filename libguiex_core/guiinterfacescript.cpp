/** 
* @file guiinterfacescript.cpp
* @brief interface for script module
* @author ken
* @date 2006-04-04
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacescript.h>
#include <libguiex_core/guiinterfacemanager.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceScript::StaticGetModuleType()
	{ 
		return "IGUIScript";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceScript::IGUIInterfaceScript( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{

	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceScript::~IGUIInterfaceScript()
	{

	}

}//namespace guiex


