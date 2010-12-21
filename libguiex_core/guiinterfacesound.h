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

		/**
		* @brief load effect.
		* @return 0 for success.
		*/
		virtual int32 LoadEffect( int32 nIdx,  const CGUIString& rFileName) = 0;

		/**
		* @brief unload effect.
		*/
		virtual void UnloadEffect( int32 nIdx ) = 0;

		/**
		* @brief play effect.
		*/
		virtual int32 PlayEffect( int32 nIdx ) = 0;

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex


#endif //__GUI_INTERFACE_SOUND_20060831_H_
