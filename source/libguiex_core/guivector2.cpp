/** 
* @file guivector2.cpp
* @brief vector2 used in this system
* @author ken
* @date 2006-04-19
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guivector2.h>
#include <libguiex_core/guimath.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const CGUIVector2 CGUIVector2::ZERO( 0, 0);
	//------------------------------------------------------------------------------
	const CGUIVector2 CGUIVector2::UNIT_X( 1, 0);
	//------------------------------------------------------------------------------
	const CGUIVector2 CGUIVector2::UNIT_Y( 0, 1);
	//------------------------------------------------------------------------------
	const CGUIVector2 CGUIVector2::NEGATIVE_UNIT_X( -1,  0);
	//------------------------------------------------------------------------------
	const CGUIVector2 CGUIVector2::NEGATIVE_UNIT_Y(  0, -1);
	//------------------------------------------------------------------------------
	const CGUIVector2 CGUIVector2::UNIT_SCALE(1, 1);
	//------------------------------------------------------------------------------
	CGUIVector2::CGUIVector2()
		: x( 0.0f ), y( 0.0f )
	{

	}
	//------------------------------------------------------------------------------
	CGUIVector2::CGUIVector2( real fX, real fY ) 
		: x( fX ), y( fY )
	{
	}
	//------------------------------------------------------------------------------
	CGUIVector2::CGUIVector2( real afCoordinate[2] )
		: x( afCoordinate[0] ),
		y( afCoordinate[1] )
	{
	}
	//------------------------------------------------------------------------------
	CGUIVector2::CGUIVector2( int32 afCoordinate[2] )
	{
		x = (real)afCoordinate[0];
		y = (real)afCoordinate[1];
	}
	//------------------------------------------------------------------------------
	CGUIVector2::CGUIVector2( const real* const r )
		: x( r[0] ), y( r[1] )
	{
	}
	//------------------------------------------------------------------------------
	CGUIVector2::CGUIVector2( const CGUIVector2& rkVector )
		: x( rkVector.x ), y( rkVector.y )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIVector2::operator [] ( size_t i ) const
	{
		assert( i < 2 );

		return *(&x+i);
	}
	//------------------------------------------------------------------------------
	real& CGUIVector2::operator [] ( size_t i )
	{
		assert( i < 2 );

		return *(&x+i);
	}

	//------------------------------------------------------------------------------
	CGUIVector2& CGUIVector2::operator = ( const CGUIVector2& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;        

		return *this;
	}
	//------------------------------------------------------------------------------
	bool CGUIVector2::operator == ( const CGUIVector2& rkVector ) const
	{
		return ( x == rkVector.x && y == rkVector.y );
	}
	//------------------------------------------------------------------------------
	bool CGUIVector2::operator != ( const CGUIVector2& rkVector ) const
	{
		return ( x != rkVector.x || y != rkVector.y  );
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::operator + ( const CGUIVector2& rkVector ) const
	{
		CGUIVector2 kSum;

		kSum.x = x + rkVector.x;
		kSum.y = y + rkVector.y;

		return kSum;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::operator - ( const CGUIVector2& rkVector ) const
	{
		CGUIVector2 kDiff;

		kDiff.x = x - rkVector.x;
		kDiff.y = y - rkVector.y;

		return kDiff;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::operator * ( real fScalar ) const
	{
		CGUIVector2 kProd;

		kProd.x = fScalar*x;
		kProd.y = fScalar*y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::operator * ( const CGUIVector2& rhs) const
	{
		CGUIVector2 kProd;

		kProd.x = rhs.x * x;
		kProd.y = rhs.y * y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::operator / ( real fScalar ) const
	{
		assert( !GUI_REAL_EQUAL( fScalar, 0.0f ) );

		CGUIVector2 kDiv;

		real fInv = 1.0f / fScalar;
		kDiv.x = x * fInv;
		kDiv.y = y * fInv;

		return kDiv;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::operator - () const
	{
		CGUIVector2 kNeg;

		kNeg.x = -x;
		kNeg.y = -y;

		return kNeg;
	}
	//------------------------------------------------------------------------------
	CGUIVector2& CGUIVector2::operator += ( const CGUIVector2& rkVector )
	{
		x += rkVector.x;
		y += rkVector.y;

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIVector2& CGUIVector2::operator -= ( const CGUIVector2& rkVector )
	{
		x -= rkVector.x;
		y -= rkVector.y;

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIVector2& CGUIVector2::operator *= ( real fScalar )
	{
		x *= fScalar;
		y *= fScalar;

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIVector2& CGUIVector2::operator /= ( real fScalar )
	{
		assert( !GUI_REAL_EQUAL( fScalar, 0.0f ) );

		real fInv = 1.0f / fScalar;

		x *= fInv;
		y *= fInv;

		return *this;
	}
	//------------------------------------------------------------------------------
	void CGUIVector2::Set( real fX, real fY)
	{
		x = fX;
		y = fY;
	}
	//------------------------------------------------------------------------------
	real CGUIVector2::Length () const
	{
		return sqrt( x * x + y * y );
	}
	//------------------------------------------------------------------------------
	real CGUIVector2::SquaredLength () const
	{
		return x * x + y * y;
	}
	//------------------------------------------------------------------------------
	real CGUIVector2::DotProduct(const CGUIVector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}
	//------------------------------------------------------------------------------
	real CGUIVector2::Normalise()
	{
		real fLength = sqrt( x * x + y * y);

		// Will also work for zero-sized vectors, but will change nothing
		if(!GUI_REAL_EQUAL( fLength, 0.0f ))
		{
			real fInvLength = 1.0f / fLength;
			x *= fInvLength;
			y *= fInvLength;
		}

		return fLength;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::MidPoint( const CGUIVector2& vec ) const
	{
		return CGUIVector2( ( x + vec.x ) * 0.5f, ( y + vec.y ) * 0.5f );
	}
	//------------------------------------------------------------------------------
	bool CGUIVector2::operator < ( const CGUIVector2& rhs ) const
	{
		if( x < rhs.x && y < rhs.y )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	bool CGUIVector2::operator > ( const CGUIVector2& rhs ) const
	{
		if( x > rhs.x && y > rhs.y )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	void CGUIVector2::MakeFloor( const CGUIVector2& cmp )
	{
		if( cmp.x < x ) x = cmp.x;
		if( cmp.y < y ) y = cmp.y;
	}
	//------------------------------------------------------------------------------
	void CGUIVector2::MakeCeil( const CGUIVector2& cmp )
	{
		if( cmp.x > x ) x = cmp.x;
		if( cmp.y > y ) y = cmp.y;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::Perpendicular(void) const
	{         
		return CGUIVector2 (-y, x);
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::CrossProduct( const CGUIVector2& rkVector ) const
	{
		return CGUIVector2(-rkVector.y, rkVector.x);
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::RandomDeviant( real angle) const
	{
		angle *=  rand() * CGUIMath::GUI_PI;
		real cosa = cos(angle);
		real sina = sin(angle);
		return  CGUIVector2(cosa * x - sina * y, sina * x + cosa * y);
	}
	//------------------------------------------------------------------------------
	bool CGUIVector2::IsZeroLength(void) const
	{
		real sqlen = (x * x) + (y * y);
		return (sqlen < (1e-06 * 1e-06));
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::NormalisedCopy(void) const
	{
		CGUIVector2 ret = *this;
		ret.Normalise();
		return ret;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIVector2::Reflect(const CGUIVector2& normal) const
	{
		return CGUIVector2( *this - ( 2.0f * this->DotProduct(normal) * normal ) );
	}
	//------------------------------------------------------------------------------



	CGUIVector2 operator * ( real fScalar, const CGUIVector2& rkVector )
	{
		CGUIVector2 kProd;

		kProd.x = fScalar * rkVector.x;
		kProd.y = fScalar * rkVector.y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	extern CGUISize operator * ( const CGUISize& rSize, const CGUIVector2& rkVector )
	{
		return CGUISize(rSize.m_fWidth*rkVector.x, rSize.m_fHeight*rkVector.y);
	}
	//------------------------------------------------------------------------------
	extern CGUIVector2 operator * ( const CGUIVector2& rkVector, const CGUISize& rSize )
	{
		return CGUIVector2(rSize.m_fWidth*rkVector.x, rSize.m_fHeight*rkVector.y);
	}
	//------------------------------------------------------------------------------



}	//namespace guiex

