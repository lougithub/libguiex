/** 
* @file guiaswidget.cpp
* @brief widget action sequence
* @author ken
* @date 2011-04-22
*/

//============================================================================//
// include
//============================================================================// 
#include "guiaswidget.h"
#include "guiwidget.h"
#include "guisystem.h"
#include "guimath.h"
#include "guiasmanager.h"


//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsWidgetAlpha
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetAlpha::CGUIAsWidgetAlpha( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsInterpolation<real>("CGUIAsWidgetAlpha" , rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetAlpha::OnUpdate( )
	{
		CGUIAsInterpolation<real>::OnUpdate( );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetAlpha(GetCurrentValue());
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsWidgetRotation
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetRotation::CGUIAsWidgetRotation(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUIVector3>("CGUIAsWidgetRotation", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetRotation::OnUpdate( )
	{
		CGUIAsInterpolation<CGUIVector3>::OnUpdate( );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetRotation(GetCurrentValue());
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsWidgetScale
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetScale::CGUIAsWidgetScale(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUISize>("CGUIAsWidgetScale", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetScale::OnUpdate( )
	{
		CGUIAsInterpolation<CGUISize>::OnUpdate( );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetScale(GetCurrentValue());
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsWidgetPosition
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetPosition::CGUIAsWidgetPosition(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUIVector2>("CGUIAsWidgetPosition", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetPosition::OnUpdate( )
	{
		CGUIAsInterpolation<CGUIVector2>::OnUpdate( );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetPosition(GetCurrentValue());
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsWidgetSize
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetSize::CGUIAsWidgetSize(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUISize>("CGUIAsWidgetSize", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetSize::OnUpdate( )
	{
		CGUIAsInterpolation<CGUISize>::OnUpdate( );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetSize(GetCurrentValue());
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsWidgetColor
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetColor::CGUIAsWidgetColor(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUIColor>("CGUIAsWidgetColor", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetColor::OnUpdate( )
	{
		CGUIAsInterpolation<CGUIColor>::OnUpdate( );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetColor(GetCurrentValue());
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsWidgetMoveTo
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetMoveTo::CGUIAsWidgetMoveTo(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs("CGUIAsWidgetMoveTo", rAsName, rSceneName)
		,m_fVelocity( 5.0f )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetMoveTo::SetVelocity( real fVelocity )
	{
		m_fVelocity = fVelocity;
	}
	//------------------------------------------------------------------------------
	real CGUIAsWidgetMoveTo::GetVelocity( ) const
	{
		return m_fVelocity;
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetMoveTo::SetDestination( const CGUIVector2& rDestination )
	{
		m_aDestination = rDestination;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIAsWidgetMoveTo::GetDestination( ) const
	{
		return m_aDestination;
	}
	//------------------------------------------------------------------------------
	real CGUIAsWidgetMoveTo::Update( real fDeltaTime )
	{
		if( !IsRetired() )
		{
			const CGUIVector2& rCurPos = GetReceiver()->GetPosition();
			if( rCurPos == m_aDestination )
			{
				Retire( true );
				return 0.0f;
			}
			CGUIVector2 aDelta = m_aDestination - rCurPos;
			real fDeltaLen = aDelta.Normalise();
			real fCurMoveLen = m_fVelocity * fDeltaTime;
			if( fDeltaLen <= fCurMoveLen )
			{
				GetReceiver()->SetPosition( m_aDestination );
				Retire( true );
				return 0.0f;
			}
			else
			{
				GetReceiver()->SetPosition( rCurPos + aDelta * fCurMoveLen );
			}
		}

		return 0.0f;
	}
	//------------------------------------------------------------------------------
}//namespace guiex

