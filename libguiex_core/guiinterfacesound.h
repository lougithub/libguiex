/** 
* @file guiinterfacesound.h
* @brief sound interface
* @author ken
* @date 2006-08-31
*/

#ifndef __GUI_INTERFACE_SOUND_20060831_H_
#define __GUI_INTERFACE_SOUND_20060831_H_

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"
#include "guistring.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUISoundData;
	class CGUIMusicData;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIInterfaceSound
	* @brief sound interface
	*/
	class GUIEXPORT IGUIInterfaceSound : public IGUIInterface
	{
	public:
		IGUIInterfaceSound( const char* szModuleName );
		virtual ~IGUIInterfaceSound();

		virtual CGUISoundData* CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath ) = 0;
		virtual void DestroySoundData( CGUISoundData* pData ) = 0;
		virtual CGUIMusicData* CreateMusicData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath ) = 0;
		virtual void DestroyMusicData( CGUIMusicData* pData ) = 0;

		virtual void PlayEffect( CGUISoundData* pSoundData ) = 0;
		virtual void StopEffect( CGUISoundData* pSoundData ) = 0;
		virtual void PauseEffect( CGUISoundData* pSoundData ) = 0;
		virtual bool IsPlayingEffect( CGUISoundData* pSoundData ) = 0;

		virtual void PlayMusic( CGUIMusicData* pSoundData ) = 0;
		virtual void StopMusic( ) = 0;
		virtual void PauseMusic( ) = 0;
		virtual bool IsPlayingMusic( ) = 0;
		virtual CGUIMusicData* GetMusicPlaying() = 0;

		virtual void Update( real fDeltaTime );

		void PlayEffect( const CGUIString& rSoundName );
		void PlayMusic( const CGUIString& rMusicName );

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex


#endif //__GUI_INTERFACE_SOUND_20060831_H_
