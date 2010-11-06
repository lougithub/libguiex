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


namespace guiex
{
//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
GUIEXPORT IGUIInterfaceCommand*	GetInterface_Command( )
{
	return static_cast<IGUIInterfaceCommand*>(	GetInterface( "IGUICommand"));
}
//------------------------------------------------------------------------------
IGUIInterfaceCommand::IGUIInterfaceCommand( )
{
}
//------------------------------------------------------------------------------
IGUIInterfaceCommand::~IGUIInterfaceCommand()
{
}
//------------------------------------------------------------------------------
}//namespace guiex
