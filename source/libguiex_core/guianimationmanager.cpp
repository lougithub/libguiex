/** 
* @file guianimationmanager.cpp
* @brief image manager.
* @author ken
* @date 2009-10-27
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guianimationmanager.h>
#include <libguiex_core/guianimation.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_SINGLETON_IMPLEMENT_EX(CGUIAnimationManager );
	//------------------------------------------------------------------------------
	CGUIAnimationManager::CGUIAnimationManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUIAnimationManager::~CGUIAnimationManager()
	{
	}
	//------------------------------------------------------------------------------
	const CGUIAnimation*	CGUIAnimationManager::CreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval)
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rSceneName, rFileName, rUVRects, fInterval );
		RegisterResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	const CGUIAnimation*	CGUIAnimationManager::CreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval)
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rSceneName, rFileNames, fInterval );
		RegisterResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::AllocateResource( const CGUIString& rResName )
	{
		const CGUIAnimation* pTemplate = GetResource( rResName );
		if( !pTemplate )
		{
			return NULL;
		}

		CGUIAnimation* pAnimation = NULL;
		if( pTemplate->eUVAnimType == CGUIAnimation::eUVAnimType_MultiFile )
		{
			pAnimation = new CGUIAnimation( pTemplate->GetName() + "_clone", pTemplate->GetSceneName(), pTemplate->m_vecFileNames, pTemplate->m_fInterval );

		}
		else
		{
			GUI_ASSERT( pTemplate->m_vecFileNames.size() > 0, "invalid animation parameter" );
			pAnimation = new CGUIAnimation( pTemplate->GetName() + "_clone", pTemplate->GetSceneName(), pTemplate->m_vecFileNames[0], pTemplate->m_vecUVRects, pTemplate->m_fInterval );
		}
		AddToAllocatePool( pAnimation );
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimationManager::DeallocateResource( CGUIAnimation* pAnimation )
	{
		GUI_ASSERT( pAnimation, "invalid parameter" );

		pAnimation->RefRelease();
		if( pAnimation->GetRefCount() == 0 )
		{
			return ReleaseAllocateResource( pAnimation );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
