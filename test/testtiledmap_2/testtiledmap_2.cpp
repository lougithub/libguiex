#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_TiledMapGame : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_TiledMapGame( const char* szLayerName );
	~CMyCanvasLayer_TiledMapGame(  );

	virtual void DestroySelf( );

protected:
	virtual void BeginRender( class IGUIInterfaceRender* pRender );
	virtual void EndRender( class IGUIInterfaceRender* pRender );

	virtual void RenderSelf(IGUIInterfaceRender* pRender);

	virtual uint32 OnMouseLeftClick( CGUIEventMouse* pEvent );

	void SetViewpointCenter( const CGUIVector2& rPos );
	CGUIIntVector2 TileCoordForPosition( const CGUIVector2& rPos );
	void SetPlayerPosition( const CGUIVector2& rPos );

protected:
	CGUITiledMap* m_pTiledMap;
	CGUITiledMapLayer* m_pMetaLayer;
	CGUITiledMapLayer* m_pForeGroundLayer;
	CGUIWgtStaticImage* m_pPlayer;
	CGUIWgtStaticText* m_pScore;

	//CGUICamera m_aCamera;
	//CGUICamera* m_pOldCamera;
};

class CGUIFrameworkTest : public CGUIFramework
{
public:
	CGUIFrameworkTest(  )
		:CGUIFramework(  )
	{
	}

protected:
	virtual int32 InitializeGame( );
};

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}

int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
	
	CGUISceneManager::Instance()->LoadResources( "common" );	
	CGUISceneManager::Instance()->LoadWidgets( "common" );

	CGUISceneManager::Instance()->LoadResources( "tiledmap" );	
	
	//add utility widget
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);		

	//create layer
	CMyCanvasLayer_TiledMapGame* pLayer = new CMyCanvasLayer_TiledMapGame( "tiled map layer" );
	pLayer->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( pLayer );
	return 0;
}




//------------------------------------------------------------------------------
//CMyCanvasLayer_TiledMapGame
//------------------------------------------------------------------------------
CMyCanvasLayer_TiledMapGame::CMyCanvasLayer_TiledMapGame( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
,m_pTiledMap( NULL )
,m_pMetaLayer( NULL )
,m_pForeGroundLayer( NULL )
,m_pPlayer( NULL )
,m_pScore(NULL)
//,m_pOldCamera(NULL)
{
	//set attribute
	SetHitable( true );
	SetGenerateClickEvent( true );
	SetMouseConsumed(true);

	//init tiled map
	m_pTiledMap = CGUITiledMapManager::Instance()->AllocateResource( "tiledmap_desert_2" );
	const CGUIIntSize& rTileSize = m_pTiledMap->GetMapInfo()->GetTileSize();
	const CGUIIntSize& rMapSize = m_pTiledMap->GetMapInfo()->GetMapSize();
	SetPixelSize( real(rMapSize.GetWidth() * rTileSize.GetWidth()), real(rMapSize.GetHeight() * rTileSize.GetHeight()));

	m_pMetaLayer = m_pTiledMap->GetLayer("Meta");
	GUI_ASSERT( m_pMetaLayer, "invalid tiled map data");
	m_pMetaLayer->GetLayerInfo()->SetVisible( false );
	m_pForeGroundLayer = m_pTiledMap->GetLayer("ForeGround");
	GUI_ASSERT( m_pForeGroundLayer, "invalid tiled map data");

	const CGUITiledMapObjectGroup* pObjects = m_pTiledMap->GetObjectGroup("Objects");
	GUI_ASSERT( pObjects, "invalid tiled map data");
	const CGUITiledMapObjectInfo* pObjectInfo = pObjects->GetObjectInfo( "SpawnPoint" );
	GUI_ASSERT( pObjectInfo, "invalid tiled map data");

	//init player
	m_pPlayer = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticImage>( "player", "");
	m_pPlayer->SetParent( this );
	m_pPlayer->SetImage( "bg", "player" );
	m_pPlayer->SetPixelPosition( real(pObjectInfo->GetPosition().x), real(pObjectInfo->GetPosition().y ));
	m_pPlayer->SetAnchorPoint( 0.5f, 0.5f );
	m_pPlayer->Create();

	//init score
	m_pScore = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "score", "");
	m_pScore->SetParent( GSystem->GetUICanvas() );
	m_pScore->SetSize( 50, 25 );
	m_pScore->SetPosition( 0,0 );
	m_pScore->SetTextInfo(CGUIStringRenderInfo(0, 16, CGUIColor(1,1,1,1)));
	m_pScore->SetTextAlignmentHorz( eTextAlignment_Horz_Left );
	m_pScore->SetTextContent(L"0");
	m_pScore->Create();
	m_pScore->Open();

	SetViewpointCenter( m_pPlayer->GetPixelPosition() );
}

