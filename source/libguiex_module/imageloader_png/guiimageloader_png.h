/** 
* @file guiimageloader_png.h
* @brief interface to load image from file or memory.
* @author ken
* @date 2011-07-06
*/


#ifndef __GUI_INTERFACE_IMAGELOADER_PNG_H_20110706__
#define __GUI_INTERFACE_IMAGELOADER_PNG_H_20110706__

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
	* @class IGUIImageLoader_png
	* @brief interface for load image by using devil library
	*/
	class GUIEXPORT IGUIImageLoader_png : public IGUIInterfaceImageLoader
	{
	public:
		IGUIImageLoader_png();
		virtual ~IGUIImageLoader_png();

		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  );
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize );
		virtual void DestroyImageData(CGUIImageData* pImageData);

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	public: 
		static const char* StaticGetModuleName();
	};


	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_INTERFACE_IMAGELOADER_PNG_H_20110706__

