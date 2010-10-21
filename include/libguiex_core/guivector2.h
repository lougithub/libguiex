/** 
* @file guivector2.h
* @brief vector 2 used in this system
* @author ken
* @date 2006-04-19
*/

#ifndef __GUI_VECTOR2_H_20060419__
#define __GUI_VECTOR2_H_20060419__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisize.h"

//============================================================================//
// declare
//============================================================================// 



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIVector2
	{
	public:
		real x, y;        

		/**
		* @brief constructor
		*/
		CGUIVector2();

		/**
		* @brief constructor
		*/
		CGUIVector2( real fX, real fY );

		/**
		* @brief constructor
		*/
		CGUIVector2( real afCoordinate[2] );

		/**
		* @brief constructor
		*/
		CGUIVector2( int32 afCoordinate[2] );

		/**
		* @brief constructor
		*/
		CGUIVector2( const real* const r );

		/**
		* @brief copy constructor
		*/
		CGUIVector2( const CGUIVector2& rkVector );

		/**
		* @brief array operator
		*/
		real operator [] ( size_t i ) const;

		/**
		* @brief array operator
		* @return reference of coordinate value
		*/
		real& operator [] ( size_t i );

		/** Assigns the value of the other vector.
		* @param rkVector The other vector
		*/
		CGUIVector2& operator = ( const CGUIVector2& rkVector );

		/** compare this vector to other vector.
		* @param rkVector The other vector
		*/
		bool operator == ( const CGUIVector2& rkVector ) const;

		/** compare this vector to other vector.
		* @param rkVector The other vector
		*/
		bool operator != ( const CGUIVector2& rkVector ) const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2 operator + ( const CGUIVector2& rkVector ) const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2 operator - ( const CGUIVector2& rkVector ) const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2 operator * ( real fScalar ) const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2 operator * ( const CGUIVector2& rhs) const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2 operator / ( real fScalar ) const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2 operator - () const;

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2& operator += ( const CGUIVector2& rkVector );

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2& operator -= ( const CGUIVector2& rkVector );

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2& operator *= ( real fScalar );

		/** 
		* @brief arithmetic operations
		*/
		CGUIVector2& operator /= ( real fScalar );

		/**
		* @brief set the value of vector
		*/
		void Set( real fX, real fY);

		/** 
		* @brief get the length of the vector.
		* @warning This operation requires a square root and is expensive in
		* terms of CPU operations. If you don't need to know the exact
		* length (e.g. for just comparing lengths) use squaredLength()
		* instead.
		* @return Returns the length (magnitude) of the vector.
		*/
		real Length () const;

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
		real SquaredLength () const;

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
		real DotProduct(const CGUIVector2& vec) const;

		/** 
		* @brief Normalises the vector.
		* @remarks This method normalises the vector such that it's
		* length / magnitude is 1. The result is called a unit vector.
		* @note 	 This function will not crash for zero-sized vectors, but there
		* will be no changes made to their components.
		* @returns The previous length of the vector.
		*/
		real Normalise();

		/** 
		* @brief get a vector at a point half way between this and the passed in vector
		* @return a vector at a point half way between this and the passed
		*/
		CGUIVector2 MidPoint( const CGUIVector2& vec ) const;

		/** 
		* @brief compare two vectors
		* @return true if the vector's scalar components are all greater
		* that the ones of the vector it is compared against.
		*/
		bool operator < ( const CGUIVector2& rhs ) const;

		/** 
		* @brief compare two vectors
		* @return true if the vector's scalar components are all smaller
		* that the ones of the vector it is compared against.
		*/
		bool operator > ( const CGUIVector2& rhs ) const;	

		/** 
		* @brief Sets this vector's components to the minimum of its own and the 
		* ones of the passed in vector.
		* @remarks	'Minimum' in this case means the combination of the lowest
		* value of x, y and z from both vectors. Lowest is taken just
		* numerically, not magnitude, so -1 < 0.
		*/
		void MakeFloor( const CGUIVector2& cmp );

		/** 
		* @brief Sets this vector's components to the maximum of its own and the 
		* ones of the passed in vector.
		* @remarks 'Maximum' in this case means the combination of the highest
		* value of x, y and z from both vectors. Highest is taken just 
		* numerically, not magnitude, so 1 > -3.
		*/
		void MakeCeil( const CGUIVector2& cmp );

		/** 
		* @brief Generates a vector perpendicular to this vector (eg an 'up' vector).
		* @remarks	This method will return a vector which is perpendicular to this
		* vector. There are an infinite number of possibilities but this 
		* method will guarantee to generate one of them. If you need more 
		* control you should use the Quaternion class.
		*/
		CGUIVector2 Perpendicular(void) const;

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
		CGUIVector2 CrossProduct( const CGUIVector2& rkVector ) const;

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
		CGUIVector2 RandomDeviant( real angle) const;

		/**
		* @return Returns true if this vector is zero length. 
		*/
		bool IsZeroLength(void) const;

		/** 
		* @brief As normalise, except that this vector is unaffected and the
		* normalised vector is returned as a copy. 
		*/
		CGUIVector2 NormalisedCopy(void) const;

		/** 
		* @brief Calculates a reflection vector to the plane with the given normal. 
		* @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		*/
		CGUIVector2 Reflect(const CGUIVector2& normal) const;


	public:

		/**
		* @brief special points
		*/
		static const CGUIVector2 ZERO;

		/**
		* @brief special points
		*/
		static const CGUIVector2 UNIT_X;

		/**
		* @brief special points
		*/
		static const CGUIVector2 UNIT_Y;

		/**
		* @brief special points
		*/
		static const CGUIVector2 NEGATIVE_UNIT_X;

		/**
		* @brief special points
		*/
		static const CGUIVector2 NEGATIVE_UNIT_Y;

		/**
		* @brief special points
		*/
		static const CGUIVector2 UNIT_SCALE;

	private:

	};



	//============================================================================//
	// function
	//============================================================================// 
	extern CGUIVector2 operator * ( real fScalar, const CGUIVector2& rkVector );
	//------------------------------------------------------------------------------
	extern CGUISize operator * ( const CGUISize& rSize, const CGUIVector2& rkVector );
	//------------------------------------------------------------------------------
	extern CGUIVector2 operator * ( const CGUIVector2& rkVector, const CGUISize& rSize );
	//------------------------------------------------------------------------------

} //namespace guiex

#endif //__GUI_VECTOR2_H_20060419__

