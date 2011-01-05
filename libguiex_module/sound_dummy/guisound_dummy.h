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
		
		virtual void Play( CGUISoundData* pSoundData );
		virtual void Stop( CGUISoundData* pSoundData );
		virtual void Pause( CGUISoundData* pSoundData );
		virtual bool IsPlaying( CGUISoundData* pSoundData );
		
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

