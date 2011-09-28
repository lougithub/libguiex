/** 
* @file guivector3.cpp
* @brief vector3 used in this system
* @author ken
* @date 2006-04-19
*/

//============================================================================//
// include
//============================================================================// 
#include "guivector3.h"
#include "guivector2.h"
#include "guiquaternion.h"
#include "guimath.h"
#include "guirotator.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 

//============================================================================//
// function
//============================================================================// 

namespace guiex
{

	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::ZERO( 0, 0, 0 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::UNIT_X( 1, 0, 0 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::UNIT_Y( 0, 1, 0 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::UNIT_Z( 0, 0, 1 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::NEGATIVE_UNIT_X( -1,  0,  0 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector3 CGUIVector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
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
	CGUIVector3::CGUIVector3( const CGUIVector2& rVec )
		: x( rVec.x )
		, y( rVec.y )
		, z( 0 )
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
	/** 
	* @brief Returns the length (magnitude) of the vector.
	* @warning	This operation requires a square root and is expensive in
	* terms of CPU operations. If you don't need to know the exact
	* length (e.g. for just comparing lengths) use squaredLength()	instead.
	* @return the length (magnitude) of the vector.
	*/
	real CGUIVector3::Length () const
	{
		return sqrt( x * x + y * y + z * z );
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief the square of the length(magnitude) of the vector.
	* @remarks	This  method is for efficiency - calculating the actual
	* length of a vector requires a square root, which is expensive
	* in terms of the operations required. This method returns the
	* square of the length of the vector, i.e. the same as the
	* length but before the square root is taken. Use this if you
	* want to find the longest / shortest vector without incurring
	* the square root.
	* @return the square of the length(magnitude) of the vector.
	*/
	real CGUIVector3::SquaredLength () const
	{
		return x * x + y * y + z * z;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Calculates the dot (scalar) product of this vector with another.
	* @remarks The dot product can be used to calculate the angle between 2
	* vectors. If both are unit vectors, the dot product is the
	* cosine of the angle; otherwise the dot product must be
	* divided by the product of the lengths of both vectors to get
	* the cosine of the angle. This result can further be used to
	* calculate the distance of a point from a plane.
	* @param vec Vector with which to calculate the dot product (together
	* with this one).
	* @return A real representing the dot product value.
	*/
	real CGUIVector3::DotProduct(const CGUIVector3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Normalises the vector.
	* @remarks This method normalises the vector such that it's
	* length / magnitude is 1. The result is called a unit vector.
	* @note This function will not crash for zero-sized vectors, but there
	* will be no changes made to their components.
	* @returns The previous length of the vector.
	*/
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
	/** 
	* @ brief Calculates the cross-product of 2 vectors, i.e. the vector that
	* lies perpendicular to them both.
	* @remarks The cross-product is normally used to calculate the normal
	* vector of a plane, by calculating the cross-product of 2
	* non-equivalent vectors which lie on the plane (e.g. 2 edges
	* of a triangle).
	* @param vec Vector which, together with this one, will be used to
	* calculate the cross-product.
	* @return A vector which is the result of the cross-product. This
	* vector will <b>NOT</b> be normalised, to maximise efficiency
	* - call CGUIVector3::normalise on the result if you wish this to
	*   be done. As for which side the resultant vector will be on, the
	*   returned vector will be on the side from which the arc from 'this'
	*   to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z) 
	*   = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
	* @param For a clearer explanation, look a the left and the bottom edges
	* of your monitor's screen. Assume that the first vector is the
	* left edge and the second vector is the bottom edge, both of
	* them starting from the lower-left corner of the screen. The
	* resulting vector is going to be perpendicular to both of them
	* and will go <i>inside</i> the screen, towards the cathode tube
	* (assuming you're using a CRT monitor, of course).
	*/
	CGUIVector3 CGUIVector3::CrossProduct( const CGUIVector3& rkVector ) const
	{
		CGUIVector3 kCross;

		kCross.x = y * rkVector.z - z * rkVector.y;
		kCross.y = z * rkVector.x - x * rkVector.z;
		kCross.z = x * rkVector.y - y * rkVector.x;

		return kCross;
	}
	//------------------------------------------------------------------------------
	/** 
	* @return a vector at a point half way between this and the passed in vector.
	*/
	CGUIVector3 CGUIVector3::MidPoint( const CGUIVector3& vec ) const
	{
		return CGUIVector3( ( x + vec.x ) * 0.5f, 
			( y + vec.y ) * 0.5f, 
			( z + vec.z ) * 0.5f );
	}
	//------------------------------------------------------------------------------
	/**
	* @return true if the vector's scalar components are all greater
	* that the ones of the vector it is compared against.
	*/
	bool CGUIVector3::operator < ( const CGUIVector3& rhs ) const
	{
		if( x < rhs.x && y < rhs.y && z < rhs.z )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	/** 
	* @return true if the vector's scalar components are all smaller
	* that the ones of the vector it is compared against.
	*/
	bool CGUIVector3::operator > ( const CGUIVector3& rhs ) const
	{
		if( x > rhs.x && y > rhs.y && z > rhs.z )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Sets this vector's components to the minimum of its own and the 
	* ones of the passed in vector.
	* @remarks 'Minimum' in this case means the combination of the lowest
	* value of x, y and z from both vectors. Lowest is taken just
	* numerically, not magnitude, so -1 < 0.
	*/
	void CGUIVector3::MakeFloor( const CGUIVector3& cmp )
	{
		if( cmp.x < x ) x = cmp.x;
		if( cmp.y < y ) y = cmp.y;
		if( cmp.z < z ) z = cmp.z;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Sets this vector's components to the maximum of its own and the 
	* ones of the passed in vector.
	* @remarks 'Maximum' in this case means the combination of the highest
	* value of x, y and z from both vectors. Highest is taken just
	* numerically, not magnitude, so 1 > -3.
	*/
	void CGUIVector3::MakeCeil( const CGUIVector3& cmp )
	{
		if( cmp.x > x ) x = cmp.x;
		if( cmp.y > y ) y = cmp.y;
		if( cmp.z > z ) z = cmp.z;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Generates a vector perpendicular to this vector (eg an 'up' vector).
	* @remarks This method will return a vector which is perpendicular to this
	* vector. There are an infinite number of possibilities but this 
	* method will guarantee to generate one of them. If you need more 
	* control you should use the Quaternion class.
	*/
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
	/** 
	* @return true if this vector is zero length. 
	*/
	bool CGUIVector3::IsZeroLength(void) const
	{
		real sqlen = (x * x) + (y * y) + (z * z);
		return (sqlen < (1e-06 * 1e-06));

	}
	//------------------------------------------------------------------------------
	/** 
	* @brief As normalise, except that this vector is unaffected and the
	* normalised vector is returned as a copy. 
	*/
	CGUIVector3 CGUIVector3::NormalisedCopy(void) const
	{
		CGUIVector3 ret = *this;
		ret.Normalise();
		return ret;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Calculates a reflection vector to the plane with the given normal . 
	* @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
	*/
	CGUIVector3 CGUIVector3::Reflect(const CGUIVector3& normal) const
	{
		return CGUIVector3( *this - ( 2 * this->DotProduct(normal) * normal ) );
	}
	//------------------------------------------------------------------------------
	/** 
	* @return whether this vector is within a positional tolerance
	* of another vector.
	* @param rhs The vector to compare with
	* and still be considered equal
	*/
	bool CGUIVector3::PositionEquals(const CGUIVector3& rhs) const
	{
		return GUI_REAL_EQUAL(x, rhs.x) &&
			GUI_REAL_EQUAL(y, rhs.y) &&
			GUI_REAL_EQUAL(z, rhs.z);
	}
	//------------------------------------------------------------------------------
	/** Generates a new random vector which deviates from this vector by a
	given angle in a random direction.
	@remarks
	This method assumes that the random number generator has already
	been seeded appropriately.
	@param
	angle The angle at which to deviate
	@param
	up Any vector perpendicular to this one (which could generated
	by cross-product of this vector and any other non-colinear
	vector). If you choose not to provide this the function will
	derive one on it's own, however if you provide one yourself the
	function will be faster (this allows you to reuse up vectors if
	you call this method more than once)
	@returns
	A random vector which deviates from this vector by angle. This
	vector will not be normalised, normalise it if you wish
	afterwards.
	*/
	CGUIVector3 CGUIVector3::RandomDeviant( real angle, const CGUIVector3& up ) const
	{
		CGUIVector3 newUp;

		if (up == CGUIVector3::ZERO)
		{
			// Generate an up vector
			newUp = this->Perpendicular();
		}
		else
		{
			newUp = up;
		}

		// Rotate up vector by random amount around this
		CGUIQuaternion q;
		q.FromAngleAxis( CGUIMath::UnitRandom() * CGUIMath::GUI_TWO_PI, *this );
		newUp = q * newUp;

		// Finally rotate this by given angle around randomised up
		q.FromAngleAxis( angle, newUp );
		return q * (*this);
	}
	//------------------------------------------------------------------------------
	real CGUIVector3::operator|( const CGUIVector3& v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	//------------------------------------------------------------------------------
	CGUIVector3 CGUIVector3::operator^( const CGUIVector3& V ) const
	{
		return CGUIVector3
			(
			y * V.z - z * V.y,
			z * V.x - x * V.z,
			x * V.y - y * V.x
			);
	}
	//------------------------------------------------------------------------------
	CGUIRotator CGUIVector3::Rotation() const
	{
		CGUIRotator R;

		// Find yaw.
		R.Yaw = atan2f(y,x) * 180.f / CGUIMath::GUI_PI;

		// Find pitch.
		R.Pitch = atan2f(z,sqrtf(x*x+y*y)) * 180.f / CGUIMath::GUI_PI;

		// Find roll.
		R.Roll = 0;

		return R;
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

