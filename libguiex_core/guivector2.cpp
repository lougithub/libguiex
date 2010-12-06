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
	/**
	* @brief special points
	*/
	const CGUIVector2 CGUIVector2::ZERO( 0, 0);
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector2 CGUIVector2::UNIT_X( 1, 0);
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector2 CGUIVector2::UNIT_Y( 0, 1);
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector2 CGUIVector2::NEGATIVE_UNIT_X( -1,  0);
	//------------------------------------------------------------------------------

	/**
	* @brief special points
	*/
	const CGUIVector2 CGUIVector2::NEGATIVE_UNIT_Y(  0, -1);
	//------------------------------------------------------------------------------
	/**
	* @brief special points
	*/
	const CGUIVector2 CGUIVector2::UNIT_SCALE(1, 1);
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIVector2::CGUIVector2()
		: x( 0.0f ), y( 0.0f )
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIVector2::CGUIVector2( real fX, real fY ) 
		: x( fX ), y( fY )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIVector2::CGUIVector2( real afCoordinate[2] )
		: x( afCoordinate[0] ),
		y( afCoordinate[1] )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
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
	/**
	* @brief constructor
	*/
	CGUIVector2::CGUIVector2( const CGUIVector2& rkVector )
		: x( rkVector.x ), y( rkVector.y )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
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
	real CGUIVector2::GetX( ) const
	{
		return x;
	}
	//------------------------------------------------------------------------------
	real CGUIVector2::GetY( ) const
	{
		return y;
	}
	//------------------------------------------------------------------------------
	void CGUIVector2::SetX( real _x )
	{
		x = x;
	}
	//------------------------------------------------------------------------------
	void CGUIVector2::SetY( real _y )
	{
		y = _y;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get the length of the vector.
	* @warning This operation requires a square root and is expensive in
	* terms of CPU operations. If you don't need to know the exact
	* length (e.g. for just comparing lengths) use squaredLength()
	* instead.
	* @return Returns the length (magnitude) of the vector.
	*/
	real CGUIVector2::Length () const
	{
		return sqrt( x * x + y * y );
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
	real CGUIVector2::SquaredLength () const
	{
		return x * x + y * y;
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
	* @returns	A real representing the dot product value.
	*/
	real CGUIVector2::DotProduct(const CGUIVector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Normalises the vector.
	* @remarks This method normalises the vector such that it's
	* length / magnitude is 1. The result is called a unit vector.
	* @note 	 This function will not crash for zero-sized vectors, but there
	* will be no changes made to their components.
	* @returns The previous length of the vector.
	*/
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
	/** 
	* @brief get a vector at a point half way between this and the passed in vector
	* @return a vector at a point half way between this and the passed
	*/
	CGUIVector2 CGUIVector2::MidPoint( const CGUIVector2& vec ) const
	{
		return CGUIVector2( ( x + vec.x ) * 0.5f, ( y + vec.y ) * 0.5f );
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief compare two vectors
	* @return true if the vector's scalar components are all greater
	* that the ones of the vector it is compared against.
	*/
	bool CGUIVector2::operator < ( const CGUIVector2& rhs ) const
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
	bool CGUIVector2::operator > ( const CGUIVector2& rhs ) const
	{
		if( x > rhs.x && y > rhs.y )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Sets this vector's components to the minimum of its own and the 
	* ones of the passed in vector.
	* @remarks	'Minimum' in this case means the combination of the lowest
	* value of x, y and z from both vectors. Lowest is taken just
	* numerically, not magnitude, so -1 < 0.
	*/
	void CGUIVector2::MakeFloor( const CGUIVector2& cmp )
	{
		if( cmp.x < x ) x = cmp.x;
		if( cmp.y < y ) y = cmp.y;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Sets this vector's components to the maximum of its own and the 
	* ones of the passed in vector.
	* @remarks 'Maximum' in this case means the combination of the highest
	* value of x, y and z from both vectors. Highest is taken just 
	* numerically, not magnitude, so 1 > -3.
	*/
	void CGUIVector2::MakeCeil( const CGUIVector2& cmp )
	{
		if( cmp.x > x ) x = cmp.x;
		if( cmp.y > y ) y = cmp.y;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Generates a vector perpendicular to this vector (eg an 'up' vector).
	* @remarks	This method will return a vector which is perpendicular to this
	* vector. There are an infinite number of possibilities but this 
	* method will guarantee to generate one of them. If you need more 
	* control you should use the Quaternion class.
	*/
	CGUIVector2 CGUIVector2::Perpendicular(void) const
	{         
		return CGUIVector2 (-y, x);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Calculates the cross-product of 2 vectors, i.e. the vector that
	* lies perpendicular to them both.
	* @remarks The cross-product is normally used to calculate the normal
	* vector of a plane, by calculating the cross-product of 2
	* non-equivalent vectors which lie on the plane (e.g. 2 edges of a triangle).
	* @param vec Vector which, together with this one, will be used to
	* calculate the cross-product.
	* @returns A vector which is the result of the cross-product. This
	* vector will <b>NOT</b> be normalised, to maximise efficiency
	* - call Vector3::normalise on the result if you wish this to
	* be done. As for which side the resultant vector will be on, the
	* returned vector will be on the side from which the arc from 'this'
	* to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z) 
	* = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
	* @param For a clearer explanation, look a the left and the bottom edges
	* of your monitor's screen. Assume that the first vector is the
	* left edge and the second vector is the bottom edge, both of
	* them starting from the lower-left corner of the screen. The
	* resulting vector is going to be perpendicular to both of them
	* and will go <i>inside</i> the screen, towards the cathode tube
	* (assuming you're using a CRT monitor, of course).
	*/
	CGUIVector2 CGUIVector2::CrossProduct( const CGUIVector2& rkVector ) const
	{
		return CGUIVector2(-rkVector.y, rkVector.x);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Generates a new random vector which deviates from this vector by a
	* given angle in a random direction.
	* @remarks This method assumes that the random number generator has already 
	* been seeded appropriately.
	* @param angle The angle at which to deviate in radians
	* @param up Any vector perpendicular to this one (which could generated 
	* by cross-product of this vector and any other non-colinear 
	* vector). If you choose not to provide this the function will 
	* derive one on it's own, however if you provide one yourself the 
	* function will be faster (this allows you to reuse up vectors if 
	* you call this method more than once) 
	* @return A random vector which deviates from this vector by angle. This 
	* vector will not be normalised, normalise it if you wish afterwards.
	*/
	CGUIVector2 CGUIVector2::RandomDeviant( real angle) const
	{
		angle *=  rand() * CGUIMath::GUI_PI;
		real cosa = cos(angle);
		real sina = sin(angle);
		return  CGUIVector2(cosa * x - sina * y, sina * x + cosa * y);
	}
	//------------------------------------------------------------------------------
	/**
	* @return Returns true if this vector is zero length. 
	*/
	bool CGUIVector2::IsZeroLength(void) const
	{
		real sqlen = (x * x) + (y * y);
		return (sqlen < (1e-06 * 1e-06));
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief As normalise, except that this vector is unaffected and the
	* normalised vector is returned as a copy. 
	*/
	CGUIVector2 CGUIVector2::NormalisedCopy(void) const
	{
		CGUIVector2 ret = *this;
		ret.Normalise();
		return ret;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Calculates a reflection vector to the plane with the given normal. 
	* @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
	*/
	CGUIVector2 CGUIVector2::Reflect(const CGUIVector2& normal) const
	{
		return CGUIVector2( *this - ( 2.0f * this->DotProduct(normal) * normal ) );
	}
	//------------------------------------------------------------------------------


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

