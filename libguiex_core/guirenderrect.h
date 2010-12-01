/** 
* @file guirenderrect.h
* @brief rect which contain render vertex
* @author ken
* @date 2007-11-26
*/

#ifndef __GUI_RENDER_RECT_20071126_H__
#define __GUI_RENDER_RECT_20071126_H__



//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector2.h"
#include "guivector3.h"
#include "guicolor.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT	CGUIRenderRect
	{
	public:
		struct SVertex
		{
			CGUIVector3		m_aVector;
			CGUIColor		m_aColor;
		};

	public:
		CGUIRenderRect()
		{

		}

		//!< whether a 2d point in this rect
		bool	IsPointInRect( const CGUIVector2& rPos) const;

	public:
		SVertex		m_vecVertex[4];	//!<the render vector list, the sequence of vector is top_left, top_right, bottom_right, bottom_left
	};
}

#endif //__GUI_RENDER_RECT_20071126_H__
