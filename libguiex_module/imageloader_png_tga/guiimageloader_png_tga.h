/** 
* @file guiimageloader_png_tga.h
* @brief interface to load image from file or memory.
* @author ken
* @date 2011-07-06
*/


#ifndef __GUI_INTERFACE_IMAGELOADER_PNG_TGA_H_20110706__
#define __GUI_INTERFACE_IMAGELOADER_PNG_TGA_H_20110706__

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
	* @class IGUIImageLoader_png_tga
	* @brief interface for load image by using devil library
	*/
	class GUIEXPORT IGUIImageLoader_png_tga : public IGUIInterfaceImageLoader
	{
	public:
		IGUIImageLoader_png_tga();
		virtual ~IGUIImageLoader_png_tga();

		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  );
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize );
		virtual void DestroyImageData(CGUIImageData* pImageData);

		virtual void DeleteSelf();

	protected:
		CGUIImageData* LoadPng( uint8* pFileData, size_t nSize );
		CGUIImageData* LoadTga( uint8* pFileData, size_t nSize );

		CGUIImageData* LoadUncompressedTGA( uint8* pFileData, size_t nSize );
		CGUIImageData* LoadCompressedTGA( uint8* pFileData, size_t nSize );

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	public: 
		static const char* StaticGetModuleName();
	};


	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_INTERFACE_IMAGELOADER_PNG_TGA_H_20110706__

