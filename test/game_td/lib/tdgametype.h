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
		eTowerType_Basement = 0,
		eTowerType_ArcherTower,
		eTowerType_Mages,
		eTowerType_Bombard,
		eTowerType_Barracks,

		__eTowerType_MAX__,
	};

	enum EBulletDamageType
	{
		eBulletDamageType_Physical = 0,
		eBulletDamageType_Magical,
	};

}

#endif //__TD_GAMETYPE_20110822_H__
