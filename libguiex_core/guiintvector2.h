/** 
* @file guiintvector2.h
* @brief vector 2 used in this system
* @author ken
* @date 2011-01-31
*/

#ifndef __GUI_INTVECTOR2_H_20110131__
#define __GUI_INTVECTOR2_H_20110131__

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
	class GUIEXPORT CGUIIntVector2
	{
	public:
		CGUIIntVector2();
		CGUIIntVector2( int32 nX, int32 nY );
		CGUIIntVector2( const CGUIIntVector2& rkVector );

		void Set( int32 nX, int32 nY);

		void SetX( int32 _x );
		int32 GetX( ) const;
		void SetY( int32 _y );
		int32 GetY( ) const;

		bool IsZeroLength(void) const;

		int32 SquaredLength () const;
		
		int32 operator [] ( size_t i ) const;
		int32& operator [] ( size_t i );
		CGUIIntVector2& operator = ( const CGUIIntVector2& rkVector );
		bool operator == ( const CGUIIntVector2& rkVector ) const;
		bool operator != ( const CGUIIntVector2& rkVector ) const;
		CGUIIntVector2 operator + ( const CGUIIntVector2& rkVector ) const;
		CGUIIntVector2 operator - ( const CGUIIntVector2& rkVector ) const;
		CGUIIntVector2 operator * ( int32 nScalar ) const;
		CGUIIntVector2 operator * ( const CGUIIntVector2& rhs) const;
		CGUIIntVector2 operator / ( int32 nScalar ) const;
		CGUIIntVector2 operator - () const;
		CGUIIntVector2& operator += ( const CGUIIntVector2& rkVector );
		CGUIIntVector2& operator -= ( const CGUIIntVector2& rkVector );
		CGUIIntVector2& operator *= ( int32 nScalar );
		CGUIIntVector2& operator /= ( int32 nScalar );
		bool operator < ( const CGUIIntVector2& rhs ) const;
		bool operator > ( const CGUIIntVector2& rhs ) const;

	public:
		int32 x, y;        
	};
}



//============================================================================//
// function
//============================================================================// 
namespace guiex
{



} //namespace guiex

#endif //__GUI_INTVECTOR2_H_20110131__

