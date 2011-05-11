/** 
* @file guifilesys_android.h
* @brief file operation in android.
* @author ken
* @date 2011-05-10
*/

#ifndef __GUI_FILESYS_ANDROID_20110510_H__
#define __GUI_FILESYS_ANDROID_20110510_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacefilesys.h>
#include <zip.h>
#include <vector>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIFileSys_android : public IGUIInterfaceFileSys
	{
	public:
		IGUIFileSys_android();
		virtual ~IGUIFileSys_android();


	public:
		/**
		* @brief read file and load data into a CGUIDataChunk
		* @return zero for success
		*/
		virtual int32 ReadFile(
			const CGUIString& rFileName, 
			CGUIDataChunk& rData,
			EOpenMode eOpenMode= eOpenMode_Binary );

		virtual void FindFiles( 
			const CGUIString& rPath,
			const CGUIString& rSuffix,
			std::vector<CGUIString>& rArrayStrings );

		virtual CGUIString GetFilename( const CGUIString& rPath );
		virtual CGUIString GetFileDir( const CGUIString& rPath );

		/**
		* @brief used to delete this object
		*/
		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	protected:
		zip* m_pAPKArchive;

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_FILESYS_ANDROID_20110510_H__
