/** 
 * @file guisound_openal.cpp
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_openal.h"
#include "guisounddata_openal.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisounddata.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_openal);
	//------------------------------------------------------------------------------
	const char* IGUISound_openal::StaticGetModuleName()
	{
		return "IGUISound_openal";
	}
	//------------------------------------------------------------------------------
	IGUISound_openal::IGUISound_openal()
	:IGUIInterfaceSound( StaticGetModuleName() )
	,m_pContext(NULL)
	,m_pDevice(NULL)
	{
	}
	//------------------------------------------------------------------------------
	IGUISound_openal::~IGUISound_openal()
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_openal::DoInitialize(void* )
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
	void IGUISound_openal::DoDestroy()
	{
		//Disable context
		alcMakeContextCurrent(NULL);
		//Release context(s)
		alcDestroyContext(m_pContext);
		//Close device
		alcCloseDevice(m_pDevice);
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* IGUISound_openal::CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
	{
		CGUISoundData_openal* pSoundData = new CGUISoundData_openal( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::DestroySoundData( CGUISoundData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		delete pData;
	}
	//------------------------------------------------------------------------------
	CGUIString IGUISound_openal::GetALErrorString(ALenum err)
	{
		switch(err)
		{
			case AL_NO_ERROR:
				return CGUIString("AL_NO_ERROR");
				break;
				
			case AL_INVALID_NAME:
				return CGUIString("AL_INVALID_NAME");
				break;
				
			case AL_INVALID_ENUM:
				return CGUIString("AL_INVALID_ENUM");
				break;
				
			case AL_INVALID_VALUE:
				return CGUIString("AL_INVALID_VALUE");
				break;
				
			case AL_INVALID_OPERATION:
				return CGUIString("AL_INVALID_OPERATION");
				break;
				
			case AL_OUT_OF_MEMORY:
				return CGUIString("AL_OUT_OF_MEMORY");
				break;
				
			default:
				return CGUIString("Unknow error for AL");
		};
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::Play( CGUISoundData* pSoundData )
	{
		CGUISoundData_openal* pOpenalSoundData = (CGUISoundData_openal*)(pSoundData);
		
		//tell the sound to loop continuously
		alSourcei( pOpenalSoundData->m_nSourceId, AL_LOOPING, AL_FALSE );
		
		//play the sound
		alSourcePlay( pOpenalSoundData->m_nSourceId );
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::Stop( CGUISoundData* pSoundData )
	{
		CGUISoundData_openal* pOpenalSoundData = (CGUISoundData_openal*)(pSoundData);
		
		// To stop the sound
		alSourceStop( pOpenalSoundData->m_nSourceId );
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::Pause( CGUISoundData* pSoundData )
	{
		CGUISoundData_openal* pOpenalSoundData = (CGUISoundData_openal*)(pSoundData);

		// To stop the sound
		alSourcePause( pOpenalSoundData->m_nSourceId );
	}
	//------------------------------------------------------------------------------
	bool IGUISound_openal::IsPlaying( CGUISoundData* pSoundData )
	{
		CGUISoundData_openal* pOpenalSoundData = (CGUISoundData_openal*)(pSoundData);

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
	
}//namespace guiex
