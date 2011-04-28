/** 
* @file guimusicmanager.cpp
* @brief sound manager.
* @author ken
* @date 2011-01-04
*/
//============================================================================//
// include 
//============================================================================// 
#include "guimusicmanager.h"
#include "guiinterfacemanager.h"
#include "guiinterfacesound.h"
#include "guisounddata.h"
#include "guipropertyconvertor.h"
#include "guistringconvertor.h"
#include "guiexception.h"
#include "guiproperty.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicManager * CGUIMusicManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIMusicManager::CGUIMusicManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIMusicManager::CGUIMusicManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIMusicManager::~CGUIMusicManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIMusicManager* CGUIMusicManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicManager::RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		const CGUIProperty* pPropPath = rProperty.GetProperty("path", "CGUIString");
		if( !pPropPath )
		{
			CGUIException::ThrowException(
				"[CGUIMusicManager::RegisterResource], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		CGUIString strPath = pPropPath->GetValue();

		CGUIMusicData* pMusicData = DoCreateMusic( rProperty.GetName(), rSceneName, strPath );
		RegisterResourceImp( pMusicData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* CGUIMusicManager::DoCreateMusic(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			CGUIException::ThrowException("[CGUIMusicManager::DoCreateMusic]: failed to get sound interface");
			return NULL;
		}
		CGUIMusicData* pMusicData = pSound->CreateMusicData( rName, rSceneName, rPath );
		return pMusicData;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			CGUIException::ThrowException("[CGUIMusicManager::DestroyResourceImp]: failed to get sound interface");
		}
		pSound->DestroyMusicData( (CGUIMusicData*)pRes );
	}
	//------------------------------------------------------------------------------
	void CGUIMusicManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			CGUIException::ThrowException("[CGUIMusicManager::DestroyResourceImp]: failed to get sound interface");
		}
		pSound->DestroyMusicData( (CGUIMusicData*)pRes );
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* CGUIMusicManager::AllocateResource( const CGUIString& rResName ) const
	{
		CGUIMusicData* pMusic = CGUIResourceManager<CGUIMusicData,CGUIMusicData>::GetRegisterResource( rResName );
		if( !pMusic )
		{
			CGUIException::ThrowException( 
				"[CGUIMusicDataManager::AllocateResource]: failed to get sound by name <%s>",
				rResName.c_str());
			return NULL;
		}
		pMusic->RefRetain();
		return pMusic;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			//music's reference count shouldn't be zero, which is retained by register function
			CGUIException::ThrowException(
				"[CGUIMusicManager::DeallocateResource]: invalid reference count [%d] for resource: <%s:%s:%s>", 
				pRes->GetRefCount(),
				pRes->GetName().c_str(), 
				pRes->GetResourceType().c_str(),
				pRes->GetSceneName().c_str() );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
