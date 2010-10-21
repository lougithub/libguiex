/** 
* @file guivector3.h
* @brief vector 3 used in this system
* @author ken
* @date 2006-04-19
*/

#ifndef __GUI_VECTOR3_H_20060419__
#define __GUI_VECTOR3_H_20060419__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIVector3
	{
	public:
		union 
		{
			struct
			{
				real x, y, z;        
			};
			real val[3];
		};

	public:

		/** 
		* @brief constructor
		*/
		CGUIVector3();

		/** 
		* @brief constructor
		* 
		* @param fX x coordinate value
		* @param fY y coordinate value
		* @param fZ z coordinate value
		*/
		CGUIVector3( real fX, real fY, real fZ );

		/** 
		* @brief constructor
		* 
		* @param afCoordinate array which contain coordinate value
		*/
		CGUIVector3( real afCoordinate[3] );

		/** 
		* @brief constructor
		* 
		* @param afCoordinate array which contain coordinate value
		*/
		CGUIVector3( int32 afCoordinate[3] );

		/** 
		* @brief constructor
		* 
		* @param r array which contain coordinate value
		*/
		CGUIVector3( const real* const r );

		/** 
		* @brief copy constructor
		* 
		* @param rkVector reference of CGUIVector3 object
		*/
		CGUIVector3( const CGUIVector3& rkVector );

		/** 
		* @brief array operator
		* 
		* @param i index of value
		* 
		* @return value of coordinate value
		*/
		real operator [] ( size_t i ) const;

		/** 
		* @brief array operator
		* 
		* @param i index of reference
		* 
		* @return reference of coordinate value
		*/
		real& operator [] ( size_t i );

		/**
		* @brief Assigns the value of the other vector.
		* @param rkVector The other vector
		*/
		CGUIVector3& operator = ( const CGUIVector3& rkVector );

		/** 
		* @brief compare operator
		*/
		bool operator == ( const CGUIVector3& rkVector ) const;

		/** 
		* @brief compare operator
		*/
		bool operator != ( const CGUIVector3& rkVector ) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator + ( const CGUIVector3& rkVector ) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator - ( const CGUIVector3& rkVector ) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator * ( real fScalar ) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator * ( const CGUIVector3& rhs) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator / ( real fScalar ) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator / ( const CGUIVector3& rhs) const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3 operator - () const;

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3& operator += ( const CGUIVector3& rkVector );

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3& operator -= ( const CGUIVector3& rkVector );

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3& operator *= ( real fScalar );

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3& operator *= ( const CGUIVector3& rkVector );

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3& operator /= ( real fScalar );

		/**
		* @brief arithmetic operations
		*/
		CGUIVector3& operator /= ( const CGUIVector3& rkVector );


		/** 
		* @brief Returns the length (magnitude) of the vector.
		* @warning	This operation requires a square root and is expensive in
		* terms of CPU operations. If you don't need to know the exact
		* length (e.g. for just comparing lengths) use squaredLength()	instead.
		* @return the length (magnitude) of the vector.
		*/
		real Length () const;

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
		* @return A real representing the dot product value.
		*/
		real DotProduct(const CGUIVector3& vec) const;

		/** 
		* @brief Normalises the vector.
		* @remarks This method normalises the vector such that it's
		* length / magnitude is 1. The result is called a unit vector.
		* @note This function will not crash for zero-sized vectors, but there
		* will be no changes made to their components.
		* @returns The previous length of the vector.
		*/
		real Normalise();

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
		CGUIVector3 CrossProduct( const CGUIVector3& rkVector ) const;

		/** 
		* @return a vector at a point half way between this and the passed in vector.
		*/
		CGUIVector3 MidPoint( const CGUIVector3& vec ) const;

		/**
		* @return true if the vector's scalar components are all greater
		* that the ones of the vector it is compared against.
		*/
		bool operator < ( const CGUIVector3& rhs ) const;

		/** 
		* @return true if the vector's scalar components are all smaller
		* that the ones of the vector it is compared against.
		*/
		bool operator > ( const CGUIVector3& rhs ) const;

		/** 
		* @brief Sets this vector's components to the minimum of its own and the 
		* ones of the passed in vector.
		* @remarks 'Minimum' in this case means the combination of the lowest
		* value of x, y and z from both vectors. Lowest is taken just
		* numerically, not magnitude, so -1 < 0.
		*/
		void MakeFloor( const CGUIVector3& cmp );

		/** 
		* @brief Sets this vector's components to the maximum of its own and the 
		* ones of the passed in vector.
		* @remarks 'Maximum' in this case means the combination of the highest
		* value of x, y and z from both vectors. Highest is taken just
		* numerically, not magnitude, so 1 > -3.
		*/
		void MakeCeil( const CGUIVector3& cmp );

		/** 
		* @brief Generates a vector perpendicular to this vector (eg an 'up' vector).
		* @remarks This method will return a vector which is perpendicular to this
		* vector. There are an infinite number of possibilities but this 
		* method will guarantee to generate one of them. If you need more 
		* control you should use the Quaternion class.
		*/
		CGUIVector3 Perpendicular(void) const;

		/** 
		* @return true if this vector is zero length. 
		*/
		bool IsZeroLength(void) const;

		/** 
		* @brief As normalise, except that this vector is unaffected and the
		* normalised vector is returned as a copy. 
		*/
		CGUIVector3 NormalisedCopy(void) const;

		/** 
		* @brief Calculates a reflection vector to the plane with the given normal . 
		* @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		*/
		CGUIVector3 Reflect(const CGUIVector3& normal) const;

		/** 
		* @return whether this vector is within a positional tolerance
		* of another vector.
		* @param rhs The vector to compare with
		* and still be considered equal
		*/
		bool PositionEquals(const CGUIVector3& rhs ) const;

		/**
		* @brief special points
		*/
		static const CGUIVector3 ZERO;

		/**
		* @brief special points
		*/
		static const CGUIVector3 UNIT_X;

		/**
		* @brief special points
		*/
		static const CGUIVector3 UNIT_Y;

		/**
		* @brief special points
		*/
		static const CGUIVector3 UNIT_Z;

		/**
		* @brief special points
		*/
		static const CGUIVector3 NEGATIVE_UNIT_X;

		/**
		* @brief special points
		*/
		static const CGUIVector3 NEGATIVE_UNIT_Y;

		/**
		* @brief special points
		*/
		static const CGUIVector3 NEGATIVE_UNIT_Z;

		/**
		* @brief special points
		*/
		static const CGUIVector3 UNIT_SCALE;


	private:
	};

	//============================================================================//
	// function
	//============================================================================// 
	extern CGUIVector3 operator * ( real fScalar, const CGUIVector3& rkVector );

}//namespace guiex
#endif //__GUI_VECTOR3_H_20060419__

