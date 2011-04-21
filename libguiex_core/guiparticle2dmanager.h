/** 
* @file guiparticle2dmanager.h
* @brief particle2d manager.
* @author ken
* @date 2011-01-17
*/

#ifndef __GUI_PARTICLE2DMANAGER_20110117_H__
#define __GUI_PARTICLE2DMANAGER_20110117_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guiparticle2dsystem.h"
#include "guiresourcemanager.h"
#include "guiproperty.h"
#include <set>
#include <map>


//============================================================================//
// declare
//============================================================================// 

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIParticle2DData  : public CGUIResource
	{
	public:
		const CGUIProperty& GetParticle2DData() const;

	protected:
		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
		friend class CGUIParticle2DManager;
		CGUIParticle2DData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty );

	protected:
		CGUIProperty m_aProperty;
	};



	/**
	* @class CGUIParticle2DManager
	* @brief particle 2d manager
	* 
	*/
	class GUIEXPORT CGUIParticle2DManager : public CGUIResourceManager <CGUIParticle2DData, CGUIParticle2DSystem>
	{
	public:
		CGUIParticle2DManager();
		virtual ~CGUIParticle2DManager();

		static CGUIParticle2DManager* Instance(); 

		virtual int32 RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty);
		
		CGUIParticle2DSystem* AllocateResource( const CGUIString& rResName );
		virtual void DeallocateResource( CGUIResource* pRes );

		CGUIParticle2DSystem* AllocateResource( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

		CGUIParticle2DSystem* DoCreateParticle2D( const CGUIString& rSceneName, const CGUIProperty& rProperty );

	private:
		static CGUIParticle2DManager* m_pSingleton;
	};

}//namespace guiex

#endif //__GUI_PARTICLE2DMANAGER_20110117_H__

