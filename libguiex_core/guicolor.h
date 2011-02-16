/** 
* @file GUIColor.h
* @brief Interface of color class used to represent color value
* @author ken
* @date 2006-03-22
*/

#ifndef __GUI_COLOR_20060322_H__
#define __GUI_COLOR_20060322_H__

//============================================================================//
//	include
//============================================================================// 
#include "guibase.h"

//============================================================================//
// define
//============================================================================// 
namespace guiex
{
	typedef uint32 GUIRGBA;
	typedef uint32 GUIARGB;
	typedef uint32 GUIABGR;
	typedef uint32 GUIBGRA;
}

//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/** 
	* @brief represent color value in this system
	*/
	class GUIEXPORT CGUIColor
	{
	public:
		static const CGUIColor ZERO;
		static const CGUIColor Black;
		static const CGUIColor White;
		static const CGUIColor Red;
		static const CGUIColor Green;
		static const CGUIColor Blue;

	public:
		CGUIColor(const CGUIColor &rColor);
		CGUIColor( );
		CGUIColor( real red, real green, real blue, real alpha );

		GUIRGBA GetAsRGBA(void) const;
		GUIARGB GetAsARGB(void) const;
		GUIBGRA GetAsBGRA(void) const;
		GUIABGR GetAsABGR(void) const;
		void SetAsRGBA(const GUIRGBA val);
		void SetAsARGB(const GUIARGB val);
		void SetAsBGRA(const GUIBGRA val);
		void SetAsABGR(const GUIABGR val);

		void SetColor(real fRed, real fGreen, real fBlue, real fAlpha = 1.0f);
		void SetColor(const CGUIColor& rColor);

		real GetAlpha() const;
		void SetAlpha(real fAlpha);

		real GetRed() const;
		void SetRed(real fRed);

		real GetGreen() const;
		void SetGreen(real fGreen);

		real GetBlue() const;
		void SetBlue(real fBlue);

		void Saturate(void);

		bool operator==(const CGUIColor& rhs) const;
		bool operator!=(const CGUIColor& rhs) const;

		/// Array accessor operator
		real operator [] ( const size_t i ) const;

		/// Array accessor operator
		real& operator [] ( const size_t i );

		// arithmetic operations
		CGUIColor operator + ( const CGUIColor& rkVector ) const;
		CGUIColor operator - ( const CGUIColor& rkVector ) const;
		CGUIColor operator * (const real fScalar ) const;
		CGUIColor operator * ( const CGUIColor& rhs) const;
		CGUIColor operator / ( const CGUIColor& rhs) const;
		CGUIColor operator / (const real fScalar ) const;

		// arithmetic updates
		CGUIColor& operator += ( const CGUIColor& rkVector );
		CGUIColor& operator -= ( const CGUIColor& rkVector );
		CGUIColor& operator *= (const real fScalar );
		CGUIColor& operator /= (const real fScalar );

		friend CGUIColor operator * (const real fScalar, const CGUIColor& rkVector );

	public:
		real r,g,b,a;

	};//class CGUIColor


} //namespace guiex

#endif//	__GUI_COLOR_20060322_H__

