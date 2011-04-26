/** 
* @file pathfinderastar.h
* @brief a star path finder
* @author ken
* @date 2011-04-25
*/

#ifndef __GUI_PATHFINDER_ASTAR_20110425_H__
#define __GUI_PATHFINDER_ASTAR_20110425_H__

//============================================================================//
// include
//============================================================================// 
#include "guias.h"
#include "guiintvector2.h"

//============================================================================//
// declare
//============================================================================// 
enum EFindPathResult
{
	eFindPathResult_Success = 0,
	eFindPathResult_NotExistent,
	eFindPathResult_Unwalkable,
};


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class CPathFinder_AStar
	{
	public:
		CPathFinder_AStar( uint32 mapWidth, uint32 mapHeight );
		~CPathFinder_AStar();

		void EnableCrossCorner( bool bEnable );
		void SetMapWalkable( uint32 x, uint32 y, bool bWalkable );

		EFindPathResult FindPath( uint32 startX, uint32 startY, uint32 targetX, uint32 targetY, std::vector<CGUIUIntVector2>& rOutPath );

	protected:
		const static uint8 walkable; // walkability array constants
		const static uint8 unwalkable; // walkability array constants

		std::vector< std::vector<uint8> > m_walkability;
		uint32 m_mapWidth;
		uint32 m_mapHeight;

		bool m_bCrossCorner;

		uint8 onClosedList;
		std::vector< std::vector<uint8> > whichList;
	};
}


#endif //__GUI_PATHFINDER_ASTAR_20110425_H__