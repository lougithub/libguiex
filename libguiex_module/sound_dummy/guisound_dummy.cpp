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
#include "guimusicdata_dummy.h"
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
	CGUIMusicData* IGUISound_dummy::CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUIMusicData_dummy* pSoundData = new CGUIMusicData_dummy( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::DestroyMusicData( CGUIMusicData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::PlayEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::StopEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::PauseEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_dummy::IsPlayingEffect( CGUISoundData* /*pSoundData*/ )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::PlayMusic( CGUIMusicData* pSoundData )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::StopMusic(  )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_dummy::PauseMusic(  )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_dummy::IsPlayingMusic(  )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_dummy::GetMusicPlaying()
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
