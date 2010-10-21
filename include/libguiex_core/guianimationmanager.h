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
		CGUIAnimation*	CreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rProjectName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			uint32 nInterval
			);

		/**
		* @brief create a animation by multible images
		*/
		CGUIAnimation*	CreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rProjectName, 
			const std::vector<CGUIString>& rFileNames,  
			uint32 nInterval
			);

	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIAnimationManager);
	};

}//namespace guiex

#endif		//__GUI_ANIMATIONMANAGER_20091027_H__

