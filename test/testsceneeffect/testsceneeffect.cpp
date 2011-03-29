#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_DrawWidget : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawWidget( const char* szLayerName );
	~CMyCanvasLayer_DrawWidget( );

	virtual void DestroySelf( );

	void ApplyEffect();

protected:
	virtual uint32 OnKeyClicked( CGUIEventKeyboard* pEvent );
	void RegisterAsPlayer();

protected:
	typedef CGUIAs* (*Fun_GenerateAsPlayer)( );
	std::vector< Fun_GenerateAsPlayer > m_vecAsPlayers;
};


class CMyCanvasLayer_DrawRect : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawRect( const char* szLayerName, const CGUIRect& rRect, const CGUIColor& rColor, int nCameraType );
	~CMyCanvasLayer_DrawRect();

	virtual void Update( real fDeltaTime );
	virtual void Render( class IGUIInterfaceRender* pRender );

	virtual void DestroySelf( );

protected:
	CGUIRect m_aRect;
	CGUIColor m_aColor;
	int m_nMoveCamera;

	CGUICamera m_aCamera;
	CGUIAsInterpolationQueue<CGUIVector3>* m_pAsQueue;
};



class CGUIFrameworkTest : public CGUIFramework
{	
protected:
	virtual int32 InitializeGame()
	{
		//load data
		CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
		CGUISceneManager::Instance()->LoadResources( "common.uip" );
		CGUISceneManager::Instance()->LoadWidgets( "common.uip" );
		CGUISceneManager::Instance()->LoadResources( "tiledmap.uip" );
		CGUISceneManager::Instance()->LoadWidgets( "tiledmap.uip" );

		//create layer
		CMyCanvasLayer_DrawRect* pLayer1 = new CMyCanvasLayer_DrawRect( "layer 1",CGUIRect( 400,284,600,484), CGUIColor( 0,1,1,1 ), 0 );	
		pLayer1->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer1 );

		CMyCanvasLayer_DrawRect* pLayer2 = new CMyCanvasLayer_DrawRect( "layer 2", CGUIRect( 400,284,600,484), CGUIColor( 1,1,1,1 ), 1);
		pLayer2->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer2 );

		CMyCanvasLayer_DrawWidget* pLayer3 = new CMyCanvasLayer_DrawWidget( "layer 3" );
		pLayer3->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer3 );

		//open ui page
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common.uip" );
		GSystem->GetUICanvas()->OpenUIPage( pWidget);
		return 0;
	}

	virtual void ReleaseGame( )
	{
		CGUICanvasLayerManager::Instance()->PopAndDestroyCanvasLayer( );
		CGUICanvasLayerManager::Instance()->PopAndDestroyCanvasLayer( );
	}
};

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}



