/** 
* @file guiimagedata_png_tga.cpp
* @brief image data, used by render and texture
* @author ken
* @date 2011-07-06
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module/imageloader_png_tga/guiimagedata_png_tga.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIImageData_png_tga::CGUIImageData_png_tga(IGUIInterfaceImageLoader* pLoader)
		:CGUIImageData(pLoader)
		,m_uWidth(0)
		,m_uHeight(0)
		,m_pData(NULL)
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageData_png_tga::~CGUIImageData_png_tga()
	{
		DestroyData();
	}
	//------------------------------------------------------------------------------
	void CGUIImageData_png_tga::DestroyData()
	{
		if( m_pData )
		{
			delete[] m_pData;
			m_pData = NULL;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set image data
	* @return data pointer, NULL for failed
	*/
	uint8* CGUIImageData_png_tga::SetImageData(uint32 uWidth, uint32 uHeight, EGuiPixelFormat ePixelFormat )
	{
		if( !uWidth || !uHeight )
		{
			//failed
			return NULL;
		}

		m_uWidth = uWidth;
		m_uHeight = uHeight;
		SetPixelFormat(ePixelFormat);
		DestroyData();

		uint32 bytesPerPixel = GetBytePerPixel(ePixelFormat);

		m_pData = new uint8[m_uWidth*m_uHeight*bytesPerPixel];
		return m_pData;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get width of image
	*/
	uint32 CGUIImageData_png_tga::GetWidth()
	{
		return m_uWidth;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get height of image
	*/
	uint32 CGUIImageData_png_tga::GetHeight()
	{
		return m_uHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get data of image
	*/
	uint8* CGUIImageData_png_tga::GetData()
	{
		return m_pData;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
