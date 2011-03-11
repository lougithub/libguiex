/** 
* @file guiasmanager.cpp
* @brief as manager.
* @author ken
* @date 2010-11-16
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiasmanager.h"
#include "guias.h"
#include "guiasfactory.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIAsData::CGUIAsData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
		:CGUIResource( rName, rSceneName, "ASDATA" )
		,m_aProperty( rProperty )
	{
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUIAsData::GetAsData() const
	{
		return m_aProperty;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsData::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsData::DoUnload()
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	CGUIAsManager * CGUIAsManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIAsManager::CGUIAsManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIAsManager::CGUIAsManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIAsManager::~CGUIAsManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIAsManager* CGUIAsManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsManager::RegisterAs( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAsData* pAsData = new CGUIAsData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResource( pAsData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::DoCreateAs( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAs* pAs = CGUIAsFactory::Instance()->GenerateAs( rProperty.GetValue(), rProperty.GetName(), rSceneName);
		if( 0 != pAs->ProcessProperty( rProperty ))
		{
			throw CGUIException(
				"[CGUIAsManager::DoCreateAs]: invalid property: <%s> <%s> <%s>", 
				rProperty.GetName().c_str(), 
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
			return NULL;
		}
		return pAs;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::DoCreateAs( const CGUIString& rName,const CGUIString& rSceneName,const CGUIString& rAsType )
	{
		guiex::CGUIAs* pAs = guiex::CGUIAsFactory::Instance()->GenerateAs( rAsType, rName, rSceneName );
		return pAs;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIAsData* pAsData = CGUIResourceManager<CGUIAsData, CGUIAs>::GetRegisterResource( rResName );
		if( !pAsData )
		{
			throw CGUIException( 
				"[CGUIAsManager::AllocateResource]: failed to get as data by name <%s>",
				rResName.c_str());
			return NULL;
		}

		CGUIAs* pAs = DoCreateAs( pAsData->GetSceneName(), pAsData->GetAsData() );
		pAs->RefRetain();
		AddToAllocatePool( pAs );

		return pAs;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResourceByType( const CGUIString& rAsType )
	{
		CGUIAs* pAs = DoCreateAs( "", "", rAsType );
		pAs->RefRetain();
		AddToAllocatePool( pAs );
		return pAs;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsManager::DeallocateResource( CGUIAs* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		pRes->RefRelease();
		if( pRes->GetRefCount() == 0 )
		{
			return ReleaseFromAllocatePool( pRes );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		CGUIAsFactory::Instance()->DestroyAs( (CGUIAs*)pRes );
	}
	//------------------------------------------------------------------------------

}//namespace guiex
