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

		virtual void Play( CGUISoundData* pSoundData ) = 0;
		virtual void Stop( CGUISoundData* pSoundData ) = 0;
		virtual void Pause( CGUISoundData* pSoundData ) = 0;
		virtual bool IsPlaying( CGUISoundData* pSoundData ) = 0;

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex


#endif //__GUI_INTERFACE_SOUND_20060831_H_
