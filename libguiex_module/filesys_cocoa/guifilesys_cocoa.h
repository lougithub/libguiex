/** 
* @file guifilesys_cocoa.h
* @brief use standard file io in mac os.
* @author ken
* @date 2010-11-21
*/

#ifndef __GUI_FILESYS_COCOA_20101121_H__
#define __GUI_FILESYS_COCOA_20101121_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacefilesys.h>
#include <vector>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIFileSys_cocoa : public IGUIInterfaceFileSys
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIFileSys_cocoa();

		/**
		* @brief destructor
		*/
		virtual ~IGUIFileSys_cocoa();


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

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_FILESYS_COCOA_20101121_H__
