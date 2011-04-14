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
		CGUIVector3();
		CGUIVector3( real fX, real fY, real fZ );
		CGUIVector3( real afCoordinate[3] );
		CGUIVector3( int32 afCoordinate[3] );
		CGUIVector3( const real* const r );
		CGUIVector3( const CGUIVector3& rkVector );

		real Length () const;
		real SquaredLength () const;
		bool IsZeroLength(void) const;

		real DotProduct(const CGUIVector3& vec) const;
		CGUIVector3 CrossProduct( const CGUIVector3& rkVector ) const;

		real Normalise();
		CGUIVector3 NormalisedCopy(void) const;

		CGUIVector3 MidPoint( const CGUIVector3& vec ) const;

		void MakeFloor( const CGUIVector3& cmp );
		void MakeCeil( const CGUIVector3& cmp );

		CGUIVector3 Perpendicular(void) const;

		CGUIVector3 Reflect(const CGUIVector3& normal) const;

		bool PositionEquals(const CGUIVector3& rhs ) const;
		CGUIVector3 RandomDeviant( real angle, const CGUIVector3& up = CGUIVector3::ZERO ) const;

		real operator [] ( size_t i ) const;
		real& operator [] ( size_t i );
		CGUIVector3& operator = ( const CGUIVector3& rkVector );
		bool operator == ( const CGUIVector3& rkVector ) const;
		bool operator != ( const CGUIVector3& rkVector ) const;
		CGUIVector3 operator + ( const CGUIVector3& rkVector ) const;
		CGUIVector3 operator - ( const CGUIVector3& rkVector ) const;
		CGUIVector3 operator * ( real fScalar ) const;
		CGUIVector3 operator * ( const CGUIVector3& rhs) const;
		CGUIVector3 operator / ( real fScalar ) const;
		CGUIVector3 operator / ( const CGUIVector3& rhs) const;
		CGUIVector3 operator - () const;
		CGUIVector3& operator += ( const CGUIVector3& rkVector );
		CGUIVector3& operator -= ( const CGUIVector3& rkVector );
		CGUIVector3& operator *= ( real fScalar );
		CGUIVector3& operator *= ( const CGUIVector3& rkVector );
		CGUIVector3& operator /= ( real fScalar );
		CGUIVector3& operator /= ( const CGUIVector3& rkVector );
		bool operator < ( const CGUIVector3& rhs ) const;
		bool operator > ( const CGUIVector3& rhs ) const;

	public:
		union 
		{
			struct
			{
				real x, y, z;        
			};
			real val[3];
		};	
		
		static const CGUIVector3 ZERO;
		static const CGUIVector3 UNIT_X;
		static const CGUIVector3 UNIT_Y;
		static const CGUIVector3 UNIT_Z;
		static const CGUIVector3 NEGATIVE_UNIT_X;
		static const CGUIVector3 NEGATIVE_UNIT_Y;
		static const CGUIVector3 NEGATIVE_UNIT_Z;
		static const CGUIVector3 UNIT_SCALE;
	};

	//============================================================================//
	// function
	//============================================================================// 
	extern CGUIVector3 operator * ( real fScalar, const CGUIVector3& rkVector );

}//namespace guiex
#endif //__GUI_VECTOR3_H_20060419__

