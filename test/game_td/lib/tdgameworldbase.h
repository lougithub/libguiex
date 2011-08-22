/** 
* @file tdgameworldbase.h
* @brief base class of game world, contain all monster, bullet and thing in fighting game
* @author ken
* @date 2011-08-19
*/

#ifndef __TD_GAMEWORLDBASE_20110819_H__
#define __TD_GAMEWORLDBASE_20110819_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameWorldBase
	{
	public:
		CTDGameWorldBase();
		virtual ~CTDGameWorldBase();

		virtual void InitGameWorld(CGUIWidget* /*pGameRoot*/){ }
		virtual void DestroyGameWorld(){ }

	protected:
		friend class CTDWgtGameWorldLayer;
		virtual void OnRender( IGUIInterfaceRender* pRender ) = 0;
		virtual void OnUpdate( real fDeltaTime ) = 0;
	};
}

#endif //__TD_GAMEWORLDBASE_20110819_H__
