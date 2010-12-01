/** 
 * @file guiimagedata_devil.cpp
 * @brief image data, used by render and texture
 * @author ken
 * @date 2006-07-13
 */


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module\imageloader_devil\guiimagedata_devil.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
//------------------------------------------------------------------------------
CGUIImageData_devil::CGUIImageData_devil(IGUIInterfaceImageLoader* pLoader)
:CGUIImageData(pLoader)
,m_pData(NULL)
,m_uWidth(0)
,m_uHeight(0)
{
}
//------------------------------------------------------------------------------
CGUIImageData_devil::~CGUIImageData_devil()
{
	DestroyData();
}
//------------------------------------------------------------------------------
void	CGUIImageData_devil::DestroyData()
{
	if( m_pData )
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}
//------------------------------------------------------------------------------
uint8*	CGUIImageData_devil::SetImageData(uint32 uWidth, uint32 uHeight )
{
	if( !uWidth || !uHeight )
	{
		//failed
		return NULL;
	}

	m_uWidth = uWidth;
	m_uHeight = uHeight;
	DestroyData();

	m_pData = new uint8[m_uWidth*m_uHeight*4];
	return m_pData;
}
//------------------------------------------------------------------------------
uint32 CGUIImageData_devil::GetWidth()
{
	return m_uWidth;
}
//------------------------------------------------------------------------------
uint32 CGUIImageData_devil::GetHeight()
{
	return m_uHeight;
}
//------------------------------------------------------------------------------
uint8*	CGUIImageData_devil::GetData()
{
	return m_pData;
}
//------------------------------------------------------------------------------
}//namespace guiex
