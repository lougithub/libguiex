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
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundManager::RegisterSound( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		const CGUIProperty* pPropPath = rProperty.GetProperty("path", "CGUIString");
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

		CGUISoundData* pSoundData = DoCreateSound( rProperty.GetName(), rSceneName, strPath );
		RegisterResource( pSoundData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* CGUISoundManager::DoCreateSound(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rPath )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			throw CGUIException("[CGUISoundManager::DoCreateSound]: failed to get sound interface");
			return NULL;
		}
		CGUISoundData* pSoundData = pSound->CreateSoundData( rName, rSceneName, rPath );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void CGUISoundManager::DestroyResourceImp( void* pRes )
	{
		IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( !pSound )
		{
			throw CGUIException("[CGUISoundManager::DestroyResourceImp]: failed to get sound interface");
		}
		pSound->DestroySoundData( ( TResourceType* )( pRes ) );
	}
	//------------------------------------------------------------------------------
	CGUISoundData* CGUISoundManager::AllocateResource( const CGUIString& rResName ) const
	{
		CGUISoundData* pSound = CGUIResourceManager<CGUISoundData>::GetResource( rResName );
		if( !pSound )
		{
			throw CGUIException( 
				"[CGUISoundDataManager::AllocateResource]: failed to get sound by name <%s>",
				rResName.c_str());
			return NULL;
		}
		pSound->RefRetain();
		return pSound;
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundManager::DeallocateResource( CGUISoundData* pSound )
	{
		GUI_ASSERT( pSound, "invalid parameter" );

		pSound->RefRelease();
		if( pSound->GetRefCount() == 0 )
		{
			//named image's reference count shouldn't be zero, which is retained by register function
			throw CGUIException(
				"[CGUISoundManager::DeallocateResource]: invalid reference count [%d] for resource: <%s:%s:%s>", 
				pSound->GetRefCount(),
				pSound->GetName().c_str(), 
				pSound->GetResourceType().c_str(),
				pSound->GetSceneName().c_str() );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
