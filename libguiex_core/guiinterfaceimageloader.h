/** 
* @file guiinterfaceimageloader.h
* @brief interface to load image from file or memory.
* @author ken
* @date 2006-07-13
*/


#ifndef __GUI_INTERFACE_IMAGELOADER_H_20060713__
#define __GUI_INTERFACE_IMAGELOADER_H_20060713__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiinterface.h"
#include "guidatachunk.h"
#include "guiimagedata.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class IGUIInterfaceImageLoader
	* @brief interface for load image
	*/
	class GUIEXPORT IGUIInterfaceImageLoader : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceImageLoader( const char* szModuleName );

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfaceImageLoader();

	public:
		/**
		* @brief load image from file
		* @return pointer of CGUIImageData, NULL for failed
		*/
		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  ) = 0;

		/**
		* @brief load image from memory
		* @return pointer of CGUIImageData, NULL for failed
		*/
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize) = 0;

		/**
		* @brief destroy image data
		*/
		virtual void	DestroyImageData(CGUIImageData* pImageData) = 0;

	public: 
		static const char* StaticGetModuleType();
	};
}//namespace guiex

#endif //__GUI_INTERFACE_IMAGELOADER_H_20060713__

