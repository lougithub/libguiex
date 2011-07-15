#include <libguiex_framework/guiframework.h>
#include <libguiex_core/pathfinderastar.h>
using namespace guiex;




//*****************************************************************************
//	CMyPlayer
//*****************************************************************************
class CMyPlayer : public CGUIWgtStaticImage
{
public:
	CMyPlayer( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtStaticImage( StaticGetType(), rName, rSceneName )
		,m_pMoveAs(NULL)
		,m_pLayer(NULL)
	{
	}

	void InitPlayer( class CMyCanvasLayer_TiledMapGame* pParent, const CGUIIntSize& rTileSize, const CGUIIntVector2& rPlayerPixelPos );

	void SetMovePath( const std::vector<CGUIUIntVector2>& rPaths )
	{
		m_vMovePaths = rPaths;

		UpdatePath();
	}
	CGUIIntVector2 GetCurrentCoord() const
	{
		uint32 x = uint32( GetPixelPosition().x / m_aTileSize.GetWidth());
		uint32 y = uint32( GetPixelPosition().y / m_aTileSize.GetHeight());
		return CGUIIntVector2(x, y);
	}

protected:
	virtual void UpdateSelf( real fDeltaTime );

	void UpdatePath();
	static void FunCallback_PlayerMoveto(CGUIAs* pAs)
	{
		((CMyPlayer*)pAs->GetReceiver())->UpdatePath();
	}
	
protected:
	std::vector<CGUIUIntVector2> m_vMovePaths;
	CGUIIntSize m_aTileSize;
	CGUIIntVector2 m_aPixelOffset;
	CGUIAsWidgetMoveTo* m_pMoveAs;
	class CMyCanvasLayer_TiledMapGame* m_pLayer;

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyPlayer );
};
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyPlayer );




class CMyCanvasLayer_TiledMapGame : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_TiledMapGame( const char* szLayerName );
	~CMyCanvasLayer_TiledMapGame(  );

	virtual void DestroySelf( );
	void SetViewpointCenter( const CGUIVector2& rPos );

protected:
	virtual void BeginRender( class IGUIInterfaceRender* pRender );
	virtual void EndRender( class IGUIInterfaceRender* pRender );

	virtual void RenderSelf(IGUIInterfaceRender* pRender);

	virtual uint32 OnMouseLeftClick( CGUIEventMouse* pEvent );

	CGUIIntVector2 TileCoordForPosition( const CGUIVector2& rPos );

