/** 
* @file pathfinderastar.cpp
* @brief a star path finder
* @author ken
* @date 2011-04-25
*/

//============================================================================//
// include
//============================================================================// 
#include "pathfinderastar.h"



//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const uint8 CPathFinder_AStar::walkable = 0;
	const uint8 CPathFinder_AStar::unwalkable = 1;
	//------------------------------------------------------------------------------
	CPathFinder_AStar::CPathFinder_AStar( uint32 mapWidth, uint32 mapHeight )
		:m_mapWidth(mapWidth)
		,m_mapHeight(mapHeight)
		,onClosedList(0)
		,m_bCrossCorner(false)
	{
		m_walkability.resize( mapWidth, std::vector<uint8>(mapHeight, walkable) );
		whichList.resize( m_mapWidth, std::vector<uint8>(m_mapHeight, 0 ));
	}
	//------------------------------------------------------------------------------
	CPathFinder_AStar::~CPathFinder_AStar()
	{

	}
	//------------------------------------------------------------------------------
	void CPathFinder_AStar::EnableCrossCorner( bool bEnable )
	{
		m_bCrossCorner = bEnable;
	}
	//------------------------------------------------------------------------------
	void CPathFinder_AStar::SetMapWalkable( uint32 x, uint32 y, bool bWalkable )
	{
		if( x >= m_mapWidth || y >= m_mapHeight )
		{
			GUI_ASSERT( 0, "invalid parameter") ;
			return;
		}
		m_walkability[x][y] = (bWalkable ? walkable : unwalkable);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief find path by a-star algorithm
	* @desc startX, startY, targetX and targetY are coordinates value of maze.
	* F = G + H 
	where
	G = the movement cost to move from the starting point A to a given square on
	the grid, following the path generated to get there. 
	H = the estimated movement cost to move from that given square on the grid to 
	the final destination, point B. This is often referred to as the heuristic, which 
	can be a bit confusing. The reason why it is called that is because it is a guess. 
	We really don¡¯t know the actual distance until we find the path, because all sorts 
	of things can be in the way (walls, water, etc.). You are given one way to calculate 
	H in this tutorial, but there are many others that you can find in other articles on
	the web.
	* @return zero for success.
	*/
	EFindPathResult CPathFinder_AStar::FindPath( uint32 startX, uint32 startY, uint32 targetX, uint32 targetY, std::vector<CGUIUIntVector2>& rOutPath )
	{
		//1. Convert location data (in pixels) to coordinates in the m_walkability array.

		//2.Quick Path Checks: Under the some circumstances no path needs to
		//	be generated ...

		//	If starting location and target are in the same location...
		if ( startX == targetX && startY == targetY )
		{
			return eFindPathResult_Success;
		}

		//	If target square is unwalkable, return that it's a nonexistent path.
		if( targetX >= m_mapWidth ||
			targetY >= m_mapHeight ||
			m_walkability[targetX][targetY] == unwalkable)
		{
			return eFindPathResult_Unwalkable;
		}

		//3.Reset some variables that need to be cleared
		if (onClosedList > 253) //reset whichList occasionally
		{
			for (uint32 x = 0; x < m_mapWidth;x++) 
			{
				for (uint32 y = 0; y < m_mapHeight;y++)
				{
					whichList [x][y] = 0;
				}
			}
			onClosedList = 0;	
		}
		onClosedList = onClosedList+2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
		uint8 onOpenList = onClosedList-1;

		std::vector<uint32> Fcost(m_mapWidth*m_mapHeight+2,0);	//1d array to store F cost of a cell on the open list
		std::vector< std::vector<uint32> > Gcost( m_mapWidth+1, std::vector<uint32>(m_mapHeight+1, 0 ));
		std::vector<uint32> Hcost(m_mapWidth*m_mapHeight+2, 0);	//1d array to store H cost of a cell on the open list

		//4.Add the starting location to the open list of squares to be checked.
		std::vector<uint32> openList(m_mapWidth*m_mapHeight+2, 0); //1 dimensional array holding ID# of open list items
		std::vector<uint32> openX(m_mapWidth*m_mapHeight+2, 0); //1d array stores the x location of an item on the open list
		std::vector<uint32> openY(m_mapWidth*m_mapHeight+2, 0); //1d array stores the y location of an item on the open list
		uint32 numberOfOpenListItems = 1;
		int32 newOpenListItemID=0;
		openList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
		openX[1] = startX ; 
		openY[1] = startY;

		std::vector< std::vector<uint32> > parentX( m_mapWidth+1, std::vector<uint32>(m_mapHeight+1, 0 )); //2d array to store parent of each cell (x)
		std::vector< std::vector<uint32> > parentY( m_mapWidth+1, std::vector<uint32>(m_mapHeight+1, 0 )); //2d array to store parent of each cell (y)

		//5.Do the following until a path is found or deemed nonexistent.
		uint32 parentXval=0, parentYval=0;
		uint32 m=0, u=0, v = 0;
		uint32 temp = 0, corner=0;
		EFindPathResult result = eFindPathResult_Success;
		do
		{
			//6.If the open list is not empty, take the first cell off of the list.
			//	This is the lowest F cost cell on the open list.
			if (numberOfOpenListItems != 0)
			{

				//7. Pop the first item off the open list.
				parentXval = openX[openList[1]];
				parentYval = openY[openList[1]]; //record cell coordinates of the item
				whichList[parentXval][parentYval] = onClosedList;//add the item to the closed list

				//	Open List = Binary Heap: Delete this item from the open list, which
				//  is maintained as a binary heap. For more information on binary heaps, see:
				//	http://www.policyalmanac.org/games/binaryHeaps.htm
				numberOfOpenListItems = numberOfOpenListItems - 1;//reduce number of open list items by 1	

				//	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
				openList[1] = openList[numberOfOpenListItems+1];//move the last item in the heap up to slot #1
				v = 1;

				//	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
				do
				{
					u = v;		
					if (2*u+1 <= numberOfOpenListItems) //if both children exist
					{
						//Check if the F cost of the parent is greater than each child.
						//Select the lowest of the two children.
						if (Fcost[openList[u]] >= Fcost[openList[2*u]]) 
						{
							v = 2*u;
						}
						if (Fcost[openList[v]] >= Fcost[openList[2*u+1]]) 
						{
							v = 2*u+1;		
						}
					}
					else
					{
						if (2*u <= numberOfOpenListItems) //if only child #1 exists
						{
							//Check if the F cost of the parent is greater than child #1	
							if (Fcost[openList[u]] >= Fcost[openList[2*u]]) 
							{
								v = 2*u;
							}
						}
					}

					if (u != v) //if parent's F is > one of its children, swap them
					{
						temp = openList[u];
						openList[u] = openList[v];
						openList[v] = temp;			
					}
					else
					{
						break; //otherwise, exit loop
					}
				}
				while (1);//reorder the binary heap


				//7.Check the adjacent squares. (Its "children" -- these path children
				//	are similar, conceptually, to the binary heap children mentioned
				//	above, but don't confuse them. They are different. Path children
				//	are portrayed in Demo 1 with grey pointers pointing toward
				//	their parents.) Add these adjacent child squares to the open list
				//	for later consideration if appropriate (see various if statements
				//	below).
				for (int32 b = (int32)parentYval-1; b <= (int32)parentYval+1; b++)
				{
					for (int32 a = (int32)parentXval-1; a <= (int32)parentXval+1; a++)
					{
						if (abs(int32(a-parentXval)) == 1 && abs(int32(b-parentYval)) == 1)
						{
							continue;
						}

						//	If not off the map (do this first to avoid array out-of-bounds errors)
						if (a != -1 && b != -1 && uint32(a) != m_mapWidth && uint32(b) != m_mapHeight)
						{
							//	If not already on the closed list (items on the closed list have
							//	already been considered and can now be ignored).			
							if (whichList[a][b] != onClosedList) 
							{ 
								//	If not a wall/obstacle square.
								if (m_walkability [a][b] != unwalkable) 
								{ 
									//	Don't cut across corners
									corner = walkable;	
									if (a == (int32)parentXval-1) 
									{
										if (b == (int32)parentYval-1)
										{
											if ( m_walkability[parentXval-1][parentYval] == unwalkable ||
												m_walkability[parentXval][parentYval-1] == unwalkable)
											{
												corner = unwalkable;
											}
										}
										else if (b == (int32)parentYval+1)
										{
											if ( m_walkability[parentXval][parentYval+1] == unwalkable ||
												m_walkability[parentXval-1][parentYval] == unwalkable) 
											{
												corner = unwalkable; 
											}
										}
									}
									else if (a == (int32)parentXval+1)
									{
										if (b == (int32)parentYval-1)
										{
											if ( m_walkability[parentXval][parentYval-1] == unwalkable || 
												m_walkability[parentXval+1][parentYval] == unwalkable) 
											{
												corner = unwalkable; 
											}
										}
										else if (b == (int32)parentYval+1)
										{
											if ( m_walkability[parentXval+1][parentYval] == unwalkable || 
												m_walkability[parentXval][parentYval+1] == unwalkable)
											{
												corner = unwalkable; 
											}
										}
									}	
									if (corner == walkable) 
									{
										//	If not already on the open list, add it to the open list.			
										if (whichList[a][b] != onOpenList) 
										{	
											//Create a new open list item in the binary heap.
											newOpenListItemID = newOpenListItemID + 1; //each new item has a unique ID #
											m = numberOfOpenListItems+1;
											openList[m] = newOpenListItemID;//place the new open list item (actually, its ID#) at the bottom of the heap
											openX[newOpenListItemID] = a;
											openY[newOpenListItemID] = b;//record the x and y coordinates of the new item

											//Figure out its G cost
											int32 addedGCost = 0;
											if (abs(int32(a-parentXval)) == 1 && abs(int32(b-parentYval)) == 1)
											{
												addedGCost = 14;//cost of going to diagonal squares	
											}
											else	
											{
												addedGCost = 10;//cost of going to non-diagonal squares				
											}
											Gcost[a][b] = Gcost[parentXval][parentYval] + addedGCost;

											//Figure out its H and F costs and parent
											Hcost[openList[m]] = 10*(abs(int32(a - targetX)) + abs(int32(b - targetY)));
											Fcost[openList[m]] = Gcost[a][b] + Hcost[openList[m]];
											parentX[a][b] = parentXval;
											parentY[a][b] = parentYval;	

											//Move the new open list item to the proper place in the binary heap.
											//Starting at the bottom, successively compare to parent items,
											//swapping as needed until the item finds its place in the heap
											//or bubbles all the way to the top (if it has the lowest F cost).
											while (m != 1) //While item hasn't bubbled to the top (m=1)	
											{
												//Check if child's F cost is < parent's F cost. If so, swap them.	
												if (Fcost[openList[m]] <= Fcost[openList[m/2]])
												{
													temp = openList[m/2];
													openList[m/2] = openList[m];
													openList[m] = temp;
													m = m/2;
												}
												else
												{
													break;
												}
											}
											numberOfOpenListItems = numberOfOpenListItems+1;//add one to the number of items in the heap

											//Change whichList to show that the new item is on the open list.
											whichList[a][b] = onOpenList;
										}

										//8.If adjacent cell is already on the open list, check to see if this 
										//	path to that cell from the starting location is a better one. 
										//	If so, change the parent of the cell and its G and F costs.	
										else //If whichList(a,b) = onOpenList
										{

											//Figure out the G cost of this possible new path
											int32 addedGCost = 0;
											if (abs(int32(a-parentXval)) == 1 && abs(int32(b-parentYval)) == 1)
											{
												addedGCost = 14;//cost of going to diagonal tiles	
											}
											else	
											{
												addedGCost = 10;//cost of going to non-diagonal tiles				
											}

											uint32 tempGcost = Gcost[parentXval][parentYval] + addedGCost;
											//If this path is shorter (G cost is lower) then change
											//the parent cell, G cost and F cost. 		
											if (tempGcost < Gcost[a][b]) //if G cost is less,
											{
												parentX[a][b] = parentXval; //change the square's parent
												parentY[a][b] = parentYval;
												Gcost[a][b] = tempGcost;//change the G cost			

												//Because changing the G cost also changes the F cost, if
												//the item is on the open list we need to change the item's
												//recorded F cost and its position on the open list to make
												//sure that we maintain a properly ordered open list.
												for (uint32 x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
												{
													if (openX[openList[x]] == a && openY[openList[x]] == b) //item found
													{
														Fcost[openList[x]] = Gcost[a][b] + Hcost[openList[x]];//change the F cost

														//See if changing the F score bubbles the item up from it's current location in the heap
														m = x;
														while (m != 1) //While item hasn't bubbled to the top (m=1)	
														{
															//Check if child is < parent. If so, swap them.	
															if (Fcost[openList[m]] < Fcost[openList[m/2]])
															{
																temp = openList[m/2];
																openList[m/2] = openList[m];
																openList[m] = temp;
																m = m/2;
															}
															else
															{
																break;
															}
														} 
														break; //exit for x = loop
													} //If openX(openList(x)) = a
												} //For x = 1 To numberOfOpenListItems
											}//If tempGcost < Gcost(a,b)

										}//else If whichList(a,b) = onOpenList	
									}//If not cutting a corner
								}//If not a wall/obstacle square.
							}//If not already on the closed list 
						}//If not off the map
					}//for (a = parentXval-1; a <= parentXval+1; a++){
				}//for (b = parentYval-1; b <= parentYval+1; b++){

			}//if (numberOfOpenListItems != 0)

			//9.If open list is empty then there is no path.	
			else
			{
				result = eFindPathResult_NotExistent;
				break;
			}  

			//If target is added to open list then path has been found.
			if (whichList[targetX][targetY] == onOpenList)
			{
				result = eFindPathResult_Success; 
				break;
			}

		}
		while (1);//Do until path is found or deemed nonexistent

		//10.Save the path if it exists.
		if( result == eFindPathResult_Success )
		{
			//a.Working backwards from the target to the starting location by checking
			//	each cell's parent, figure out the length of the path.
			int32 pathX = targetX;
			int32 pathY = targetY;
			int32 pathLength = 0;
			do
			{
				//Look up the parent of the current cell.	
				int32 tempx = parentX[pathX][pathY];		
				pathY = parentY[pathX][pathY];
				pathX = tempx;

				//Figure out the path length
				pathLength = pathLength + 1;
			}
			while (pathX != startX || pathY != startY);

			//b.Resize the data bank to the right size in bytes
			rOutPath.resize( pathLength );

			//c. Now copy the path information over to the databank. Since we are
			//	working backwards from the target to the start location, we copy
			//	the information to the data bank in reverse order. The result is
			//	a properly ordered set of path data, from the first step to the
			//	last.
			pathX = targetX;
			pathY = targetY;
			do
			{
				pathLength -= 1;
				rOutPath[pathLength].SetX( pathX );
				rOutPath[pathLength].SetY( pathY );

				//d.Look up the parent of the current cell.	
				int32 tempx = parentX[pathX][pathY];		
				pathY = parentY[pathX][pathY];
				pathX = tempx;

				//e.If we have reached the starting square, exit the loop.	
			}
			while (pathX != startX || pathY != startY);	
		}
		return result;
	}
	//------------------------------------------------------------------------------
}
