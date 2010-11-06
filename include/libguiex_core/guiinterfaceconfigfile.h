/** 
 * @file guiinterfaceconfigfile.h
 * @brief file config file for widget
 * @author ken
 * @date 2006-09-13
 */

#ifndef __GUI_INTERFACE_CONFIGFILE_H_20060913__
#define __GUI_INTERFACE_CONFIGFILE_H_20060913__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiinterface.h"
#include "guistring.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
	class CGUIProjectInfo;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIInterfaceConfigFile
	* @brief interface for read config file
	*/
	class GUIEXPORT IGUIInterfaceConfigFile : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceConfigFile();

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfaceConfigFile();

	public:
		/**
		* @brief read project config file and generate project info.
		* @return pointer of project info object.
		*/
		virtual CGUIProjectInfo* LoadProjectInfoFile( const CGUIString& rFileName ) = 0;

		/**
		* @brief read config file and generate widget system
		* @return the root widget, return NULL for error, or there isn't any widget
		*/
		virtual CGUIWidget*	LoadWidgetConfigFile(const CGUIString& rFileName, const CGUIString& rProjectName) = 0;

		/**
		* @brief read resource config file
		* @return 0 for successful
		*/
		virtual int32 LoadResourceConfigFile(const CGUIString& rFileName, const CGUIString& m_strProjectName ) = 0;
	};
}//namespace guiex

#endif //__GUI_INTERFACE_CONFIGFILE_H_20060913__
