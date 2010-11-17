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
	int32 CGUIAnimationManager::RegisterAnimation(
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const CGUIString& rFileName, 
		const std::vector<CGUIRect>& rUVRects,
		real fInterval,
		const CGUISize& rSize )
	{
		CGUIAnimation* pAnimation = DoCreateAnimation(
			rName,
			rSceneName,
			rFileName, 
			rUVRects,
			fInterval,
			rSize );
		RegisterResource( pAnimation );
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimationManager::RegisterAnimation( 
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const std::vector<CGUIString>& rFileNames,  
		real fInterval,
		const CGUISize& rSize )
	{
		CGUIAnimation* pAnimation = DoCreateAnimation(
			rName,
			rSceneName,
			rFileNames, 
			fInterval,
			rSize );
		RegisterResource( pAnimation );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::DoCreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize )
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rSceneName, rFileName, rUVRects, fInterval, rSize );
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::DoCreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval,
			const CGUISize& rSize )
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rSceneName, rFileNames, fInterval, rSize );
		RegisterResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::AllocateResource( const CGUIString& rResName )
	{
		const CGUIAnimation* pTemplate = GetResource( rResName );
		if( !pTemplate )
		{
			throw CGUIException( 
				"[CGUIAnimationManager::AllocateResource]: failed to get image by name <%s>",
				rResName.c_str());
			return NULL;
		}

		CGUIAnimation* pAnimation = NULL;
		if( pTemplate->eUVAnimType == CGUIAnimation::eUVAnimType_MultiFile )
		{
			pAnimation = DoCreateAnimation(
				"",
				"",
				pTemplate->m_vecFileNames, 
				pTemplate->m_fInterval,
				pTemplate->GetSize() );
		}
		else
		{
			pAnimation = DoCreateAnimation(
				"",
				"",
				pTemplate->m_vecFileNames[0], 
				pTemplate->m_vecUVRects,
				pTemplate->m_fInterval,
				pTemplate->GetSize() );
		}
		pAnimation->RefRetain();
		AddToAllocatePool( pAnimation );
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::AllocateResource(
		const CGUIString& rFileName, 
		const std::vector<CGUIRect>& rUVRects,
		real fInterval,
		const CGUISize& rSize )
	{
		CGUIAnimation* pAnimation = DoCreateAnimation(
			"",
			"",
			rFileName, 
			rUVRects, 
			fInterval,
			rSize );
		pAnimation->RefRetain();
		AddToAllocatePool( pAnimation );
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::AllocateResource( 
		const std::vector<CGUIString>& rFileNames,  
		real fInterval,
		const CGUISize& rSize )
	{
		CGUIAnimation* pAnimation = DoCreateAnimation(
			"",
			"",
			rFileNames, 
			fInterval,
			rSize );
		pAnimation->RefRetain();
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
