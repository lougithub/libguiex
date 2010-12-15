/** 
* @file guiasmanager.cpp
* @brief as manager.
* @author ken
* @date 2010-11-16
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiasmanager.h>
#include <libguiex_core/guias.h>
#include <libguiex_core/guiasfactory.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
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
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsManager::RegisterAs( 
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty )
	{
		CGUIAs* pAs = DoCreateAs( rSceneName, rProperty); 
		RegisterResource( pAs );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::DoCreateAs( 
		const CGUIString& rSceneName, 
		const CGUIProperty& rProperty )
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
	CGUIAs* CGUIAsManager::DoCreateAs( 
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIString& rAsType )
	{
		guiex::CGUIAs* pAs = guiex::CGUIAsFactory::Instance()->GenerateAs( rAsType, rName, rSceneName );
		return pAs;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIAs* pAs = CGUIResourceManager<CGUIAs>::GetResource( rResName );
		if( !pAs )
		{
			throw CGUIException( 
				"[CGUIAsManager::AllocateResource]: failed to get as by name <%s>",
				rResName.c_str());
			return NULL;
		}
		CGUIAs* pCloneAs = pAs->Clone();

		//AddToAllocatePool( pCloneAs );
		//it has been added to pool by function AllocateResourceByType

		return pCloneAs;
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
	void CGUIAsManager::DestroyResourceImp( CGUIAs * pRes )
	{
		CGUIAsFactory::Instance()->DestroyAs( pRes );
	}
	//------------------------------------------------------------------------------

}//namespace guiex
