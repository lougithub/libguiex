/** 
* @file guiimagemanager.h
* @brief image manager.
* @author ken
* @date 2006-07-17
*/

#ifndef __GUI_IMAGEMANAGER_20060717_H__
#define __GUI_IMAGEMANAGER_20060717_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
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
	class CGUIProperty;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIImageManager
	* @brief image manager
	* 
	*/
	class GUIEXPORT CGUIImageManager : public CGUIResourceManager <CGUIImage, CGUIImage>
	{
	public:
		CGUIImageManager();
		virtual ~CGUIImageManager();

		static CGUIImageManager* Instance(); 


		int32 RegisterImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rPath, 
			const CGUIRect& rUVRect=CGUIRect(0.0f,0.0f,1.0f,1.0f), 
			EImageOrientation eImageOrientation = eImageOrientation_Normal,
			const CGUISize& rSize = CGUISize(0,0));

		int32 RegisterImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIColor& rColor,
			const CGUISize& rSize = CGUISize(0,0));

		int32 RegisterImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat,
			const CGUISize& rSize = CGUISize(0,0));

		virtual int32 RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty);

		CGUIImage* AllocateResource( const CGUIString& rImageName ) const;

		CGUIImage* AllocateResource( 			
			const CGUIString& rPath, 
			const CGUIRect& rUVRect, 
			EImageOrientation eImageOrientation = eImageOrientation_Normal,
			const CGUISize& rSize = CGUISize(0,0) ) const;

		CGUIImage* AllocateResource( 			
			const CGUIColor& rColor,
			const CGUISize& rSize = CGUISize(0,0) ) const;

		CGUIImage* AllocateResource( 			
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat,
			const CGUISize& rSize = CGUISize() ) const;

		virtual void DeallocateResource( CGUIResource* pRes );

	protected:
		virtual	void DestroyRegisterResourceImp( CGUIResource* pRes ); 
		virtual	void DestroyAllocateResourceImp( CGUIResource* pRes ); 

		CGUIImage* DoCreateImage( 
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty) const;

		CGUIImage* DoCreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rPath, 
			const CGUIRect& rUVRect, 
			EImageOrientation eImageOrientation,
			const CGUISize& rSize) const;

		CGUIImage* DoCreateImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIColor& rColor,
			const CGUISize& rSize) const;

		CGUIImage* DoCreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat,
			const CGUISize& rSize) const;

	private:
		static CGUIImageManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_IMAGEMANAGER_20060717_H__

