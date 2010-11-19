/** 
* @file guifontmanager.cpp
* @brief font manager.
* @author ken
* @date 2009-10-27
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guifontmanager.h>
#include <libguiex_core/guifontdata.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_SINGLETON_IMPLEMENT_EX(CGUIFontManager );
	//------------------------------------------------------------------------------
	CGUIFontManager::CGUIFontManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUIFontManager::~CGUIFontManager()
	{
	}
	//------------------------------------------------------------------------------
	const CGUIFontData*	CGUIFontManager::CreateGUIFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath, 
			uint32 nFontIndex)
	{
		CGUIFontData* pFont = new CGUIFontData( rName, rSceneName, rPath, nFontIndex );
		RegisterResource(pFont);
		return pFont;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
