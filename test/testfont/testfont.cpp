#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_DrawFont : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_DrawFont( const char* szLayerName );
	~CMyCanvasLayer_DrawFont( );

	virtual void DestroySelf( );


protected:
	virtual void RenderSelf(IGUIInterfaceRender* pRender);

};



class CGUIFrameworkTest : public CGUIFramework
{	
protected:
	virtual int32 InitializeGame()
	{
		//load data
		CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
		CGUISceneManager::Instance()->LoadResources( "common" );
		CGUISceneManager::Instance()->LoadWidgets( "common" );

		//create layer
		CMyCanvasLayer_DrawFont* pLayer3 = new CMyCanvasLayer_DrawFont( "layer font" );
		pLayer3->Initialize();
		CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer3 );

		//open ui page
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
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
//	CMyCanvasLayer_DrawFont
//*****************************************************************************
//------------------------------------------------------------------------------
CMyCanvasLayer_DrawFont::CMyCanvasLayer_DrawFont( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
{
}

//------------------------------------------------------------------------------
CMyCanvasLayer_DrawFont::~CMyCanvasLayer_DrawFont( )
{
	GSystem->UngisterGlobalKeyReceiver( this );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawFont::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawFont::RenderSelf(IGUIInterfaceRender* pRender)
{
	IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
	CGUIStringRender aStringRender(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	CGUIStringRenderInfo aRenderInfo;
	aRenderInfo.m_uFontID = 0;
	aRenderInfo.m_aColor = CGUIColor( 1.0f,1.0f,1.0f,1.0f );
	CGUIVector2 aPos( 0.0f, 10.0f );

	//scale 1
	aRenderInfo.m_fFontScale = 1.0f;
	aStringRender.SetStringInfo(&aRenderInfo);
	aPos.y += pFont->GetFontHeight( aRenderInfo );
	DrawString( pRender, aStringRender, aPos );
	pRender->DrawLine( aPos, CGUIVector2( aPos.x+pFont->GetStringWidth( aStringRender), aPos.y), 1, 0, CGUIColor(0.0f,1.0f,0.0f,1.0f), CGUIColor(0.0f,1.0f,0.0f,1.0f));
	aPos.y += 10.0f;

	real fBeginScale = 0.4f;
	real fEndScale = 3.0f;
	int nGrade = 6;

	real fScalePerGrade = (fEndScale - fBeginScale) / nGrade;
	char buf[32];
	for( int i=0; i<nGrade; ++i )
	{
		aRenderInfo.m_fFontScale = fBeginScale + fScalePerGrade*i;

		snprintf( buf, 32, "%3.2f", aRenderInfo.m_fFontScale );
		CGUIStringRender strScaleString( buf );
		strScaleString.SetStringInfo( &aRenderInfo );
		aPos.x = 0.0f;
		aPos.y += pFont->GetFontHeight( aRenderInfo );
		DrawString( pRender, strScaleString, aPos );

		real fScaleWidth = pFont->GetStringWidth( strScaleString );
		aPos.x = fScaleWidth + 10.0f;

		aStringRender.SetStringInfo(&aRenderInfo);
		DrawString( pRender, aStringRender, aPos );
		pRender->DrawLine( aPos, CGUIVector2( aPos.x+pFont->GetStringWidth( aStringRender), aPos.y), 1, 0, CGUIColor(0.0f,1.0f,0.0f,1.0f), CGUIColor(0.0f,1.0f,0.0f,1.0f));
		aPos.y += 10.0f;
	}

	CGUICanvasLayer::RenderSelf( pRender );
}
//------------------------------------------------------------------------------



