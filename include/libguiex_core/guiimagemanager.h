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
		* @brief create a image by image path;
		*/
		const CGUIImage*	CreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIString& rPath, 
			const CGUIRect& rUVRect=CGUIRect(0.0f,0.0f,1.0f,1.0f), 
			EImageOperation eImageOperation = IMAGE_NONE);

		/**
		* @brief create a image with given color
		*/
		const CGUIImage*	CreateImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIColor& rColor );


		/**
		* @brief create a named image by property
		*
		*<property name="btn_ok_hover" type="CGUIImage">
		*		<property name="path" type="CGUIString" value="./data/sample.png"/>
		*		<property name="uv" type="CGUIRect" value="0,0,0.5,1" />
		*		<property name="operation" type="EImageOperation" value="IMAGE_NONE" />
		*</property>
		*
		*<property name="btn_ok_hover" type="CGUIImage">
		*		<property name="color" type="CGUIColor" value="0.5,0.0,0.6"/>
		*</property>
		*
		*/
		const CGUIImage*	CreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const CGUIProperty& rProperty );

		/**
		 * create image from memory
		 */
		const CGUIImage*	CreateImage(
			const CGUIString& rName,
			const CGUIString& rSceneName,
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat );

		const CGUIImage* AllocateResource( const CGUIString& rResName ) const;
		int32 DeallocateResource( const CGUIImage* pImage );


	protected:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIImageManager);
	};

}//namespace guiex

#endif		//__GUI_IMAGEMANAGER_20060717_H__

