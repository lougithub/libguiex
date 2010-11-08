/** 
* @file guirect.h
* @brief Rect used in this system
* @author ken
* @date 2006-04-24
*/

#ifndef __GUI_RECT_20060424_H__
#define __GUI_RECT_20060424_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector2.h"
#include <algorithm>

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
	class GUIEXPORT CGUIRect
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIRect(void);

		/**
		* @brief Constructor
		*/
		CGUIRect(real left, real top, real right, real bottom);

		/**
		* @brief Constructor
		*/
		CGUIRect( const CGUIVector2& pos, const CGUISize& sz);

		/**
		* @brief copy Constructor
		*/
		CGUIRect(const CGUIRect& rRect);

		/**
		* @brief get position of rect
		* @Return top-left position of Rect as a Point
		*/
		CGUIVector2	GetPosition(void) const;

		/**
		* @brief Set rect value
		*/
		void	SetRect(const CGUIRect& rRect);
		void	SetRect(real left, real top, real right, real bottom);
		void	SetRect(const CGUIVector2& pos, const CGUISize& sz);

		/**
		* @brief get width of rect
		* @return width of Rect area
		*/
		real	GetWidth(void) const;

		/**
		* @brief get height of rect
		* @return height of Rect area
		*/
		real	GetHeight(void) const;

		/**
		* @rief get size of the rect
		* @return the size of the Rect area
		*/
		CGUISize	GetSize(void) const;

		/**
		* @brief set position of the rect
		*/
		void	SetPosition(const CGUIVector2& pt);

		/**
		* @brief set the width of the Rect
		*/
		void	SetWidth(real width);

		/**
		* @brief set the height of the Rect object
		*/
		void	SetHeight(real height);

		/**
		* @brief set the size of the Rect area
		*/
		void	SetSize(const CGUISize& sze);

		/**
		* @brief get the intersection of this rect with the given rect.
		* @return a Rect that is the intersection of 'this' Rect with the Rect 'rect'
		* @note if the return rect's width is 0 or rect's height is 0, then this rect was outside rect.
		* @return the intersection rect
		*/
		CGUIRect	GetIntersection(const CGUIRect& rect) const;

		/**
		* @brief set an offset the Rect object
		* @param pt CGUIVector2 object containing the offsets to be applied to the Rect.
		* @return this Rect after the offset is performed
		*/
		CGUIRect&	Offset(const CGUIVector2& pt);

		/**
		* @brief judge is the given point falls within this rect
		* @param pt Point object describing the position to test.
		* @return true if given point is within this Rect's area, else false
		*/
		bool	IsPointInRect(const CGUIVector2& pt) const;

		/**
		* @brief constrain rect's size to given size if it is bigger than it.
		* @param sz Size object that describes the maximum dimensions that this Rect should be limited to.
		* @return 'this' Rect object after the constrain operation
		*/
		CGUIRect&	ConstrainSizeMax(const CGUISize& sz);


		/**
		* @brief constrain rect's size to given size if it is smaller than it.
		* @param sz Size object that describes the minimum dimensions that this Rect should be limited to.
		* @return  Rect object after the constrain operation
		*/
		CGUIRect&	ConstrainSizeMin(const CGUISize& sz);

		/**
		* @brief check the size of the Rect object and if it is bigger than max_sz or 
		* smaller than min_sz, resize it so it isn't.
		* @param max_sz Size object that describes the maximum dimensions that this Rect should be limited to.
		* @param min_sz Size object that describes the minimum dimensions that this Rect should be limited to.
		* @return Rect object after the constrain operation
		*/
		CGUIRect&	ConstrainSize(const CGUISize& max_sz, const CGUISize& min_sz);

		/** 
		* @brief compare operator
		*/
		bool	operator==(const CGUIRect& rhs) const;

		/** 
		* @brief compare operator
		*/
		bool	operator!=(const CGUIRect& rhs) const;

		/** 
		* @brief assign operator
		*/
		CGUIRect&	operator=(const CGUIRect& rhs);

		/** 
		* @brief operator for ponit
		*/
		CGUIRect operator+(const CGUIVector2& rPoint) const;

		CGUIRect operator+(const CGUIRect& rRect) const;

		const CGUIRect& operator+=(const CGUIRect& rRect);

		/** 
		* @brief algorithm operator
		*/
		CGUIRect operator*(real scalar) const;

		/** 
		* @brief algorithm operator
		*/
		const CGUIRect& operator*=(real scalar);

	public:
		real	m_fTop;
		real	m_fBottom;
		real	m_fLeft;
		real	m_fRight;
	};

}	//namespace guiex


//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	inline CGUIVector2	CGUIRect::GetPosition(void) const		
	{
		return CGUIVector2(m_fLeft, m_fTop);
	}
	//------------------------------------------------------------------------------
	inline real	CGUIRect::GetWidth(void) const		
	{
		return m_fRight - m_fLeft;
	}
	//------------------------------------------------------------------------------
	inline real	CGUIRect::GetHeight(void) const		
	{
		return m_fBottom - m_fTop;
	}
	//------------------------------------------------------------------------------
	inline CGUISize	CGUIRect::GetSize(void) const		
	{
		return CGUISize(GetWidth(), GetHeight());
	}
	//------------------------------------------------------------------------------
	inline void	CGUIRect::SetWidth(real width)		
	{
		m_fRight = m_fLeft + width;
	}
	//------------------------------------------------------------------------------
	inline void	CGUIRect::SetHeight(real height)		
	{
		m_fBottom = m_fTop + height;
	}
	//------------------------------------------------------------------------------
	inline void	CGUIRect::SetSize(const CGUISize& sze)	
	{
		SetWidth(sze.m_fWidth); 
		SetHeight(sze.m_fHeight);
	}
	//------------------------------------------------------------------------------
	inline void	CGUIRect::SetRect(const CGUIRect& rRect)
	{
		m_fLeft = rRect.m_fLeft;
		m_fRight = rRect.m_fRight;
		m_fTop = rRect.m_fTop;
		m_fBottom = rRect.m_fBottom;
	}
	//------------------------------------------------------------------------------
	inline void	CGUIRect::SetRect(real left, real top, real right, real bottom)
	{
		m_fLeft = left;
		m_fRight = right;
		m_fTop = top;
		m_fBottom = bottom;
	}
	//------------------------------------------------------------------------------
	inline void	CGUIRect::SetRect( const CGUIVector2& pos, const CGUISize& sz) 
	{
		m_fTop = pos.y;
		m_fBottom = pos.y + sz.m_fHeight;
		m_fLeft = pos.x;
		m_fRight = pos.x + sz.m_fWidth;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIRect::operator==(const CGUIRect& rhs) const
	{
		return ((m_fLeft == rhs.m_fLeft) && 
			(m_fRight == rhs.m_fRight) && 
			(m_fTop == rhs.m_fTop) &&
			(m_fBottom == rhs.m_fBottom));
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIRect::operator!=(const CGUIRect& rhs) const		
	{
		return !operator==(rhs);
	}
	//------------------------------------------------------------------------------
	inline CGUIRect CGUIRect::operator*(real scalar) const      
	{
		return CGUIRect(m_fLeft * scalar, m_fTop * scalar, m_fRight * scalar, m_fBottom * scalar); 
	}
	//------------------------------------------------------------------------------ 
	inline const CGUIRect& CGUIRect::operator*=(real scalar)    
	{
		m_fLeft *= scalar; m_fTop *= scalar; m_fRight *= scalar; m_fBottom *= scalar; return *this; 
	}
	//------------------------------------------------------------------------------
	inline CGUIRect& CGUIRect::Offset(const CGUIVector2& pt)
	{
		m_fLeft		+= pt.x;
		m_fRight	+= pt.x;
		m_fTop		+= pt.y;
		m_fBottom	+= pt.y;
		return *this;
	}
	//------------------------------------------------------------------------------
	inline CGUIRect& CGUIRect::operator=(const CGUIRect& rhs)
	{
		m_fLeft = rhs.m_fLeft;
		m_fTop = rhs.m_fTop;
		m_fRight = rhs.m_fRight;
		m_fBottom = rhs.m_fBottom;

		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIRect CGUIRect::operator+(const CGUIVector2& rPoint) const
	{
		return CGUIRect(m_fLeft + rPoint.x, m_fTop + rPoint.y, m_fRight + rPoint.x, m_fBottom + rPoint.y); 
	}
	//------------------------------------------------------------------------------ 
	inline CGUIRect CGUIRect::operator+(const CGUIRect& rRect) const
	{
		return CGUIRect(
			m_fLeft<rRect.m_fLeft ? m_fLeft : rRect.m_fLeft,
			m_fTop<rRect.m_fTop ? m_fTop : rRect.m_fTop,
			m_fRight>rRect.m_fRight ? m_fRight : rRect.m_fRight,
			m_fBottom>rRect.m_fBottom ? m_fBottom: rRect.m_fBottom );
	}
	//------------------------------------------------------------------------------ 
	inline const CGUIRect& CGUIRect::operator+=(const CGUIRect& rRect)
	{
		this->SetRect(CGUIRect(
			m_fLeft<rRect.m_fLeft ? m_fLeft : rRect.m_fLeft,
			m_fTop<rRect.m_fTop ? m_fTop : rRect.m_fTop,
			m_fRight>rRect.m_fRight ? m_fRight : rRect.m_fRight,
			m_fBottom>rRect.m_fBottom ? m_fBottom: rRect.m_fBottom));
		return *this;
	}
	//------------------------------------------------------------------------------ 
}
#endif	//__GUI_RECT_20060424_H__

