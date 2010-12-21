/** 
* @file guiinterfacestringconv.cpp
* @brief used to conver string between different code
* @author ken
* @date 2010-11-17
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacestringconv.h>
#include <libguiex_core/guiinterfacemanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT IGUIInterfaceStringConv*	GetInterface_StringConv( )
	{
		return CGUIInterfaceManager::Instance()->GetInterfaceStringConv();
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	const char* IGUIInterfaceStringConv::StaticGetModuleType()
	{ 
		return "IGUIStringConv";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceStringConv::IGUIInterfaceStringConv( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName)
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceStringConv::~IGUIInterfaceStringConv()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex
