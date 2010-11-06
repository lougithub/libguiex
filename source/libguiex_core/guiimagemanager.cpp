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
#include <libguiex_core/guiprojectinfo.h>
#include <libguiex_core/guiprojectinfomanager.h>
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
	CGUIImage*	CGUIImageManager::CreateImage(
		const CGUIString& rName,
		const CGUIString& rProjectName,
		const CGUIString& rPath, 
		const CGUIRect& rUVRect, 
		EImageOperation eImageOperation)
	{
		CGUIImage* pImage = new CGUIImage( rName, rProjectName, rPath, rUVRect, eImageOperation );
		AddResource(pImage);
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage*	CGUIImageManager::CreateImage( 
		const CGUIString& rName,
		const CGUIString& rProjectName,
		const CGUIColor& rColor )
	{
		CGUIImage* pImage = new CGUIImage( rName, rProjectName, rColor );
		AddResource(pImage);
		return pImage;
	}
	//------------------------------------------------------------------------------
	CGUIImage*	CGUIImageManager::CreateImage(
		const CGUIString& rName,
		const CGUIString& rProjectName,
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat )
	{
		CGUIImage* pImage = new CGUIImage( rName, rProjectName, buffPtr, buffWidth, buffHeight, ePixelFormat  );
		AddResource(pImage);
		return pImage;
	}
		//------------------------------------------------------------------------------
	CGUIImage*	CGUIImageManager::CreateImage(
		const CGUIString& rName,
		const CGUIString& rProjectName,
		const CGUIProperty& rProperty )
	{
		/**
		*<property name="btn_ok_hover" type="CGUIImage">
		*		<property name="path" type="CGUIString" value="./data/sample.png"/>
		*		<property name="uv" type="CGUIRect" value="0,0,0.5,1" />
		*		<property name="operation" type="EImageOperation" value="IMAGE_NONE" />
		*</property>
		*
		*<property name="btn_ok_hover" type="CGUIImage">
		*		<property name="color" type="CGUIColor" value="0.5,0.0,0.6"/>
		*</property>
		*/
		if( rProperty.HasProperty("path"))
		{
			//a PATH image
			const CGUIProperty* pPptPath = rProperty.GetProperty("path");
			const CGUIProperty* pPptUV = rProperty.GetProperty("uv");
			const CGUIProperty* pPPtImgOp = rProperty.GetProperty("operation");

			if( !pPptPath )
			{
				throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str());
				return NULL;
			}

			CGUIRect aUVRect( 0.f,0.f,1.f,1.f );
			EImageOperation eImageOperation = IMAGE_NONE;
			if( pPptUV )
			{
				PropertyToValue( *pPptUV, aUVRect );
			}
			if( pPPtImgOp )
			{
				PropertyToValue( *pPPtImgOp, eImageOperation );
			}

			return( CreateImage( 
				rName,
				rProjectName,
				pPptPath->GetValue(),
				aUVRect,
				eImageOperation));
		}
		else if(rProperty.HasProperty("color"))
		{
			//a COLOR image
			const CGUIProperty* pPptColor = rProperty.GetProperty("color");
			if( !pPptColor )
			{
				throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str());
				return NULL;
			}
			CGUIColor aColor;
			PropertyToValue( *pPptColor, aColor );
			return( CreateImage( 
				rName,
				rProjectName,
				aColor));
		}
		else
		{
			throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetTypeAsString().c_str());
			return NULL;
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex
