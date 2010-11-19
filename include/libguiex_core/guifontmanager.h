/** 
* @file guifontmanager.h
* @brief font manager.
* @author ken
* @date 2009-10-27
*/

#ifndef __GUI_FONTMANAGER_20091027_H__
#define __GUI_FONTMANAGER_20091027_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guisingleton.h"
#include "guifontdata.h"
#include "guiresourcemanager.h"
#include <set>
#include <map>


//============================================================================//
// declare
//============================================================================// 

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIFontManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIFontManager : public CGUIResourceManager <CGUIFontData>
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIFontManager();

		/**
		* @brief destructor;
		*/
		virtual ~CGUIFontManager();

		/**
		* @brief create a font
		*/
		const CGUIFontData*	CreateGUIFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath, 
			uint32 nFontIndex
			);

	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIFontManager);
	};

}//namespace guiex

#endif		//__GUI_FONTMANAGER_20091027_H__

