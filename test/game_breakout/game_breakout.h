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

//============================================================================//
// class
//============================================================================// 


//*****************************************************************************
//	CMyBall
//*****************************************************************************
class CMyBall : public guiex::CGUIWgtStaticImage
{
public:
	CMyBall( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName );

	void InitBall( guiex::CGUIWidget* pParent );
	b2Fixture* GetFixture();

protected:
	virtual void OnUpdate( guiex::real fDeltaTime );

protected:
	b2Body* m_pBody;
	b2Fixture* m_pFixture;

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyBall );
};


//*****************************************************************************
//	CMyPaddle
//*****************************************************************************
class CMyPaddle : public guiex::CGUIWgtStaticImage
{
public:
	CMyPaddle( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName );

	void InitPaddle( guiex::CGUIWidget* pParent );

protected:
	virtual void OnUpdate( guiex::real fDeltaTime );

	virtual uint32 OnDragBegin( guiex::CGUIEventDrag* pEvent );
	virtual uint32 OnDragProcess( guiex::CGUIEventDrag* pEvent );
	virtual uint32 OnDragEnd( guiex::CGUIEventDrag* pEvent );

protected:
	b2Body* m_pBody;
	b2Fixture* m_pFixture;
	b2MouseJoint *m_mouseJoint;

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyPaddle );
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
//	CMyCanvasLayer_BreakoutGame
//*****************************************************************************
class CMyCanvasLayer_BreakoutGame : public guiex::CGUICanvasLayer
{
public:
	CMyCanvasLayer_BreakoutGame( const char* szLayerName );
	~CMyCanvasLayer_BreakoutGame(  );

	virtual void Initialize( );

	virtual void DestroySelf( );

	b2Body* GetGroundBody();

protected:
	virtual void OnUpdate( guiex::real fDeltaTime );

protected:
	CMyBall* m_pBall;

	b2Body *m_groundBody;
	b2Fixture *m_bottomFixture;
	MyContactListener *m_contactListener;

	guiex::IGUIPhysics_box2d* m_pPhysics;
};


//*****************************************************************************
//	CGUIFrameworkTest
//*****************************************************************************
class CGUIFrameworkTest : public guiex::CGUIFramework
{
public:
	CGUIFrameworkTest(  );
	CMyCanvasLayer_BreakoutGame* GetGameLayer();

	static CGUIFrameworkTest* ms_pFrameWork;

protected:
	virtual guiex::int32 InitializeGame( );

protected:
	CMyCanvasLayer_BreakoutGame* m_pGameLayer;
};


#endif //__GAME_TURRET_20110414_H__