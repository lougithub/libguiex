/** 
 * @file guivector4.h
 * @brief vector 4 used in this system
 * @author ken
 * @date 2006-04-19
 */

#ifndef __GUI_VECTOR4_H_20060419__
#define __GUI_VECTOR4_H_20060419__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


namespace guiex
{
//============================================================================//
// class
//============================================================================// 
class GUIEXPORT CGUIVector4
{
public:
	union 
	{
		struct 
		{
			real x, y, z, w;        
		};
		real val[4];
	};

public:
	/** 
	 * @brief constructor
	 */
	CGUIVector4();

	/** 
	 * @brief constructor
	 */
	CGUIVector4( real fX, real fY, real fZ, real fW ) ;

	/** 
	 * @brief constructor
	 */
	CGUIVector4( real afCoordinate[4] );

	/** 
	 * @brief constructor
	 */
	CGUIVector4( int afCoordinate[4] );

	/** 
	 * @brief constructor
	 */
	CGUIVector4( const real* const r );
	/** 
	 * @brief copy constructor
	 */
	CGUIVector4( const CGUIVector4& rkVector );

	/** 
	 * @brief array operator
	 * 
	 * @param i index of array
	 * 
	 * @return value of coordinate value
	 */
	real operator [] ( size_t i ) const;

	/** 
	 * @brief array operator
	 * 
	 * @param i index of array
	 * 
	 * @return reference of coordinate value
	 */
	real& operator [] ( size_t i );

	/** 
	 * @brief Assigns the value of the other vector.
	 * @param rkVector The other vector
	 */
	CGUIVector4& operator = ( const CGUIVector4& rkVector );

	/** 
	 * @brief compare operator
	 */
	bool operator == ( const CGUIVector4& rkVector ) const;

	/** 
	 * @brief compare operator
	 */
	bool operator != ( const CGUIVector4& rkVector ) const;

	/** 
	 * @brief assign operator
	 */
	CGUIVector4& operator = (const CGUIVector3& rhs);

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator + ( const CGUIVector4& rkVector ) const;

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator - ( const CGUIVector4& rkVector ) const;
	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator * ( real fScalar ) const;

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator * ( const CGUIVector4& rhs) const;

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator / ( real fScalar ) const;

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator / ( const CGUIVector4& rhs) const;


	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4 operator - () const;
	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4& operator += ( const CGUIVector4& rkVector );

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4& operator -= ( const CGUIVector4& rkVector );

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4& operator *= ( real fScalar );
	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4& operator *= ( const CGUIVector4& rkVector );

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4& operator /= ( real fScalar );

	/** 
	 * @brief arithmetic operator
	 */
	CGUIVector4& operator /= ( const CGUIVector4& rkVector );

	/** 
	 * @brief Calculates the dot (scalar) product of this vector with another.
	 * @param vec Vector with which to calculate the dot product (together with this one).
	 * @returns	A real representing the dot product value.
	 */
	real DotProduct(const CGUIVector4& vec) const;
};


//============================================================================//
// function
//============================================================================// 
extern CGUIVector4 operator * ( real fScalar, const CGUIVector4& rkVector );


}//namespace guiex

#endif //__GUI_VECTOR4_H_20060419__



