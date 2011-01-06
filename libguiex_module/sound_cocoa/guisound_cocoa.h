/** 
 * @file guissound_cocoa.h
 * @brief sound module, use cocoa
 * @author ken
 * @date 2006-08-31
 */

#ifndef __GUI_SOUND_COCOA_20060831H__
#define __GUI_SOUND_COCOA_20060831H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacesound.h>
#include <map>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUISound_cocoa : public IGUIInterfaceSound
	{
	public:
		/** 
		 * @brief constructor
		 */
		IGUISound_cocoa();

		virtual CGUISoundData* CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroySoundData( CGUISoundData* pData );
		virtual CGUIMusicData* CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroyMusicData( CGUIMusicData* pData );

		virtual void Play( CGUISoundData* pSoundData );
		virtual void Stop( CGUISoundData* pSoundData );
		virtual void Pause( CGUISoundData* pSoundData );
		virtual bool IsPlaying( CGUISoundData* pSoundData );
		
		virtual void PlayMusic( CGUIMusicData* pSoundData );
		virtual void StopMusic( CGUIMusicData* pSoundData );
		virtual void PauseMusic( CGUIMusicData* pSoundData );
		virtual bool IsPlayingMusic( CGUIMusicData* pSoundData );

		virtual void DeleteSelf();

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
		
	protected:
		ALCcontext * m_pContext;
		ALCdevice * m_pDevice;
	
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_COCOA_20060831H__

