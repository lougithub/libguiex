/** 
* @file guiconfigfile_tinyxml.h
* @brief use xml and tinyxml to analyse the config file
* @author ken
* @date 2006-09-13
*/

#ifndef __GUI_CONFIGFILE_TINYXML_20060913_H__
#define __GUI_CONFIGFILE_TINYXML_20060913_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiinterfaceconfigfile.h>
#include <list>

//============================================================================//
// declare
//============================================================================// 
class TiXmlElement;
namespace guiex
{
	class CGUIProperty;
	class CGUIWidget;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIConfigFile_tinyxml : public IGUIInterfaceConfigFile
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIConfigFile_tinyxml();

		/**
		* @brief destructor
		*/
		virtual ~IGUIConfigFile_tinyxml();

	public:
		/**
		* @brief read config file and generate widget system
		* @return pointer of page root.
		*/
		virtual CGUIWidget*	LoadWidgetConfigFile(const CGUIString& rFileName, const CGUIString& rProjectName);

		/**
		* @brief read image config file
		* @return 0 for successful
		*/
		virtual int32 LoadResourceConfigFile(const CGUIString& rFileName, const CGUIString& rProjectName );

		/**
		* @brief read project config file and generate project info.
		* @return pointer of project info object.
		*/
		virtual CGUIProjectInfo* LoadProjectInfoFile( const CGUIString& rFileName );

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

	protected:
		/**
		* @brief process the node of config file which is a xml file.
		* @return 0 for success
		*/
		int32 ProcessNode( TiXmlElement* pNode, std::list<CGUIWidget*>& rWidgetSet );

		/**
		* @brief process property, get sub property set of this property node
		* @return 0 for success
		*/
		int32 ProcessProperty( TiXmlElement* pNode, CGUIProperty&	rPropSet );

		CGUIString DoGetFilename( const CGUIString& rPath );
		CGUIString DoGetFileDir( const CGUIString& rPath ); 

	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_CONFIGFILE_TINYXML_20060913_H__
