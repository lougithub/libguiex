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
	GUI_SINGLETON_IMPLEMENT_EX(CGUIAsManager );
	//------------------------------------------------------------------------------
	CGUIAsManager::CGUIAsManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUIAsManager::~CGUIAsManager()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsManager::RegisterAs( 
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty )
	{
		CGUIAs* pAs = DoCreateAs( rName, rSceneName, rProperty); 
		RegisterResource( pAs );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::DoCreateAs( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAs* pAs = NULL;

		//TODO...
		return pAs;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResource( const CGUIString& rResName )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResourceByType( const CGUIString& rAsType )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsManager::DeallocateResource( CGUIAs* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		pRes->RefRelease();
		if( pRes->GetRefCount() == 0 )
		{
			return ReleaseAllocateResource( pRes );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::DoDestroyResource( CGUIAs * pRes )
	{
		if( pRes->GetRefCount() != 0 )
		{
			throw CGUIException( "resource refrence is still in using[%d]: <%s:%s:%s>", 
				pRes->GetRefCount(),
				pRes->GetName().c_str(), 
				pRes->GetResourceType().c_str(),
				pRes->GetSceneName().c_str() );
		}
		CGUIAsFactory::Instance()->DestroyAs( pRes );
	}
	//------------------------------------------------------------------------------

}//namespace guiex
