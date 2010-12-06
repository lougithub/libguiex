/** 
* @file guisize.h
* @brief size used in libguiex system
* @author ken
* @date 2006-04-24
*/

#ifndef __GUI_SIZE_H_20060424__
#define __GUI_SIZE_H_20060424__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"

//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @brief class that hold the size of object.
	*/
	class GUIEXPORT CGUISize 
	{
	public:
		CGUISize();
		CGUISize(real fWidth, real fHeight);
		CGUISize(const CGUISize& rSize);

		void SetSize( const CGUISize& rSize);
		void SetValue( real fWidth, real fHeight );

		void SetWidth( real fWidth);
		real GetWidth( ) const ;

		void SetHeight( real fHeight);
		real GetHeight( ) const ;

		bool IsEqualZero( ) const;
		bool IsEqual( const CGUISize& rSize ) const;

		CGUISize& operator= (const CGUISize& rSize);
		bool operator==(const CGUISize& other) const;
		bool operator!=(const CGUISize& other) const;
		bool operator<(const CGUISize& other) const;
		CGUISize operator+(const CGUISize& other) const;
		CGUISize& operator+=(const CGUISize& other);
		CGUISize operator-(const CGUISize& other) const;
		CGUISize& operator-=(const CGUISize& other);
		CGUISize operator*(real fScalar) const;
		CGUISize operator*(const CGUISize& other) const;
		CGUISize& operator*=(real fScalar);
		CGUISize& operator*=(const CGUISize& other);
		CGUISize operator/(real fScalar) const;
		CGUISize& operator/=(real fScalar);
		CGUISize operator/(const CGUISize& other) const;
		CGUISize& operator/=(const CGUISize& other);

	public:
		real m_fWidth;
		real m_fHeight;
	};


}//namespace guiex

#endif //__GUI_SIZE_H_20060424__
