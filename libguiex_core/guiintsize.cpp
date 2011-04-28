/** 
* @file guisize.cpp
* @brief size used in libguiex system
* @author ken
* @date 2006-04-24
*/

//============================================================================//
// include
//============================================================================// 
#include "guiintsize.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIIntSize::CGUIIntSize()
		:m_uWidth(0)
		,m_uHeight(0)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIIntSize::CGUIIntSize(uint32 uWidth, uint32 uHeight)
		:m_uWidth(uWidth)
		,m_uHeight(uHeight)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIIntSize::CGUIIntSize(const CGUIIntSize& rSize)
	{
		SetSize(rSize);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set size
	*/
	void CGUIIntSize::SetSize( const CGUIIntSize& rSize)
	{
		m_uWidth = rSize.m_uWidth;
		m_uHeight = rSize.m_uHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set width and height
	*/
	void CGUIIntSize::SetValue( uint32 uWidth, uint32 uHeight )
	{
		m_uWidth = uWidth;
		m_uHeight = uHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set width
	*/
	void CGUIIntSize::SetWidth( uint32 uWidth)
	{
		m_uWidth = uWidth;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set width
	*/
	uint32 CGUIIntSize::GetWidth( ) const 
	{
		return m_uWidth;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set height
	*/
	void CGUIIntSize::SetHeight( uint32 uHeight)
	{
		m_uHeight = uHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get height
	*/
	uint32 CGUIIntSize::GetHeight(  ) const 
	{
		return m_uHeight;
	}
	//------------------------------------------------------------------------------
	bool CGUIIntSize::IsEqualZero() const
	{
		return ( m_uWidth == 0 && m_uHeight == 0 );
	}
	//------------------------------------------------------------------------------
	bool CGUIIntSize::IsEqual( const CGUIIntSize& rSize ) const
	{
		if( m_uWidth == rSize.m_uWidth &&
			m_uHeight == rSize.m_uHeight )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator= (const CGUIIntSize& rSize)
	{
		SetSize( rSize );
		return *this;
	}
	//------------------------------------------------------------------------------
	bool CGUIIntSize::operator==(const CGUIIntSize& other) const
	{
		if( m_uWidth == other.m_uWidth &&
			m_uHeight == other.m_uHeight )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIIntSize::operator!=(const CGUIIntSize& other) const
	{
		return !(*this == other);
	}
	//------------------------------------------------------------------------------
	bool CGUIIntSize::operator<( const CGUIIntSize& other ) const
	{
		return( m_uWidth < other.m_uWidth ||
			!(other.m_uWidth< m_uWidth) && m_uHeight < other.m_uHeight);
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIIntSize::operator+( const CGUIIntSize& other ) const
	{
		return CGUIIntSize( m_uWidth+other.m_uWidth, m_uHeight+other.m_uHeight );
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator+=( const CGUIIntSize& other )
	{
		m_uWidth += other.m_uWidth;
		m_uHeight += other.m_uHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIIntSize::operator-( const CGUIIntSize& other ) const
	{
		return CGUIIntSize( m_uWidth-other.m_uWidth, m_uHeight-other.m_uHeight );
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator-=(const CGUIIntSize& other)
	{
		m_uWidth -= other.m_uWidth;
		m_uHeight -= other.m_uHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIIntSize::operator*(uint32 uScalar) const
	{
		return CGUIIntSize( m_uWidth*uScalar, m_uHeight*uScalar );
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIIntSize::operator*(const CGUIIntSize& other) const
	{
		return CGUIIntSize( m_uWidth*other.m_uWidth, m_uHeight*other.m_uHeight );
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator*=(uint32 uScalar)
	{
		m_uWidth *= uScalar;
		m_uHeight *= uScalar;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator*=(const CGUIIntSize& other)
	{
		m_uWidth *= other.m_uWidth;
		m_uHeight *= other.m_uHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIIntSize::operator/(uint32 uScalar) const
	{
		assert( uScalar != 0 );
		return CGUIIntSize( m_uWidth/uScalar, m_uHeight/uScalar );
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator/=(uint32 uScalar)
	{
		assert( uScalar != 0 );
		m_uWidth /= uScalar;
		m_uHeight /= uScalar;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIIntSize::operator/(const CGUIIntSize& other) const
	{
		assert( other.m_uWidth != 0 );
		assert( other.m_uHeight != 0 );
		return CGUIIntSize( m_uWidth/other.m_uWidth, m_uHeight/other.m_uHeight );
	}
	//------------------------------------------------------------------------------
	CGUIIntSize& CGUIIntSize::operator/=(const CGUIIntSize& other)
	{
		assert( other.m_uWidth != 0 );
		assert( other.m_uHeight != 0 );
		m_uWidth /= other.m_uWidth;
		m_uHeight /= other.m_uHeight;
		return *this;
	}
	//------------------------------------------------------------------------------

}//namespace guiex


