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
		m_strImageName = "BGIMAGE";
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
	void CGUIWgtStaticImage::SetCurrentImage( const CGUIString& rImageName)
	{
		if( m_pImageBG = GetImage(rImageName))
		{
			m_strImageName = rImageName;
		}
		else
		{
			//error
			throw CGUIException("[CGUIWgtStaticImage::SetCurrentImage]: failed to get image by name <%s>!",rImageName.c_str());
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWgtStaticImage::GetCurrentImage(  ) const
	{
		return m_strImageName;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
