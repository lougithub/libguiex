/** 
* @file guianimationmanager.cpp
* @brief image manager.
* @author ken
* @date 2009-10-27
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guianimationmanager.h>
#include <libguiex_core\guianimation.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiexception.h>

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
			const CGUIString& rProjectName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			uint32 nInterval)
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rProjectName, rFileName, rUVRects, nInterval );
		AddResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation*	CGUIAnimationManager::CreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rProjectName, 
			const std::vector<CGUIString>& rFileNames,  
			uint32 nInterval)
	{
		CGUIAnimation* pAnimation = new CGUIAnimation( rName, rProjectName, rFileNames, nInterval );
		AddResource(pAnimation);
		return pAnimation;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
