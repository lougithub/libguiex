/** 
* @file guirect.cpp
* @brief CGUIRect used in this system
* @author ken
* @date 2006-04-24
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guirect.h>
#include <libguiex_core/guisize.h>
#include <libguiex_core/guivector2.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(void) 
		:m_fTop(0.0f)
		,m_fBottom(0.0f)
		,m_fLeft(0.0f)
		,m_fRight(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(real left, real top, real right, real bottom)
		:m_fTop(top)
		,m_fBottom(bottom)
		,m_fLeft(left)
		,m_fRight(right)
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect( const CGUIVector2& pos, const CGUISize& sz) 
		:m_fTop(pos.y)
		,m_fBottom(pos.y + sz.m_fHeight)
		,m_fLeft(pos.x)
		,m_fRight(pos.x + sz.m_fWidth)
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(const CGUIRect& rRect)
	{
		SetRect(rRect);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the intersection of this rect with the given rect.
	* @return a Rect that is the intersection of 'this' Rect with the Rect 'rect'
	* @note if the return rect's width is 0 or rect's height is 0, then this rect was outside rect.
	* @return the intersection rect
	*/
	CGUIRect CGUIRect::GetIntersection(const CGUIRect& rect) const
	{
		// check for total exclusion
		if ((m_fRight > rect.m_fLeft) &&
			(m_fLeft < rect.m_fRight) &&
			(m_fBottom > rect.m_fTop) &&
			(m_fTop < rect.m_fBottom))
		{
			return CGUIRect(
				(m_fLeft > rect.m_fLeft) ? m_fLeft : rect.m_fLeft,
				(m_fTop > rect.m_fTop) ? m_fTop : rect.m_fTop,
				(m_fRight < rect.m_fRight) ? m_fRight : rect.m_fRight,
				(m_fBottom < rect.m_fBottom) ? m_fBottom : rect.m_fBottom);
		}
		else
		{
			return CGUIRect(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIRect::IsIntersection(const CGUIRect& rect) const
	{
		// check for total exclusion
		if ((m_fRight > rect.m_fLeft) &&
			(m_fLeft < rect.m_fRight) &&
			(m_fBottom > rect.m_fTop) &&
			(m_fTop < rect.m_fBottom))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief judge is the given point falls within this rect
	* @param pt Point object describing the position to test.
	* @return true if given point is within this Rect's area, else false
	*/
	bool CGUIRect::IsPointInRect(const CGUIVector2& pt) const
	{
		if ((m_fLeft > pt.x) ||
			(m_fRight <= pt.x) ||
			(m_fTop > pt.y) ||
			(m_fBottom <= pt.y))
		{
			return false;
		}

		return true;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set position of the rect
	*/
	void CGUIRect::SetPosition(const CGUIVector2& pt)
	{
		CGUISize sz(GetSize());

		m_fLeft = pt.x;
		m_fTop  = pt.y;
		SetSize(sz);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constrain rect's size to given size if it is bigger than it.
	* @param sz Size object that describes the maximum dimensions that this Rect should be limited to.
	* @return 'this' Rect object after the constrain operation
	*/
	CGUIRect& CGUIRect::ConstrainSizeMax(const CGUISize& sz)
	{
		if (GetWidth() > sz.m_fWidth)
		{
			SetWidth(sz.m_fWidth);
		}

		if (GetHeight() > sz.m_fHeight)
		{
			SetHeight(sz.m_fHeight);
		}

		return *this;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constrain rect's size to given size if it is smaller than it.
	* @param sz Size object that describes the minimum dimensions that this Rect should be limited to.
	* @return  Rect object after the constrain operation
	*/
	CGUIRect& CGUIRect::ConstrainSizeMin(const CGUISize& sz)
	{
		if (GetWidth() < sz.m_fWidth)
		{
			SetWidth(sz.m_fWidth);
		}

		if (GetHeight() < sz.m_fHeight)
		{
			SetHeight(sz.m_fHeight);
		}

		return *this;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief check the size of the Rect object and if it is bigger than max_sz or 
	* smaller than min_sz, resize it so it isn't.
	* @param max_sz Size object that describes the maximum dimensions that this Rect should be limited to.
	* @param min_sz Size object that describes the minimum dimensions that this Rect should be limited to.
	* @return Rect object after the constrain operation
	*/
	CGUIRect& CGUIRect::ConstrainSize(const CGUISize& max_sz, const CGUISize& min_sz)
	{
		CGUISize curr_sz(GetSize());

		if (curr_sz.m_fWidth > max_sz.m_fWidth)
		{
			SetWidth(max_sz.m_fWidth);
		}
		else if (curr_sz.m_fWidth < min_sz.m_fWidth)
		{
			SetWidth(min_sz.m_fWidth);
		}

		if (curr_sz.m_fHeight > max_sz.m_fHeight)
		{
			SetHeight(max_sz.m_fHeight);
		}
		else if (curr_sz.m_fHeight < min_sz.m_fHeight)
		{
			SetHeight(min_sz.m_fHeight);
		}

		return *this;
	}
}	//namespace guiex


