#include <libguiex_framework/guiframework.h>
#include <libguiex_widget_game/guiwgt_game.h>

using namespace guiex;

class CMyCanvasLayer_Particle : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_Particle( const char* szLayerName );
	~CMyCanvasLayer_Particle( );

	virtual void DestroySelf( );

	void ApplyEffect();

protected:
	virtual uint32 OnKeyClicked( CGUIEventKeyboard* pEvent );
	void RegisterParticlePlayer();

protected:
	typedef CGUIParticle2DSystem* (*Fun_GenerateParticlePlayer)( );
	std::vector< Fun_GenerateParticlePlayer > m_vecParticlePlayers;

	CGUIWgtParticle2D* m_pWidgetParticle;
};

class CGUIFrameworkTest : public CGUIFramework
{
	public:
		CGUIFrameworkTest(  )
			:CGUIFramework(  )
		{
		}

	protected:
		virtual int32 InitializeGame( );
};

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}

int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
	CGUISceneManager::Instance()->LoadResources( "particle2d.uip" );	
	CGUISceneManager::Instance()->LoadWidgets( "particle2d.uip" );

	CMyCanvasLayer_Particle* pLayer3 = new CMyCanvasLayer_Particle( "layer 3" );
	pLayer3->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer3 );

	CGUISceneManager::Instance()->LoadResources( "common.uip" );	
	CGUISceneManager::Instance()->LoadWidgets( "common.uip" );
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common.uip" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);	
	return 0;
}



//*****************************************************************************
//	CMyCanvasLayer_Particle
//*****************************************************************************
void Btn_OnClick_ToggleEffect( CGUIEventMouse* pEvent )
{
	((CMyCanvasLayer_Particle*)(pEvent->GetReceiver()->GetParent()))->ApplyEffect();
}
//------------------------------------------------------------------------------
CMyCanvasLayer_Particle::CMyCanvasLayer_Particle( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
{
	m_pWidgetParticle = (CGUIWgtParticle2D*)CGUIWidgetManager::Instance()->GetPage( "particle2d.xml", "particle2d.uip" );
	m_pWidgetParticle->SetParent( this );
	m_pWidgetParticle->Open();

	CGUIWgtButton* pButtonWidget = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtButton>( "btnToggleEffect", "");
	pButtonWidget->SetParent( this );
	pButtonWidget->SetImage( "BTN_NORMAL", "button_blue_normal" );
	pButtonWidget->SetImage( "BTN_HOVER", "button_blue_hover" );
	pButtonWidget->SetImage( "BTN_PUSH", "button_blue_push" );
	pButtonWidget->SetImage( "BTN_DISABLE", "button_blue_disable" );
	pButtonWidget->SetPosition( 0, 0 );
	pButtonWidget->SetTextContentUTF8("effect", CGUIWgtButton::eButtonState_Normal);
	pButtonWidget->Create();
	pButtonWidget->Open();
	pButtonWidget->RegisterNativeCallbackFunc( "OnMouseLeftClick", Btn_OnClick_ToggleEffect );

	GSystem->RegisterGlobalKeyReceiver( this );

	RegisterParticlePlayer();
	ApplyEffect();
}

//------------------------------------------------------------------------------
CMyCanvasLayer_Particle::~CMyCanvasLayer_Particle( )
{
	GSystem->UngisterGlobalKeyReceiver( this );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_Particle::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_Particle::ApplyEffect()
{
	static int i=m_vecParticlePlayers.size() - 1;

	CGUIParticle2DSystem* pParticle = m_vecParticlePlayers[i]();
	m_pWidgetParticle->SetParticle2D( pParticle );

	i = (i+1)%m_vecParticlePlayers.size();
}
//------------------------------------------------------------------------------
uint32 CMyCanvasLayer_Particle::OnKeyClicked( CGUIEventKeyboard* pEvent )
{
	if( pEvent->GetKeyCode() == KC_F1 )
	{	
		ApplyEffect();

		pEvent->Consume(true);
	}

	return CGUICanvasLayer::OnKeyClicked( pEvent );
}
//------------------------------------------------------------------------------
CGUIAs* Fun_GenerateAsPlayer_CGUIAsPageTurn3D( )
{
	//CGUIAsPageTurn3D
	CGUIAsPageTurn3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsPageTurn3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	return pAs;
}

//CGUIWgtParticle2D* Fun_GenerateParticle2d_sample1( )
//{
//	//CGUIAsWaves3D
//	CGUIAsWaves3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWaves3D>();
//	pAs->SetGridSize( CGUIIntSize(30, 30) );
//	pAs->SetWavesInfo( 5, 20 );
//	return pAs;
//}

void CMyCanvasLayer_Particle::RegisterParticlePlayer()
{
	//m_vecParticlePlayers.push_back( Fun_GenerateParticle2d_sample1 );
}
//------------------------------------------------------------------------------



