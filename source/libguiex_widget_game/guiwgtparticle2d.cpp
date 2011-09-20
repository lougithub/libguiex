/** 
* @file guiwgtparticle2d.cpp
* @brief used to show a particle 2d system
* @author ken
* @date 2011-01-14
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiwgtparticle2d.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiparticle2dmanager.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guilogmsgmanager.h>
 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtParticle2D);
	//------------------------------------------------------------------------------
	CGUIWgtParticle2D::CGUIWgtParticle2D( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitStaticParticle2DSystem();
	}
	//------------------------------------------------------------------------------
	CGUIWgtParticle2D::CGUIWgtParticle2D( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitStaticParticle2DSystem();
	}
	//------------------------------------------------------------------------------
	CGUIWgtParticle2D::~CGUIWgtParticle2D()
	{
		GUI_ASSERT( !m_pParticle2DSystem, "resource leak" );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::InitStaticParticle2DSystem()
	{
		m_pParticle2DSystem = NULL;
		m_aOffsetMatrix = CGUIMatrix4::IDENTITY;
		m_bAutoDestoryOnFinish = false;
		m_aParticleOffset.x = m_aParticleOffset.y = 0.5f;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::OnDestroy()
	{
		if( m_pParticle2DSystem )
		{
			m_pParticle2DSystem->RefRelease();
			m_pParticle2DSystem = NULL;
		}

		CGUIWidget::OnDestroy();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderSelf( pRender );

		if( m_pParticle2DSystem )
		{
			pRender->PushMatrix();
			pRender->MultMatrix( m_aOffsetMatrix );

			m_pParticle2DSystem->Render( pRender );

			pRender->PopMatrix();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		m_aOffsetMatrix.setTrans( CGUIVector3( GetPixelSize().GetWidth() * m_aParticleOffset.x, GetPixelSize().GetHeight() * m_aParticleOffset.y, 0.0f ));
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::OnUpdate( real fDeltaTime )
	{
		CGUIWidget::OnUpdate(fDeltaTime );

		if( m_pParticle2DSystem )
		{
			m_pParticle2DSystem->Update( fDeltaTime );
			if( !m_pParticle2DSystem->IsActive() )
			{
				m_pParticle2DSystem->RefRelease();
				m_pParticle2DSystem = NULL;
				//CGUIWidgetManager::Instance()->DelayedDestroyWidget( this );
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::SetParticle2D( const CGUIString& rParticle2DName )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWgtParticle2D::SetParticle2D]: play particle %s", rParticle2DName.c_str()));

		if( rParticle2DName.empty() )
		{
			//clear
			SetParticle2D( NULL );
		}
		else
		{
			CGUIParticle2DSystem* pParticle2D = CGUIParticle2DManager::Instance()->AllocateResource( rParticle2DName );
			SetParticle2D( pParticle2D );
			pParticle2D->RefRelease();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::SetParticle2D( class CGUIParticle2DSystem* pParticle2D )
	{
		if( m_pParticle2DSystem == pParticle2D )
		{
			return;
		}

		if( m_pParticle2DSystem )
		{
			m_pParticle2DSystem->RefRelease();
			m_pParticle2DSystem = NULL;
		}

		if( pParticle2D )
		{
			m_pParticle2DSystem = pParticle2D;
			m_pParticle2DSystem->RefRetain();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::SetParticleOffset( const CGUIVector2& rOffset)
	{
		m_aParticleOffset = rOffset;
		m_aOffsetMatrix.setTrans( CGUIVector3( GetPixelSize().GetWidth() * m_aParticleOffset.x, GetPixelSize().GetHeight() * m_aParticleOffset.y, 0.0f ));
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIWgtParticle2D::GetParticleOffset(  ) const
	{
		return m_aParticleOffset;
	}		
	//------------------------------------------------------------------------------
	int32 CGUIWgtParticle2D::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Particle2D && rProperty.GetName() == "particle2d" )
		{
			if( m_pParticle2DSystem )
			{
				rProperty.SetValue( m_pParticle2DSystem->GetName() );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "offset" )
		{
			ValueToProperty( GetParticleOffset(), rProperty );
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtParticle2D::ProcessProperty( const CGUIProperty& rProperty)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		if( rProperty.GetType() == ePropertyType_Particle2D && rProperty.GetName() == "particle2d")
		{
			SetParticle2D( rProperty.GetValue());
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "offset")
		{
			CGUIVector2 aValue;
			PropertyToValue( rProperty, aValue );
			SetParticleOffset( aValue );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

