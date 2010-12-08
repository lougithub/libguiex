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
	class CGUIProperty;
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
		virtual int32 LoadConfigFile( const CGUIString& rFileName, CGUIProperty& rPropertySet ) = 0;

	public: 
		static const char* StaticGetModuleName();
	};
}//namespace guiex

#endif //__GUI_INTERFACE_CONFIGFILE_H_20060913__