//*****************************************************************************
//	CMyCanvasLayer_DrawRect
//*****************************************************************************
//------------------------------------------------------------------------------
CMyCanvasLayer_DrawRect::CMyCanvasLayer_DrawRect( const char* szLayerName, const CGUIRect& rRect, const CGUIColor& rColor, int nCameraType )
:CGUICanvasLayer( szLayerName )
,m_aRect( rRect )
,m_aColor( rColor )
,m_nMoveCamera( nCameraType )
,m_pAsQueue( NULL )
{
	m_pAsQueue = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolationQueue<CGUIVector3> >();
	m_pAsQueue->SetLooping( true );

	switch( m_nMoveCamera )
	{
	case 0:
		{
			m_aCamera.SetOffsetFov( -15 );

			CGUIAsInterpolation<CGUIVector3>* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs1->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs1->SetInterpolationValue( CGUIVector3(-1000,-1000,-1000), CGUIVector3(1000,1000,1000), 3.0f );
			m_pAsQueue->AddItem( pAs1 );
			pAs1->RefRelease();

			CGUIAsInterpolation<CGUIVector3>* pAs2 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs2->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs2->SetInterpolationValue( CGUIVector3(1000,1000,1000), CGUIVector3(-1000,-1000,-1000), 3.0f );
			m_pAsQueue->AddItem( pAs2 );
			pAs2->RefRelease();
		}
		break;

	case 1:
		{
			m_aCamera.SetOffsetFov( 30 );

			CGUIAsInterpolation<CGUIVector3>* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs1->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs1->SetInterpolationValue( CGUIVector3(-500,0,0), CGUIVector3(500,0,0), 3.0f );
			m_pAsQueue->AddItem( pAs1 );
			pAs1->RefRelease();

			CGUIAsInterpolation<CGUIVector3>* pAs2 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs2->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs2->SetInterpolationValue( CGUIVector3(500,0,0), CGUIVector3(-500,0,0), 3.0f );
			m_pAsQueue->AddItem( pAs2 );
			pAs2->RefRelease();
		}
		break;
	}

}
//------------------------------------------------------------------------------
CMyCanvasLayer_DrawRect::~CMyCanvasLayer_DrawRect()
{
	CGUIAsManager::Instance()->DeallocateResource( m_pAsQueue );
	m_pAsQueue = NULL;
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawRect::Update( real fDeltaTime )
{
	CGUICanvasLayer::Update( fDeltaTime );

	m_pAsQueue->Update( fDeltaTime );

	switch( m_nMoveCamera )
	{
	case 0:
		m_aCamera.SetOffsetEye( m_pAsQueue->GetCurrentValue());
		break;
	case 1:
		m_aCamera.SetOffsetCenter( m_pAsQueue->GetCurrentValue());
		break;
	}
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawRect::Render( class IGUIInterfaceRender* pRender )
{
	CGUICanvasLayer::Render( pRender );

	CGUICamera* pOldCamera = pRender->ApplyCamera( &m_aCamera );

	pRender->PushMatrix();

	pRender->DrawRect(
		m_aRect, 
		3,
		0,
		m_aColor,
		m_aColor,
		m_aColor,
		m_aColor );

	pRender->PopMatrix();

	pRender->ApplyCamera( pOldCamera );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawRect::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------


//*****************************************************************************
//	CMyCanvasLayer_DrawWidget
//*****************************************************************************
void Btn_OnClick_ToggleEffect( CGUIEventMouse* pEvent )
{
	((CMyCanvasLayer_DrawWidget*)(pEvent->GetReceiver()->GetParent()))->ApplyEffect();
}
//------------------------------------------------------------------------------
CMyCanvasLayer_DrawWidget::CMyCanvasLayer_DrawWidget( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
{
	CGUIWidget* pWidget = NULL;

	pWidget = CGUIWidgetManager::Instance()->GetPage( "sample1.xml", "tiledmap.uip" );
	pWidget->SetMovable( true );
	pWidget->SetParent( this );
	pWidget->Open();

	pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
	pWidget->SetMovable( true );
	pWidget->SetParent( this );
	pWidget->Open();

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
	SetAutoPlayAs( true );

	RegisterAsPlayer();
	ApplyEffect();
}

//------------------------------------------------------------------------------
CMyCanvasLayer_DrawWidget::~CMyCanvasLayer_DrawWidget( )
{
	GSystem->UngisterGlobalKeyReceiver( this );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::ApplyEffect()
{
	static int i=m_vecAsPlayers.size() - 1;

	CGUIAs* pAs = m_vecAsPlayers[i]();
	pAs->SetTotalTime( 3.0f );
	pAs->SetReceiver( this );
	this->SetAs( "effect", pAs );
	pAs->RefRelease();

	i = (i+1)%m_vecAsPlayers.size();
}
//------------------------------------------------------------------------------
uint32 CMyCanvasLayer_DrawWidget::OnKeyClicked( CGUIEventKeyboard* pEvent )
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

CGUIAs* Fun_GenerateAsPlayer_CGUIAsWaves3D( )
{
	//CGUIAsWaves3D
	CGUIAsWaves3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWaves3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetWavesInfo( 5, 20 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsShaky3D( )
{
	//CGUIAsShaky3D
	CGUIAsShaky3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShaky3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetShakyInfo( 30, false );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsShakyTiles3D( )
{
	//CGUIAsShakyTiles3D
	CGUIAsShakyTiles3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShakyTiles3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetShakyTiles3DInfo( 30, false );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsShatteredTiles3D( )
{
	//CGUIAsShatteredTiles3D
	CGUIAsShatteredTiles3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShatteredTiles3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetShatteredTiles3DInfo( 30, false );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsShuffleTiles( )
{
	//CGUIAsShuffleTiles
	CGUIAsShuffleTiles* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShuffleTiles>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetShuffleTilesInfo( rand() );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsFadeOutTRTiles( )
{
	//CGUIAsFadeOutTRTiles
	CGUIAsFadeOutTRTiles* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFadeOutTRTiles>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsFadeOutBLTiles( )
{
	//CGUIAsFadeOutBLTiles
	CGUIAsFadeOutBLTiles* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFadeOutBLTiles>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsFadeOutUpTiles( )
{
	//CGUIAsFadeOutUpTiles
	CGUIAsFadeOutUpTiles* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFadeOutUpTiles>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsFadeOutDownTiles( )
{
	//CGUIAsFadeOutDownTiles
	CGUIAsFadeOutDownTiles* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFadeOutDownTiles>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsTurnOffTiles( )
{
	//CGUIAsTurnOffTiles
	CGUIAsTurnOffTiles* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsTurnOffTiles>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetTurnOffTilesInfo( rand() );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsWavesTiles3D( )
{
	//CGUIAsWavesTiles3D
	CGUIAsWavesTiles3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWavesTiles3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetWavesInfo( 5, 20 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsJumpTiles3D( )
{
	//CGUIAsJumpTiles3D
	CGUIAsJumpTiles3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsJumpTiles3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetJumpTilesInfo( 5, 20 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsSplitRows( )
{
	//CGUIAsSplitRows
	CGUIAsSplitRows* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsSplitRows>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetSplitRowsInfo( 5 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsSplitCols( )
{
	//CGUIAsSplitCols
	CGUIAsSplitCols* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsSplitCols>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetSplitColsInfo( 5 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsFlipX3D( )
{
	//CGUIAsFlipX3D
	CGUIAsFlipX3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFlipX3D>();
	pAs->SetGridSize( CGUIIntSize(1, 1) );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsFlipY3D( )
{
	//CGUIAsFlipY3D
	CGUIAsFlipY3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFlipY3D>();
	pAs->SetGridSize( CGUIIntSize(1, 1) );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsLens3D( )
{
	//CGUIAsLens3D
	CGUIAsLens3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsLens3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetLens3DInfo( CGUIVector2(200,200), 200 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsRipple3D( )
{
	//CGUIAsRipple3D
	CGUIAsRipple3D* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsRipple3D>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetRipple3DInfo( CGUIVector2(200,200), 200, 3, 30 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsLiquid( )
{
	//CGUIAsLiquid
	CGUIAsLiquid* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsLiquid>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetLiquidInfo( 1, 30 );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsWaves( )
{
	//CGUIAsWaves
	CGUIAsWaves* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWaves>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetWavesInfo( 1, 30, true, true );
	return pAs;
}

CGUIAs* Fun_GenerateAsPlayer_CGUIAsTwirl( )
{
	//CGUIAsTwirl
	CGUIAsTwirl* pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsTwirl>();
	pAs->SetGridSize( CGUIIntSize(30, 30) );
	pAs->SetTwirlInfo( CGUIVector2(200,200), 2, 30 );
	return pAs;
}

void CMyCanvasLayer_DrawWidget::RegisterAsPlayer()
{
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsPageTurn3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsWaves3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsShaky3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsShakyTiles3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsShatteredTiles3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsShuffleTiles );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsFadeOutTRTiles );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsFadeOutBLTiles );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsFadeOutUpTiles );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsFadeOutDownTiles );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsTurnOffTiles );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsWavesTiles3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsJumpTiles3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsSplitRows );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsSplitCols );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsFlipX3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsFlipY3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsLens3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsRipple3D );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsLiquid );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsWaves );
	m_vecAsPlayers.push_back( Fun_GenerateAsPlayer_CGUIAsTwirl );

}
//------------------------------------------------------------------------------



