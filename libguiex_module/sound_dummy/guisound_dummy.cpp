/** 
 * @file guisound_dummy.cpp
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_dummy.h"
#include "guisounddata_dummy.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_dummy);
	//------------------------------------------------------------------------------
	const char* IGUISound_dummy::StaticGetModuleName()
	{
		return "IGUISound_dummy";
	}
	//------------------------------------------------------------------------------
	IGUISound_dummy::IGUISound_dummy()
	:IGUIInterfaceSound( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_dummy::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* IGUISound_dummy::CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUISoundData_dummy* pSoundData = new CGUISoundData_dummy( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::DestroySoundData( CGUISoundData* pData )
	{
		delete pData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::Play( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::Stop( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::Pause( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_dummy::IsPlaying( CGUISoundData* /*pSoundData*/ )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	
}//namespace guiex
