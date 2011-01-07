/** 
 * @file guissound_dummy.h
 * @brief sound module, use dummy
 * @author ken
 * @date 2006-08-31
 */

#ifndef __GUI_SOUND_DUMMY_20060831H__
#define __GUI_SOUND_DUMMY_20060831H__

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
	class GUIEXPORT IGUISound_dummy : public IGUIInterfaceSound
	{
	public:
		/** 
		 * @brief constructor
		 */
		IGUISound_dummy();

		virtual CGUISoundData* CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroySoundData( CGUISoundData* pData );
		virtual CGUIMusicData* CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual void DestroyMusicData( CGUIMusicData* pData );

		virtual void PlayEffect( CGUISoundData* pSoundData );
		virtual void StopEffect( CGUISoundData* pSoundData );
		virtual void PauseEffect( CGUISoundData* pSoundData );
		virtual bool IsPlayingEffect( CGUISoundData* pSoundData );

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
		
	
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_DUMMY_20060831H__

