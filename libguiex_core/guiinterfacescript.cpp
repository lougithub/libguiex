/** 
* @file guiinterfacescript.cpp
* @brief interface for script module
* @author ken
* @date 2006-04-04
*/

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacescript.h"
#include "guiinterfacemanager.h"

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


