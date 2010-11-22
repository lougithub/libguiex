/** 
* @file guianimationmanager.h
* @brief image manager.
* @author ken
* @date 2009-10-27
*/

#ifndef __GUI_ANIMATIONMANAGER_20091027_H__
#define __GUI_ANIMATIONMANAGER_20091027_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guisingleton.h"
#include "guianimation.h"
#include "guiresourcemanager.h"
#include <set>
#include <map>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIProperty;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIAnimationManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIAnimationManager : public CGUIResourceManager <CGUIAnimation>
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAnimationManager();

		/**
		* @brief destructor;
		*/
		virtual ~CGUIAnimationManager();
		
		int32 RegisterAnimation(
			const CGUIString& rSceneName, 
			const CGUIProperty& rProperty );

		/**
		* @brief create a animation by single image;
		*/
		int32 RegisterAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize = CGUISize(0,0));

		/**
		* @brief create a animation by multible images
		*/
		int32 RegisterAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval,
			const CGUISize& rSize = CGUISize(0,0));


		CGUIAnimation* AllocateResource( const CGUIString& rResName );
		CGUIAnimation* AllocateResource(
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize = CGUISize(0,0)
			);
		CGUIAnimation* AllocateResource( 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval,
			const CGUISize& rSize = CGUISize(0,0));

		int32 DeallocateResource( CGUIAnimation* pAnimation );

	protected:
		CGUIAnimation* DoCreateAnimation(
			const CGUIString& rSceneName, 
			const CGUIProperty& rProperty );

		/**
		* @brief create a animation by single image;
		*/
		CGUIAnimation* DoCreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize );

		/**
		* @brief create a animation by multible images
		*/
		CGUIAnimation* DoCreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval,
			const CGUISize& rSize );

	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIAnimationManager);
	};

}//namespace guiex

#endif		//__GUI_ANIMATIONMANAGER_20091027_H__

