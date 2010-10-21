/** 
* @file guiimageloader_devil.h
* @brief interface to load image from file or memory.
* @author ken
* @date 2006-07-13
*/


#ifndef __GUI_INTERFACE_IMAGELOADER_DEVIL_H_20060713__
#define __GUI_INTERFACE_IMAGELOADER_DEVIL_H_20060713__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiinterfaceimageloader.h>

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
	* @class IGUIImageLoader_devil
	* @brief interface for load image by using devil library
	*/
	class GUIEXPORT IGUIImageLoader_devil : public IGUIInterfaceImageLoader
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIImageLoader_devil();

		/** 
		* @brief destructor
		*/
		virtual ~IGUIImageLoader_devil();

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
	};


	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_INTERFACE_IMAGELOADER_DEVIL_H_20060713__

