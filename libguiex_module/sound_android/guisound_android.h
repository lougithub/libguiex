/** 
 * @file guissound_android.h
 * @brief sound module, use android
 * @author ken
 * @date 2011-05-10
 */

#ifndef __GUI_SOUND_ANDROID_20110510_H__
#define __GUI_SOUND_ANDROID_20110510_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacesound.h>
#include <map>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUISound_android : public IGUIInterfaceSound
	{
	public:
		/** 
		 * @brief constructor
		 */
		IGUISound_android();

		virtual CGUISoundData* CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroySoundData( CGUISoundData* pData );
		virtual CGUIMusicData* CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroyMusicData( CGUIMusicData* pData );

		virtual void PlayEffect( CGUISoundData* pSoundData );
		virtual void StopEffect( CGUISoundData* pSoundData );
		virtual void PauseEffect( CGUISoundData* pSoundData );
		virtual bool IsPlayingEffect( CGUISoundData* pSoundData );

		virtual void PlayMusic( CGUIMusicData* pSoundData );
		virtual void StopMusic(  );
		virtual void PauseMusic(  );
		virtual bool IsPlayingMusic(  );
		virtual CGUIMusicData* GetMusicPlaying();

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();
	
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_ANDROID_20110510_H__

