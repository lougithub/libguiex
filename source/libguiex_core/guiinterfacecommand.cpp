/** 
* @file guiinterfacecommand.cpp
* @brief command from out of application, use script
* to control ui system
* @author ken
* @date 2006-09-26
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacecommand.h>
#include <libguiex_core/guiinterfacemanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUIEXPORT IGUIInterfaceCommand*	GetInterface_Command( )
	{
		return CGUIInterfaceManager::Instance()->GetInterfaceCommand();
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceCommand::IGUIInterfaceCommand( )
		:IGUIInterface("IGUICommand")
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceCommand::~IGUIInterfaceCommand()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex
