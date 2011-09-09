/** 
* @file guiimageloader_editor.cpp
* @author ken
* @date 2011-09-08
*/

//============================================================================//
// include
//============================================================================//
#include "guiimageloader_editor.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================//
namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIImageLoader_Editor::StaticGetModuleName()
	{
		return "IGUIImageLoader_Editor";
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_Editor::IGUIImageLoader_Editor()
		:IGUIInterfaceImageLoader( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_Editor::LoadFromFile( const CGUIString& rFileName  )
	{

		wxString rImagePath = Gui2wxString( GSystem->GetDataPath() + rFileName );
		wxImage* pWxImage = LoadwxImageByFilename(rImagePath);
		if( !pWxImage )
		{
			GUI_THROW( GUI_FORMAT("[IGUIImageLoader_Editor::LoadFromFile] - Failed to load image: %s", rFileName.c_str()));
			return NULL;
		}

		CGUIImageData* pData = new CGUIImageData(this);
		EGuiPixelFormat	eImageFormat;	
		if( pWxImage->HasAlpha() )
		{
			eImageFormat = GUI_PF_RGBA_32;
		}
		else
		{
			eImageFormat = GUI_PF_RGB_24;
		}

		uint8* tmpBuff = pData->SetImageData( pWxImage->GetWidth(), pWxImage->GetHeight(), eImageFormat);
		uint8* pRGB = pWxImage->GetData();
		uint8* pAlpha = pWxImage->GetAlpha();
		uint32 nPixelCount = pWxImage->GetWidth() * pWxImage->GetHeight();
		for(uint32 i = 0; i < nPixelCount; ++i )
		{
			memcpy( tmpBuff, pRGB, 3 );
			tmpBuff += 3;
			pRGB += 3;
			if( eImageFormat == GUI_PF_RGBA_32 )
			{
				*tmpBuff = *pAlpha;
				++tmpBuff;
				++pAlpha;
			}
		}

		delete pWxImage;

		return pData;
	}
	//------------------------------------------------------------------------------

}
