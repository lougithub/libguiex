/** 
* @file tdgamestate.h
* @brief game state
* @author ken
* @date 2011-08-20
*/

#ifndef __TD_GAMESTATE_20110829_H__
#define __TD_GAMESTATE_20110829_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameState
	{
	public:
		CTDGameState( ){}
		virtual ~CTDGameState(){}

		virtual void EnterState(){}
		virtual void LeaveState(){}
		virtual void UpdateState( real fDeltaTime ){}
	};
}

#endif //__TD_GAMESTATE_20110829_H__
