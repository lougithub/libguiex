/** 
* @file guiintvector2.cpp
* @brief vector 2 used in this system
* @author ken
* @date 2011-01-31
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiintvector2.h>
#include <libguiex_core/guimath.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIIntVector2::CGUIIntVector2()
		: x( 0 ), y( 0 )
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIIntVector2::CGUIIntVector2( int32 nX, int32 nY ) 
		: x( nX ), y( nY )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIIntVector2::CGUIIntVector2( const CGUIIntVector2& rkVector )
		: x( rkVector.x ), y( rkVector.y )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	int32 CGUIIntVector2::operator [] ( size_t i ) const
	{
		assert( i < 2 );

		return *(&x+i);
	}
	//------------------------------------------------------------------------------

	int32& CGUIIntVector2::operator [] ( size_t i )
	{
		assert( i < 2 );

		return *(&x+i);
	}

	//------------------------------------------------------------------------------
	CGUIIntVector2& CGUIIntVector2::operator = ( const CGUIIntVector2& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;        

		return *this;
	}
	//------------------------------------------------------------------------------
	bool CGUIIntVector2::operator == ( const CGUIIntVector2& rkVector ) const
	{
		return ( x == rkVector.x && y == rkVector.y );
	}
	//------------------------------------------------------------------------------
	bool CGUIIntVector2::operator != ( const CGUIIntVector2& rkVector ) const
	{
		return ( x != rkVector.x || y != rkVector.y  );
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2 CGUIIntVector2::operator + ( const CGUIIntVector2& rkVector ) const
	{
		CGUIIntVector2 kSum;

		kSum.x = x + rkVector.x;
		kSum.y = y + rkVector.y;

		return kSum;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2 CGUIIntVector2::operator - ( const CGUIIntVector2& rkVector ) const
	{
		CGUIIntVector2 kDiff;

		kDiff.x = x - rkVector.x;
		kDiff.y = y - rkVector.y;

		return kDiff;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2 CGUIIntVector2::operator * ( int32 nScalar ) const
	{
		CGUIIntVector2 kProd;

		kProd.x = nScalar*x;
		kProd.y = nScalar*y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2 CGUIIntVector2::operator * ( const CGUIIntVector2& rhs) const
	{
		CGUIIntVector2 kProd;

		kProd.x = rhs.x * x;
		kProd.y = rhs.y * y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2 CGUIIntVector2::operator / ( int32 nScalar ) const
	{
		assert( nScalar != 0 ) ;

		CGUIIntVector2 kDiv;
		kDiv.x /= nScalar;
		kDiv.y /= nScalar;

		return kDiv;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2 CGUIIntVector2::operator - () const
	{
		CGUIIntVector2 kNeg;

		kNeg.x = -x;
		kNeg.y = -y;

		return kNeg;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2& CGUIIntVector2::operator += ( const CGUIIntVector2& rkVector )
	{
		x += rkVector.x;
		y += rkVector.y;

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2& CGUIIntVector2::operator -= ( const CGUIIntVector2& rkVector )
	{
		x -= rkVector.x;
		y -= rkVector.y;

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2& CGUIIntVector2::operator *= ( int32 nScalar )
	{
		x *= nScalar;
		y *= nScalar;

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIIntVector2& CGUIIntVector2::operator /= ( int32 nScalar )
	{
		assert( nScalar != 0 );

		x /= nScalar;
		y /= nScalar;

		return *this;
	}
	//------------------------------------------------------------------------------
	void CGUIIntVector2::Set( int32 nX, int32 nY)
	{
		x = nX;
		y = nY;
	}
	//------------------------------------------------------------------------------
	int32 CGUIIntVector2::GetX( ) const
	{
		return x;
	}
	//------------------------------------------------------------------------------
	int32 CGUIIntVector2::GetY( ) const
	{
		return y;
	}
	//------------------------------------------------------------------------------
	void CGUIIntVector2::SetX( int32 _x )
	{
		x = x;
	}
	//------------------------------------------------------------------------------
	void CGUIIntVector2::SetY( int32 _y )
	{
		y = _y;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the square of the length of the vector
	* @remarks This  method is for efficiency - calculating the actual
	* length of a vector requires a square root, which is expensive
	* in terms of the operations required. This method returns the
	* square of the length of the vector, i.e. the same as the
	* length but before the square root is taken. Use this if you
	* want to find the longest / shortest vector without incurring
	* the square root.
	* @return Returns the square of the length(magnitude) of the vector.
	*/
	int32 CGUIIntVector2::SquaredLength () const
	{
		return x * x + y * y;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief compare two vectors
	* @return true if the vector's scalar components are all greater
	* that the ones of the vector it is compared against.
	*/
	bool CGUIIntVector2::operator < ( const CGUIIntVector2& rhs ) const
	{
		if( x < rhs.x && y < rhs.y )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief compare two vectors
	* @return true if the vector's scalar components are all smaller
	* that the ones of the vector it is compared against.
	*/
	bool CGUIIntVector2::operator > ( const CGUIIntVector2& rhs ) const
	{
		if( x > rhs.x && y > rhs.y )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	/**
	* @return Returns true if this vector is zero length. 
	*/
	bool CGUIIntVector2::IsZeroLength(void) const
	{
		int32 sqlen = (x * x) + (y * y);
		return (sqlen < (1e-06 * 1e-06));
	}
	//------------------------------------------------------------------------------
}	//namespace guiex

