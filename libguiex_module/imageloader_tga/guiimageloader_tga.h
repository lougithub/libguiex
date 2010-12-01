/** 
* @file guiimageloader_tga.h
* @brief interface to load image from file or memory.
* @author ken
* @date 2010-11-10
*/


#ifndef __GUI_INTERFACE_IMAGELOADER_TGA_H_20101110__
#define __GUI_INTERFACE_IMAGELOADER_TGA_H_20101110__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfaceimageloader.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIImageData_devil;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class IGUIImageLoader_tga
	* @brief interface for load image by using devil library
	*/
	class GUIEXPORT IGUIImageLoader_tga : public IGUIInterfaceImageLoader
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIImageLoader_tga();

		/** 
		* @brief destructor
		*/
		virtual ~IGUIImageLoader_tga();

		/**
		* @brief load image from file
		* @return pointer of CGUIImageData, NULL for failed
		*/
		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  );

		/**
		* @brief load image from memory
		* @return pointer of CGUIImageData, NULL for failed
		*/
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize );

		/**
		* @brief destroy image data
		*/
		virtual void	DestroyImageData(CGUIImageData* pImageData);

		/**
		* @brief used to delete this object
		*/
		virtual void	DeleteSelf();

	protected:
		/** 
		* @brief initialize render
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy render
		* @return 0 for success
		*/
		virtual void DoDestroy();

		CGUIImageData* LoadUncompressedTGA( uint8* pFileData, size_t nSize );
		CGUIImageData* LoadCompressedTGA( uint8* pFileData, size_t nSize );

	};


	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_INTERFACE_IMAGELOADER_TGA_H_20101110__

