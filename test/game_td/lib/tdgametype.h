/** 
* @file tdgametype.h
* @brief types used in td game
* @author ken
* @date 2011-08-22
*/

#ifndef __TD_GAMETYPE_20110822_H__
#define __TD_GAMETYPE_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	enum ETowerType
	{
		eTowerType_Base = 0,
		eTowerType_ArcherTower,
		eTowerType_Mages,
		eTowerType_DwarvenBoombard,
		eTowerType_Barracks,

		__eTowerType_MAX__,
	};
}

#endif //__TD_GAMETYPE_20110822_H__
