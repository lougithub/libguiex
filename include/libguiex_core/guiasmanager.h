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
#include "guisingleton.h"
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
		/**
		* @brief constructor
		*/
		CGUIAsManager();

		/**
		* @brief destructor;
		*/
		virtual ~CGUIAsManager();

		int32 RegisterAs( 
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty );

		CGUIAs* AllocateResource( const CGUIString& rResName );
		CGUIAs* AllocateResourceByType( const CGUIString& rAsType );
		int32 DeallocateResource( CGUIAs* pRes );

		template<class T>
		T* AllocateResourceByTypeChecked( const CGUIString& rAsType )
		{
			CGUIAs* pAs = AllocateResourceByType( rAsType );
			GUI_ASSERT( pAs->GetType() == T::StaticGetAsType(), "wrong As type" );
			return static_cast<T*>( pAs );
		}

	protected:
		/**
		* @brief create a as by given property
		*/
		CGUIAs* DoCreateAs( 
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty );

		CGUIAs* DoCreateAs( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rAsType );

	protected:
		virtual	void DoDestroyResource( CGUIAs* pRes ); 


	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIAsManager);
	};

}//namespace guiex

#endif		//__GUI_ASMANAGER_20101116_H__

