/** 
 * @file guissound_openal.h
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

#ifndef __GUI_SOUND_OPENAL_20060831H__
#define __GUI_SOUND_OPENAL_20060831H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacesound.h>
#if GUIEX_PLATFORM_MAC
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUISound_openal : public IGUIInterfaceSound
	{
	public:
		IGUISound_openal();
		virtual ~IGUISound_openal();
		
		virtual CGUISoundData* CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroySoundData( CGUISoundData* pData );
		virtual CGUIMusicData* CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroyMusicData( CGUIMusicData* pData );

		virtual void PlayEffect( CGUISoundData* pSoundData );
		virtual void StopEffect( CGUISoundData* pSoundData );
		virtual void PauseEffect( CGUISoundData* pSoundData );
		virtual bool IsPlayingEffect( CGUISoundData* pSoundData );
		
		virtual void PlayMusic( CGUIMusicData* pSoundData, bool bLoop );
		virtual void StopMusic(  );
		virtual void PauseMusic(  );
		virtual void ResumeMusic( );
		virtual void RewindMusic(  );
		virtual bool IsPlayingMusic(  );
		virtual CGUIMusicData* GetMusicPlaying();

		virtual void Update( real fDeltaTime );
		
		virtual void DeleteSelf();

		CGUIString GetALErrorString(ALenum err);
		
	protected:
		/** 
		 * @brief initialize sound
		 * @return 0 for success
		 */
		virtual int DoInitialize(void* );
		
		/** 
		 * @brief destroy sound 
		 * @return 0 for success
		 */
		virtual void DoDestroy();
		
	private:
		ALCcontext * m_pContext;
		ALCdevice * m_pDevice;

		class CGUIMusicData_openal* m_pCurrentMusic;
		
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_OPENAL_20060831H__

