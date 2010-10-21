/** 
* @file guiwgtprogress.cpp
* @brief progress bar
* @author ken
* @date 2006-09-25
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtprogress.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guistringconvertor.h>

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

		m_nMaximumValue = 0;
		m_aCurrentValue = 0;
		SetFlag(eFLAG_OPEN_WITH_PARENT, true);
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
			SetSize(pImage->GetSize());
		}
		else if( rName == "FOREGROUND_IMG")
		{
			m_pImageFg = pImage;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtProgress::RenderSelf(IGUIInterfaceRender* pRender)
	{
		//rect of progress
		CGUIRect aRect = GetRect();

		//background image
		DrawImage( pRender, m_pImageBg, aRect, pRender->GetAndIncZ(),&GetClipRect());

		//foreground image
		if( m_nMaximumValue > 0 && m_aCurrentValue > 0)
		{
			CGUIRect aRectFg(aRect);
			aRectFg.SetSize(CGUISize( 
				aRect.GetWidth() * (real(m_aCurrentValue)/m_nMaximumValue),
				aRect.GetHeight()));
			DrawImage( pRender, m_pImageFg, aRect, pRender->GetAndIncZ(),&aRectFg);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtProgress::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		if( rName == "MaximumValue")
		{
			SetMaximum( CGUIStringConvertor::StringToUInt(rValue));
		}
		else if( rName == "CurrentValue")
		{
			Update( CGUIStringConvertor::StringToUInt(rValue));
		}
		else
		{
			CGUIWidget::SetValue(rName, rValue);
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtProgress::GetValue(const CGUIString& rName) const
	{
		if( rName == "MaximumValue")
		{
			return CGUIStringConvertor::UIntToString(GetMaximum());
		}
		else if( rName == "CurrentValue")
		{
			return CGUIStringConvertor::UIntToString(GetCurrentValue());
		}
		else
		{
			return GetValue(rName);
		}
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtProgress::SetMaximum( uint32 nMaxmium )
	{
		m_nMaximumValue = nMaxmium;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtProgress::GetMaximum() const
	{
		return m_nMaximumValue;
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtProgress::Update( uint32 nValue )
	{
		m_aCurrentValue = nValue;
	}
	//------------------------------------------------------------------------------
	uint32 		CGUIWgtProgress::GetCurrentValue( ) const
	{
		return m_aCurrentValue;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
