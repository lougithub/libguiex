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
#include "guiproperty.h"
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
	class GUIEXPORT CGUIAnimationData  : public CGUIResource
	{
	public:
		const CGUIProperty& GetAnimationData() const;

	protected:
		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
		friend class CGUIAnimationManager;
		CGUIAnimationData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty );

	protected:
		CGUIProperty m_aProperty;
	};


	/**
	* @class CGUIAnimationManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIAnimationManager : public CGUIResourceManager <CGUIAnimationData, CGUIAnimation>
	{
	public:

		CGUIAnimationManager();
		virtual ~CGUIAnimationManager();
		
		static CGUIAnimationManager* Instance(); 

		int32 RegisterAnimation(
			const CGUIString& rSceneName, 
			const CGUIProperty& rProperty );

		CGUIAnimation* AllocateResource( const CGUIString& rResName );

		virtual void DeallocateResource( CGUIResource* pRes );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

	private:
		static CGUIAnimationManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_ANIMATIONMANAGER_20091027_H__

