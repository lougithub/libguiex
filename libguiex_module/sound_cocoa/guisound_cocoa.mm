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
#include "guimusicdata_cocoa.h"
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
		m_pDevice = alcOpenDevice(NULL);
		if (m_pDevice == NULL)
		{
			return -1;
		}
		
		//Create context(s)
		m_pContext=alcCreateContext(m_pDevice, NULL);
		
		//Set active context
		alcMakeContextCurrent(m_pContext);
		
		// Clear Error Code
		alGetError();
		
		float vec[6];
		vec[0] = 0.0f; //forward vector x value
		vec[1] = 0.0f; //forward vector y value
		vec[2] = -1.0f; //forward vector z value
		vec[3] = 0; //up vector x value
		vec[4] = 1.0f; //up vector y value
		vec[5] = 0.0f; //up vector z value
		
		//set current listener position
		alListener3f(AL_POSITION, 0, 0, 0);
		
		//set current listener orientation
		alListenerfv(AL_ORIENTATION, vec);
		
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DoDestroy()
	{
		//Disable context
		alcMakeContextCurrent(NULL);
		//Release context(s)
		alcDestroyContext(m_pContext);
		//Close device
		alcCloseDevice(m_pDevice);

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
	CGUIMusicData* IGUISound_cocoa::CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DestroyMusicData( CGUIMusicData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}	
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::Play( CGUISoundData* pSoundData )
	{
		CGUISoundData_cocoa* pOpenalSoundData = (CGUISoundData_cocoa*)(pSoundData);
		
		//tell the sound to loop continuously
		alSourcei( pOpenalSoundData->m_nSourceId, AL_LOOPING, AL_FALSE );
		
		//play the sound
		alSourcePlay( pOpenalSoundData->m_nSourceId );
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::Stop( CGUISoundData* pSoundData )
	{
		CGUISoundData_cocoa* pOpenalSoundData = (CGUISoundData_cocoa*)(pSoundData);
		
		// To stop the sound
		alSourceStop( pOpenalSoundData->m_nSourceId );
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::Pause( CGUISoundData* pSoundData )
	{
		CGUISoundData_cocoa* pOpenalSoundData = (CGUISoundData_cocoa*)(pSoundData);

		// To stop the sound
		alSourcePause( pOpenalSoundData->m_nSourceId );
	}
	//------------------------------------------------------------------------------
	bool IGUISound_cocoa::IsPlaying( CGUISoundData* pSoundData )
	{
		CGUISoundData_cocoa* pOpenalSoundData = (CGUISoundData_cocoa*)(pSoundData);

		// To stop the sound
		ALint state;
		alGetSourcei( pOpenalSoundData->m_nSourceId, AL_SOURCE_STATE, &state );
		if( state == AL_PLAYING )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::PlayMusic( CGUIMusicData* pSoundData )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::StopMusic( CGUIMusicData* pSoundData )
	{

	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::PauseMusic( CGUIMusicData* pSoundData )
	{

	}
	//------------------------------------------------------------------------------
	bool IGUISound_cocoa::IsPlayingMusic( CGUIMusicData* pSoundData )
	{
		return false;
	}
	//------------------------------------------------------------------------------
	
}//namespace guiex