protected:
	friend class CMyPlayer;

	CGUITiledMap* m_pTiledMap;
	CGUITiledMapLayer* m_pMetaLayer;
	CGUITiledMapLayer* m_pForeGroundLayer;
	CMyPlayer* m_pPlayer;
	CGUIWgtStaticText* m_pScore;

	CPathFinder_AStar* m_pPathFinder;
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
,m_pScore( NULL)
,m_pPathFinder( NULL )
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

	//init pathfinder
	m_pPathFinder = new CPathFinder_AStar( rMapSize.GetWidth(), rMapSize.GetHeight() );
	for( uint32 x=0; x<rMapSize.GetWidth(); ++x )
	{
		for( uint32 y=0; y<rMapSize.GetHeight(); ++y )
		{
			uint32 tileGid = m_pMetaLayer->GetTileGID( x, y );
			if (tileGid) 
			{
				const std::map<CGUIString, CGUIString>* pProperties = m_pTiledMap->GetTileProperties( tileGid );
				if( pProperties )
				{
					//check collidable
					std::map<CGUIString, CGUIString>::const_iterator itorCollide = pProperties->find( "Collidable" );
					if( itorCollide != pProperties->end() && itorCollide->second == "True" )
					{
						m_pPathFinder->SetMapWalkable(x,y,false);
					}
				}
			}
		}
	}


	//init player
	m_pPlayer = CGUIWidgetManager::Instance()->CreateCustomWidget<CMyPlayer>( "player", "", false);
	m_pPlayer->InitPlayer( this, rTileSize, pObjectInfo->GetPosition() );

	//init score
	m_pScore = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "score", "");
	m_pScore->SetParent( GSystem->GetUICanvas() );
	m_pScore->SetSize( 50, 25 );
	m_pScore->SetPosition( 0,0 );
	m_pScore->SetTextInfo(CGUIStringRenderInfo(2, 1, CGUIColor(1,1,1,1)));
	m_pScore->SetTextAlignmentHorz( eTextAlignment_Horz_Left );
	m_pScore->SetTextContentUTF8("0");
	m_pScore->Create();
	m_pScore->Open();

	SetViewpointCenter( m_pPlayer->GetPixelPosition() );
}
//------------------------------------------------------------------------------
CMyCanvasLayer_TiledMapGame::~CMyCanvasLayer_TiledMapGame(  )
{
	if( m_pTiledMap )
	{
		m_pTiledMap->RefRelease();
		m_pTiledMap = NULL;
	}

	if( m_pPathFinder )
	{
		delete m_pPathFinder;
		m_pPathFinder = NULL;
	}
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
	if (touchLocation.x <= GetPixelSize().GetWidth() &&
		touchLocation.y <= GetPixelSize().GetHeight() &&
		touchLocation.y >= 0 &&
		touchLocation.x >= 0 ) 
	{
		CGUIIntVector2 targetCoord = TileCoordForPosition(touchLocation);
		uint32 tileGid = m_pMetaLayer->GetTileGID( targetCoord.x, targetCoord.y );
		if (tileGid) 
		{
			const std::map<CGUIString, CGUIString>* pProperties = m_pTiledMap->GetTileProperties( tileGid );
			if( pProperties )
			{
				//check collidable
				std::map<CGUIString, CGUIString>::const_iterator itorCollide = pProperties->find( "Collidable" );
				if( itorCollide != pProperties->end() && itorCollide->second == "True" )
				{
					return CGUIWidget::OnMouseLeftClick(pEvent);
				}
			}
		}

		CGUIIntVector2 aPlayerCoord = m_pPlayer->GetCurrentCoord();
		std::vector<CGUIUIntVector2> aPath;
		if( eFindPathResult_Success != m_pPathFinder->FindPath( aPlayerCoord.x, aPlayerCoord.y, targetCoord.x, targetCoord.y, aPath) )
		{
			return CGUIWidget::OnMouseLeftClick(pEvent);
		}
		m_pPlayer->SetMovePath( aPath );
	}


	return CGUIWidget::OnMouseLeftClick(pEvent);
}
//------------------------------------------------------------------------------
CGUIIntVector2 CMyCanvasLayer_TiledMapGame::TileCoordForPosition( const CGUIVector2& rPos )
{
	const CGUIIntSize& rTileSize = m_pTiledMap->GetMapInfo()->GetTileSize();
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



//------------------------------------------------------------------------------
void CMyPlayer::UpdatePath()
{
	if( !m_vMovePaths.empty() )
	{
		CGUIVector2 aTargetPos;
		CGUIUIntVector2 aTargetCoord = m_vMovePaths[0];
		m_vMovePaths.erase( m_vMovePaths.begin());
		aTargetPos.x = aTargetCoord.x * m_aTileSize.GetWidth() + m_aPixelOffset.x;
		aTargetPos.y =aTargetCoord.y * m_aTileSize.GetHeight() + m_aPixelOffset.y;

		m_pMoveAs->Reset();
		m_pMoveAs->SetDestination( aTargetPos );
		PlayAs( m_pMoveAs );

		uint32 tileGid = m_pLayer->m_pMetaLayer->GetTileGID( aTargetCoord.x, aTargetCoord.y );
		if (tileGid) 
		{
			const std::map<CGUIString, CGUIString>* pProperties = m_pLayer->m_pTiledMap->GetTileProperties( tileGid );
			if( pProperties )
			{
				//check collectable
				std::map<CGUIString, CGUIString>::const_iterator itorCollect = pProperties->find( "Collectable" );
				if( itorCollect != pProperties->end() && itorCollect->second == "True" )
				{
					//eat one
					//update map
					m_pLayer->m_pMetaLayer->RemoveTileAt( aTargetCoord.x, aTargetCoord.y );
					m_pLayer->m_pForeGroundLayer->RemoveTileAt( aTargetCoord.x, aTargetCoord.y );
					//update score
					CGUIString strScroe = m_pLayer->m_pScore->GetTextContentUTF8();
					uint32 uScore;
					StringToValue( strScroe, uScore );
					++uScore;
					ValueToString( uScore, strScroe );
					m_pLayer->m_pScore->SetTextContentUTF8( strScroe );
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CMyPlayer::InitPlayer( class CMyCanvasLayer_TiledMapGame* pParent, const CGUIIntSize& rTileSize, const CGUIIntVector2& rPlayerPixelPos )
{
	m_aTileSize = rTileSize;
	m_aPixelOffset.x = rPlayerPixelPos.x % rTileSize.GetWidth();
	m_aPixelOffset.y = rPlayerPixelPos.y % rTileSize.GetHeight();

	SetParent( pParent );
	m_pLayer = pParent;
	SetImage( "bg", "player" );
	SetAnchorPoint( 0.5f, 0.5f );
	SetPixelPosition( real(rPlayerPixelPos.x), real(rPlayerPixelPos.y ));
	Create();

	//add as
	CGUIAsCallFunc* pAsCallFunc = CGUIAsManager::Instance()->AllocateResource<CGUIAsCallFunc>();
	pAsCallFunc->SetReceiver( this );
	pAsCallFunc->SetFuncCallback( FunCallback_PlayerMoveto );

	m_pMoveAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWidgetMoveTo>();
	m_pMoveAs->SetReceiver( this );
	m_pMoveAs->SetVelocity( 90.0f );
	m_pMoveAs->SetDestination( GetPixelPosition() );
	m_pMoveAs->AddSuccessor( pAsCallFunc );
	pAsCallFunc->RefRelease();

	SetAs( "As_MoveTo", m_pMoveAs );
	m_pMoveAs->RefRelease();
}
//------------------------------------------------------------------------------
void CMyPlayer::UpdateSelf( real fDeltaTime )
{
	if( IsAsPlaying( m_pMoveAs ))
	{
		m_pLayer->SetViewpointCenter( GetPixelPosition() );
	}

	CGUIWgtStaticImage::UpdateSelf( fDeltaTime );
}
//------------------------------------------------------------------------------
