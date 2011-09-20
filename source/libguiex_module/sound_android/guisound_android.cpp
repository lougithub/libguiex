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
#include <libguiex_core/guilogmsgmanager.h>


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
	IGUISound_android::IGUISound_android()
	:IGUIInterfaceSound( StaticGetModuleName() )
	,m_pCurrentMusic( NULL )
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
		CGUISoundData_android* pSoundData = new CGUISoundData_android( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::DestroySoundData( CGUISoundData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_android::CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUIMusicData_android* pMusicData = new CGUIMusicData_android( rName, rSceneName, rPath );
		return pMusicData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::DestroyMusicData( CGUIMusicData* pData )
	{
		if( m_pCurrentMusic == pData )
		{
			StopMusic();
		}
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PlayEffect( CGUISoundData* pSoundData )
	{
		CGUISoundData_android* pAndroidSoundData = (CGUISoundData_android*)(pSoundData);
		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "PlayEffect", pAndroidSoundData->GetEffectID());

		GUI_TRACE( GUI_FORMAT("[IGUISound_android::PlayEffect]: play effect %d", pAndroidSoundData->GetEffectID()));
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::StopEffect( CGUISoundData* pSoundData )
	{
		CGUISoundData_android* pAndroidSoundData = (CGUISoundData_android*)(pSoundData);
		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "StopEffect", pAndroidSoundData->GetEffectID());

		GUI_TRACE( GUI_FORMAT("[IGUISound_android::StopEffect]: stop effect %d", pAndroidSoundData->GetEffectID()));
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PauseEffect( CGUISoundData* pSoundData )
	{
		CGUISoundData_android* pAndroidSoundData = (CGUISoundData_android*)(pSoundData);
		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "PauseEffect", pAndroidSoundData->GetEffectID());
	}
	//------------------------------------------------------------------------------
	bool IGUISound_android::IsPlayingEffect( CGUISoundData* pSoundData )
	{
		CGUISoundData_android* pAndroidSoundData = (CGUISoundData_android*)(pSoundData);
		bool bRet;
		CGUISystem::CallJavaMethod_bool("org/guiex/lib/GuiexLibSound", "IsPlayingEffect",bRet, pAndroidSoundData->GetEffectID());
		return bRet;
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PlayMusic( CGUIMusicData* pSoundData, bool bLoop )
	{
		if( m_pCurrentMusic != pSoundData )
		{
			StopMusic();
		}

		m_pCurrentMusic = (CGUIMusicData_android*)(pSoundData);
		if( !m_pCurrentMusic )
		{
			return;
		}
		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "PlayMusic", bLoop);
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::StopMusic(  )
	{
		if( m_pCurrentMusic )
		{
			CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "StopMusic");
			m_pCurrentMusic->Unload();
			m_pCurrentMusic = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::PauseMusic(  )
	{
		if( m_pCurrentMusic )
		{
			// To pause the sound
			CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "PauseMusic");
		}
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::ResumeMusic(  )
	{
		if( m_pCurrentMusic )
		{
			// To pause the sound
			CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "ResumeMusic");
		}
	}
	//------------------------------------------------------------------------------
	void IGUISound_android::RewindMusic(  )
	{
		if( m_pCurrentMusic )
		{
			// To pause the sound
			CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "RewindMusic");
		}
	}
	//------------------------------------------------------------------------------
	bool IGUISound_android::IsPlayingMusic(  )
	{
		if( m_pCurrentMusic )
		{
			bool bRet;
			CGUISystem::CallJavaMethod_bool("org/guiex/lib/GuiexLibSound", "IsPlayingMusic", bRet);
			return bRet;
		}
		return false;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* IGUISound_android::GetMusicPlaying()
	{
		return m_pCurrentMusic;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
