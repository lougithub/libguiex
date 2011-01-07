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
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFontManager * CGUIFontManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIFontManager::CGUIFontManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIFontManager::CGUIFontManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIFontManager::~CGUIFontManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIFontManager* CGUIFontManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIFontManager::RegisterFont( 
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty)
	{
		const CGUIProperty* pPropPath = rProperty.GetProperty("path", "CGUIString");
		if( !pPropPath )
		{
			throw guiex::CGUIException(
				"[CGUIFontManager::RegisterSound], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		CGUIString strPath = pPropPath->GetValue();

		const CGUIProperty* pPropIndex = rProperty.GetProperty("index", "int16");
		if( !pPropIndex )
		{
			throw guiex::CGUIException(
				"[CGUIFontManager::RegisterSound], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		uint32 nIndex = 0;
		PropertyToValue( *pPropIndex, nIndex);

		CGUIFontData* pFontData = DoCreateFont( rProperty.GetName(), rSceneName, strPath, nIndex );
		RegisterResource( pFontData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIFontData* CGUIFontManager::DoCreateFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath, 
			uint32 nFontID)
	{
		if( nFontID >= GUI_FONT_MAX_NUM )
		{
			throw CGUIException("[CGUIFontManager::CreateGUIFont]: nFontID should be smaller than %d", GUI_FONT_MAX_NUM );
			return NULL;
		}

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException("[CGUIFontManager::CreateGUIFont]: failed to get font interface");
			return NULL;
		}
		CGUIFontData* pFontData = pFont->CreateFontData( rName, rSceneName, rPath, nFontID );
		return pFontData;
	}
	//------------------------------------------------------------------------------
	void CGUIFontManager::DestroyResourceImp( void* pRes )
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException("[CGUIFontManager::DestroyResourceImp]: failed to get font interface");
		}
		pFont->DestroyFontData( ( TResourceType* )( pRes ) );
	}
	//------------------------------------------------------------------------------
}//namespace guiex
