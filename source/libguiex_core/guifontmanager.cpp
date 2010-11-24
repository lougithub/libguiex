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
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIFontManager::RegisterFont( 
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty)
	{
		CGUIString strName = rProperty.GetName();
		
		const CGUIProperty* pPropPath = rProperty.GetProperty("path");
		if( !pPropPath )
		{
			throw guiex::CGUIException(
				"[CGUIFontManager::RegisterAs], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		CGUIString strPath = pPropPath->GetValue();

		const CGUIProperty* pPropIndex = rProperty.GetProperty("index");
		if( !pPropIndex )
		{
			throw guiex::CGUIException(
				"[CGUIFontManager::RegisterAs], invalid property <%s:%s:%s>!", 
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
			uint32 nFontIndex)
	{
		if( nFontIndex >= GUI_FONT_MAX_NUM )
		{
			throw CGUIException("[CGUIFontManager::CreateGUIFont]: nFontIndex should be smaller than %d", GUI_FONT_MAX_NUM );
			return NULL;
		}

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException("[CGUIFontManager::CreateGUIFont]: failed to get font interface");
			return NULL;
		}
		CGUIFontData* pFontData = pFont->CreateFontData( rName, rSceneName, rPath, nFontIndex );
		return pFontData;
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
