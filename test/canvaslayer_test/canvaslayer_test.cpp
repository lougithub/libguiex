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
		m_aAsLiteQueue.AddItem( CGUIAsLite_Interpolation<CGUIVector3>( 
			m_aCamera.GetEye()+CGUIVector3(1000,1000,1000), 
			m_aCamera.GetEye()+CGUIVector3(-1000,-1000,-1000), 
			3,
			eInterpolationType_EaseInOut));
		m_aAsLiteQueue.AddItem( CGUIAsLite_Interpolation<CGUIVector3>(
			m_aCamera.GetEye()+CGUIVector3(-1000,-1000,-1000),
			m_aCamera.GetEye()+CGUIVector3(1000,1000,1000),
			3, 
			eInterpolationType_EaseInOut));
		m_aAsLiteQueue.SetLooping( true );

		CGUIWidget* pWidget = NULL;

		pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_ok.xml", "common.uip" );
		m_vecWidgets.push_back( pWidget );
		pWidget->Open()	;

		pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
		m_vecWidgets.push_back( pWidget );
		pWidget->Open()	;
	}

	virtual void Update( real fDeltaTime )
	{
		for( std::vector<CGUIWidget*>::iterator itor = m_vecWidgets.begin();
			itor != m_vecWidgets.end();
			++itor )
		{
			(*itor)->Update( fDeltaTime );
		}

		m_aAsLiteQueue.Update( fDeltaTime );
		m_aCamera.SetEye( m_aAsLiteQueue.GetCurrentValue());
	}

	virtual void Render( class IGUIInterfaceRender* pRender )
	{
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
	CGUIAsLite_Queue<CGUIVector3> m_aAsLiteQueue; 
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
		CGUICamera* pOldCamera = pRender->ApplyCamera( &m_aCamera );
	
		pRender->DrawRect(CGUIMatrix4::IDENTITY,
						  m_aRect, 
						  3,
						  pRender->GetAndIncZ(),
						  m_aColor.GetARGB(),
						  m_aColor.GetARGB(),
						  m_aColor.GetARGB(),
						  m_aColor.GetARGB() );
		
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

		//create layer
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( 
			new CMyCanvasLayer_DrawRect( "layer 1",CGUIRect( 400,284,600,484), CGUIColor( 0,1,1,1 ), 0 ));	
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( 
			new CMyCanvasLayer_DrawRect( "layer 2", CGUIRect( 400,284,600,484), CGUIColor( 1,1,1,1 ), 1));
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( 
			new CMyCanvasLayer_DrawWidget( "layer 3" ));

		//open ui page
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "showfps.xml", "common.uip" );
		GSystem->OpenUIPage( pWidget);
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






