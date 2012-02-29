/** 
* @file guimath.h
* @brief math utility
* @author ken
* @date 2007-11-23
*/


#ifndef __GUI_MATH_20071123_H__
#define __GUI_MATH_20071123_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include <limits>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIDegree;
	class CGUIVector3;
	class CGUIVector4;
	class CGUIMatrix4;
	class CGUIPlane;
}



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** Wrapper class which indicates a given angle value is in Radians.
	@remarks CGUIRadian values are interchangeable with CGUIDegree values, and conversions
	will be done automatically between them.
	*/
	class GUIEXPORT CGUIRadian
	{
		real mRad;

	public:
		explicit CGUIRadian ( real r=0 ) : mRad(r) {}
		CGUIRadian ( const CGUIDegree& d );
		const CGUIRadian& operator = ( const real& f ) { mRad = f; return *this; }
		const CGUIRadian& operator = ( const CGUIRadian& r ) { mRad = r.mRad; return *this; }
		const CGUIRadian& operator = ( const CGUIDegree& d );

		real valueDegrees() const; // see bottom of this file
		real valueRadians() const { return mRad; }

		CGUIRadian operator + ( const CGUIRadian& r ) const { return CGUIRadian ( mRad + r.mRad ); }
		CGUIRadian operator + ( const CGUIDegree& d ) const;
		CGUIRadian& operator += ( const CGUIRadian& r ) { mRad += r.mRad; return *this; }
		CGUIRadian& operator += ( const CGUIDegree& d );
		CGUIRadian operator - () { return CGUIRadian(-mRad); }
		CGUIRadian operator - ( const CGUIRadian& r ) const { return CGUIRadian ( mRad - r.mRad ); }
		CGUIRadian operator - ( const CGUIDegree& d ) const;
		CGUIRadian& operator -= ( const CGUIRadian& r ) { mRad -= r.mRad; return *this; }
		CGUIRadian& operator -= ( const CGUIDegree& d );
		CGUIRadian operator * ( real f ) const { return CGUIRadian ( mRad * f ); }
		CGUIRadian operator * ( const CGUIRadian& f ) const { return CGUIRadian ( mRad * f.mRad ); }
		CGUIRadian& operator *= ( real f ) { mRad *= f; return *this; }
		CGUIRadian operator / ( real f ) const { return CGUIRadian ( mRad / f ); }
		CGUIRadian& operator /= ( real f ) { mRad /= f; return *this; }

		bool operator <  ( const CGUIRadian& r ) const { return mRad <  r.mRad; }
		bool operator <= ( const CGUIRadian& r ) const { return mRad <= r.mRad; }
		bool operator == ( const CGUIRadian& r ) const { return mRad == r.mRad; }
		bool operator != ( const CGUIRadian& r ) const { return mRad != r.mRad; }
		bool operator >= ( const CGUIRadian& r ) const { return mRad >= r.mRad; }
		bool operator >  ( const CGUIRadian& r ) const { return mRad >  r.mRad; }
	};

	/** Wrapper class which indicates a given angle value is in Degrees.
	@remarks CGUIDegree values are interchangeable with CGUIRadian values, and conversions
	will be done automatically between them.
	*/
	class GUIEXPORT CGUIDegree
	{
		real mDeg; // if you get an error here - make sure to define/typedef 'real' first

	public:
		explicit CGUIDegree ( real d=0 ) : mDeg(d) {}
		CGUIDegree ( const CGUIRadian& r ) : mDeg(r.valueDegrees()) {}
		const CGUIDegree& operator = ( const real& f ) { mDeg = f; return *this; }
		const CGUIDegree& operator = ( const CGUIDegree& d ) { mDeg = d.mDeg; return *this; }
		const CGUIDegree& operator = ( const CGUIRadian& r ) { mDeg = r.valueDegrees(); return *this; }

		real valueDegrees() const { return mDeg; }
		real valueRadians() const; // see bottom of this file

		CGUIDegree operator + ( const CGUIDegree& d ) const { return CGUIDegree ( mDeg + d.mDeg ); }
		CGUIDegree operator + ( const CGUIRadian& r ) const { return CGUIDegree ( mDeg + r.valueDegrees() ); }
		CGUIDegree& operator += ( const CGUIDegree& d ) { mDeg += d.mDeg; return *this; }
		CGUIDegree& operator += ( const CGUIRadian& r ) { mDeg += r.valueDegrees(); return *this; }
		CGUIDegree operator - () { return CGUIDegree(-mDeg); }
		CGUIDegree operator - ( const CGUIDegree& d ) const { return CGUIDegree ( mDeg - d.mDeg ); }
		CGUIDegree operator - ( const CGUIRadian& r ) const { return CGUIDegree ( mDeg - r.valueDegrees() ); }
		CGUIDegree& operator -= ( const CGUIDegree& d ) { mDeg -= d.mDeg; return *this; }
		CGUIDegree& operator -= ( const CGUIRadian& r ) { mDeg -= r.valueDegrees(); return *this; }
		CGUIDegree operator * ( real f ) const { return CGUIDegree ( mDeg * f ); }
		CGUIDegree operator * ( const CGUIDegree& f ) const { return CGUIDegree ( mDeg * f.mDeg ); }
		CGUIDegree& operator *= ( real f ) { mDeg *= f; return *this; }
		CGUIDegree operator / ( real f ) const { return CGUIDegree ( mDeg / f ); }
		CGUIDegree& operator /= ( real f ) { mDeg /= f; return *this; }

		bool operator <  ( const CGUIDegree& d ) const { return mDeg <  d.mDeg; }
		bool operator <= ( const CGUIDegree& d ) const { return mDeg <= d.mDeg; }
		bool operator == ( const CGUIDegree& d ) const { return mDeg == d.mDeg; }
		bool operator != ( const CGUIDegree& d ) const { return mDeg != d.mDeg; }
		bool operator >= ( const CGUIDegree& d ) const { return mDeg >= d.mDeg; }
		bool operator >  ( const CGUIDegree& d ) const { return mDeg >  d.mDeg; }
	};


	/** Class to provide access to common mathematical functions.
	*/
	class GUIEXPORT CGUIMath 
	{
	public:
		static real UnitRandom ();  // in [0,1]

		static real RangeRandom (real fLow, real fHigh);  // in [fLow,fHigh]

		static real SymmetricRandom ();  // in [-1,1]

		static real DegreesToRadians(real degrees) ;
		static real RadiansToDegrees(real radians);

		/** Checks wether a given point is inside a triangle, in a
		2-dimensional (Cartesian) space.
		@remarks
		The vertices of the triangle must be given in either
		trigonometrical (anticlockwise) or inverse trigonometrical
		(clockwise) order.
		@param
		px The X-coordinate of the point.
		@param
		py The Y-coordinate of the point.
		@param
		ax The X-coordinate of the triangle's first vertex.
		@param
		ay The Y-coordinate of the triangle's first vertex.
		@param
		bx The X-coordinate of the triangle's second vertex.
		@param
		by The Y-coordinate of the triangle's second vertex.
		@param
		cx The X-coordinate of the triangle's third vertex.
		@param
		cy The Y-coordinate of the triangle's third vertex.
		@returns
		If the point resides in the triangle, <b>true</b> is
		returned.
		@par
		If the point is outside the triangle, <b>false</b> is
		returned.
		*/
		static bool pointInTri2D( real px, real pz, real ax, real az, real bx, real bz, real cx, real cz );

		/** Compare 2 reals, using tolerance for inaccuracies.
		*/
		static bool RealEqual(real a, real b,real tolerance = std::numeric_limits<real>::epsilon());

		/** Calculates the tangent space vector for a given set of positions / texture coords. */
		static CGUIVector3 calculateTangentSpaceVector(
			const CGUIVector3& position1, const CGUIVector3& position2, const CGUIVector3& position3,
			real u1, real v1, real u2, real v2, real u3, real v3);

		/** Build a reflection matrix for the passed in plane. */
		static CGUIMatrix4 buildReflectionMatrix(const CGUIPlane& p);
		/** Calculate a face normal, including the w component which is the offset from the origin. */
		static CGUIVector4 calculateFaceNormal(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3);
		/** Calculate a face normal, no w-information. */
		static CGUIVector3 calculateBasicFaceNormal(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3);
		/** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
		static CGUIVector4 calculateFaceNormalWithoutNormalize(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3);
		/** Calculate a face normal without normalize, no w-information. */
		static CGUIVector3 calculateBasicFaceNormalWithoutNormalize(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3);

		static uint16 GetNextPOT( uint16 value );

		static const real GUI_POS_INFINITY;
		static const real GUI_NEG_INFINITY;
		static const real GUI_PI;
		static const real GUI_TWO_PI;
		static const real GUI_HALF_PI;
		static const real GUI_Deg2Rad;
		static const real GUI_Rad2Deg;
	};


	// these functions could not be defined within the class definition of class
	// CGUIRadian because they required class CGUIDegree to be defined
	inline CGUIRadian::CGUIRadian ( const CGUIDegree& d ) : mRad(d.valueRadians())
	{
	}
	inline const CGUIRadian& CGUIRadian::operator = ( const CGUIDegree& d ) 
	{
		mRad = d.valueRadians(); return *this;
	}
	inline CGUIRadian CGUIRadian::operator + ( const CGUIDegree& d ) const
	{
		return CGUIRadian ( mRad + d.valueRadians() );
	}
	inline CGUIRadian& CGUIRadian::operator += ( const CGUIDegree& d ) 
	{
		mRad += d.valueRadians();
		return *this;
	}

	inline CGUIRadian CGUIRadian::operator - ( const CGUIDegree& d ) const 
	{
		return CGUIRadian ( mRad - d.valueRadians() );
	}
	inline CGUIRadian& CGUIRadian::operator -= ( const CGUIDegree& d ) 
	{
		mRad -= d.valueRadians();
		return *this;
	}

	// these functions must be defined down here, because they rely on the
	// angle unit conversion functions in class CGUIMath:

	inline real CGUIRadian::valueDegrees() const
	{
		return CGUIMath::RadiansToDegrees ( mRad );
	}

	inline real CGUIDegree::valueRadians() const
	{
		return CGUIMath::DegreesToRadians ( mDeg );
	}

	inline CGUIRadian operator * ( real a, const CGUIRadian& b )
	{
		return CGUIRadian ( a * b.valueRadians() );
	}

	inline CGUIRadian operator / ( real a, const CGUIRadian& b )
	{
		return CGUIRadian ( a / b.valueRadians() );
	}

	inline CGUIDegree operator * ( real a, const CGUIDegree& b )
	{
		return CGUIDegree ( a * b.valueDegrees() );
	}

	inline CGUIDegree operator / ( real a, const CGUIDegree& b )
	{
		return CGUIDegree ( a / b.valueDegrees() );
	}

	template<class _Ty> inline
		const _Ty& GUIMax(const _Ty& _Left, const _Ty& _Right )
	{
		return (( _Left < _Right) ? _Right : _Left);
	}

	template<class _Ty> inline
		const _Ty& GUIMin(const _Ty& _Left, const _Ty& _Right )
	{
		return (( _Right < _Left ) ? _Right : _Left);
	}

	template<class T> inline
		const T& GUIClamp(const T& X, const T& Min, const T& Max )
	{
		return X<Min ? Min : X<Max ? X : Max;
	}

}	//namespace guiex
#endif
