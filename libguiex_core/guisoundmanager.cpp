/** 
* @file guisoundmanager.cpp
* @brief sound manager.
* @author ken
* @date 2011-01-04
*/
//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guisoundmanager.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacesound.h>
#include <libguiex_core/guisounddata.h>
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
	CGUISoundManager * CGUISoundManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUISoundManager::CGUISoundManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUISoundManager::CGUISoundManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUISoundManager::~CGUISoundManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUISoundManager* CGUISoundManager::Instance()
	{
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundManager::RegisterSound( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		CGUIString strName = rProperty.GetName();
		
		const CGUIProperty* pPropPath = rProperty.GetProperty("path");
		if( !pPropPath )
		{
			throw guiex::CGUIException(
				"[CGUISoundManager::RegisterSound], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		CGUIString strPath = pPropPath->GetValue();

		const CGUIProperty* pPropID = rProperty.GetProperty("id");
		if( !pPropID )
		{
			throw guiex::CGUIException(
				"[CGUISoundManager::RegisterSound], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		uint32 nID = 0;
		PropertyToValue( *pPropID, nID);

		CGUISoundData* pSoundData = DoCreateSound( rProperty.GetName(), rSceneName, strPath, nID );
		RegisterResource( pSoundData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* CGUISoundManager::DoCreateSound(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath, 
			uint32 nSoundID)
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			throw CGUIException("[CGUISoundManager::DoCreateSound]: failed to get sound interface");
			return NULL;
		}
		CGUISoundData* pSoundData = pSound->CreateSoundData( rName, rSceneName, rPath, nSoundID );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void CGUISoundManager::DestroyResourceImp( CGUISoundData * pRes )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			throw CGUIException("[CGUISoundManager::DestroyResourceImp]: failed to get sound interface");
		}
		pSound->DestroySoundData( pRes );
	}
	//------------------------------------------------------------------------------
}//namespace guiex
