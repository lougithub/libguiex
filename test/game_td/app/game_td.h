/** 
* @file game_td.h
* @author ken
* @date 2011-08-15
*/

#ifndef __GAME_TD_20110815_H__
#define __GAME_TD_20110815_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_framework/guiframework.h>
#include "tdgameworld.h"
#include <vector>
#include <set>

//============================================================================//
// class
//============================================================================// 


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

protected:
	guiex::CTDGameWorld* m_pGameWorld;
};

//*****************************************************************************
//	CGUIFrameworkTDGame
//*****************************************************************************
class CGUIFrameworkTDGame : public guiex::CGUIFramework
{
public:
	CGUIFrameworkTDGame(  );

	virtual void Update( guiex::real fDeltaTime );

protected:
	virtual guiex::int32 InitializeGame( );
	virtual void RegisterWidgetGenerators( );
	virtual void RegisterInterfaces_Script( );

protected:
	CMyCanvasLayer_GameLayer* m_pGameLayer;

	static CGUIFrameworkTDGame* ms_pFrameWork;
};


#endif //__GAME_TD_20110815_H__
