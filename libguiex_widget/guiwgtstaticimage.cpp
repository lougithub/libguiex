/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtstaticimage.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtStaticImage);
	//------------------------------------------------------------------------------
	CGUIWgtStaticImage::CGUIWgtStaticImage( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtStatic(StaticGetType(), rName, rSceneName)
	{
		InitStaticImage();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticImage::CGUIWgtStaticImage( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtStatic(rType, rName, rSceneName)
	{
		InitStaticImage();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticImage::InitStaticImage()
	{
		m_pImageBG = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticImage::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			if( GetSize().IsEqualZero() && m_pImageBG )
			{
				SetPixelSize(m_pImageBG->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticImage::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetBoundArea( ));
	}
	//------------------------------------------------------------------------------


}//namespace guiex

