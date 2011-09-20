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
		IGUIImageLoader_tga();
		virtual ~IGUIImageLoader_tga();

		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  );
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize );

		virtual void DestroyImageData(CGUIImageData* pImageData);

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		CGUIImageData* LoadUncompressedTGA( uint8* pFileData, size_t nSize );
		CGUIImageData* LoadCompressedTGA( uint8* pFileData, size_t nSize );

	public: 
		static const char* StaticGetModuleName();
	};


	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_INTERFACE_IMAGELOADER_TGA_H_20101110__

