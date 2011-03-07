#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_DrawWidget : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawWidget( const char* szLayerName )
		:CGUICanvasLayer( szLayerName )
	{
		m_aCamera.Restore();
		m_aCamera.SetFov( 45 );

		CGUIWidget* pWidget = NULL;

		pWidget = CGUIWidgetManager::Instance()->GetPage( "sample1.xml", "tilemap.uip" );
		pWidget->SetMovable( true );
		m_vecWidgets.push_back( pWidget );
		pWidget->Open()	;

		pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
		pWidget->SetMovable( true );
		m_vecWidgets.push_back( pWidget );
		pWidget->Open()	;
	}

	~CMyCanvasLayer_DrawWidget( )
	{
		for( uint32 i=0; i<m_vecWidgets.size(); ++i )
		{
			m_vecWidgets[i]->Close();
		}
		m_vecWidgets.clear();		
	}

	//------------------------------------------------------------------------------
	CGUIWidget* GetWidgetUnderPoint( const CGUIVector2& rPos )
	{
		//capture input
		for( std::vector<CGUIWidget*>::reverse_iterator itor = m_vecWidgets.rbegin();
			itor != m_vecWidgets.rend();
			++itor )
		{
			CGUIWidget* pWidget = (*itor)->GetWidgetAtPoint(rPos);
			if( pWidget )
			{
				return pWidget;
			}
		}
		return NULL;
	}

	virtual void Update( real fDeltaTime )
	{
		CGUICanvasLayer::Update( fDeltaTime );

		for( std::vector<CGUIWidget*>::reverse_iterator itor = m_vecWidgets.rbegin();
			itor != m_vecWidgets.rend();
			++itor )
		{
			(*itor)->Update( fDeltaTime );
		}
	}

	virtual void Render( class IGUIInterfaceRender* pRender )
	{
		CGUICanvasLayer::Render( pRender );

		CGUICamera* pOldCamera = pRender->ApplyCamera( &m_aCamera );

		for( std::vector<CGUIWidget*>::iterator itor = m_vecWidgets.begin();
			itor != m_vecWidgets.end();
			++itor )
		{
			(*itor)->Render( pRender );
		}

		pRender->ApplyCamera( pOldCamera );
	}
	
	virtual void DestroySelf( )
	{
		delete this;
	}

protected:
	std::vector<CGUIWidget*> m_vecWidgets;
	CGUICamera m_aCamera;
};


class CMyCanvasLayer_DrawRect : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawRect( const char* szLayerName, const CGUIRect& rRect, const CGUIColor& rColor, int nCameraType )
	:CGUICanvasLayer( szLayerName )
	,m_aRect( rRect )
	,m_aColor( rColor )
	,m_nMoveCamera( nCameraType )
	{
		m_aCamera.Restore();
	
		switch( m_nMoveCamera )
		{
		case 0:
			m_aCamera.SetFov( 45 );
			m_aAsLiteQueue.AddItem( CGUIAsLite_Interpolation<CGUIVector3>( 
				m_aCamera.GetEye()+CGUIVector3(-1000,-1000,-1000), 
				m_aCamera.GetEye()+CGUIVector3(1000,1000,1000), 
				3,
				eInterpolationType_EaseInOut));

			m_aAsLiteQueue.AddItem( CGUIAsLite_Interpolation<CGUIVector3>(
				m_aCamera.GetEye()+CGUIVector3(1000,1000,1000),
				m_aCamera.GetEye()+CGUIVector3(-1000,-1000,-1000),
				3, 
				eInterpolationType_EaseInOut));
			break;
		
		case 1:
			m_aCamera.SetFov( 90 );
			m_aAsLiteQueue.AddItem( CGUIAsLite_Interpolation<CGUIVector3>( 
				m_aCamera.GetCenter()+CGUIVector3(-500,0,0), 
				m_aCamera.GetCenter()+CGUIVector3(500,0,0), 
				5,
				eInterpolationType_EaseInOut));

			m_aAsLiteQueue.AddItem( CGUIAsLite_Interpolation<CGUIVector3>(
				m_aCamera.GetCenter()+CGUIVector3(500,0,0), 
				m_aCamera.GetCenter()+CGUIVector3(-500,0,0), 
				5, 
				eInterpolationType_EaseInOut));
			break;
		}

		m_aAsLiteQueue.SetLooping( true );
	}

	virtual void Update( real fDeltaTime )
	{
		CGUICanvasLayer::Update( fDeltaTime );

		m_aAsLiteQueue.Update( fDeltaTime );

		switch( m_nMoveCamera )
		{
		case 0:
			m_aCamera.SetEye( m_aAsLiteQueue.GetCurrentValue());
			break;
		case 1:
			m_aCamera.SetCenter( m_aAsLiteQueue.GetCurrentValue());
			break;
		}
	}

	virtual void Render( class IGUIInterfaceRender* pRender )
	{
		CGUICanvasLayer::Render( pRender );

		CGUICamera* pOldCamera = pRender->ApplyCamera( &m_aCamera );
	
		pRender->DrawRect(CGUIMatrix4::IDENTITY,
						  m_aRect, 
						  3,
						  pRender->GetAndIncZ(),
						  m_aColor,
						  m_aColor,
						  m_aColor,
						  m_aColor );
		
		pRender->ApplyCamera( pOldCamera );
	}
	
	virtual void DestroySelf( )
	{
		delete this;
	}
	
protected:
	CGUIRect m_aRect;
	CGUIColor m_aColor;
	int m_nMoveCamera;

	CGUICamera m_aCamera;
	CGUIAsLite_Queue<CGUIVector3> m_aAsLiteQueue; 
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






