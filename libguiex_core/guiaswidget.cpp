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
	void CGUIAsWidgetAlpha::ApplyValue( )
	{
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetAlpha(GetCurrentValue());
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsWidgetChildrenAlpha
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetChildrenAlpha::CGUIAsWidgetChildrenAlpha( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsInterpolation<real>("CGUIAsWidgetChildrenAlpha" , rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetChildrenAlpha::ApplyValue( )
	{
		GUI_ASSERT( GetReceiver(), "no receiver");
		CGUIWidget* pChild = GetReceiver()->GetChild();
		while( pChild )
		{
			pChild->SetAlpha(GetCurrentValue());
			pChild = pChild->GetNextSibling();
		}
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
	void CGUIAsWidgetRotation::ApplyValue( )
	{
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
	void CGUIAsWidgetScale::ApplyValue( )
	{
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
	void CGUIAsWidgetPosition::ApplyValue( )
	{
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
	void CGUIAsWidgetSize::ApplyValue( )
	{
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
	void CGUIAsWidgetColor::ApplyValue( )
	{
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


	//*****************************************************************************
	//	CGUIAsWidgetVisible
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetVisible::CGUIAsWidgetVisible(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs("CGUIAsWidgetVisible", rAsName, rSceneName)
		,m_bVisible( true )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetVisible::SetVisible( bool bVisible )
	{
		m_bVisible = bVisible;
	}
	//------------------------------------------------------------------------------
	bool CGUIAsWidgetVisible::IsVisible( ) const
	{
		return m_bVisible;
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetVisible::OnRetired()
	{
		CGUIAs::OnRetired();

		GetReceiver()->SetVisible( m_bVisible );
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsWidgetVisible::ProcessProperty( const CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsWidgetChildrenVisible">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="0" />
		*		<property name="elapsed_time" type="real" value="0" />
		*		<property name="visibility" type="bool" value="false" />
		*</property>
		*/
		int32 ret = CGUIAs::ProcessProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		{
			const CGUIProperty* pPPtVisibility = rProperty.GetProperty("visibility", "bool" );
			if( pPPtVisibility )
			{
				PropertyToValue( *pPPtVisibility, m_bVisible );
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsWidgetVisible::GenerateProperty( CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsWidgetChildrenVisible">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="0" />
		*		<property name="elapsed_time" type="real" value="0" />
		*		<property name="visibility" type="bool" value="false" />
		*</property>
		*/
		int32 ret = CGUIAs::GenerateProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		{
			CGUIProperty aProperty( "visibility", "bool" );
			ValueToProperty( m_bVisible, aProperty );
			rProperty.AddProperty( aProperty );
		}

		return 0;
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsWidgetChildrenVisible
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWidgetChildrenVisible::CGUIAsWidgetChildrenVisible(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs("CGUIAsWidgetChildrenVisible", rAsName, rSceneName)
		,m_bVisible( true )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetChildrenVisible::SetVisible( bool bVisible )
	{
		m_bVisible = bVisible;
	}
	//------------------------------------------------------------------------------
	bool CGUIAsWidgetChildrenVisible::IsVisible( ) const
	{
		return m_bVisible;
	}
	//------------------------------------------------------------------------------
	void CGUIAsWidgetChildrenVisible::OnRetired()
	{
		CGUIAs::OnRetired();

		CGUIWidget* pChild = GetReceiver()->GetChild();
		while( pChild )
		{
			pChild->SetVisible( m_bVisible );
			pChild = pChild->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsWidgetChildrenVisible::ProcessProperty( const CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsWidgetChildrenVisible">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="0" />
		*		<property name="elapsed_time" type="real" value="0" />
		*		<property name="visibility" type="bool" value="false" />
		*</property>
		*/
		int32 ret = CGUIAs::ProcessProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		{
			const CGUIProperty* pPPtVisibility = rProperty.GetProperty("visibility", "bool" );
			if( pPPtVisibility )
			{
				PropertyToValue( *pPPtVisibility, m_bVisible );
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsWidgetChildrenVisible::GenerateProperty( CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsWidgetChildrenVisible">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="0" />
		*		<property name="elapsed_time" type="real" value="0" />
		*		<property name="visibility" type="bool" value="false" />
		*</property>
		*/
		int32 ret = CGUIAs::GenerateProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		{
			CGUIProperty aProperty( "visibility", "bool" );
			ValueToProperty( m_bVisible, aProperty );
			rProperty.AddProperty( aProperty );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex

