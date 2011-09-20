/** 
* @file guiimagemanager.cpp
* @brief image manager.
* @author ken
* @date 2006-07-17
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiimagemanager.h"
#include "guiimage.h"
#include "guiproperty.h"
#include "guistringconvertor.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guiexception.h"
#include "guipropertyconvertor.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIImageManager * CGUIImageManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIImageManager::CGUIImageManager()
		:CGUIResourceManager <CGUIImage, CGUIImage>("Image")
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIImageManager::CGUIImageManage]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIImageManager::~CGUIImageManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIImageManager* CGUIImageManager::Instance()
	{
		return m_pSingleton;
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
		CGUIImage* pImage = new CGUIImage( rName, rSceneName, rPath, rUVRect, eImageOrientation, rSize );
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::DoCreateImage( 
		const CGUIString& rName,
		const CGUIString& rSceneName,
		const CGUIColor& rColor,
		const CGUISize& rSize) const
	{
		CGUIImage* pImage = new CGUIImage( rName, rSceneName, rColor, rSize );
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
		CGUIImage* pImage = new CGUIImage( rName, rSceneName, buffPtr, buffWidth, buffHeight, ePixelFormat, rSize  );
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::DoCreateImage(
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
		if( rProperty.HasProperty("path", "CGUIString"))
		{
			//a PATH image
			const CGUIProperty* pPptPath = rProperty.GetProperty("path", "CGUIString");
			const CGUIProperty* pPptUV = rProperty.GetProperty("uv", "CGUIRect");
			const CGUIProperty* pPPtImgOp = rProperty.GetProperty("orientation", "EImageOrientation");
			const CGUIProperty* pSize = rProperty.GetProperty("size", "CGUISize");

			if( !pPptPath )
			{
				GUI_THROW( GUI_FORMAT("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str()));
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
			CGUISize aTextureSize( 0.f,0.f );
			if( !pPptPath )
			{
				GUI_THROW( GUI_FORMAT("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str()));
				return NULL;
			}
			PropertyToValue( *pSize, aTextureSize );
			CGUISize aImageSize = aUVRect.GetSize();
			if( eImageOrientation == eImageOrientation_90CW ||
				eImageOrientation == eImageOrientation_90CCW )
			{
				std::swap( aImageSize.m_fWidth, aImageSize.m_fHeight );
			}

			aUVRect.m_fLeft /= aTextureSize.GetWidth();
			aUVRect.m_fRight /= aTextureSize.GetWidth();
			aUVRect.m_fTop /= aTextureSize.GetHeight();
			aUVRect.m_fBottom /= aTextureSize.GetHeight();

			return DoCreateImage( rProperty.GetName(), rSceneName, pPptPath->GetValue(), aUVRect, eImageOrientation, aImageSize);
		}
		else if(rProperty.HasProperty("color", "CGUIColor"))
		{
			//a COLOR image
			const CGUIProperty* pPptColor = rProperty.GetProperty("color", "CGUIColor");
			const CGUIProperty* pSize = rProperty.GetProperty("size", "CGUISize");
			
			if( !pPptColor )
			{
				GUI_THROW( GUI_FORMAT("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str()));
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
			return DoCreateImage( rProperty.GetName(), rSceneName, aColor, aSize);
		}
		else
		{
			GUI_THROW( GUI_FORMAT("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str()));
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIImageManager::RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		CGUIImage* pImage = DoCreateImage(
			rSceneName,
			rProperty );
		RegisterResourceImp( pImage );
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
		RegisterResourceImp( pImage );
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
		RegisterResourceImp( pImage );
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
		RegisterResourceImp( pImage );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIImageManager::AllocateResource( const CGUIString& rResName ) const
	{
		CGUIImage* pImage = CGUIResourceManager<CGUIImage, CGUIImage>::GetRegisterResource( rResName );
		if( !pImage )
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIImageManager::AllocateResource]: failed to get image by name <%s>",
				rResName.c_str()));
			return NULL;
		}
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
	void CGUIImageManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			if( pRes->GetName() == "" &&
				pRes->GetSceneName() == "" )
			{
				//this image is not registered as a named image, free it
				ReleaseFromAllocatePool( pRes );
			}
			else
			{
				//named image's reference count shouldn't be zero, which is retained by register function
				GUI_THROW( GUI_FORMAT(
					"[CGUIImageManager::DeallocateResource]: invalid reference count [%d] for resource: <%s:%s:%s>", 
					pRes->GetRefCount(),
					pRes->GetName().c_str(), 
					pRes->GetResourceType().c_str(),
					pRes->GetSceneName().c_str()));
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIImageManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUIImageManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
