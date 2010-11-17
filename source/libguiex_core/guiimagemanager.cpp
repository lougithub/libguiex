/** 
* @file guiimagemanager.cpp
* @brief image manager.
* @author ken
* @date 2006-07-17
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_SINGLETON_IMPLEMENT_EX(CGUIImageManager );
	//------------------------------------------------------------------------------
	CGUIImageManager::CGUIImageManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageManager::~CGUIImageManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::DoCreateImage(
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIString& rPath, 
		const CGUIRect& rUVRect, 
		EImageOrientation eImageOrientation,
		const CGUISize& rSize ) const
	{
		CGUIImage* pImage = new CGUIImage( rName, rSceneName, rPath, rUVRect, eImageOrientation );
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::DoCreateImage( 
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIColor& rColor,
		const CGUISize& rSize) const
	{
		CGUIImage* pImage = new CGUIImage( rName, rSceneName, rColor );
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::DoCreateImage(
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat,
		const CGUISize& rSize) const
	{
		CGUIImage* pImage = new CGUIImage( rName, rSceneName, buffPtr, buffWidth, buffHeight, ePixelFormat  );
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::DoCreateImage(
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty ) const
	{
		/**
		*<property name="btn_ok_hover" type="CGUIImage">
		*		<property name="path" type="CGUIString" value="./data/sample.png"/>
		*		<property name="uv" type="CGUIRect" value="0,0,0.5,1" />
		*		<property name="orientation" type="EImageOrientation" value="eImageOrientation_Normal" />
		*		<property name="size" type="CGUISize" value="1,1" />
		*</property>
		*
		*<property name="btn_ok_hover" type="CGUIImage">
		*		<property name="color" type="CGUIColor" value="0.5,0.0,0.6"/>
		*		<property name="size" type="CGUISize" value="4,4" />
		*</property>
		*/
		if( rProperty.HasProperty("path"))
		{
			//a PATH image
			const CGUIProperty* pPptPath = rProperty.GetProperty("path");
			const CGUIProperty* pPptUV = rProperty.GetProperty("uv");
			const CGUIProperty* pPPtImgOp = rProperty.GetProperty("orientation");
			const CGUIProperty* pSize = rProperty.GetProperty("size");

			if( !pPptPath )
			{
				throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str());
				return NULL;
			}

			//uv
			CGUIRect aUVRect( 0.f,0.f,1.f,1.f );
			if( pPptUV )
			{
				PropertyToValue( *pPptUV, aUVRect );
			}

			//image orientation
			EImageOrientation eImageOrientation = eImageOrientation_Normal;
			if( pPPtImgOp )
			{
				PropertyToValue( *pPPtImgOp, eImageOrientation );
			}

			//size
			CGUISize aSize( 0.f,0.f );
			if( pSize )
			{
				PropertyToValue( *pSize, aSize );
			}

			return DoCreateImage( rName, rSceneName, pPptPath->GetValue(), aUVRect, eImageOrientation, aSize);
		}
		else if(rProperty.HasProperty("color"))
		{
			//a COLOR image
			const CGUIProperty* pPptColor = rProperty.GetProperty("color");
			const CGUIProperty* pSize = rProperty.GetProperty("size");
			
			if( !pPptColor )
			{
				throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str());
				return NULL;
			}
			//size
			CGUISize aSize( 0.f,0.f );
			if( pSize )
			{
				PropertyToValue( *pSize, aSize );
			}

			CGUIColor aColor;
			PropertyToValue( *pPptColor, aColor );
			return DoCreateImage( rName, rSceneName, aColor, aSize);
		}
		else
		{
			throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str());
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIImageManager::RegisterImage( 
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty)
	{
		CGUIImage* pImage = DoCreateImage(
			rName,
			rSceneName,
			rProperty );
		RegisterResource( pImage );
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIImageManager::RegisterImage(
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIString& rPath, 
		const CGUIRect& rUVRect, 
		EImageOrientation eImageOrientation,
		const CGUISize& rSize)
	{
		CGUIImage* pImage = DoCreateImage(
			rName,
			rSceneName,
			rPath, 
			rUVRect, 
			eImageOrientation,
			rSize);
		RegisterResource( pImage );
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIImageManager::RegisterImage( 
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIColor& rColor,
		const CGUISize& rSize)
	{
		CGUIImage* pImage = DoCreateImage(
			rName,
			rSceneName,
			rColor,
			rSize);
		RegisterResource( pImage );
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIImageManager::RegisterImage(
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat,
		const CGUISize& rSize)
	{
		CGUIImage* pImage = DoCreateImage(
			rName,
			rSceneName,
			buffPtr, 
			buffWidth, 
			buffHeight, 
			ePixelFormat,
			rSize);
		RegisterResource( pImage );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::AllocateResource( const CGUIString& rResName ) const
	{
		CGUIImage* pImage = CGUIResourceManager<CGUIImage>::GetResource( rResName );
		if( !pImage )
		{
			throw CGUIException( 
				"[CGUIImageManager::AllocateResource]: failed to get image by name <%s>",
				rResName.c_str());
			return NULL;
		}
		pImage->RefRetain();
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::AllocateResource( 			
		const CGUIProperty& rProperty ) const
	{
		CGUIImage* pImage = DoCreateImage(
			"",
			"",
			rProperty );
		pImage->RefRetain();
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::AllocateResource( 			
		const CGUIString& rPath, 
		const CGUIRect& rUVRect, 
		EImageOrientation eImageOrientation,
		const CGUISize& rSize ) const
	{
		CGUIImage* pImage = DoCreateImage(
			"",
			"",
			rPath, 
			rUVRect, 
			eImageOrientation,
			rSize );
		pImage->RefRetain();
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::AllocateResource( 			
		const CGUIColor& rColor,
		const CGUISize& rSize ) const
	{
		CGUIImage* pImage = DoCreateImage(
			"",
			"",
			rColor,
			rSize );
		pImage->RefRetain();
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::AllocateResource( 			
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat,
		const CGUISize& rSize ) const
	{
		CGUIImage* pImage = DoCreateImage(
			"",
			"",
			buffPtr, 
			buffWidth, 
			buffHeight, 
			ePixelFormat,
			rSize );
		pImage->RefRetain();
		return pImage;
	}
	//------------------------------------------------------------------------------
	int32 CGUIImageManager::DeallocateResource( CGUIImage* pImage )
	{
		GUI_ASSERT( pImage, "invalid parameter" );

		pImage->RefRelease();
		if( pImage->GetRefCount() == 0 )
		{
			if( pImage->GetName() == "" &&
				pImage->GetSceneName() == "" )
			{
				//this image is not registered as a named image, free it
				return ReleaseAllocateResource( pImage );
			}
			else
			{
				//named image's reference count shouldn't be zero, which is retained by register function
				throw CGUIException(
					"[CGUIImageManager::DeallocateResource]: invalid reference count [%d] for resource: <%s:%s:%s>", 
					pImage->GetRefCount(),
					pImage->GetName().c_str(), 
					pImage->GetResourceType().c_str(),
					pImage->GetSceneName().c_str() );
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
