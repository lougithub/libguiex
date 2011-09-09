/** 
* @file guiimageloader_editor.h
* @author ken
* @date 2011-09-08
*/
#ifndef __GUI_IMAGELOADER_EDITOR_20110908_H__
#define __GUI_IMAGELOADER_EDITOR_20110908_H__


//============================================================================//
// include
//============================================================================//
#include <libguiex_module\imageloader_png\guiimageloader_png.h>

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT IGUIImageLoader_Editor : public IGUIInterfaceImageLoader
	{
	public:
		IGUIImageLoader_Editor();
		virtual ~IGUIImageLoader_Editor(){}

		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  );
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize ){return NULL;}
		virtual void DestroyImageData(CGUIImageData* pImageData){delete pImageData;}
		virtual void DeleteSelf(){delete this;}

	protected:
		virtual int DoInitialize(void* ) {return 0;}
		virtual void DoDestroy(){}

	public: 
		static const char* StaticGetModuleName();
	};

}


#endif //__GUI_IMAGELOADER_EDITOR_20110908_H__