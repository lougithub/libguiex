/** 
* @file guiwgtanimation.cpp
* @brief base class, for static widget
* @author ken
* @date 2006-08-10
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtanimation.h>
#include <libguiex_core/guianimation.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtAnimation);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtAnimation::ms_strType = "CGUIWgtAnimation";
	//------------------------------------------------------------------------------
	CGUIWgtAnimation::CGUIWgtAnimation( const CGUIString& rName, const CGUIString& rProjectName  )
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitAnimation();
	}
	//------------------------------------------------------------------------------
	CGUIWgtAnimation::CGUIWgtAnimation( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitAnimation();
	}
	//------------------------------------------------------------------------------
	CGUIWgtAnimation::~CGUIWgtAnimation()
	{
		m_pAnimationCur = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::InitAnimation()
	{
		SetFocusable(false);

		m_pAnimationName = "ANIMATION_DEFAULT";
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::SetAnimation(const CGUIString& rAnimationName, 
		CGUIAnimation* pAnimation)
	{
		if( rAnimationName == "ANIMATION_DEFAULT" )
		{
			m_pAnimationCur = pAnimation;
			if( GetSize().IsEqualZero() && pAnimation )
			{
				SetPixelSize( pAnimation->GetSize() );
			}
		}
		CGUIWidget::SetAnimation(rAnimationName,pAnimation);;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::SetCurrentAnimation( const CGUIString& rAnimationName)
	{
		if( m_pAnimationCur = GetAnimation(rAnimationName))
		{
			m_pAnimationName = rAnimationName;
		}
		else
		{
			//error
			throw CGUIException("[CGUIWgtAnimation::SetCurrentAnimation]: failed to get animation by name <%s>!",rAnimationName.c_str());
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWgtAnimation::GetCurrentAnimation(  ) const
	{
		return m_pAnimationName;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		if( rName == "Animation")
		{
			SetCurrentAnimation(rValue);
		}
		else
		{
			CGUIWidget::SetValue(rName, rValue);
		}
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtAnimation::GetValue(const CGUIString& rName) const
	{
		if( rName == "Animation")
		{
			return GetCurrentAnimation();
		}
		else
		{
			return CGUIWidget::GetValue(rName);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::UpdateSelf( real fDeltaTime )
	{
		if( m_pAnimationCur)
		{
			m_pAnimationCur->Update( fDeltaTime );
		}

		CGUIWidget::UpdateSelf( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::RenderSelf(IGUIInterfaceRender* pRender)
	{
		if( m_pAnimationCur)
		{
			DrawAnimation( pRender,m_pAnimationCur, GetBoundArea());
		}
	}
	//------------------------------------------------------------------------------


}//namespace guiex
