/** 
 * @file guiimagedata.cpp
 * @brief image data, used by render and texture
 * @author ken
 * @date 2006-07-13
 */


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiimagedata.h>
#include <libguiex_core/guiinterfaceimageloader.h>

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
void	CGUIImageData::Release()
{
	if( m_pLoader)
	{
		m_pLoader->DestroyImageData(this);
	}
}
//------------------------------------------------------------------------------
}
