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

		CGUIInterfaceManager::Instance()->GetInterfaceRender()->SetClearColor( 0.9,0.9,.9,1 );
		return 0;
	}

	virtual void ReleaseGame( )
	{
		CGUICanvasLayerManager::Instance()->PopAndDestroyCanvasLayer( );
		CGUICanvasLayerManager::Instance()->PopAndDestroyCanvasLayer( );
	}
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


//*****************************************************************************
//	CMyCanvasLayer_DrawFont
//*****************************************************************************
//------------------------------------------------------------------------------
CMyCanvasLayer_DrawFont::CMyCanvasLayer_DrawFont( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
{
	CGUIStringRenderInfo aRenderInfo;

	CGUIWgtStaticText* pWidget_statictext1 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "label_1", "");
	pWidget_statictext1->SetParent( this );
	pWidget_statictext1->SetSize( 250, 25 );
	pWidget_statictext1->SetPosition( 100, 300 );
	pWidget_statictext1->SetTextContent((wchar*)(L"我爱 hello libguiex"));
	aRenderInfo.m_uFontID = 0;
	aRenderInfo.m_aColor.SetColor(0,0,0);
	pWidget_statictext1->SetTextInfo(aRenderInfo);
	pWidget_statictext1->Create();

	CGUIWgtStaticText* pWidget_statictext2 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "label_2", "");
	pWidget_statictext2->SetParent( this );
	pWidget_statictext2->SetSize( 250, 25 );
	pWidget_statictext2->SetPosition( 100, 350 );
	pWidget_statictext2->SetTextContent((wchar*)(L"我爱 hello libguiex"));
	aRenderInfo.m_uFontID = 0;
	//aRenderInfo.m_aColor.SetColor(1,1,1);
	pWidget_statictext2->SetTextInfo(aRenderInfo);
	pWidget_statictext2->Create();

	CGUIWgtStaticText* pWidget_statictext3 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "label_3", "");
	pWidget_statictext3->SetParent( this );
	pWidget_statictext3->SetSize( 250, 25 );
	pWidget_statictext3->SetPosition( 100, 400 );
	pWidget_statictext3->SetTextContentUTF8("hello libguiex");
	aRenderInfo.m_uFontID = 2;
	pWidget_statictext3->SetTextInfo(aRenderInfo);
	pWidget_statictext3->Create();

	CGUIWgtStaticText* pWidget_statictext4 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "label_4", "");
	pWidget_statictext4->SetParent( this );
	pWidget_statictext4->SetSize( 300, 250 );
	pWidget_statictext4->SetPosition( 100, 500 );
	pWidget_statictext4->SetTextContent((wchar*)(L"我爱北京天安门!11月.21号早: 昨天玩了一整晚小米,总的来说对小米还是非常满意的, 现在对小米的屏幕感觉好多了, 可能小米的屏幕比较柔和, 对比度不是很高, 所以看起来感觉没摩托的屏那么清爽, 小米屏显白色时总觉得有点发黄, 显黑色的部分又觉得有点蓝"));
	aRenderInfo.m_uFontID = 0;
	//aRenderInfo.m_aColor.SetColor(0,0,0);
	pWidget_statictext4->SetTextInfo(aRenderInfo);
	pWidget_statictext4->Create();
}

//------------------------------------------------------------------------------
CMyCanvasLayer_DrawFont::~CMyCanvasLayer_DrawFont( )
{
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawFont::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_DrawFont::RenderSelf(IGUIInterfaceRender* pRender)
{
	CGUIVector2 aPos( 0.0f, 10.0f );
	IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
	CGUIStringRender aStringRender("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	aStringRender.m_aStringInfo.m_uFontID = 0;
	aStringRender.m_aStringInfo.m_aColor = CGUIColor( 1.0f,1.0f,1.0f,1.0f );
	aStringRender.m_aStringInfo.m_fFontScale = 1.0f;

	const std::map<CGUIString, CGUIFontData*>& rMapFontList = CGUIFontManager::Instance()->GetRegisterResourceMap();
	for( std::map<CGUIString,CGUIFontData*>::const_iterator itor = rMapFontList.begin();
		itor != rMapFontList.end();
		++itor)
	{
		aStringRender.m_aStringInfo.m_uFontID = itor->second->GetFontID();
		aPos.y += pFont->GetFontHeight( aStringRender.m_aStringInfo );
		DrawString( pRender, aStringRender, aPos );
		//pRender->DrawLine( aPos, CGUIVector2( aPos.x+pFont->GetStringWidth( aStringRender), aPos.y), 1, 0, CGUIColor::Green, CGUIColor::Green);

		CGUIRect aRect( aPos.x, aPos.y - pFont->GetFontHeight( aStringRender.m_aStringInfo ), aPos.x+pFont->GetStringWidth( aStringRender), aPos.y );
		pRender->DrawRect( aRect, 1,false, 0, CGUIColor::Green );
		
		aPos.y += pFont->GetFontHeight( aStringRender.m_aStringInfo );
	}

	CGUICanvasLayer::RenderSelf( pRender );
}
//------------------------------------------------------------------------------



