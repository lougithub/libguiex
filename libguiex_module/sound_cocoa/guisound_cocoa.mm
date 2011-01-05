/** 
 * @file guisound_cocoa.cpp
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_cocoa.h"
#include "guisounddata_cocoa.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_cocoa);
	//------------------------------------------------------------------------------
	const char* IGUISound_cocoa::StaticGetModuleName()
	{
		return "IGUISound_cocoa";
	}
	//------------------------------------------------------------------------------
	IGUISound_cocoa::IGUISound_cocoa()
	:IGUIInterfaceSound( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_cocoa::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* IGUISound_cocoa::CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUISoundData_cocoa* pSoundData = new CGUISoundData_cocoa( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DestroySoundData( CGUISoundData* pData )
	{
		delete pData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::Play( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::Stop( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::Pause( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_cocoa::IsPlaying( CGUISoundData* /*pSoundData*/ )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	
}//namespace guiex
