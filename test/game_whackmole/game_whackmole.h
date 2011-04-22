/** 
* @file game_whackmole.h
* @author ken
* @date 2011-04-14
*/

#ifndef __GAME_WHACKMOLE_20110414_H__
#define __GAME_WHACKMOLE_20110414_H__


//============================================================================//
// include
//============================================================================// 
#include <libguiex_framework/guiframework.h>
#include <vector>
using namespace guiex;


//============================================================================//
// class
//============================================================================// 

//*****************************************************************************
//	CMyMole
//*****************************************************************************
class CMyMole : public CGUIWgtAnimation
{
public:
	CMyMole( const CGUIString& rName, const CGUIString& rSceneName );

	void InitMole( CGUIWgtTag* pTag );
	void UpdateMole( real fDeltaTime );

protected:
	void UpdateMole_Hide(real fDeltaTime);
	void UpdateMole_Popping(real fDeltaTime);
	void UpdateMole_Popped(real fDeltaTime);
	void UpdateMole_Shrinking(real fDeltaTime);

	void ShrinkMole();

	bool IsTappable() const;

	virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );

protected:
	enum EMoleState
	{
		eMoleState_Hide,
		eMoleState_Popping,
		eMoleState_Popped,
		eMoleState_Shrinking,
	};
	EMoleState m_eMoleState;

	CGUIVector2 m_aHidePosition;
	CGUIVector2 m_aPopPosition;
	CGUIAsWidgetMoveTo* m_pAs;

	real m_fPoppedTime;

	GUI_CUSTOM_WIDGET_DECLARE( CMyMole );
};

//*****************************************************************************
//	CMyCanvasLayer_WhackMoleGame
//*****************************************************************************
class CMyCanvasLayer_WhackMoleGame : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_WhackMoleGame( const char* szLayerName );
	~CMyCanvasLayer_WhackMoleGame(  );

	virtual void DestroySelf( );


protected:
	virtual void OnUpdate( real fDeltaTime );

	static void FunOnTimer(CGUIEventTimer* pEvent );

protected:
	std::vector<CMyMole*> m_vecMole;
};


//*****************************************************************************
//	CGUIFrameworkTest
//*****************************************************************************
class CGUIFrameworkTest : public CGUIFramework
{
public:
	CGUIFrameworkTest(  );
	CMyCanvasLayer_WhackMoleGame* GetGameLayer();

	static CGUIFrameworkTest* ms_pFrameWork;

protected:
	virtual int32 InitializeGame( );

protected:
	CMyCanvasLayer_WhackMoleGame* m_pGameLayer;
};


#endif //__GAME_WHACKMOLE_20110414_H__