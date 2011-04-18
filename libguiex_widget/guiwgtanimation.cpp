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
	CGUIWgtAnimation::CGUIWgtAnimation( const CGUIString& rName, const CGUIString& rSceneName  )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitAnimation();
	}
	//------------------------------------------------------------------------------
	CGUIWgtAnimation::CGUIWgtAnimation( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
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
	void CGUIWgtAnimation::OnSetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation )
	{
		if( rName == "ANIMATION_DEFAULT" )
		{
			m_pAnimationCur = pAnimation;
			if( GetSize().IsEqualZero() && pAnimation )
			{
				SetPixelSize( pAnimation->GetSize() );
			}
		}
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
	const CGUIString& CGUIWgtAnimation::GetCurrentAnimationName(  ) const
	{
		return m_pAnimationName;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIWgtAnimation::GetCurrentAnimation(  ) const
	{
		return m_pAnimationCur;
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
