/** 
* @file guimusicmanager.cpp
* @brief sound manager.
* @author ken
* @date 2011-01-04
*/
//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guimusicmanager.h>
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
	int32 CGUIMusicManager::RegisterMusic( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		const CGUIProperty* pPropPath = rProperty.GetProperty("path", "CGUIString");
		if( !pPropPath )
		{
			throw guiex::CGUIException(
				"[CGUIMusicManager::RegisterMusic], invalid property <%s:%s:%s>!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return -1;
		}
		CGUIString strPath = pPropPath->GetValue();

		CGUIMusicData* pMusicData = DoCreateMusic( rProperty.GetName(), rSceneName, strPath );
		RegisterResource( pMusicData );
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
			throw CGUIException("[CGUIMusicManager::DoCreateMusic]: failed to get sound interface");
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
			throw CGUIException("[CGUIMusicManager::DestroyResourceImp]: failed to get sound interface");
		}
		pSound->DestroyMusicData( (CGUIMusicData*)pRes );
	}
	//------------------------------------------------------------------------------
	void CGUIMusicManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			throw CGUIException("[CGUIMusicManager::DestroyResourceImp]: failed to get sound interface");
		}
		pSound->DestroyMusicData( (CGUIMusicData*)pRes );
	}
	//------------------------------------------------------------------------------
	CGUIMusicData* CGUIMusicManager::AllocateResource( const CGUIString& rResName ) const
	{
		CGUIMusicData* pMusic = CGUIResourceManager<CGUIMusicData,CGUIMusicData>::GetRegisterResource( rResName );
		if( !pMusic )
		{
			throw CGUIException( 
				"[CGUIMusicDataManager::AllocateResource]: failed to get sound by name <%s>",
				rResName.c_str());
			return NULL;
		}
		pMusic->RefRetain();
		return pMusic;
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicManager::DeallocateResource( CGUIMusicData* pMusic )
	{
		GUI_ASSERT( pMusic, "invalid parameter" );

		pMusic->RefRelease();
		if( pMusic->GetRefCount() == 0 )
		{
			//named image's reference count shouldn't be zero, which is retained by register function
			throw CGUIException(
				"[CGUIMusicManager::DeallocateResource]: invalid reference count [%d] for resource: <%s:%s:%s>", 
				pMusic->GetRefCount(),
				pMusic->GetName().c_str(), 
				pMusic->GetResourceType().c_str(),
				pMusic->GetSceneName().c_str() );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
