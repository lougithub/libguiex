/** 
* @file guiwgtprogress.cpp
* @brief progress bar
* @author ken
* @date 2006-09-25
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiwgtprogress.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guistringconvertor.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtProgress);
	//------------------------------------------------------------------------------
	CGUIWgtProgress::CGUIWgtProgress(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitProgress();
	}
	//------------------------------------------------------------------------------
	CGUIWgtProgress::CGUIWgtProgress( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitProgress();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::InitProgress()
	{
		m_pImageBg = NULL;
		m_pImageFg = NULL;
		m_pImageSpark = NULL;
		m_pImageBorder = NULL;

		m_fMaximumValue = 0.0f;
		m_fCurrentValue = 0.0f;

		m_nBGAdjustLeft = 0;
		m_nBGAdjustRight = 0;
		m_nBGAdjustTop = 0;
		m_nBGAdjustBottom = 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::OnCreate()
	{
		CGUIWidget::OnCreate();

		if( !m_pImageBg )
		{
			GUI_THROW( "[CGUIWgtProgress::OnCreate]: the image <background> hasn't been found!");
		}
		if( !m_pImageFg )
		{
			GUI_THROW( "[CGUIWgtProgress::OnCreate]: the image <foreground> hasn't been found!");
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "background")
		{
			m_pImageBg = pImage;
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "foreground")
		{
			m_pImageFg = pImage;
		}
		else if( rName == "border")
		{
			m_pImageBorder = pImage;
			if( pImage && (GetSize().IsEqualZero() || (m_pImageBg && GetSize().IsEqual(m_pImageBg->GetSize()))))
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "spark")
		{
			m_pImageSpark = pImage;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		m_aBgRenderArea = GetClientArea();
		m_aBgRenderArea.m_fLeft += m_nBGAdjustLeft;
		m_aBgRenderArea.m_fRight += m_nBGAdjustRight;
		m_aBgRenderArea.m_fTop += m_nBGAdjustTop;
		m_aBgRenderArea.m_fBottom += m_nBGAdjustBottom;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::RenderSelf(IGUIInterfaceRender* pRender)
	{
		//background image
		DrawImage( pRender, m_pImageBg, m_aBgRenderArea);

		//foreground image
		if( m_fMaximumValue > 0.0f && m_fCurrentValue > 0.0f)
		{
			if( m_fMaximumValue > m_fCurrentValue )
			{
				CGUISize aRenderSize = m_aBgRenderArea.GetSize();
				aRenderSize.m_fWidth *= (m_fCurrentValue / m_fMaximumValue);
				CGUIRect aRenderRect = m_aBgRenderArea;
				aRenderRect.SetSize( aRenderSize );
				DrawImage( pRender, m_pImageFg, aRenderRect );
			}
			else
			{
				DrawImage( pRender, m_pImageFg, m_aBgRenderArea);
			}
		}

		if( m_pImageBorder )
		{
			DrawImage( pRender, m_pImageBorder, GetBoundArea());
		}

		if( m_pImageSpark )
		{
			if( m_fMaximumValue > m_fCurrentValue )
			{
				real fHeightDiffer = (m_aBgRenderArea.GetSize().GetHeight() - m_pImageSpark->GetSize().GetHeight()) / 2;
				CGUIRect aRenderRect( 
					m_aBgRenderArea.m_fLeft + m_aBgRenderArea.GetSize().GetWidth() * (m_fCurrentValue / m_fMaximumValue ) - m_pImageSpark->GetSize().GetWidth() / 2,
					m_aBgRenderArea.m_fTop + fHeightDiffer,
					m_aBgRenderArea.m_fLeft + m_aBgRenderArea.GetSize().GetWidth() * (m_fCurrentValue / m_fMaximumValue ) + m_pImageSpark->GetSize().GetWidth() / 2,
					m_aBgRenderArea.m_fBottom - fHeightDiffer );
				DrawImage( pRender, m_pImageSpark, aRenderRect );
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set maximum value of progress
	*/
	void CGUIWgtProgress::SetMaxValue( real fMaxmium )
	{
		m_fMaximumValue = fMaxmium;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get maximum value of progress
	*/
	real CGUIWgtProgress::GetMaxValue() const
	{
		return m_fMaximumValue;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set current value of progress
	*/
	void CGUIWgtProgress::SetCurrentValue( real fValue )
	{
		m_fCurrentValue = fValue;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set current value of progress
	*/
	real CGUIWgtProgress::GetCurrentValue( ) const
	{
		return m_fCurrentValue;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtProgress::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Real && rProperty.GetName() == "max_value" )
		{
			ValueToProperty( m_fMaximumValue, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Real && rProperty.GetName() == "current_value" )
		{
			ValueToProperty( m_fCurrentValue, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_left" )
		{
			ValueToProperty( m_nBGAdjustLeft, rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_right" )
		{
			ValueToProperty( m_nBGAdjustRight, rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_top" )
		{
			ValueToProperty( m_nBGAdjustTop, rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_bottom" )
		{
			ValueToProperty( m_nBGAdjustBottom, rProperty);
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtProgress::ProcessProperty( const CGUIProperty& rProperty)
	{
		if( rProperty.GetType() == ePropertyType_Real && rProperty.GetName() == "max_value")
		{
			PropertyToValue( rProperty, m_fMaximumValue);
		}
		else if( rProperty.GetType() == ePropertyType_Real && rProperty.GetName() == "current_value")
		{
			PropertyToValue( rProperty, m_fCurrentValue);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_left")
		{
			PropertyToValue( rProperty, m_nBGAdjustLeft);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_right")
		{
			PropertyToValue( rProperty, m_nBGAdjustRight);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_top")
		{
			PropertyToValue( rProperty, m_nBGAdjustTop);
		}
		else if( rProperty.GetType() == ePropertyType_Int16 && rProperty.GetName() == "bg_adjust_bottom")
		{
			PropertyToValue( rProperty, m_nBGAdjustBottom);
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
