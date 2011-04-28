/** 
* @file guilocalizationloader_tinyxml.h
* @brief use xml and tinyxml to analyse the localization file
* @author ken
* @date 2011-04-28
*/

#ifndef __GUI_CONFIGFILE_LOCALIZATIONLOADER_20110428_H__
#define __GUI_CONFIGFILE_LOCALIZATIONLOADER_20110428_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacelocalizationloader.h>
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
	class GUIEXPORT IGUILocalizationLoader_tinyxml : public IGUIInterfaceLocalizationLoader
	{
	public:
		IGUILocalizationLoader_tinyxml();
		virtual ~IGUILocalizationLoader_tinyxml();

	public:
		virtual void DeleteSelf();

		virtual int32 LoadLocalizationFile( const CGUIString& rFileName, 
			const CGUIString& rCurrentLocalConfig,
			std::vector<std::pair<CGUIString, CGUIStringW> >& rLocalizations );

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_CONFIGFILE_LOCALIZATIONLOADER_20110428_H__
