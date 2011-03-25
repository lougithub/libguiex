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
		CGUISceneManager::Instance()->LoadResources( "tilemap.uip" );
		CGUISceneManager::Instance()->LoadWidgets( "tilemap.uip" );

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
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "showfps.xml", "common.uip" );
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

	m_aCamera.Restore();

	switch( m_nMoveCamera )
	{
	case 0:
		{
			m_aCamera.SetFov( 45 );

			CGUIAsInterpolation<CGUIVector3>* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs1->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs1->SetInterpolationValue( m_aCamera.GetEye()+CGUIVector3(-1000,-1000,-1000), m_aCamera.GetEye()+CGUIVector3(1000,1000,1000), 3.0f );
			m_pAsQueue->AddItem( pAs1 );
			pAs1->RefRelease();

			CGUIAsInterpolation<CGUIVector3>* pAs2 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs2->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs2->SetInterpolationValue( m_aCamera.GetEye()+CGUIVector3(1000,1000,1000), m_aCamera.GetEye()+CGUIVector3(-1000,-1000,-1000), 3.0f );
			m_pAsQueue->AddItem( pAs2 );
			pAs2->RefRelease();
		}
		break;

	case 1:
		{
			m_aCamera.SetFov( 90 );

			CGUIAsInterpolation<CGUIVector3>* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs1->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs1->SetInterpolationValue( m_aCamera.GetCenter()+CGUIVector3(-500,0,0), m_aCamera.GetCenter()+CGUIVector3(500,0,0), 3.0f );
			m_pAsQueue->AddItem( pAs1 );
			pAs1->RefRelease();

			CGUIAsInterpolation<CGUIVector3>* pAs2 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
			pAs2->SetInterpolationType( eInterpolationType_EaseInOut );
			pAs2->SetInterpolationValue( m_aCamera.GetCenter()+CGUIVector3(500,0,0), m_aCamera.GetCenter()+CGUIVector3(-500,0,0), 3.0f );
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
		m_aCamera.SetEye( m_pAsQueue->GetCurrentValue());
		break;
	case 1:
		m_aCamera.SetCenter( m_pAsQueue->GetCurrentValue());
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

	pWidget = CGUIWidgetManager::Instance()->GetPage( "sample1.xml", "tilemap.uip" );
	pWidget->SetMovable( true );
	pWidget->SetParent( this );
	pWidget->Open();

	pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
	pWidget->SetMovable( true );
	pWidget->SetParent( this );
	pWidget->Open();

	pWidget = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtButton>( "btnToggleEffect", "");
	pWidget->SetParent( this );
	pWidget->SetImage( "BTN_NORMAL", "button_blue_normal" );
	pWidget->SetImage( "BTN_HOVER", "button_blue_hover" );
	pWidget->SetImage( "BTN_PUSH", "button_blue_push" );
	pWidget->SetImage( "BTN_DISABLE", "button_blue_disable" );
	pWidget->SetPosition( 0, 0 );
	pWidget->SetTextContentUTF8("effect");
	pWidget->Create();
	pWidget->Open();
	pWidget->RegisterNativeCallbackFunc( "OnMouseLeftClick", Btn_OnClick_ToggleEffect );

	GSystem->RegisterGlobalKeyReceiver( this );
	SetAutoPlayAs( true );

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
	static int i=6;

	CGUIAsGridBase* pAs = NULL;
	switch( i )
	{
	case 0:
		//CGUIAsPageTurn3D
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsPageTurn3D>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		break;

	case 1:
		//CGUIAsWaves3D
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWaves3D>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		((CGUIAsWaves3D*)pAs)->SetWavesInfo( 5, 20 );
		break;

	case 2:
		//CGUIAsShaky3D
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShaky3D>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		((CGUIAsShaky3D*)pAs)->SetShakyInfo( 30, false );
		break;

	case 3:
		//CGUIAsShakyTiles3D
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShakyTiles3D>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		((CGUIAsShakyTiles3D*)pAs)->SetShakyTiles3DInfo( 30, false );
		break;

	case 4:
		//CGUIAsShatteredTiles3D
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShatteredTiles3D>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		((CGUIAsShatteredTiles3D*)pAs)->SetShatteredTiles3DInfo( 30, false );
		break;
		
	case 5:
		//CGUIAsShuffleTiles
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsShuffleTiles>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		((CGUIAsShuffleTiles*)pAs)->SetShuffleTilesInfo( rand() );
		break;

	case 6:
		//CGUIAsFadeOutTRTiles
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsFadeOutTRTiles>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		break;
		

	default:
		//CGUIAsPageTurn3D
		pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsPageTurn3D>();
		pAs->SetGridSize( CGUIIntSize(30, 30) );
		i = 0;
		break;
	}
	++i;

	pAs->SetTotalTime( 3.0f );
	pAs->SetReceiver( this );
	this->SetAs( "effect", pAs );
	pAs->RefRelease();
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



