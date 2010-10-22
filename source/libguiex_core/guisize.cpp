/** 
* @file guisize.cpp
* @brief size used in libguiex system
* @author ken
* @date 2006-04-24
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guisize.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUISize::CGUISize()
		:CGUIPropertyable( ePropertyType_Size )
		,m_fWidth(0.0f)
		,m_fHeight(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	CGUISize::CGUISize(real fWidth, real fHeight)
		:CGUIPropertyable( ePropertyType_Size )
		,m_fWidth(fWidth)
		,m_fHeight(fHeight)
	{
	}
	//------------------------------------------------------------------------------
	CGUISize::CGUISize(const CGUISize& rSize)
	{
		SetSize(rSize);
	}
	//------------------------------------------------------------------------------
	void	CGUISize::SetSize( const CGUISize& rSize)
	{
		m_fWidth = rSize.m_fWidth;
		m_fHeight = rSize.m_fHeight;
	}
	//------------------------------------------------------------------------------
	void	CGUISize::SetWidth( real fWidth)
	{
		m_fWidth = fWidth;
	}
	//------------------------------------------------------------------------------
	real	CGUISize::GetWidth( ) const 
	{
		return m_fWidth;
	}
	//------------------------------------------------------------------------------
	void	CGUISize::SetHeight( real fHeight)
	{
		m_fHeight = fHeight;
	}
	//------------------------------------------------------------------------------
	real	CGUISize::GetHeight(  ) const 
	{
		return m_fHeight;
	}
	//------------------------------------------------------------------------------
	bool	CGUISize::IsEqualZero() const
	{
		return GUI_REAL_EQUAL(0, m_fWidth * m_fHeight );
	}
	//------------------------------------------------------------------------------
	bool	CGUISize::IsEqual( const CGUISize& rSize ) const
	{
		if( GUI_REAL_EQUAL( m_fWidth, other.m_fWidth ) &&
			GUI_REAL_EQUAL( m_fHeight, other.m_fHeight ))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator= (const CGUISize& rSize)
	{
		SetSize( rSize );
		return *this;
	}
	//------------------------------------------------------------------------------
	bool CGUISize::operator==(const CGUISize& other) const
	{
		if( m_fWidth == other.m_fWidth &&
			m_fHeight == other.m_fHeight )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	bool CGUISize::operator!=(const CGUISize& other) const
	{
		return !(*this == other);
	}
	//------------------------------------------------------------------------------
	bool CGUISize::operator<( const CGUISize& other ) const
	{
		return( m_fWidth < other.m_fWidth ||
			!(other.m_fWidth< m_fWidth) && m_fHeight < other.m_fHeight);
	}
	//------------------------------------------------------------------------------
	CGUISize CGUISize::operator+( const CGUISize& other ) const
	{
		return CGUISize( m_fWidth+other.m_fWidth, m_fHeight+other.m_fHeight );
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator+=( const CGUISize& other )
	{
		m_fWidth += other.m_fWidth;
		m_fHeight += other.m_fHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUISize CGUISize::operator-( const CGUISize& other ) const
	{
		return CGUISize( m_fWidth-other.m_fWidth, m_fHeight-other.m_fHeight );
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator-=(const CGUISize& other)
	{
		m_fWidth -= other.m_fWidth;
		m_fHeight -= other.m_fHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUISize CGUISize::operator*(real fScalar) const
	{
		return CGUISize( m_fWidth*fScalar, m_fHeight*fScalar );
	}
	//------------------------------------------------------------------------------
	CGUISize CGUISize::operator*(const CGUISize& other) const
	{
		return CGUISize( m_fWidth*other.m_fWidth, m_fHeight*other.m_fHeight );
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator*=(real fScalar)
	{
		m_fWidth *= fScalar;
		m_fHeight *= fScalar;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator*=(const CGUISize& other)
	{
		m_fWidth *= other.m_fWidth;
		m_fHeight *= other.m_fHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUISize CGUISize::operator/(real fScalar) const
	{
		assert( !GUI_REAL_EQUAL( fScalar, 0.0f ) );
		return CGUISize( m_fWidth/fScalar, m_fHeight/fScalar );
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator/=(real fScalar)
	{
		assert( !GUI_REAL_EQUAL( fScalar, 0.0f ) );
		m_fWidth /= fScalar;
		m_fHeight /= fScalar;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUISize CGUISize::operator/(const CGUISize& other) const
	{
		assert( !GUI_REAL_EQUAL( other.m_fWidth, 0.0f ) );
		assert( !GUI_REAL_EQUAL( other.m_fHeight, 0.0f ) );
		return CGUISize( m_fWidth/other.m_fWidth, m_fHeight/other.m_fHeight );
	}
	//------------------------------------------------------------------------------
	CGUISize& CGUISize::operator/=(const CGUISize& other)
	{
		assert( !GUI_REAL_EQUAL( other.m_fWidth, 0.0f ) );
		assert( !GUI_REAL_EQUAL( other.m_fHeight, 0.0f ) );
		m_fWidth /= other.m_fWidth;
		m_fHeight /= other.m_fHeight;
		return *this;
	}
	//------------------------------------------------------------------------------
	void CGUISize::SaveToProperty( CGUIProperty& rProperty ) const
	{
		rProperty.SetValue( guiex::CGUIStringConvertor::SizeToString( this ));
	}
	//------------------------------------------------------------------------------
	void CGUISize::LoadFromProperty( const CGUIProperty& rProperty )
	{
		guiex::CGUIStringConvertor::StringToSize( rProperty.GetValue(), *this );
	}
	//------------------------------------------------------------------------------

}//namespace guiex


