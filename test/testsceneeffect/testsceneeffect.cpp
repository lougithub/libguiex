#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_DrawWidget : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawWidget( const char* szLayerName );
	~CMyCanvasLayer_DrawWidget( );

	virtual void Render( class IGUIInterfaceRender* pRender );
	virtual void RenderSelf(IGUIInterfaceRender* pRender);

	virtual void DestroySelf( );

protected:
	CGUIImage *m_pImage;
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






//------------------------------------------------------------------------------
CMyCanvasLayer_DrawWidget::CMyCanvasLayer_DrawWidget( const char* szLayerName )
	:CGUICanvasLayer( szLayerName )
	,m_pImage(NULL)
{
	//CGUIWidget* pWidget = NULL;

	//pWidget = CGUIWidgetManager::Instance()->GetPage( "sample1.xml", "tilemap.uip" );
	//pWidget->SetMovable( true );
	//pWidget->SetParent( this );
	//pWidget->Open()	;

	//pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
	//pWidget->SetMovable( true );
	//pWidget->SetParent( this );
	//pWidget->Open()	;

	m_pImage = CGUIImageManager::Instance()->AllocateResource( "scrollbar_downbutton_up" );

	CGUIAsPageTurn3D* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsPageTurn3D>();
	//pAs1->SetLooping( true );
	pAs1->SetTotalTime( 5.0f );
	pAs1->SetGridSize( CGUIIntSize(30, 30) );
	pAs1->SetReceiver( this );
	this->SetAutoPlayAs( true );
	this->SetAs( "pageturn", pAs1 );
	pAs1->RefRelease();

	//CGUIAsWaves3D* pAs1 = CGUIAsManager::Instance()->AllocateResource<CGUIAsWaves3D>();
	//pAs1->SetLooping( true );
	//pAs1->SetTotalTime( 10.0f );
	//pAs1->SetWavesInfo( 18, 15.0f);
	//pAs1->SetGridSize( CGUIIntSize(30, 30) );
	//pAs1->SetReceiver( this );
	//this->SetAutoPlayAs( true );
	//this->SetAs( "wave3d", pAs1 );
	//pAs1->RefRelease();
}

//------------------------------------------------------------------------------
CMyCanvasLayer_DrawWidget::~CMyCanvasLayer_DrawWidget( )
{
	m_pImage->RefRelease();
	m_pImage = NULL;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::RenderSelf(IGUIInterfaceRender* pRender)
{
	//DrawImage( pRender, m_pImage, CGUIRect( 0,0, 200,200 ));
	//pRender->DrawLine( CGUIVector2( 500,10), CGUIVector2(10,500), 5, 0.2f, CGUIColor(1,0,0,1), CGUIColor( 0,1,0,1));
	//pRender->DrawLine( CGUIVector2( 10,500), CGUIVector2(990,500), 5, 0.2f, CGUIColor(1,0,0,1), CGUIColor( 0,1,0,1));
	// pRender->DrawLine( CGUIVector2( 990,500), CGUIVector2(500,10), 5, 0.2f, CGUIColor(1,0,0,1), CGUIColor( 0,1,0,1));
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::Render( class IGUIInterfaceRender* pRender )
{
	CGUICanvasLayer::Render( pRender );

	//CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource("scrollbar_downbutton_up" );
	//pImage->Draw( pRender, CGUIRect( CGUIVector2(200,200), pImage->GetSize()*10 ), 0, CGUIColor(), 1.0f );
	//pImage->RefRelease();
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawWidget::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------




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
