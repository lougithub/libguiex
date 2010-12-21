/** 
* @file guiinterfacefilesys.h
* @brief file system's interface
* @author ken
* @date 2006-07-06
*/

#ifndef __GUI_INTERFACE_FILESYSTEM_H_20060706__
#define __GUI_INTERFACE_FILESYSTEM_H_20060706__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiinterface.h"
#include "guidatachunk.h"
#include "guistring.h"
#include <vector>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIInterfaceFileSys
	* @brief interface of file system
	*/
	class GUIEXPORT IGUIInterfaceFileSys : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceFileSys( const char* szModuleName );

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfaceFileSys();


		/**
		* @brief open mode  for how file is read
		*/
		enum	EOpenMode
		{
			eOpenMode_Binary,
			eOpenMode_String
		};

	public:
		/**
		* @brief read file and load data into a CGUIDataChunk
		* @param EOpenMode 
		*		- eOpenMode_Binary the file contain binary data
		*		- eOpenMode_String the file contain string, so rData will be terminated by NULL character
		* @return zero for success
		*/
		virtual int32 ReadFile( 
			const CGUIString& rFileName, 
			CGUIDataChunk& rData, 
			EOpenMode eOpenMode = eOpenMode_Binary ) = 0;

		virtual void FindFiles( 
			const CGUIString& rPath,
			const CGUIString& rSuffix,
			std::vector<CGUIString>& rArrayStrings ) = 0;

		virtual CGUIString GetFilename( const CGUIString& rPath ) = 0;
		virtual CGUIString GetFileDir( const CGUIString& rPath ) = 0;

	public: 
		static const char* StaticGetModuleType();
	};
}//namespace guiex

#endif //__GUI_INTERFACE_FILESYSTEM_H_20060706__
