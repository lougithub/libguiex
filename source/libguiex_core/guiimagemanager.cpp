/** 
* @file guiimagemanager.cpp
* @brief image manager.
* @author ken
* @date 2006-07-17
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guiimagemanager.h>
#include <libguiex_core\guiimage.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiprojectinfo.h>
#include <libguiex_core\guiprojectinfomanager.h>
#include <libguiex_core\guiexception.h>
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
		*<property name="btn_ok_hover" type="NAMED_IMAGE">
		*		<property name="PATH" type="STRING" value="./data/sample.png"/>
		*		<property name="UV" type="RECT" value="0,0,0.5,1" />
		*		<property name="IMAGE_OPERATION" type="ENUM" value="IMAGE_NONE" />
		*</property>
		*
		*<property name="btn_ok_hover" type="NAMED_IMAGE">
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6"/>
		*</property>
		*/
		if( rProperty.HasProperty("PATH"))
		{
			//a PATH image
			const CGUIProperty* pPptPath = rProperty.GetProperty("PATH");
			const CGUIProperty* pPptUV = rProperty.GetProperty("UV");
			const CGUIProperty* pPPtImgOp = rProperty.HasProperty("IMAGE_OPERATION")?rProperty.GetProperty("IMAGE_OPERATION"):NULL;

			return( CreateImage( 
				rName,
				rProjectName,
				pPptPath->GetValue(),
				CGUIStringConvertor::StringToRect( pPptUV->GetValue()),
				pPPtImgOp?CGUIStringConvertor::StringToImageOperation(pPPtImgOp->GetValue()):IMAGE_NONE));
		}
		else if(rProperty.HasProperty("COLOR"))
		{
			//a COLOR image
			const CGUIProperty* pPptColor = rProperty.GetProperty("COLOR");

			return( CreateImage( 
				rName,
				rProjectName,
				CGUIStringConvertor::StringToColor(pPptColor->GetValue())));
		}
		else
		{
			throw CGUIException("[CGUIImageManager::CreateImage]: invalid image property: <%s> <%s>", rProperty.GetName().c_str(), rProperty.GetType().c_str());
			return NULL;
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex
