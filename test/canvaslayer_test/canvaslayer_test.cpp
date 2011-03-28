#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_DrawWidget : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawWidget( const char* szLayerName );
	~CMyCanvasLayer_DrawWidget(  );

	virtual void Update( real fDeltaTime );
	virtual void Render( class IGUIInterfaceRender* pRender );
	virtual void DestroySelf( );

protected:
	CGUICamera m_aCamera;
	CGUIAsInterpolationQueue<CGUIVector3>* m_pAsQueue;
};


class CMyCanvasLayer_DrawRect : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawRect( const char* szLayerName, const CGUIRect& rRect, const CGUIColor& rColor, int nCameraType );
	~CMyCanvasLayer_DrawRect( );

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
		CMyCanvasLayer_DrawWidget* pLayer3 = new CMyCanvasLayer_DrawWidget( "layer 3" );
		pLayer3->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer3 );

		CMyCanvasLayer_DrawRect* pLayer1 = new CMyCanvasLayer_DrawRect( "layer 1",CGUIRect( 400,284,600,484), CGUIColor( 0,1,1,1 ), 0 );
		pLayer1->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer1 );
		
		CMyCanvasLayer_DrawRect* pLayer2 = new CMyCanvasLayer_DrawRect( "layer 2", CGUIRect( 400,284,600,484), CGUIColor( 1,1,1,1 ), 1);
		pLayer2->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer2 );

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








//CMyCanvasLayer_DrawWidget
//------------------------------------------------------------------------------
CMyCanvasLayer_DrawWidget::CMyCanvasLayer_DrawWidget( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
,m_pAsQueue( NULL )
{
	m_pAsQueue = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolationQueue<CGUIVector3> >();

	m_aCamera.SetOffsetFov( -15.0f );

	CGUIAsInterpolation<CGUIVector3>* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
	pAs1->SetInterpolationType( eInterpolationType_EaseInOut );
	pAs1->SetInterpolationValue( CGUIVector3(1000,1000,1000), CGUIVector3(-1000,-1000,-1000), 3.0f );
	m_pAsQueue->AddItem( pAs1 );
	pAs1->RefRelease();

	CGUIAsInterpolation<CGUIVector3>* pAs2 = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<CGUIVector3> >();
	pAs2->SetInterpolationType( eInterpolationType_EaseInOut );
	pAs2->SetInterpolationValue( CGUIVector3(-1000,-1000,-1000), CGUIVector3(1000,1000,1000), 3.0f );
	m_pAsQueue->AddItem( pAs2 );
	pAs2->RefRelease();

	m_pAsQueue->SetLooping( true );

	CGUIWidget* pWidget = NULL;
	pWidget = CGUIWidgetManager::Instance()->GetPage( "sample1.xml", "tiledmap.uip" );
	pWidget->SetParent( this );
	pWidget->Open()	;
	pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
	pWidget->SetParent( this );
	pWidget->Open()	;
}

//------------------------------------------------------------------------------
CMyCanvasLayer_DrawWidget::~CMyCanvasLayer_DrawWidget(  )
{
	CGUIAsManager::Instance()->DeallocateResource( m_pAsQueue );
	m_pAsQueue = NULL;
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::Update( real fDeltaTime )
{
	CGUICanvasLayer::Update( fDeltaTime );

	m_pAsQueue->Update( fDeltaTime );
	m_aCamera.SetOffsetEye( m_pAsQueue->GetCurrentValue());
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::Render( class IGUIInterfaceRender* pRender )
{
	CGUICamera* pOldCamera = pRender->ApplyCamera( &m_aCamera );

	CGUICanvasLayer::Render( pRender );

	pRender->ApplyCamera( pOldCamera );
}
//------------------------------------------------------------------------------

void CMyCanvasLayer_DrawWidget::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------


//CMyCanvasLayer_DrawRect
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

CMyCanvasLayer_DrawRect::~CMyCanvasLayer_DrawRect( )
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
	pRender->MatrixMode( eMatrixMode_MODELVIEW );
	pRender->LoadIdentityMatrix();

	pRender->DrawRect(
		m_aRect, 
		3,
		0,
		m_aColor,
		m_aColor,
		m_aColor,
		m_aColor );

	pRender->ApplyCamera( pOldCamera );
	pRender->PopMatrix();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawRect::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
