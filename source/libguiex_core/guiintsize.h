/** 
* @file guisize.h
* @brief int size used in libguiex system
* @author ken
* @date 2011-01-31
*/

#ifndef __GUI_INTSIZE_H_20110131__
#define __GUI_INTSIZE_H_20110131__

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
	class GUIEXPORT CGUIIntSize 
	{
	public:
		CGUIIntSize();
		CGUIIntSize(uint32 uWidth, uint32 uHeight);
		CGUIIntSize(const CGUIIntSize& rSize);

		void SetSize( const CGUIIntSize& rSize);
		void SetValue( uint32 uWidth, uint32 uHeight );

		void SetWidth( uint32 uWidth);
		uint32 GetWidth( ) const ;

		void SetHeight( uint32 uHeight);
		uint32 GetHeight( ) const ;

		bool IsEqualZero( ) const;
		bool IsEqual( const CGUIIntSize& rSize ) const;

		CGUIIntSize& operator= (const CGUIIntSize& rSize);
		bool operator==(const CGUIIntSize& other) const;
		bool operator!=(const CGUIIntSize& other) const;
		bool operator<(const CGUIIntSize& other) const;
		CGUIIntSize operator+(const CGUIIntSize& other) const;
		CGUIIntSize& operator+=(const CGUIIntSize& other);
		CGUIIntSize operator-(const CGUIIntSize& other) const;
		CGUIIntSize& operator-=(const CGUIIntSize& other);
		CGUIIntSize operator*(uint32 uScalar) const;
		CGUIIntSize operator*(const CGUIIntSize& other) const;
		CGUIIntSize& operator*=(uint32 uScalar);
		CGUIIntSize& operator*=(const CGUIIntSize& other);
		CGUIIntSize operator/(uint32 uScalar) const;
		CGUIIntSize& operator/=(uint32 uScalar);
		CGUIIntSize operator/(const CGUIIntSize& other) const;
		CGUIIntSize& operator/=(const CGUIIntSize& other);

	public:
		uint32 m_uWidth;
		uint32 m_uHeight;
	};


}//namespace guiex

#endif //__GUI_INTSIZE_H_20110131__
