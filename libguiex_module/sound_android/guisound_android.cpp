/** 
 * @file guisound_android.cpp
 * @brief sound module, use android
 * @author ken
 * @date 2011-05-10
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_android.h"
#include "guisounddata_android.h"
#include "guimusicdata_android.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_android);
	//------------------------------------------------------------------------------
	const char* IGUISound_android::StaticGetModuleName()
	{
		return "IGUISound_android";
	}
	//------------------------------------------------------------------------------
	IGUISound_android::IGUISound_opensles()
	:IGUIInterfaceSound( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_android::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* IGUISound_android::CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUISoundData_android* pSoundData = new CGUISoundData_opensles( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::DestroySoundData( CGUISoundData* pData )
	{
		delete pData;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_android::CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::DestroyMusicData( CGUIMusicData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PlayEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_android::StopEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PauseEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_android::IsPlayingEffect( CGUISoundData* /*pSoundData*/ )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PlayMusic( CGUIMusicData* pSoundData )
	{

	}
	//---------------------------CGUIMusicData_android---------------------------------------------------
	void IGUISound_android::StopMusic(  )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PauseMusic(  )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_android::IsPlayingMusic(  )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_android::GetMusicPlaying()
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
