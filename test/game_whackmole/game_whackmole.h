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
using namespace guiex;


//============================================================================//
// class
//============================================================================// 


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

protected:
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