/** 
 * @file guiinterfacelocalizationloader.h
 * @brief load localization file
 * @author ken
 * @date 2011-04-28
 */

#ifndef __GUI_INTERFACE_LOCALIZATIONLOADER_H_20110428__
#define __GUI_INTERFACE_LOCALIZATIONLOADER_H_20110428__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiinterface.h"
#include "guistring.h"

#include <vector>


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
	* @class IGUIInterfaceLocalizationLoader
	* @brief interface for read localization file
	*/
	class GUIEXPORT IGUIInterfaceLocalizationLoader : public IGUIInterface
	{
	public:
		IGUIInterfaceLocalizationLoader( const char* szModuleName );
		virtual ~IGUIInterfaceLocalizationLoader();

	public:
		virtual int32 LoadLocalizationFile( const CGUIString& rFileName, 
			const CGUIString& rCurrentLocalConfig,
			std::vector<std::pair<CGUIString, CGUIStringW> >& rLocalizations ) = 0;

	public: 
		static const char* StaticGetModuleType();
	};
}//namespace guiex

#endif //__GUI_INTERFACE_LOCALIZATIONLOADER_H_20110428__
