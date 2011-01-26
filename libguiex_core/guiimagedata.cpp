/** 
* @file guiimagedata.cpp
* @brief image data, used by render and texture
* @author ken
* @date 2006-07-13
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiimagedata.h"
#include "guiinterfaceimageloader.h"
#include "guiexception.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIImageData::CGUIImageData(IGUIInterfaceImageLoader* pLoader)
		:m_pLoader(pLoader)
		,m_ePixelFormat(GUI_PF_RGBA_32)
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageData::~CGUIImageData()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIImageData::Release()
	{
		if( m_pLoader)
		{
			m_pLoader->DestroyImageData(this);
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIImageData::GetBytePerPixel( EGuiPixelFormat ePixelFormat )
	{
		uint32 bytesPerPixel = 0;
		switch( ePixelFormat )
		{
		case GUI_PF_LUMINANCE_8:
			bytesPerPixel = 1;
			break;
		case GUI_PF_LUMINANCE_ALPHA_16:
			bytesPerPixel = 2;
			break;
		case GUI_PF_RGB_24:
			bytesPerPixel = 3;
			break;
		case GUI_PF_RGBA_32:
			bytesPerPixel = 4;
			break;
		case GUI_PF_ARGB_32:
			bytesPerPixel = 4;
			break;
		default:
			throw CGUIException("[CGUIImageData_tga::SetImageData] - unknown pixel format!");
			return 0;
		}

		return bytesPerPixel;
	}
	//------------------------------------------------------------------------------
}
