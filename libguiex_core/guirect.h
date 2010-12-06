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


 //============================================================================//
 // class
 //============================================================================// 

 namespace guiex
{
	class GUIEXPORT CGUIRect
	{
	public:
		CGUIRect(void);
		CGUIRect(real left, real top, real right, real bottom);
		CGUIRect( const CGUIVector2& pos, const CGUISize& sz);
		CGUIRect(const CGUIRect& rRect);

		CGUIVector2	GetPosition(void) const;

		void SetRect(const CGUIRect& rRect);
		void SetRect(real left, real top, real right, real bottom);
		void SetRect(const CGUIVector2& pos, const CGUISize& sz);

		real GetWidth(void) const;
		real GetHeight(void) const;
		CGUISize GetSize(void) const;
		void SetWidth(real width);
		void SetHeight(real height);
		void SetSize(const CGUISize& sze);
		void SetPosition(const CGUIVector2& pt);

		CGUIRect GetIntersection(const CGUIRect& rect) const;

		CGUIRect& Offset(const CGUIVector2& pt);

		bool IsPointInRect(const CGUIVector2& pt) const;

		CGUIRect& ConstrainSizeMax(const CGUISize& sz);
		CGUIRect& ConstrainSizeMin(const CGUISize& sz);
		CGUIRect& ConstrainSize(const CGUISize& max_sz, const CGUISize& min_sz);

		bool IsEqual( const CGUIRect& rhs ) const;

		bool operator==(const CGUIRect& rhs) const;
		bool operator!=(const CGUIRect& rhs) const;
		CGUIRect& operator=(const CGUIRect& rhs);
		CGUIRect operator+(const CGUIVector2& rPoint) const;
		CGUIRect operator+(const CGUIRect& rRect) const;
		const CGUIRect& operator+=(const CGUIRect& rRect);
		CGUIRect operator*(real scalar) const;
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
	/**
	* @brief get position of rect
	* @Return top-left position of Rect as a Point
	*/
	inline CGUIVector2 CGUIRect::GetPosition(void) const		
	{
		return CGUIVector2(m_fLeft, m_fTop);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get width of rect
	* @return width of Rect area
	*/
	inline real	CGUIRect::GetWidth(void) const		
	{
		return m_fRight - m_fLeft;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get height of rect
	* @return height of Rect area
	*/
	inline real	CGUIRect::GetHeight(void) const		
	{
		return m_fBottom - m_fTop;
	}
	//------------------------------------------------------------------------------
	/**
	* @rief get size of the rect
	* @return the size of the Rect area
	*/
	inline CGUISize	CGUIRect::GetSize(void) const		
	{
		return CGUISize(GetWidth(), GetHeight());
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the width of the Rect
	*/
	inline void	CGUIRect::SetWidth(real width)		
	{
		m_fRight = m_fLeft + width;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the height of the Rect object
	*/
	inline void	CGUIRect::SetHeight(real height)		
	{
		m_fBottom = m_fTop + height;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the size of the Rect area
	*/
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
	inline bool CGUIRect::IsEqual( const CGUIRect& rhs ) const
	{
		return (GUI_REAL_EQUAL(m_fLeft , rhs.m_fLeft) && 
			GUI_REAL_EQUAL(m_fRight , rhs.m_fRight) && 
			GUI_REAL_EQUAL(m_fTop , rhs.m_fTop) &&
			GUI_REAL_EQUAL(m_fBottom , rhs.m_fBottom));
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
	/**
	* @brief set an offset the Rect object
	* @param pt CGUIVector2 object containing the offsets to be applied to the Rect.
	* @return this Rect after the offset is performed
	*/
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

