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
	CGUIAnimation*	CGUIAnimationManager::CreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval)
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rSceneName, rFileName, rUVRects, fInterval );
		AddResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation*	CGUIAnimationManager::CreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval)
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rSceneName, rFileNames, fInterval );
		AddResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
