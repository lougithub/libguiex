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
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIVector2
	{
	public:
		CGUIVector2();
		CGUIVector2( real fX, real fY );
		CGUIVector2( real afCoordinate[2] );
		CGUIVector2( int32 afCoordinate[2] );
		CGUIVector2( const real* const r );
		CGUIVector2( const CGUIVector2& rkVector );

		void Set( real fX, real fY);

		void SetX( real _x );
		real GetX( ) const;
		void SetY( real _y );
		real GetY( ) const;

		real Length () const;
		bool IsZeroLength(void) const;

		bool Equal( const CGUIVector2& vec ) const;

		real SquaredLength () const;
		
		real DotProduct(const CGUIVector2& vec) const;
		CGUIVector2 CrossProduct( const CGUIVector2& rkVector ) const;

		real Normalise();

		CGUIVector2 MidPoint( const CGUIVector2& vec ) const;

		void MakeFloor( const CGUIVector2& cmp );
		void MakeCeil( const CGUIVector2& cmp );

		CGUIVector2 Perpendicular(void) const;

		CGUIVector2 RandomDeviant( real angle) const;

		CGUIVector2 NormalisedCopy(void) const;

		CGUIVector2 Reflect(const CGUIVector2& normal) const;

		real operator [] ( size_t i ) const;
		real& operator [] ( size_t i );
		CGUIVector2& operator = ( const CGUIVector2& rkVector );
		bool operator == ( const CGUIVector2& rkVector ) const;
		bool operator != ( const CGUIVector2& rkVector ) const;
		CGUIVector2 operator + ( const CGUIVector2& rkVector ) const;
		CGUIVector2 operator - ( const CGUIVector2& rkVector ) const;
		CGUIVector2 operator * ( real fScalar ) const;
		CGUIVector2 operator * ( const CGUIVector2& rhs) const;
		CGUIVector2 operator / ( real fScalar ) const;
		CGUIVector2 operator - () const;
		CGUIVector2& operator += ( const CGUIVector2& rkVector );
		CGUIVector2& operator -= ( const CGUIVector2& rkVector );
		CGUIVector2& operator *= ( real fScalar );
		CGUIVector2& operator /= ( real fScalar );
		bool operator < ( const CGUIVector2& rhs ) const;
		bool operator > ( const CGUIVector2& rhs ) const;

	public:
		real x, y;        

		static const CGUIVector2 ZERO;
		static const CGUIVector2 UNIT_X;
		static const CGUIVector2 UNIT_Y;
		static const CGUIVector2 NEGATIVE_UNIT_X;
		static const CGUIVector2 NEGATIVE_UNIT_Y;
		static const CGUIVector2 UNIT_SCALE;
	};
}



//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	extern CGUIVector2 operator * ( real fScalar, const CGUIVector2& rkVector );
	//------------------------------------------------------------------------------
	extern CGUISize operator * ( const CGUISize& rSize, const CGUIVector2& rkVector );
	//------------------------------------------------------------------------------
	extern CGUIVector2 operator * ( const CGUIVector2& rkVector, const CGUISize& rSize );
	//------------------------------------------------------------------------------

} //namespace guiex

#endif //__GUI_VECTOR2_H_20060419__

