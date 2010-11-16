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
	const CGUIAs* CGUIAsManager::CreateAs( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAs* pAs = NULL;

		//TODO...

		RegisterResource(pAs);
		return pAs;
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
