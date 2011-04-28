/** 
* @file guirenderrect.cpp
* @brief rect which contain render vertex
* @author ken
* @date 2007-11-26
*/

//============================================================================//
// include 
//============================================================================// 
#include "guirenderrect.h"
#include "guimath.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//-----------------------------------------------------------------------
	bool	CGUIRenderRect::IsPointInRect( const CGUIVector2& rPos) const
	{
		if( CGUIMath::pointInTri2D( rPos.x,rPos.y,
			m_vecVertex[0].m_aVector.x,m_vecVertex[0].m_aVector.y,
			m_vecVertex[1].m_aVector.x,m_vecVertex[1].m_aVector.y,
			m_vecVertex[3].m_aVector.x,m_vecVertex[3].m_aVector.y)	||
			CGUIMath::pointInTri2D( rPos.x,rPos.y,
			m_vecVertex[1].m_aVector.x,m_vecVertex[1].m_aVector.y,
			m_vecVertex[2].m_aVector.x,m_vecVertex[2].m_aVector.y,
			m_vecVertex[3].m_aVector.x,m_vecVertex[3].m_aVector.y)
			)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//-----------------------------------------------------------------------

}
