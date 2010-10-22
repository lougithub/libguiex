/** 
 * @file guivector3.cpp
 * @brief vector3 used in this system
 * @author ken
 * @date 2006-04-19
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guivector3.h>

//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------ 


namespace guiex
{
//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::ZERO( 0, 0, 0 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::UNIT_X( 1, 0, 0 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::UNIT_Y( 0, 1, 0 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::UNIT_Z( 0, 0, 1 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::NEGATIVE_UNIT_X( -1,  0,  0 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
//------------------------------------------------------------------------------
const CGUIVector3 CGUIVector3::UNIT_SCALE(1, 1, 1);
//------------------------------------------------------------------------------
CGUIVector3::CGUIVector3()
: x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}
//------------------------------------------------------------------------------
CGUIVector3::CGUIVector3( real fX, real fY, real fZ ) 
: x( fX ), y( fY ), z( fZ )
{
}
//------------------------------------------------------------------------------
CGUIVector3::CGUIVector3( real afCoordinate[3] )
: x( afCoordinate[0] ),
y( afCoordinate[1] ),
z( afCoordinate[2] )
{
}
//------------------------------------------------------------------------------
CGUIVector3::CGUIVector3( int afCoordinate[3] )
{
	x = (real)afCoordinate[0];
	y = (real)afCoordinate[1];
	z = (real)afCoordinate[2];
}
//------------------------------------------------------------------------------
CGUIVector3::CGUIVector3( const real* const r )
: x( r[0] ), y( r[1] ), z( r[2] )
{
}
//------------------------------------------------------------------------------
CGUIVector3::CGUIVector3( const CGUIVector3& rkVector )
: x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
{
}
//------------------------------------------------------------------------------
real CGUIVector3::operator [] ( size_t i ) const
{
	assert( i < 3 );

	return *(&x+i);
}
//------------------------------------------------------------------------------
real& CGUIVector3::operator [] ( size_t i )
{
	assert( i < 3 );

	return *(&x+i);
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator = ( const CGUIVector3& rkVector )
{
	x = rkVector.x;
	y = rkVector.y;
	z = rkVector.z;            

	return *this;
}
//------------------------------------------------------------------------------
bool CGUIVector3::operator == ( const CGUIVector3& rkVector ) const
{
	return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
}
//------------------------------------------------------------------------------
bool CGUIVector3::operator != ( const CGUIVector3& rkVector ) const
{
	return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator + ( const CGUIVector3& rkVector ) const
{
	CGUIVector3 kSum;

	kSum.x = x + rkVector.x;
	kSum.y = y + rkVector.y;
	kSum.z = z + rkVector.z;

	return kSum;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator - ( const CGUIVector3& rkVector ) const
{
	CGUIVector3 kDiff;

	kDiff.x = x - rkVector.x;
	kDiff.y = y - rkVector.y;
	kDiff.z = z - rkVector.z;

	return kDiff;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator * ( real fScalar ) const
{
	CGUIVector3 kProd;

	kProd.x = fScalar*x;
	kProd.y = fScalar*y;
	kProd.z = fScalar*z;

	return kProd;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator * ( const CGUIVector3& rhs) const
{
	CGUIVector3 kProd;

	kProd.x = rhs.x * x;
	kProd.y = rhs.y * y;
	kProd.z = rhs.z * z;

	return kProd;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator / ( real fScalar ) const
{
	assert( !GUI_REAL_EQUAL( fScalar, 0.0f ));

	CGUIVector3 kDiv;

	real fInv = 1.0f / fScalar;
	kDiv.x = x * fInv;
	kDiv.y = y * fInv;
	kDiv.z = z * fInv;

	return kDiv;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator / ( const CGUIVector3& rhs) const
{
	CGUIVector3 kDiv;

	kDiv.x = x / rhs.x;
	kDiv.y = y / rhs.y;
	kDiv.z = z / rhs.z;

	return kDiv;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::operator - () const
{
	CGUIVector3 kNeg;

	kNeg.x = -x;
	kNeg.y = -y;
	kNeg.z = -z;

	return kNeg;
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator += ( const CGUIVector3& rkVector )
{
	x += rkVector.x;
	y += rkVector.y;
	z += rkVector.z;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator -= ( const CGUIVector3& rkVector )
{
	x -= rkVector.x;
	y -= rkVector.y;
	z -= rkVector.z;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator *= ( real fScalar )
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	return *this;
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator *= ( const CGUIVector3& rkVector )
{
	x *= rkVector.x;
	y *= rkVector.y;
	z *= rkVector.z;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator /= ( real fScalar )
{
	assert( !GUI_REAL_EQUAL( fScalar, 0.0f ));

	real fInv = 1.0f / fScalar;

	x *= fInv;
	y *= fInv;
	z *= fInv;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector3& CGUIVector3::operator /= ( const CGUIVector3& rkVector )
{
	x /= rkVector.x;
	y /= rkVector.y;
	z /= rkVector.z;

	return *this;
}
//------------------------------------------------------------------------------
real CGUIVector3::Length () const
{
	return sqrt( x * x + y * y + z * z );
}
//------------------------------------------------------------------------------
real CGUIVector3::SquaredLength () const
{
	return x * x + y * y + z * z;
}
//------------------------------------------------------------------------------
real CGUIVector3::DotProduct(const CGUIVector3& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}
//------------------------------------------------------------------------------
real CGUIVector3::Normalise()
{
	real fLength = sqrt( x * x + y * y + z * z );

	// Will also work for zero-sized vectors, but will change nothing
	if ( !GUI_REAL_EQUAL(fLength,0.0f ))
	{
		real fInvLength = 1.0f / fLength;
		x *= fInvLength;
		y *= fInvLength;
		z *= fInvLength;
	}

	return fLength;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::CrossProduct( const CGUIVector3& rkVector ) const
{
	CGUIVector3 kCross;

	kCross.x = y * rkVector.z - z * rkVector.y;
	kCross.y = z * rkVector.x - x * rkVector.z;
	kCross.z = x * rkVector.y - y * rkVector.x;

	return kCross;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::MidPoint( const CGUIVector3& vec ) const
{
	return CGUIVector3( ( x + vec.x ) * 0.5f, 
		( y + vec.y ) * 0.5f, 
		( z + vec.z ) * 0.5f );
}
//------------------------------------------------------------------------------
bool CGUIVector3::operator < ( const CGUIVector3& rhs ) const
{
	if( x < rhs.x && y < rhs.y && z < rhs.z )
		return true;
	return false;
}
//------------------------------------------------------------------------------
bool CGUIVector3::operator > ( const CGUIVector3& rhs ) const
{
	if( x > rhs.x && y > rhs.y && z > rhs.z )
		return true;
	return false;
}
//------------------------------------------------------------------------------
void CGUIVector3::MakeFloor( const CGUIVector3& cmp )
{
	if( cmp.x < x ) x = cmp.x;
	if( cmp.y < y ) y = cmp.y;
	if( cmp.z < z ) z = cmp.z;
}
//------------------------------------------------------------------------------
void CGUIVector3::MakeCeil( const CGUIVector3& cmp )
{
	if( cmp.x > x ) x = cmp.x;
	if( cmp.y > y ) y = cmp.y;
	if( cmp.z > z ) z = cmp.z;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::Perpendicular(void) const
{
	static const real fSquareZero = 1e-06f * 1e-06f;

	CGUIVector3 perp = this->CrossProduct( CGUIVector3::UNIT_X );

	// Check length
	if( perp.SquaredLength() < fSquareZero )
	{
		/* This vector is the Y axis multiplied by a scalar, so we have 
		to use another axis.
		*/
		perp = this->CrossProduct( CGUIVector3::UNIT_Y );
	}

	return perp;
}
//------------------------------------------------------------------------------
bool CGUIVector3::IsZeroLength(void) const
{
	real sqlen = (x * x) + (y * y) + (z * z);
	return (sqlen < (1e-06 * 1e-06));

}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::NormalisedCopy(void) const
{
	CGUIVector3 ret = *this;
	ret.Normalise();
	return ret;
}
//------------------------------------------------------------------------------
CGUIVector3 CGUIVector3::Reflect(const CGUIVector3& normal) const
{
	return CGUIVector3( *this - ( 2 * this->DotProduct(normal) * normal ) );
}
//------------------------------------------------------------------------------
bool CGUIVector3::PositionEquals(const CGUIVector3& rhs) const
{
	return GUI_REAL_EQUAL(x, rhs.x) &&
		GUI_REAL_EQUAL(y, rhs.y) &&
		GUI_REAL_EQUAL(z, rhs.z);
}
//------------------------------------------------------------------------------


CGUIVector3 operator * ( real fScalar, const CGUIVector3& rkVector )
{
	CGUIVector3 kProd;

	kProd.x = fScalar * rkVector.x;
	kProd.y = fScalar * rkVector.y;
	kProd.z = fScalar * rkVector.z;

	return kProd;
}
} //namespace guiex

