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
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefont.h>
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
	int32 CGUIFontManager::CreateGUIFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath, 
			uint32 nFontIndex)
	{
		if( nFontIndex >= GUI_FONT_MAX_NUM )
		{
			throw CGUIException("[CGUIFontManager::CreateGUIFont]: nFontIndex should be smaller than %d", GUI_FONT_MAX_NUM );
			return -1;
		}

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException("[CGUIFontManager::CreateGUIFont]: failed to get font interface");
			return -1;
		}
		CGUIFontData* pFontData = pFont->CreateFontData( rName, rSceneName, rPath, nFontIndex );
		RegisterResource(pFontData);
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIFontManager::DoDestroyResource( CGUIFontData * pRes )
	{
		if( pRes->GetRefCount() != 0 )
		{
			throw CGUIException( "resource reference is still in using[%d]: <%s:%s:%s>", 
				pRes->GetRefCount(),
				pRes->GetName().c_str(), 
				pRes->GetResourceType().c_str(),
				pRes->GetSceneName().c_str() );
		}
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException("[CGUIFontManager::DoDestroyResource]: failed to get font interface");
		}
		pFont->DestroyFontData( pRes );
	}
	//------------------------------------------------------------------------------
}//namespace guiex
