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
	class GUIEXPORT CGUIAnimationManager : public CGUIResourceManager <CGUIAnimation, CGUIAnimation>
	{
	public:

		CGUIAnimationManager();
		virtual ~CGUIAnimationManager();
		
		static CGUIAnimationManager* Instance(); 

		int32 RegisterAnimation(
			const CGUIString& rSceneName, 
			const CGUIProperty& rProperty );

		int32 RegisterAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize = CGUISize(0,0));

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

		virtual void DeallocateResource( CGUIResource* pRes );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

		CGUIAnimation* DoCreateAnimation(
			const CGUIString& rSceneName, 
			const CGUIProperty& rProperty );

		CGUIAnimation* DoCreateAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize );

		CGUIAnimation* DoCreateAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval,
			const CGUISize& rSize );

	private:
		static CGUIAnimationManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_ANIMATIONMANAGER_20091027_H__

