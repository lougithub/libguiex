/** 
* @file guiwgtprogress.cpp
* @brief progress bar
* @author ken
* @date 2006-09-25
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtprogress.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtProgress);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtProgress::ms_strType = "CGUIWgtProgress";
	//------------------------------------------------------------------------------
	CGUIWgtProgress::CGUIWgtProgress(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitProgress();
	}
	//------------------------------------------------------------------------------
	CGUIWgtProgress::CGUIWgtProgress( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitProgress();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::InitProgress()
	{
		m_pImageBg = NULL;
		m_pImageFg = NULL;

		m_fMaximumValue = 0.0f;
		m_fCurrentValue = 0.0f;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtProgress::Create()
	{
		if( !m_pImageBg )
		{
			throw CGUIException("[CGUIWgtProgress::Create]: the image <BACKGROUND_IMG> hasn't been found!");
		}
		if( !m_pImageFg )
		{
			throw CGUIException("[CGUIWgtProgress::Create]: the image <FOREGROUND_IMG> hasn't been found!");
		}

		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtProgress::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BACKGROUND_IMG")
		{
			m_pImageBg = pImage;
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "FOREGROUND_IMG")
		{
			m_pImageFg = pImage;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtProgress::RenderSelf(IGUIInterfaceRender* pRender)
	{
		//background image
		DrawImage( pRender, m_pImageBg, GetBoundArea());

		//foreground image
		if( m_fMaximumValue > 0.0f && m_fCurrentValue > 0.0f)
		{
			if( m_fMaximumValue > m_fCurrentValue )
			{
				CGUISize aRenderSize = GetBoundArea().GetSize();
				aRenderSize.m_fWidth *= (m_fCurrentValue / m_fMaximumValue);
				CGUIRect aRenderRect = GetBoundArea();
				aRenderRect.SetSize( aRenderSize );
				DrawImage( pRender, m_pImageFg, aRenderRect );
			}
			else
			{
				DrawImage( pRender, m_pImageFg, GetBoundArea());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::SetMaximumProgress( real fMaxmium )
	{
		m_fMaximumValue = fMaxmium;
	}
	//------------------------------------------------------------------------------
	real CGUIWgtProgress::GetMaximumProgress() const
	{
		return m_fMaximumValue;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::SetCurrentProgress( real fValue )
	{
		m_fCurrentValue = fValue;
	}
	//------------------------------------------------------------------------------
	real CGUIWgtProgress::GetCurrentProgress( ) const
	{
		return m_fCurrentValue;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
