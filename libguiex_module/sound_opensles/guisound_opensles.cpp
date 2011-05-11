/** 
 * @file guisound_opensles.cpp
 * @brief sound module, use opensles
 * @author ken
 * @date 2011-05-10
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_opensles.h"
#include "guisounddata_opensles.h"
#include "guimusicdata_opensles.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_opensles);
	//------------------------------------------------------------------------------
	const char* IGUISound_opensles::StaticGetModuleName()
	{
		return "IGUISound_opensles";
	}
	//------------------------------------------------------------------------------
	IGUISound_opensles::IGUISound_opensles()
	:IGUIInterfaceSound( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_opensles::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* IGUISound_opensles::CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUISoundData_opensles* pSoundData = new CGUISoundData_opensles( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::DestroySoundData( CGUISoundData* pData )
	{
		delete pData;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_opensles::CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::DestroyMusicData( CGUIMusicData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::PlayEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::StopEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::PauseEffect( CGUISoundData* /*pSoundData*/ )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_opensles::IsPlayingEffect( CGUISoundData* /*pSoundData*/ )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::PlayMusic( CGUIMusicData* pSoundData )
	{

	}
	//---------------------------CGUIMusicData_opensles---------------------------------------------------
	void IGUISound_opensles::StopMusic(  )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_opensles::PauseMusic(  )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_opensles::IsPlayingMusic(  )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_opensles::GetMusicPlaying()
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
