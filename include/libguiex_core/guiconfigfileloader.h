/** 
* @file guiconfigfileloader.h
* @brief utility function for load config file.
* @author ken
* @date 2010-11-25
*/

#ifndef __GUI_CONFIG_FILE_LOADER_H_20101125__
#define __GUI_CONFIG_FILE_LOADER_H_20101125__


//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIConfigFileLoader
	* @brief utility function for load config file.
	*/
	class GUIEXPORT CGUIConfigFileLoader
	{
	public:
		static int32 LoadResourceConfigFile( const CGUIString& rPath, const CGUIString& rSceneName, const CGUIString& rFileName );
		static CGUIWidget* LoadWidgetConfigFile( const CGUIString& rPath, const CGUIString& rSceneName, const CGUIString& rFileName );
	};
}//namespace guiex

#endif //__GUI_CONFIG_FILE_LOADER_H_20101125__
