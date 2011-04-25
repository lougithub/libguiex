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
	std::vector< CGUIString > m_vecParticlePlayers;

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
	CGUISceneManager::Instance()->LoadResources( "particle2d" );	
	CGUISceneManager::Instance()->LoadWidgets( "particle2d" );

	CMyCanvasLayer_Particle* pLayer3 = new CMyCanvasLayer_Particle( "layer 3" );
	pLayer3->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer3 );

	CGUISceneManager::Instance()->LoadResources( "common" );	
	CGUISceneManager::Instance()->LoadWidgets( "common" );
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
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
	m_pWidgetParticle = (CGUIWgtParticle2D*)CGUIWidgetManager::Instance()->GetPage( "particle2d.xml", "particle2d" );
	m_pWidgetParticle->SetParent( this );
	m_pWidgetParticle->Open();

	CGUIWgtButton* pButtonWidget = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtButton>( "btnToggleEffect", "");
	pButtonWidget->SetParent( this );
	pButtonWidget->SetImage( "normal", "button_blue_normal" );
	pButtonWidget->SetImage( "hover", "button_blue_hover" );
	pButtonWidget->SetImage( "push", "button_blue_push" );
	pButtonWidget->SetImage( "disable", "button_blue_disable" );
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

	m_pWidgetParticle->SetParticle2D( m_vecParticlePlayers[i] );

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
void CMyCanvasLayer_Particle::RegisterParticlePlayer()
{
	m_vecParticlePlayers.push_back( "DemoRain" );
	m_vecParticlePlayers.push_back( "DemoGalaxy" );
	m_vecParticlePlayers.push_back( "DemoSpiral" );
	m_vecParticlePlayers.push_back( "DemoFlower" );
	m_vecParticlePlayers.push_back( "DemoFirework" );
	m_vecParticlePlayers.push_back( "DemoSun" );
	m_vecParticlePlayers.push_back( "DemoSnow" );
	m_vecParticlePlayers.push_back( "DemoExplosion" );
}
//------------------------------------------------------------------------------



