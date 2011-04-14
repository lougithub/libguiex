/** 
* @file game_turret.h
* @author ken
* @date 2011-04-14
*/

#ifndef __GAME_TURRET_20110414_H__
#define __GAME_TURRET_20110414_H__


//============================================================================//
// include
//============================================================================// 
#include <libguiex_framework/guiframework.h>
using namespace guiex;


//============================================================================//
// class
//============================================================================// 

//*****************************************************************************
//	CMyMonster
//*****************************************************************************
class CMyMonster : public CGUIWgtStaticImage
{
public:
	CMyMonster( const CGUIString& rName, const CGUIString& rSceneName );

	void InitMonster( CGUIWidget* pParent, bool bStrong, CGUIWgtStaticImage* pTurret );

	uint32 GetHp() const;
	void SetHp( uint32 uHp);

protected:
	static void FunRetiredCallback_TargetMoveto(CGUIAs* pAs);
	void ResetTargetPosition( );
	CGUIVector2 GenerateTargetPosition();

protected:
	bool m_bStrongMonster;
	real m_fVelocity;
	CGUIWgtStaticImage* m_pTurret;
	uint32 m_uHp;

	GUI_CUSTOM_WIDGET_DECLARE( CMyMonster );
};


//*****************************************************************************
//	CGUIAsShoot
//*****************************************************************************
class CGUIAsShoot : public CGUICustomAs
{
public:
	CGUIAsShoot( class CMyCanvasLayer_TurretGame* pGameLayer );

	void SetShootPos(const CGUIVector2& rShootPos);

	virtual real Update( real fDeltaTime );

protected:
	class CMyCanvasLayer_TurretGame* m_pGameLayer;
	CGUIVector2 m_aShootPos;
};


//*****************************************************************************
//	CMyProjectile
//*****************************************************************************
class CMyProjectile : public CGUIWgtStaticImage
{
public:
	CMyProjectile( const CGUIString& rName, const CGUIString& rSceneName );
	void InitProjectile( CGUIWidget* pParent,CGUIWgtStaticImage* pTurret, const CGUIVector2& rShootPos );

protected:
	static void FunRetiredCallback_ProjectileFinish(CGUIAs* pAs);

protected:
	CGUIWgtStaticImage* m_pTurret;

protected:
	GUI_CUSTOM_WIDGET_DECLARE( CMyProjectile );
};



//*****************************************************************************
//	CMyCanvasLayer_TurretGame
//*****************************************************************************
class CMyCanvasLayer_TurretGame : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_TurretGame( const char* szLayerName );
	~CMyCanvasLayer_TurretGame(  );

	virtual void DestroySelf( );


	void BeginShoot( const CGUIVector2& rShootPos );
	void Shoot( const CGUIVector2& rShootPos );

	void DestroyProjectile( CMyProjectile* pProjectile );
	void DestroyTarget( CGUIWidget* pTarget );

	void IncreaseScroe( );

protected:
	virtual void OnUpdate( real fDeltaTime );

	void InitTurret();

	static void Btn_OnClick_Panel( CGUIEventMouse* pEvent );

protected:
	std::vector<CMyProjectile*> m_vecProjectile;
	std::vector<CMyMonster*> m_vecTarget;
	CGUIWgtStaticImage* m_pTurret;
	CGUIWgtStaticText* m_pScore;
};


//*****************************************************************************
//	CGUIFrameworkTest
//*****************************************************************************
class CGUIFrameworkTest : public CGUIFramework
{
public:
	CGUIFrameworkTest(  );
	CMyCanvasLayer_TurretGame* GetTurretGameLayer();

	static CGUIFrameworkTest* ms_pFrameWork;

protected:
	virtual int32 InitializeGame( );

protected:
	CMyCanvasLayer_TurretGame* m_pTurretGameLayer;
};


#endif //__GAME_TURRET_20110414_H__