/** 
* @file guiinterfacesound.cpp
* @brief sound interface
* @author ken
* @date 2006-08-31
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacesound.h>
#include <libguiex_core/guiinterfacemanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------ 
	GUIEXPORT IGUIInterfaceSound* GetInterface_Sound( )
	{
		return CGUIInterfaceManager::Instance()->GetInterfaceSound();
	}
	//------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceSound::StaticGetModuleType()
	{ 
		return "IGUISound";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceSound::IGUIInterfaceSound( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{

	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceSound::~IGUIInterfaceSound()
	{

	}
	//------------------------------------------------------------------------------ 
}//namespace guiex


