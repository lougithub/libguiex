/** 
* @file game_breakout.h
* @author ken
* @date 2011-04-18
*/


#ifndef __GAME_TURRET_20110414_H__
#define __GAME_TURRET_20110414_H__


//============================================================================//
// include
//============================================================================// 
#include <libguiex_framework/guiframework.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <set>

//============================================================================//
// class
//============================================================================// 

//*****************************************************************************
//	CMyBodyBase
//*****************************************************************************
enum EBodyType
{
	eBodyType_Ball = 0,
	eBodyType_Block,
	eBodyType_Paddle,
};
class CMyBodyBase : public guiex::CGUIWgtStaticImage
{
public:
	b2Body* GetBody();
	b2Fixture* GetFixture();
	EBodyType GetBodyType() const;

protected:
	CMyBodyBase( const guiex::CGUIString& rType, const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName, EBodyType eType );
	virtual void OnUpdate( guiex::real fDeltaTime );
	virtual void OnDestroy();


protected:
	EBodyType m_eType;
	b2Body* m_pBody;
	b2Fixture* m_pFixture;
};


//*****************************************************************************
//	CMyBall
//*****************************************************************************
class CMyBall : public CMyBodyBase
{
public:
	CMyBall( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName );

	void InitBall( guiex::CGUIWidget* pParent );

protected:
	virtual void OnUpdate( guiex::real fDeltaTime );

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyBall );
};


//*****************************************************************************
//	CMyPaddle
//*****************************************************************************
class CMyPaddle : public CMyBodyBase
{
public:
	CMyPaddle( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName );

	void InitPaddle( guiex::CGUIWidget* pParent );

protected:
	virtual uint32 OnDragBegin( guiex::CGUIEventDrag* pEvent );
	virtual uint32 OnDragProcess( guiex::CGUIEventDrag* pEvent );
	virtual uint32 OnDragEnd( guiex::CGUIEventDrag* pEvent );

protected:
	b2MouseJoint *m_mouseJoint;
	b2Joint* m_pPrismaticJoint;

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyPaddle );
};


//*****************************************************************************
//	CMyBlock
//*****************************************************************************
class CMyBlock : public CMyBodyBase
{
public:
	CMyBlock( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName );

	void InitBlock( guiex::CGUIWidget* pParent, const guiex::CGUIVector2& rPos );

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyBlock );
};


struct MyContact 
{
	b2Fixture *fixtureA;
	b2Fixture *fixtureB;
	bool operator==(const MyContact& other) const
	{
		return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
	}
};


//*****************************************************************************
//	MyContactListener
//*****************************************************************************
class MyContactListener : public b2ContactListener 
{
public:
	std::vector<MyContact>_contacts;

	MyContactListener();
	~MyContactListener();

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};


//*****************************************************************************
//	CMyCanvasLayer_GameLayer
//*****************************************************************************
class CMyCanvasLayer_GameLayer : public guiex::CGUICanvasLayer
{
public:
	CMyCanvasLayer_GameLayer( const char* szLayerName );
	~CMyCanvasLayer_GameLayer(  );

	virtual void Initialize( );
	virtual void Finalize( );

	virtual void DestroySelf( );

	b2Body* GetGroundBody();

protected:
	virtual void OnUpdate( guiex::real fDeltaTime );

protected:
	CGUIWidget* m_pBackground;
	CMyBall* m_pBall;
	std::set<CMyBodyBase*> m_arrayBlocks;

	b2Body *m_groundBody;
	b2Fixture *m_bottomFixture;
	MyContactListener *m_contactListener;

	guiex::IGUIPhysics_box2d* m_pPhysics;
};



//*****************************************************************************
//	CMyCanvasLayer_MenuLayer
//*****************************************************************************
class CMyCanvasLayer_MenuLayer : public guiex::CGUICanvasLayer
{
public:
	CMyCanvasLayer_MenuLayer( const char* szLayerName, const char* szLayerConfigName );
	~CMyCanvasLayer_MenuLayer(  );

	virtual void Initialize( );
	virtual void Finalize( );

	virtual void DestroySelf( );

protected:
	guiex::CGUIWidget* m_pMenuRoot;
	guiex::CGUIString m_strLayerConfigName;
};

//*****************************************************************************
//	CGUIFrameworkTest
//*****************************************************************************
class CGUIFrameworkTest : public guiex::CGUIFramework
{
public:
	CGUIFrameworkTest(  );
	CMyCanvasLayer_GameLayer* GetGameLayer();
	void CreateGameLayer( );
	void DestroyGameLayer( );

	CMyCanvasLayer_MenuLayer* GetMenuLayer();
	void CreateMenuLayer( const char* szLayerConfigName );
	void DestroyMenuLayer( );

	void BeginOpenMenuLayer( const char* szLayerConfigName );

	static CGUIFrameworkTest* ms_pFrameWork;


protected:
	virtual guiex::int32 InitializeGame( );

	static void FunCallback_DestroyGameLayer(guiex::CGUIAs* pAs);

	static void Btn_OnClick_Replay( guiex::CGUIEventMouse* pEvent );

protected:
	CMyCanvasLayer_GameLayer* m_pGameLayer;
	CMyCanvasLayer_MenuLayer* m_pMenuLayer;
};


#endif //__GAME_TURRET_20110414_H__