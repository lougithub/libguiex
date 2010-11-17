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
	class GUIEXPORT CGUIImageManager : public CGUIResourceManager <CGUIImage>
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIImageManager();

		/**
		* @brief destructor;
		*/
		virtual ~CGUIImageManager();

		/**
		* @brief register a named image by property
		*/
		int32 RegisterImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty);

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

		const CGUIImage* AllocateResource( const CGUIString& rImageName ) const;

		const CGUIImage* AllocateResource( 			
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty ) const;

		const CGUIImage* AllocateResource( 			
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rPath, 
			const CGUIRect& rUVRect=CGUIRect(0.0f,0.0f,1.0f,1.0f), 
			EImageOrientation eImageOrientation = eImageOrientation_Normal,
			const CGUISize& rSize = CGUISize(0,0) ) const;

		const CGUIImage* AllocateResource( 			
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIColor& rColor,
			const CGUISize& rSize = CGUISize(0,0) ) const;

		const CGUIImage* AllocateResource( 			
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat,
			const CGUISize& rSize = CGUISize() ) const;

		int32 DeallocateResource( const CGUIImage* pImage );

	protected:
		/**
		* @brief create a image by property;
		*/
		CGUIImage* DoCreateImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty) const;

		/**
		* @brief create a image by image path;
		*/
		CGUIImage* DoCreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rPath, 
			const CGUIRect& rUVRect, 
			EImageOrientation eImageOrientation,
			const CGUISize& rSize) const;

		/**
		* @brief create a image with given color
		*/
		CGUIImage* DoCreateImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIColor& rColor,
			const CGUISize& rSize) const;

		/**
		 * create image from memory
		 */
		CGUIImage* DoCreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat,
			const CGUISize& rSize) const;


	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIImageManager);
	};

}//namespace guiex

#endif		//__GUI_IMAGEMANAGER_20060717_H__

