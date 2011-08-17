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

class CMyCanvasLayer_MouseParticle : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_MouseParticle( const char* szLayerName );
	~CMyCanvasLayer_MouseParticle( );

	virtual void DestroySelf( );
	virtual void Render( class IGUIInterfaceRender* pRender );
	virtual void Update( real fDeltaTime );

	void OpenParticle();
	void CloseParticle();
	void MoveParticle( const CGUIVector2& rMousePos );

protected:
	CGUIParticle2DSystem* m_pParticle2DSystem;
	//CGUIMatrix4 m_aOffsetMatrix;

};

class CMyMouseListener : public CGUIMouseListener
{
public:
	CMyMouseListener( CMyCanvasLayer_MouseParticle* pLayer )
		:m_pLayer( pLayer )
	{
	}
	virtual ~CMyMouseListener()
	{
	}

	virtual void OnMouseButtonDown( const IGUIInterfaceMouse::SMouseEvent& rMouseEvent )
	{
		m_pLayer->OpenParticle();
	}
	virtual void OnMouseButtonUp( const IGUIInterfaceMouse::SMouseEvent& rMouseEvent )
	{
		m_pLayer->CloseParticle();
	}
	virtual void OnMouseMove(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent)
	{
		m_pLayer->MoveParticle( rMouseEvent.m_aMousePos );
	}

protected:
	CMyCanvasLayer_MouseParticle* m_pLayer;
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

CGUIFrameworkBase* GUIEXCreateFramework( )
{
	return new CGUIFrameworkTest( );
}

const char* GUIEXGetDataDir()
{
	//should relative to dir "data"
	return "test";
}

int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
	CGUISceneManager::Instance()->LoadResources( "particle2d" );	
	CGUISceneManager::Instance()->LoadWidgets( "particle2d" );

	CMyCanvasLayer_Particle* pLayer3 = new CMyCanvasLayer_Particle( "layer 3" );
	pLayer3->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer3 );

	CMyCanvasLayer_MouseParticle* pLayerMouseEffect = new CMyCanvasLayer_MouseParticle( "layer MouseEffect" );
	pLayerMouseEffect->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayerMouseEffect );

	CMyMouseListener* pListener = new CMyMouseListener( pLayerMouseEffect );
	GSystem->AddMouseListener( pListener );

	CGUISceneManager::Instance()->LoadResources( "common" );	
	CGUISceneManager::Instance()->LoadWidgets( "common" );
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);	
	return 0;
}


//*****************************************************************************
//	CMyCanvasLayer_Particle
//*****************************************************************************
CMyCanvasLayer_MouseParticle::CMyCanvasLayer_MouseParticle( const char* szLayerName )
:CGUICanvasLayer( szLayerName, true )
{
	m_pParticle2DSystem = CGUIParticle2DManager::Instance()->AllocateResource( "DemoFirework" );
	m_pParticle2DSystem->StopSystem();
	//m_aOffsetMatrix = CGUIMatrix4::IDENTITY;
}
//------------------------------------------------------------------------------
CMyCanvasLayer_MouseParticle::~CMyCanvasLayer_MouseParticle( )
{
	m_pParticle2DSystem->RefRelease();
	m_pParticle2DSystem = NULL;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MouseParticle::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MouseParticle::OpenParticle()
{
	m_pParticle2DSystem->StartSystem( false);
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MouseParticle::CloseParticle()
{
	m_pParticle2DSystem->StopSystem();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MouseParticle::MoveParticle( const CGUIVector2& rMousePos )
{
	//m_aOffsetMatrix.setTrans( CGUIVector3( rMousePos.x, rMousePos.y, 0.0f ));
	m_pParticle2DSystem->SetRelativePosition( rMousePos );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MouseParticle::Render( class IGUIInterfaceRender* pRender )
{
	CGUICanvasLayer::Render( pRender );

	//pRender->PushMatrix();
	//pRender->MultMatrix( m_aOffsetMatrix );
	m_pParticle2DSystem->Render( pRender );
	//pRender->PopMatrix();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MouseParticle::Update( real fDeltaTime )
{
	CGUICanvasLayer::Update( fDeltaTime );
	m_pParticle2DSystem->Update( fDeltaTime );
}
//------------------------------------------------------------------------------

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
	static int i=0;

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
	m_vecParticlePlayers.push_back( "DemoGalaxy" );
	m_vecParticlePlayers.push_back( "DemoRain" );
	m_vecParticlePlayers.push_back( "DemoSpiral" );
	m_vecParticlePlayers.push_back( "DemoFlower" );
	m_vecParticlePlayers.push_back( "DemoFirework" );
	m_vecParticlePlayers.push_back( "DemoSun" );
	m_vecParticlePlayers.push_back( "DemoSnow" );
	m_vecParticlePlayers.push_back( "DemoExplosion" );
}
//------------------------------------------------------------------------------



