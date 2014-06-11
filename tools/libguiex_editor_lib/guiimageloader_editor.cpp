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
		uint32 uDataLineBytes = pData->GetDataWidth()*pData->GetBytePerPixel();
		uint32 uFileLineBytes = pWxImage->GetWidth()*3;
		for( uint32 i=0; i<pWxImage->GetHeight(); ++i )
		{
			uint32 uDataWidthStart = i*uDataLineBytes;
			uint32 uFileWidthStart = i*uFileLineBytes;
			for( uint32 j=0; j<pWxImage->GetWidth(); ++j )
			{
				uint32 uDataStart = uDataWidthStart + j*pData->GetBytePerPixel();
				uint32 uFileStart = uFileWidthStart + j*3;

				memcpy( tmpBuff+uDataStart, pRGB+uFileStart, 3 );
				if( eImageFormat == GUI_PF_RGBA_32 )
				{
					tmpBuff[uDataStart+3] = *pAlpha;
					++pAlpha;
				}
			}
		}

		delete pWxImage;

		return pData;
	}
	//------------------------------------------------------------------------------

}
