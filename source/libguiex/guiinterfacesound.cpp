/** 
 * @file guiinterfacesound.cpp
 * @brief sound interface
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiinterfacesound.h>
#include <libguiex_core\guiinterfacemanager.h>


namespace guiex
{
//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------ 
GUIEXPORT IGUIInterfaceSound*	GetInterface_Sound( )
{
	return static_cast<IGUIInterfaceSound*>(	GetInterface( "IGUISound"));
}
//------------------------------------------------------------------------------
IGUIInterfaceSound::IGUIInterfaceSound( )
{

}
//------------------------------------------------------------------------------ 
IGUIInterfaceSound::~IGUIInterfaceSound()
{

}
//------------------------------------------------------------------------------ 

}//namespace guiex


