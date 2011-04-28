/** 
 * @file guivector4.cpp
 * @brief vector4 used in this system
 * @author ken
 * @date 2006-04-19
 */

//============================================================================//
// include
//============================================================================// 
#include "guivector3.h"
#include "guivector4.h"
//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------ 


namespace guiex
{
//============================================================================//
// function
//============================================================================// 
CGUIVector4::CGUIVector4()
:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}
//------------------------------------------------------------------------------
CGUIVector4::CGUIVector4( real fX, real fY, real fZ, real fW ) 
: x( fX ), y( fY ), z( fZ ), w( fW)
{
}
//------------------------------------------------------------------------------
CGUIVector4::CGUIVector4( real afCoordinate[4] )
: x( afCoordinate[0] ),
y( afCoordinate[1] ),
z( afCoordinate[2] ), 
w (afCoordinate[3] )
{
}
//------------------------------------------------------------------------------
CGUIVector4::CGUIVector4( int afCoordinate[4] )
{
	x = (real)afCoordinate[0];
	y = (real)afCoordinate[1];
	z = (real)afCoordinate[2];
	w = (real)afCoordinate[3];
}
//------------------------------------------------------------------------------
CGUIVector4::CGUIVector4( const real* const r )
: x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
{
}
//------------------------------------------------------------------------------
CGUIVector4::CGUIVector4( const CGUIVector4& rkVector )
: x( rkVector.x ), y( rkVector.y ), z( rkVector.z ), w (rkVector.w)
{
}
//------------------------------------------------------------------------------
real CGUIVector4::operator [] ( size_t i ) const
{
	assert( i < 4 );

	return *(&x+i);
}
//------------------------------------------------------------------------------
real& CGUIVector4::operator [] ( size_t i )
{
	assert( i < 4 );

	return *(&x+i);
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator = ( const CGUIVector4& rkVector )
{
	x = rkVector.x;
	y = rkVector.y;
	z = rkVector.z;            
	w = rkVector.w;            

	return *this;
}
//------------------------------------------------------------------------------
bool CGUIVector4::operator == ( const CGUIVector4& rkVector ) const
{
	return ( x == rkVector.x && 
		y == rkVector.y && 
		z == rkVector.z &&
		w == rkVector.w );
}
//------------------------------------------------------------------------------
bool CGUIVector4::operator != ( const CGUIVector4& rkVector ) const
{
	return ( x != rkVector.x || 
		y != rkVector.y || 
		z != rkVector.z ||
		w != rkVector.w );
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator = (const CGUIVector3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 1.0f;
	return *this;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator + ( const CGUIVector4& rkVector ) const
{
	CGUIVector4 kSum;

	kSum.x = x + rkVector.x;
	kSum.y = y + rkVector.y;
	kSum.z = z + rkVector.z;
	kSum.w = w + rkVector.w;

	return kSum;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator - ( const CGUIVector4& rkVector ) const
{
	CGUIVector4 kDiff;

	kDiff.x = x - rkVector.x;
	kDiff.y = y - rkVector.y;
	kDiff.z = z - rkVector.z;
	kDiff.w = w - rkVector.w;

	return kDiff;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator * ( real fScalar ) const
{
	CGUIVector4 kProd;

	kProd.x = fScalar*x;
	kProd.y = fScalar*y;
	kProd.z = fScalar*z;
	kProd.w = fScalar*w;

	return kProd;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator * ( const CGUIVector4& rhs) const
{
	CGUIVector4 kProd;

	kProd.x = rhs.x * x;
	kProd.y = rhs.y * y;
	kProd.z = rhs.z * z;
	kProd.w = rhs.w * w;

	return kProd;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator / ( real fScalar ) const
{
	assert( !GUI_REAL_EQUAL( fScalar, 0.0f ));

	CGUIVector4 kDiv;

	real fInv = 1.0f / fScalar;
	kDiv.x = x * fInv;
	kDiv.y = y * fInv;
	kDiv.z = z * fInv;
	kDiv.w = w * fInv;

	return kDiv;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator / ( const CGUIVector4& rhs) const
{
	CGUIVector4 kDiv;

	kDiv.x = x / rhs.x;
	kDiv.y = y / rhs.y;
	kDiv.z = z / rhs.z;
	kDiv.w = w / rhs.w;

	return kDiv;
}
//------------------------------------------------------------------------------
CGUIVector4 CGUIVector4::operator - () const
{
	CGUIVector4 kNeg;

	kNeg.x = -x;
	kNeg.y = -y;
	kNeg.z = -z;
	kNeg.w = -w;

	return kNeg;
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator += ( const CGUIVector4& rkVector )
{
	x += rkVector.x;
	y += rkVector.y;
	z += rkVector.z;
	w += rkVector.w;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator -= ( const CGUIVector4& rkVector )
{
	x -= rkVector.x;
	y -= rkVector.y;
	z -= rkVector.z;
	w -= rkVector.w;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator *= ( real fScalar )
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;
	return *this;
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator *= ( const CGUIVector4& rkVector )
{
	x *= rkVector.x;
	y *= rkVector.y;
	z *= rkVector.z;
	w *= rkVector.w;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator /= ( real fScalar )
{
	assert( !GUI_REAL_EQUAL( fScalar, 0.0f ));

	real fInv = 1.0f / fScalar;

	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;

	return *this;
}
//------------------------------------------------------------------------------
CGUIVector4& CGUIVector4::operator /= ( const CGUIVector4& rkVector )
{
	x /= rkVector.x;
	y /= rkVector.y;
	z /= rkVector.z;
	w /= rkVector.w;

	return *this;
}
//------------------------------------------------------------------------------
real CGUIVector4::DotProduct(const CGUIVector4& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
}


//------------------------------------------------------------------------------
CGUIVector4 operator * ( real fScalar, const CGUIVector4& rkVector )
{
	CGUIVector4 kProd;

	kProd.x = fScalar * rkVector.x;
	kProd.y = fScalar * rkVector.y;
	kProd.z = fScalar * rkVector.z;
	kProd.w = fScalar * rkVector.w;

	return kProd;
}

} //namespace guiex

