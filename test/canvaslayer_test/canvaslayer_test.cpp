#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer : public CGUICanvasLayer
{
public:
	CMyCanvasLayer( const CGUIRect& rRect, const CGUIColor& rColor )
	:CGUICanvasLayer( "test layer")
	,m_aRect( rRect )
	,m_aColor( rColor )
	{
	}
	
	virtual void Render( class IGUIInterfaceRender* pRender )
	{
		pRender->DrawRect(CGUIMatrix4::IDENTITY,
						  m_aRect, 
						  3,
						  pRender->GetAndIncZ(),
						  m_aColor.GetARGB(),
						  m_aColor.GetARGB(),
						  m_aColor.GetARGB(),
						  m_aColor.GetARGB() );
		
	}
	
	virtual void DestroySelf( )
	{
		delete this;
	}
	
protected:
	CGUIRect m_aRect;
	CGUIColor m_aColor;
};

class CGUIFrameworkTest : public CGUIFramework
{	
protected:
	virtual int32 InitializeGame()
	{
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( new CMyCanvasLayer( CGUIRect( 10,10,100,100), CGUIColor( 1,1,1,1 )));
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( new CMyCanvasLayer( CGUIRect( 100,100,200,200), CGUIColor( 0,1,1,1 )));	
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






