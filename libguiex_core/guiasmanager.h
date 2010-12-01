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
#include "guiimage.h"
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
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIAsManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIAsManager : public CGUIResourceManager <CGUIAs>
	{
	public:
		CGUIAsManager();
		virtual ~CGUIAsManager();

		static CGUIAsManager* Instance(); 

		int32 RegisterAs( 
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty );

		CGUIAs* AllocateResource( const CGUIString& rResName );
		CGUIAs* AllocateResourceByType( const CGUIString& rAsType );
		int32 DeallocateResource( CGUIAs* pRes );

		template<class T>
		T* AllocateResource(  )
		{
			CGUIAs* pAs = AllocateResourceByType( T::StaticGetType() );
			GUI_ASSERT( pAs->GetType() == T::StaticGetType(), "wrong As type" );
			return static_cast<T*>( pAs );
		}

	protected:
		CGUIAs* DoCreateAs( 
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty );

		CGUIAs* DoCreateAs( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rAsType );

		virtual	void DoDestroyResource( CGUIAs* pRes ); 

	private:
		static CGUIAsManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_ASMANAGER_20101116_H__

