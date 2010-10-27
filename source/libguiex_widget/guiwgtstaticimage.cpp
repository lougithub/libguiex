/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtstaticimage.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtStaticImage);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtStaticImage::ms_strType = "CGUIWgtStaticImage";
	//------------------------------------------------------------------------------
	CGUIWgtStaticImage::CGUIWgtStaticImage( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtStatic(ms_strType, rName, rProjectName)
	{
		InitStaticImage();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticImage::CGUIWgtStaticImage( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtStatic(rType, rName, rProjectName)
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
	void		CGUIWgtStaticImage::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			if( NEWGetSize().IsEqualZero() && m_pImageBG )
			{
				NEWSetPixelSize(m_pImageBG->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticImage::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetWidgetRect( ), pRender->GetAndIncZ());
	}
	//------------------------------------------------------------------------------
	void				CGUIWgtStaticImage::SetCurrentImage( const CGUIString& rImageName)
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
	const CGUIString&			CGUIWgtStaticImage::GetCurrentImage(  ) const
	{
		return m_strImageName;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticImage::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		if( rName == "Image")
		{
			SetCurrentImage(rValue);
		}
		else
		{
			CGUIWidget::SetValue(rName, rValue);
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtStaticImage::GetValue(const CGUIString& rName) const
	{
		if( rName == "Image")
		{
			return GetCurrentImage();
		}
		else
		{
			return CGUIWidget::GetValue(rName);
		}
	}
	//------------------------------------------------------------------------------


}//namespace guiex

