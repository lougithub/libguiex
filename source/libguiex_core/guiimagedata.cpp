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
#include "guimath.h"
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
		,m_uImageWidth(0)
		,m_uImageHeight(0)
		,m_uDataWidth(0)
		,m_uDataHeight(0)
		,m_pData(NULL)
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageData::~CGUIImageData()
	{
		DestroyData();
	}
	//------------------------------------------------------------------------------
	void CGUIImageData::DestroyData()
	{
		if( m_pData )
		{
			delete[] m_pData;
			m_pData = NULL;
		}
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
		case GUI_PF_ALPHA_8:
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
			GUI_THROW( "[CGUIImageData_tga::SetImageData] - unknown pixel format!");
			return 0;
		}

		return bytesPerPixel;
	}
	//------------------------------------------------------------------------------
	EGuiPixelFormat	CGUIImageData::GetPixelFormat()
	{
		return m_ePixelFormat;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIImageData::GetBytePerPixel() const
	{
		return GetBytePerPixel(m_ePixelFormat);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get width of data
	*/
	uint32 CGUIImageData::GetDataWidth()
	{
		return m_uDataWidth;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get height of Data
	*/
	uint32 CGUIImageData::GetDataHeight()
	{
		return m_uDataHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get width of image
	*/
	uint32 CGUIImageData::GetImageWidth()
	{
		return m_uImageWidth;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get height of image
	*/
	uint32 CGUIImageData::GetImageHeight()
	{
		return m_uImageHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get data of image
	*/
	uint8* CGUIImageData::GetData()
	{
		return m_pData;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set image data
	* @return data pointer, NULL for failed
	*/
	uint8* CGUIImageData::SetImageData(uint32 uWidth, uint32 uHeight, EGuiPixelFormat ePixelFormat )
	{
		if( !uWidth || !uHeight )
		{
			//failed
			return NULL;
		}
		
		m_uImageWidth = uWidth;
		m_uImageHeight = uHeight;

#if !GUI_TEXTURE_NPOT_SUPPORT
		//make width and height is power of 2
		uWidth = CGUIMath::GetNextPOT(uWidth);
		uHeight = CGUIMath::GetNextPOT(uHeight);
#endif

		m_uDataWidth = uWidth;
		m_uDataHeight = uHeight;
		m_ePixelFormat = ePixelFormat;
		DestroyData();

		uint32 bytesPerPixel = GetBytePerPixel(ePixelFormat);

		m_pData = new uint8[uWidth*uHeight*bytesPerPixel];
		memset( m_pData, 255, uWidth*uHeight*bytesPerPixel );
		return m_pData;
	}
	//------------------------------------------------------------------------------
}