//------------------------------------------------------------------------------
CMyCanvasLayer_TiledMapGame::~CMyCanvasLayer_TiledMapGame(  )
{
	m_pTiledMap->RefRelease();
	m_pTiledMap = NULL;
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_TiledMapGame::BeginRender( class IGUIInterfaceRender* pRender )
{
	//m_pOldCamera = pRender->ApplyCamera( &m_aCamera );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TiledMapGame::EndRender( class IGUIInterfaceRender* pRender )
{
	//pRender->ApplyCamera( m_pOldCamera );
	//m_pOldCamera = NULL;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TiledMapGame::RenderSelf(IGUIInterfaceRender* pRender)
{
	m_pTiledMap->Render( pRender );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TiledMapGame::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
uint32 CMyCanvasLayer_TiledMapGame::OnMouseLeftClick( CGUIEventMouse* pEvent )
{
	CGUIVector2 touchLocation = pEvent->GetLocalPosition();		

	CGUIVector2 playerPos = m_pPlayer->GetPixelPosition();
	CGUIVector2 diff = touchLocation - playerPos;
	if (abs(diff.x) > abs(diff.y)) 
	{
		if (diff.x > 0) 
		{
			playerPos.x += m_pTiledMap->GetMapInfo()->GetTileSize().GetWidth();
		}
		else 
		{
			playerPos.x -= m_pTiledMap->GetMapInfo()->GetTileSize().GetWidth(); 
		}    
	} 
	else 
	{
		if (diff.y > 0) 
		{
			playerPos.y += m_pTiledMap->GetMapInfo()->GetTileSize().GetHeight();
		}
		else
		{
			playerPos.y -= m_pTiledMap->GetMapInfo()->GetTileSize().GetHeight();
		}
	}

	if (playerPos.x <= GetPixelSize().GetWidth() &&
		playerPos.y <= GetPixelSize().GetHeight() &&
		playerPos.y >= 0 &&
		playerPos.x >= 0 ) 
	{
		SetPlayerPosition( playerPos );
	}


	return CGUIWidget::OnMouseLeftClick(pEvent);
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TiledMapGame::SetPlayerPosition( const CGUIVector2& rPos )
{
	CGUIIntVector2 tileCoord = TileCoordForPosition(rPos);
	uint32 tileGid = m_pMetaLayer->GetTileGID( tileCoord.x, tileCoord.y );
	if (tileGid) 
	{
		const std::map<CGUIString, CGUIString>* pProperties = m_pTiledMap->GetTileProperties( tileGid );
		if( pProperties )
		{
			//check collidable
			std::map<CGUIString, CGUIString>::const_iterator itorCollide = pProperties->find( "Collidable" );
			if( itorCollide != pProperties->end() && itorCollide->second == "True" )
			{
				return;
			}

			//check collectable
			std::map<CGUIString, CGUIString>::const_iterator itorCollect = pProperties->find( "Collectable" );
			if( itorCollect != pProperties->end() && itorCollect->second == "True" )
			{
				//eat one
				//update map
				m_pMetaLayer->RemoveTileAt( tileCoord.x, tileCoord.y );
				m_pForeGroundLayer->RemoveTileAt( tileCoord.x, tileCoord.y );
				//update score
				CGUIString strScroe = m_pScore->GetTextContentUTF8();
				uint32 uScore;
				StringToValue( strScroe, uScore );
				++uScore;
				ValueToString( uScore, strScroe );
				m_pScore->SetTextContentUTF8( strScroe );
			}
		}
	}
	m_pPlayer->SetPixelPosition( rPos );
	m_pPlayer->Refresh();
	SetViewpointCenter( m_pPlayer->GetPixelPosition() );
}
//------------------------------------------------------------------------------
CGUIIntVector2 CMyCanvasLayer_TiledMapGame::TileCoordForPosition( const CGUIVector2& rPos )
{
	const CGUIIntSize& rTileSize = m_pTiledMap->GetMapInfo()->GetTileSize();
	const CGUIIntSize& rMapSize = m_pTiledMap->GetMapInfo()->GetMapSize();
	uint32 x = uint32( rPos.x / rTileSize.GetWidth());
	uint32 y = uint32( rPos.y / rTileSize.GetHeight());
	return CGUIIntVector2(x, y);
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TiledMapGame::SetViewpointCenter( const CGUIVector2& rPos )
{
	const CGUIIntSize& rWinSize = GSystem->GetScreenSize();

	real x = GUIMax( rPos.x, rWinSize.GetWidth() / 2.0f );
	real y = GUIMax( rPos.y, rWinSize.GetHeight() / 2.0f );
	x = GUIMin( x, GetPixelSize().GetWidth() - rWinSize.GetWidth() / 2 );
	y = GUIMin( y, GetPixelSize().GetHeight() - rWinSize.GetHeight()/2 );
	
	CGUIVector2 actualPosition( x, y );
	CGUIVector2 centerOfView( rWinSize.GetWidth()/2.0f, rWinSize.GetHeight()/2.0f );
	CGUIVector2 viewPoint = centerOfView - actualPosition;

	SetPixelPosition( viewPoint );
	Refresh();

	//m_aCamera.SetOffsetCenter( CGUIVector3( viewPoint.x, viewPoint.y, m_aCamera.GetOffsetCenter().z ));
	//m_aCamera.SetOffsetEye( CGUIVector3( viewPoint.x, viewPoint.y, m_aCamera.GetOffsetEye().z ));
}
//------------------------------------------------------------------------------
