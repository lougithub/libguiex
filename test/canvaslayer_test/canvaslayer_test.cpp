#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer : public CGUICanvasLayer
{
public:
	CMyCanvasLayer( const CGUIRect& rRect, const CGUIColor& rColor, int nCameraType )
	:CGUICanvasLayer( "test layer")
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
		CGUICamera* pOldCamera = NULL;
		pOldCamera = pRender->ApplyCamera( &m_aCamera );
	
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
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( new CMyCanvasLayer( CGUIRect( 400,284,600,484), CGUIColor( 0,1,1,1 ), 0 ));	
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( new CMyCanvasLayer( CGUIRect( 400,284,600,484), CGUIColor( 1,1,1,1 ), 1));
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






