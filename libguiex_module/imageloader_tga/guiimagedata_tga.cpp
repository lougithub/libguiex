/** 
* @file guiimagedata_tga.cpp
* @brief image data, used by render and texture
* @author ken
* @date 2006-07-13
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module/imageloader_tga/guiimagedata_tga.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIImageData_tga::CGUIImageData_tga(IGUIInterfaceImageLoader* pLoader)
		:CGUIImageData(pLoader)
		,m_uWidth(0)
		,m_uHeight(0)
		,m_pData(NULL)
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageData_tga::~CGUIImageData_tga()
	{
		DestroyData();
	}
	//------------------------------------------------------------------------------
	void CGUIImageData_tga::DestroyData()
	{
		if( m_pData )
		{
			delete[] m_pData;
			m_pData = NULL;
		}
	}
	//------------------------------------------------------------------------------
	uint8* CGUIImageData_tga::SetImageData(uint32 uWidth, uint32 uHeight, EGuiPixelFormat ePixelFormat )
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
	uint32 CGUIImageData_tga::GetWidth()
	{
		return m_uWidth;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIImageData_tga::GetHeight()
	{
		return m_uHeight;
	}
	//------------------------------------------------------------------------------
	uint8* CGUIImageData_tga::GetData()
	{
		return m_pData;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
