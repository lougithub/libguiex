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
	template< class T >
	class GUIEXPORT CGUITemplateVector2
	{
	public:
		CGUITemplateVector2();
		CGUITemplateVector2( T nX, T nY );
		CGUITemplateVector2( const CGUITemplateVector2& rkVector );

		void Set( T nX, T nY);

		void SetX( T _x );
		T GetX( ) const;
		void SetY( T _y );
		T GetY( ) const;

		bool IsZeroLength(void) const;

		T SquaredLength () const;
		
		T operator [] ( size_t i ) const;
		T& operator [] ( size_t i );
		CGUITemplateVector2& operator = ( const CGUITemplateVector2& rkVector );
		bool operator == ( const CGUITemplateVector2& rkVector ) const;
		bool operator != ( const CGUITemplateVector2& rkVector ) const;
		CGUITemplateVector2 operator + ( const CGUITemplateVector2& rkVector ) const;
		CGUITemplateVector2 operator - ( const CGUITemplateVector2& rkVector ) const;
		CGUITemplateVector2 operator * ( T nScalar ) const;
		CGUITemplateVector2 operator * ( const CGUITemplateVector2& rhs) const;
		CGUITemplateVector2 operator / ( T nScalar ) const;
		CGUITemplateVector2 operator - () const;
		CGUITemplateVector2& operator += ( const CGUITemplateVector2& rkVector );
		CGUITemplateVector2& operator -= ( const CGUITemplateVector2& rkVector );
		CGUITemplateVector2& operator *= ( T nScalar );
		CGUITemplateVector2& operator /= ( T nScalar );
		bool operator < ( const CGUITemplateVector2& rhs ) const;
		bool operator > ( const CGUITemplateVector2& rhs ) const;

	public:
		T x, y;        
	};

	typedef CGUITemplateVector2<int32> CGUIIntVector2;
	typedef CGUITemplateVector2<uint32> CGUIUIntVector2;
}


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	template< class T >
	inline CGUITemplateVector2<T>::CGUITemplateVector2()
		: x( 0 ), y( 0 )
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	template< class T >
	inline CGUITemplateVector2<T>::CGUITemplateVector2( T nX, T nY ) 
		: x( nX ), y( nY )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	template< class T >
	inline CGUITemplateVector2<T>::CGUITemplateVector2( const CGUITemplateVector2& rkVector )
		: x( rkVector.x ), y( rkVector.y )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	template< class T >
	inline T CGUITemplateVector2<T>::operator [] ( size_t i ) const
	{
		assert( i < 2 );

		return *(&x+i);
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline T& CGUITemplateVector2<T>::operator [] ( size_t i )
	{
		assert( i < 2 );

		return *(&x+i);
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T>& CGUITemplateVector2<T>::operator = ( const CGUITemplateVector2& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;        

		return *this;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline bool CGUITemplateVector2<T>::operator == ( const CGUITemplateVector2& rkVector ) const
	{
		return ( x == rkVector.x && y == rkVector.y );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline bool CGUITemplateVector2<T>::operator != ( const CGUITemplateVector2& rkVector ) const
	{
		return ( x != rkVector.x || y != rkVector.y  );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T> CGUITemplateVector2<T>::operator + ( const CGUITemplateVector2& rkVector ) const
	{
		CGUITemplateVector2 kSum;

		kSum.x = x + rkVector.x;
		kSum.y = y + rkVector.y;

		return kSum;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T> CGUITemplateVector2<T>::operator - ( const CGUITemplateVector2& rkVector ) const
	{
		CGUITemplateVector2 kDiff;

		kDiff.x = x - rkVector.x;
		kDiff.y = y - rkVector.y;

		return kDiff;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T> CGUITemplateVector2<T>::operator * ( T nScalar ) const
	{
		CGUITemplateVector2 kProd;

		kProd.x = nScalar*x;
		kProd.y = nScalar*y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T> CGUITemplateVector2<T>::operator * ( const CGUITemplateVector2& rhs) const
	{
		CGUITemplateVector2 kProd;

		kProd.x = rhs.x * x;
		kProd.y = rhs.y * y;

		return kProd;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T> CGUITemplateVector2<T>::operator / ( T nScalar ) const
	{
		assert( nScalar != 0 ) ;

		CGUITemplateVector2 kDiv;
		kDiv.x /= nScalar;
		kDiv.y /= nScalar;

		return kDiv;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T> CGUITemplateVector2<T>::operator - () const
	{
		CGUITemplateVector2 kNeg;

		kNeg.x = -x;
		kNeg.y = -y;

		return kNeg;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T>& CGUITemplateVector2<T>::operator += ( const CGUITemplateVector2& rkVector )
	{
		x += rkVector.x;
		y += rkVector.y;

		return *this;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T>& CGUITemplateVector2<T>::operator -= ( const CGUITemplateVector2& rkVector )
	{
		x -= rkVector.x;
		y -= rkVector.y;

		return *this;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T>& CGUITemplateVector2<T>::operator *= ( T nScalar )
	{
		x *= nScalar;
		y *= nScalar;

		return *this;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUITemplateVector2<T>& CGUITemplateVector2<T>::operator /= ( T nScalar )
	{
		assert( nScalar != 0 );

		x /= nScalar;
		y /= nScalar;

		return *this;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUITemplateVector2<T>::Set( T nX, T nY)
	{
		x = nX;
		y = nY;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline T CGUITemplateVector2<T>::GetX( ) const
	{
		return x;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline T CGUITemplateVector2<T>::GetY( ) const
	{
		return y;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUITemplateVector2<T>::SetX( T _x )
	{
		x = _x;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUITemplateVector2<T>::SetY( T _y )
	{
		y = _y;
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
	template< class T >
	inline T CGUITemplateVector2<T>::SquaredLength () const
	{
		return x * x + y * y;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief compare two vectors
	* @return true if the vector's scalar components are all greater
	* that the ones of the vector it is compared against.
	*/
	template< class T >
	inline bool CGUITemplateVector2<T>::operator < ( const CGUITemplateVector2& rhs ) const
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
	template< class T >
	inline bool CGUITemplateVector2<T>::operator > ( const CGUITemplateVector2& rhs ) const
	{
		if( x > rhs.x && y > rhs.y )
			return true;
		return false;
	}
	//------------------------------------------------------------------------------
	/**
	* @return Returns true if this vector is zero length. 
	*/
	template< class T >
	inline bool CGUITemplateVector2<T>::IsZeroLength(void) const
	{
		T sqlen = (x * x) + (y * y);
		return (sqlen < (1e-06 * 1e-06));
	}
	//------------------------------------------------------------------------------
} //namespace guiex

#endif //__GUI_INTVECTOR2_H_20110131__

