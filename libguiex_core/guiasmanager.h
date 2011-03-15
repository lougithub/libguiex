/** 
* @file guiasmanager.h
* @brief as manager.
* @author ken
* @date 2010-11-16
*/

#ifndef __GUI_ASMANAGER_20101116_H__
#define __GUI_ASMANAGER_20101116_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guias.h"
#include "guistring.h"
#include "guiresourcemanager.h"
#include <set>
#include <map>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIAs;
	class CGUIProperty;

	typedef CGUIAs* (*TAsGenerator)( const CGUIString& rAsName, const CGUIString& rSceneName );
		
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIAsData  : public CGUIResource
	{
	public:
		const CGUIProperty& GetAsData() const;

	protected:
		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		friend class CGUIAsManager;
		CGUIAsData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty );

	protected:
		CGUIProperty m_aProperty;
	};



	/**
	* @class CGUIAsManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIAsManager : public CGUIResourceManager <CGUIAsData, CGUIAs>
	{
	public:
		CGUIAsManager();
		virtual ~CGUIAsManager();

		static CGUIAsManager* Instance(); 

		int32 RegisterAs( const CGUIString& rSceneName, const CGUIProperty& rProperty );

		CGUIAs* AllocateResource( const CGUIString& rResName );
		CGUIAs* AllocateResource( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName );
		template<class T> 
		T* AllocateResource( const CGUIString& rAsName="", const CGUIString& rSceneName="" );
		virtual void DeallocateResource( CGUIResource* pRes );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

	private:
		static CGUIAsManager* m_pSingleton;
		typedef std::map<CGUIString, TAsGenerator> TMapAsGenerator;
		TMapAsGenerator m_mapAsGenerator;
	};

	template<class T> 
	inline T* CGUIAsManager::AllocateResource( const CGUIString& rAsName, const CGUIString& rSceneName )
	{
		T* pAs = new T( rAsName, rSceneName );
		if( !pAs || pAs->GetType() != T::StaticGetType() )
		{
			throw CGUIException(
				"[CGUIAsManager::AllocateResource] failed to generate as <%s>",
				T::StaticGetType());
			return NULL;	
		}

		AddToAllocatePool( pAs );
		pAs->RefRetain();
		return pAs;
	}

}//namespace guiex

#endif		//__GUI_ASMANAGER_20101116_H__

