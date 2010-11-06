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
		/**
		* @brief constructor
		*/
		CGUISize();

		/**
		* @brief constructor
		*/
		CGUISize(real fWidth, real fHeight);

		/**
		* @brief constructor
		*/
		CGUISize(const CGUISize& rSize);

		/**
		* @brief set size
		*/
		void	SetSize( const CGUISize& rSize);


		/**
		* @brief set width
		*/
		void	SetWidth( real fWidth);

		/**
		* @brief set width
		*/
		real	GetWidth( ) const ;

		/**
		* @brief set height
		*/
		void	SetHeight( real fHeight);

		/**
		* @brief get height
		*/
		real	GetHeight(  ) const ;

		bool	IsEqualZero( ) const;

		bool	IsEqual( const CGUISize& rSize ) const;


		/**
		* @brief assign operator
		*/
		CGUISize& operator= (const CGUISize& rSize);

		/**
		* @brief compare operator
		*/
		bool operator==(const CGUISize& other) const;

		/**
		* @brief compare operator
		*/
		bool operator!=(const CGUISize& other) const;

		/**
		* @brief compare operator
		*/
		bool operator<(const CGUISize& other) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize operator+(const CGUISize& other) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize& operator+=(const CGUISize& other);

		/**
		* @brief algorithm operator
		*/
		CGUISize operator-(const CGUISize& other) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize& operator-=(const CGUISize& other);

		/**
		* @brief algorithm operator
		*/
		CGUISize operator*(real fScalar) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize operator*(const CGUISize& other) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize& operator*=(real fScalar);

		/**
		* @brief algorithm operator
		*/
		CGUISize& operator*=(const CGUISize& other);

		/**
		* @brief algorithm operator
		*/
		CGUISize operator/(real fScalar) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize& operator/=(real fScalar);

		/**
		* @brief algorithm operator
		*/
		CGUISize operator/(const CGUISize& other) const;

		/**
		* @brief algorithm operator
		*/
		CGUISize& operator/=(const CGUISize& other);

	public:
		real m_fWidth;
		real m_fHeight;
	};


}//namespace guiex

#endif //__GUI_SIZE_H_20060424__
