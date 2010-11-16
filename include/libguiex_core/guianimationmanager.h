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

		/**
		* @brief create a animation by single image;
		*/
		const CGUIAnimation*	CreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval
			);

		/**
		* @brief create a animation by multible images
		*/
		const CGUIAnimation*	CreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval
			);

		CGUIAnimation* AllocateResource( const CGUIString& rResName );

		int32 DeallocateResource( CGUIAnimation* pAnimation );



	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIAnimationManager);
	};

}//namespace guiex

#endif		//__GUI_ANIMATIONMANAGER_20091027_H__

