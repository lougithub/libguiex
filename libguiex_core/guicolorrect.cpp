/** 
* @file guicolorrect.cpp
* @brief color rect, used by render
* @author ken
* @date 2006-07-05
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guicolorrect.h>


//============================================================================//
// function 
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	/**
	* @brief Default constructor
	*/
	CGUIColorRect::CGUIColorRect(void)
		:m_top_left()
		,m_top_right()
		,m_bottom_left()
		,m_bottom_right()
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief Constructor for CGUIColorRect objects (via single colour).
	*/
	CGUIColorRect::CGUIColorRect(const CGUIColor& col)
		:m_top_left(col)
		,m_top_right(col)
		,m_bottom_left(col)
		,m_bottom_right(col)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Constructor for CGUIColorRect objects
	*/
	CGUIColorRect::CGUIColorRect(const CGUIColor& top_left, const CGUIColor& top_right, 
		const CGUIColor& bottom_left, const CGUIColor& bottom_right)
		:m_top_left(top_left)
		,m_top_right(top_right)
		,m_bottom_left(bottom_left)
		,m_bottom_right(bottom_right)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set the alpha value to use for all four corners of the CGUIColorRect.
	*/
	void CGUIColorRect::SetAlpha(real alpha)
	{
		m_top_left.SetAlpha(alpha);
		m_top_right.SetAlpha(alpha);
		m_bottom_left.SetAlpha(alpha);
		m_bottom_right.SetAlpha(alpha);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Set the colour of all four corners simultaneously.
	*/
	void CGUIColorRect::SetColours(const CGUIColor& col)
	{
		m_top_left = m_top_right = m_bottom_left = m_bottom_right = col;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Determinate whether the CGUIColorRect is monochromatic or variegated.
	*/
	bool CGUIColorRect::IsMonochromatic() const
	{
		return m_top_left == m_top_right &&
			m_top_left == m_bottom_left &&
			m_top_left == m_bottom_right;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Gets a portion of this CGUIColorRect as a subset CGUIColorRect
	* @return A CGUIColorRect from the specified range
	*/
	CGUIColorRect CGUIColorRect::GetSubRectangle( real left, real right, real top, real bottom ) const
	{
		return CGUIColorRect(
			GetColorAtPoint(left, top),
			GetColorAtPoint(right, top),
			GetColorAtPoint(left, bottom),
			GetColorAtPoint(right, bottom)
			);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Get the colour at a point in the rectangle
	*/
	CGUIColor CGUIColorRect::GetColorAtPoint( real x, real y ) const
	{
		CGUIColor h1((m_top_right - m_top_left) * x + m_top_left);
		CGUIColor h2((m_bottom_right - m_bottom_left) * x + m_bottom_left);
		return CGUIColor((h2 - h1) * y + h1);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Module the alpha components of each corner's colour by a constant.
	*/
	void CGUIColorRect::ModulateAlpha(real alpha)
	{
		m_top_left.SetAlpha(m_top_left.GetAlpha()*alpha);
		m_top_right.SetAlpha(m_top_right.GetAlpha()*alpha);
		m_bottom_left.SetAlpha(m_bottom_left.GetAlpha()*alpha);
		m_bottom_right.SetAlpha(m_bottom_right.GetAlpha()*alpha);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Modulate all components of this colour rect with corresponding components from another colour rect.
	*/
	CGUIColorRect& CGUIColorRect::operator*=(const CGUIColorRect& other)
	{
		m_top_left *= other.m_top_left;
		m_top_right *= other.m_top_right;
		m_bottom_left *= other.m_bottom_left;
		m_bottom_right *= other.m_bottom_right;

		return *this;
	}
	//------------------------------------------------------------------------------

}//namespaceguiex
