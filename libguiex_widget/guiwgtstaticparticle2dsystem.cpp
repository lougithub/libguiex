/** 
* @file guiwgtstaticparticle2dsystem.cpp
* @brief used to show a particle 2d system
* @author ken
* @date 2011-01-14
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtstaticparticle2dsystem.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiparticle2dsystemquad.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtStaticParticle2DSystem);
	//------------------------------------------------------------------------------
	CGUIWgtStaticParticle2DSystem::CGUIWgtStaticParticle2DSystem( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtStatic(StaticGetType(), rName, rSceneName)
	{
		InitStaticParticle2DSystem();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticParticle2DSystem::CGUIWgtStaticParticle2DSystem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtStatic(rType, rName, rSceneName)
	{
		InitStaticParticle2DSystem();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticParticle2DSystem::InitStaticParticle2DSystem()
	{
		m_pParticle2DSystem = NULL;
		m_pParticleImage = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticParticle2DSystem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		m_pParticle2DSystem->Render( pRender, getFullTransform() );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticParticle2DSystem::OnCreate()
	{
		CGUIWidget::OnCreate();

		GUI_ASSERT( m_pParticle2DSystem==NULL, "CGUIWgtStaticParticle2DSystem::OnCreate: invalid pointer" );
		m_pParticle2DSystem = new CGUIParticle2DSystemQuad( 1000 );
		m_pParticle2DSystem->SetTexture( m_pParticleImage->GetFullFilePath() );
		m_pParticle2DSystem->SetEmissionRate( 30.0f );
		m_pParticle2DSystem->SetLife( 3.0f );
		m_pParticle2DSystem->SetStartColor( CGUIColor( 1.0f,0.0f,0.0f,1.0f ));
		m_pParticle2DSystem->SetEndColor( CGUIColor( 1.0f,1.0f,1.0f,1.0f ) );
		m_pParticle2DSystem->SetStartSize( 32 );
		m_pParticle2DSystem->SetSpeed( -5.0f );
		m_pParticle2DSystem->SetSpeedVar( -5.0f );
		m_pParticle2DSystem->SetGravity( CGUIVector2(0.0f, 10.0f) );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticParticle2DSystem::OnDestroy()
	{
		GUI_ASSERT( m_pParticle2DSystem!=NULL, "CGUIWgtStaticParticle2DSystem::OnDestroy: invalid pointer" );
		delete m_pParticle2DSystem;
		m_pParticle2DSystem = NULL;

		CGUIWidget::OnDestroy();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticParticle2DSystem::OnUpdate( real fDeltaTime )
	{
		CGUIWidget::OnUpdate(fDeltaTime );

		m_pParticle2DSystem->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticParticle2DSystem::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "PARTICLEIMAGE")
		{
			m_pParticleImage = pImage;
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

