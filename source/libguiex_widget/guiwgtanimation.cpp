/** 
* @file guiwgtanimation.cpp
* @brief base class, for static widget
* @author ken
* @date 2006-08-10
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtanimation.h"
#include <libguiex_core/guianimation.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertyconvertor.h>

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
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::OnSetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation )
	{
		if( rName == "animation" )
		{
			m_pAnimationCur = pAnimation;
			if( GetSize().IsEqualZero() && pAnimation )
			{
				SetPixelSize( pAnimation->GetSize() );
			}
		}
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIWgtAnimation::GetCurrentAnimation(  ) const
	{
		return m_pAnimationCur;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::SetCurrentAnimation( const CGUIString& rName )
	{
		CGUIAnimation* pAni = GetAnimation( rName );
		m_pAnimationCur = pAni;

		if( !pAni )
		{
			GUI_THROW( GUI_FORMAT( "failed to find animation by name %d", rName.c_str()));
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
	int32 CGUIWgtAnimation::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Animation && rProperty.GetName() == "animation" )
		{
			CGUIAnimation* pAnimation = GetAnimation(rProperty.GetName());
			if( pAnimation )
			{
				rProperty.SetValue( pAnimation->GetName() );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtAnimation::ProcessProperty( const CGUIProperty& rProperty)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		if( rProperty.GetType() == ePropertyType_Animation && rProperty.GetName() == "animation")
		{			
			if(!rProperty.GetValue().empty())
			{
				SetAnimation( "animation", rProperty.GetValue());
			}
			else
			{
				//clear image
				SetAnimation( "animation", NULL);
			}

		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex
