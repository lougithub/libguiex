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
#include <libguiex_core/guiinterfaceconfigfile.h>
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
		IGUIConfigFile_tinyxml();
		virtual ~IGUIConfigFile_tinyxml();

	public:
		virtual int32 LoadConfigFile( const CGUIString& rFileName, CGUIProperty& rPropertySet );

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	protected:
		int32 ProcessProperty( TiXmlElement* pNode, CGUIProperty& rPropSet );

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_CONFIGFILE_TINYXML_20060913_H__
