/** 
* @file guifilesys_stdio.h
* @brief use standard file io.
* @author ken
* @date 2006-07-06
*/

#ifndef __GUI_FILESYS_STDIO_20060706_H__
#define __GUI_FILESYS_STDIO_20060706_H__

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


	class GUIEXPORT IGUIFileSys_stdio : public IGUIInterfaceFileSys
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIFileSys_stdio();

		/**
		* @brief destructor
		*/
		virtual ~IGUIFileSys_stdio();


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

#endif //__GUI_FILESYS_STDIO_20060706_H__
