/** 
* @file guicolor.cpp
* @brief implement of class CGUIColor.
* @author ken
* @date 2006-03-23
*/

//============================================================================//
// include
//============================================================================// 
#include "guicolor.h"
#include "guimath.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	const CGUIColor CGUIColor::ZERO = CGUIColor(0.0,0.0,0.0,0.0);
	const CGUIColor CGUIColor::Black = CGUIColor(0.0,0.0,0.0,1.0f);
	const CGUIColor CGUIColor::White = CGUIColor(1.0,1.0,1.0,1.0f);
	const CGUIColor CGUIColor::Red = CGUIColor(1.0,0.0,0.0,1.0f);
	const CGUIColor CGUIColor::Green = CGUIColor(0.0,1.0,0.0,1.0f);
	const CGUIColor CGUIColor::Blue = CGUIColor(0.0,0.0,1.0,1.0f);

	//------------------------------------------------------------------------------ 
	/** 
	* @brief copy constructor
	*/
	CGUIColor::CGUIColor(const CGUIColor &rColor)
		:r(rColor.r)
		,g(rColor.g)
		,b(rColor.b)
		,a(rColor.a)
	{
	}
	//------------------------------------------------------------------------------ 
	CGUIColor::CGUIColor( )
		:r(1.0f)
		,g(1.0f)
		,b(1.0f)
		,a(1.0f)
	{
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief constructor
	* @param fRed red factor of color, from 0.0 to 1.0
	* @param fGreen green factor of color, from 0.0 to 1.0
	* @param fBlue blue factor of color, from 0.0 to 1.0
	* @param fAlpha alpha factor of color, from 0.0 to 1.0, default is 1.0f
	*/
	CGUIColor::CGUIColor( real red, real green, real blue, real alpha )
		:r(red), g(green), b(blue), a(alpha)
	{
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	GUIABGR CGUIColor::GetAsABGR(void) const
#else
	GUIRGBA CGUIColor::GetAsRGBA(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 16;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 += val8;

		return val32;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	GUIBGRA CGUIColor::GetAsBGRA(void) const
#else
	GUIARGB CGUIColor::GetAsARGB(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 = val8 << 24;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 8;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	GUIARGB CGUIColor::GetAsARGB(void) const
#else
	GUIBGRA CGUIColor::GetAsBGRA(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 16;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	GUIRGBA CGUIColor::GetAsRGBA(void) const
#else
	GUIABGR CGUIColor::GetAsABGR(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 = val8 << 24;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 8;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	void CGUIColor::SetAsABGR(const GUIABGR val)
#else
	void CGUIColor::SetAsRGBA(const GUIRGBA val)
#endif
	{
		uint32 val32 = val;

		// Red
		r = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	void CGUIColor::SetAsBGRA(const GUIBGRA val)
#else
	void CGUIColor::SetAsARGB(const GUIARGB val)
#endif
	{
		uint32 val32 = val;

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		b = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	void CGUIColor::SetAsARGB(const GUIARGB val)
#else
	void CGUIColor::SetAsBGRA(const GUIBGRA val)
#endif
	{
		uint32 val32 = val;

		// Blue
		b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
#if GUI_ENDIAN == GUI_ENDIAN_BIG
	void CGUIColor::SetAsRGBA(const GUIRGBA val)
#else
	void CGUIColor::SetAsABGR(const GUIABGR val)
#endif
	{
		uint32 val32 = val;

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Red
		r = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
	/** 
	* @brief get alpha factor
	* @return alpha factor, from 0.0 to 1.0
	*/
	real CGUIColor::GetAlpha() const
	{
		return a;
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief set alpha
	* @param fAlpha alhpa value , from 0.0 to 1.0
	*/
	void CGUIColor::SetAlpha(real fAlpha)
	{
		//a = GUIClamp( fAlpha, 0.0f, 1.0f );
		a = fAlpha;
	}
	//------------------------------------------------------------------------------ 
	real CGUIColor::GetRed() const
	{
		return r;
	}
	//------------------------------------------------------------------------------ 
	void CGUIColor::SetRed(real fRed)
	{
		//r = GUIClamp( fRed, 0.0f, 1.0f );
		r = fRed;
	}
	//------------------------------------------------------------------------------ 
	real CGUIColor::GetGreen() const
	{
		return g;
	}
	//------------------------------------------------------------------------------ 
	void CGUIColor::SetGreen(real fGreen)
	{
		//g = GUIClamp( fGreen, 0.0f, 1.0f );
		g = fGreen;
	}
	//------------------------------------------------------------------------------ 
	real CGUIColor::GetBlue() const
	{
		return b;
	}
	//------------------------------------------------------------------------------ 
	void CGUIColor::SetBlue(real fBlue)
	{
		//b = GUIClamp( fBlue, 0.0f, 1.0f );
		b = fBlue;
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief set value by color r,g,b,a
	* @param fRed red value, from 0.0 to 1.0
	* @param fGreen green value, from 0.0 to 1.0
	* @param fBlue blue value, from 0.0 to 1.0
	* @param fAlpha alpha value, from 0.0 to 1.0, default is 1.0f
	*/
	void CGUIColor::SetColor(real fRed, real fGreen, real fBlue, real fAlpha/* = 1.0f*/)
	{
		r = fRed;
		g = fGreen;
		b = fBlue;
		a = fAlpha;
	}
	//------------------------------------------------------------------------------ 
	/// Array accessor operator
	real CGUIColor::operator [] ( const size_t i ) const
	{
		assert( i < 4 );

		return *(&r+i);
	}
	//------------------------------------------------------------------------------ 

	/// Array accessor operator
	real& CGUIColor::operator [] ( const size_t i )
	{
		assert( i < 4 );

		return *(&r+i);
	}
	//------------------------------------------------------------------------------ 
	// arithmetic operations
	CGUIColor CGUIColor::operator + ( const CGUIColor& rkVector ) const
	{
		CGUIColor kSum;

		kSum.r = r + rkVector.r;
		kSum.g = g + rkVector.g;
		kSum.b = b + rkVector.b;
		kSum.a = a + rkVector.a;

		return kSum;
	}
	//------------------------------------------------------------------------------ 
	CGUIColor CGUIColor::operator - ( const CGUIColor& rkVector ) const
	{
		CGUIColor kDiff;

		kDiff.r = r - rkVector.r;
		kDiff.g = g - rkVector.g;
		kDiff.b = b - rkVector.b;
		kDiff.a = a - rkVector.a;

		return kDiff;
	}
	//------------------------------------------------------------------------------ 
	CGUIColor CGUIColor::operator * (const real fScalar ) const
	{
		CGUIColor kProd;

		kProd.r = fScalar*r;
		kProd.g = fScalar*g;
		kProd.b = fScalar*b;
		kProd.a = fScalar*a;

		return kProd;
	}
	//------------------------------------------------------------------------------ 
	CGUIColor CGUIColor::operator * ( const CGUIColor& rhs) const
	{
		CGUIColor kProd;

		kProd.r = rhs.r * r;
		kProd.g = rhs.g * g;
		kProd.b = rhs.b * b;
		kProd.a = rhs.a * a;

		return kProd;
	}
	//------------------------------------------------------------------------------ 
	CGUIColor CGUIColor::operator / ( const CGUIColor& rhs) const
	{
		CGUIColor kProd;

		kProd.r = rhs.r / r;
		kProd.g = rhs.g / g;
		kProd.b = rhs.b / b;
		kProd.a = rhs.a / a;

		return kProd;
	}
	//------------------------------------------------------------------------------ 
	CGUIColor CGUIColor::operator / (const real fScalar ) const
	{
		assert( fScalar != 0.0 );

		CGUIColor kDiv;

		real fInv = 1.0f / fScalar;
		kDiv.r = r * fInv;
		kDiv.g = g * fInv;
		kDiv.b = b * fInv;
		kDiv.a = a * fInv;

		return kDiv;
	}
	//------------------------------------------------------------------------------ 
	// arithmetic updates
	CGUIColor& CGUIColor::operator += ( const CGUIColor& rkVector )
	{
		r += rkVector.r;
		g += rkVector.g;
		b += rkVector.b;
		a += rkVector.a;

		return *this;
	}
	//------------------------------------------------------------------------------ 
	CGUIColor& CGUIColor::operator -= ( const CGUIColor& rkVector )
	{
		r -= rkVector.r;
		g -= rkVector.g;
		b -= rkVector.b;
		a -= rkVector.a;

		return *this;
	}
	//------------------------------------------------------------------------------ 

	CGUIColor& CGUIColor::operator *= (const real fScalar )
	{
		r *= fScalar;
		g *= fScalar;
		b *= fScalar;
		a *= fScalar;
		return *this;
	}
	//------------------------------------------------------------------------------ 

	CGUIColor& CGUIColor::operator /= (const real fScalar )
	{
		assert( fScalar != 0.0 );

		real fInv = 1.0f / fScalar;

		r *= fInv;
		g *= fInv;
		b *= fInv;
		a *= fInv;

		return *this;
	}
	//------------------------------------------------------------------------------ 
	void CGUIColor::Saturate(void)
	{
		if (r < 0)
			r = 0;
		else if (r > 1)
			r = 1;

		if (g < 0)
			g = 0;
		else if (g > 1)
			g = 1;

		if (b < 0)
			b = 0;
		else if (b > 1)
			b = 1;

		if (a < 0)
			a = 0;
		else if (a > 1)
			a = 1;
	}
	//---------------------------------------------------------------------
	bool CGUIColor::operator==(const CGUIColor& rhs) const
	{
		return (r == rhs.r &&
			g == rhs.g &&
			b == rhs.b &&
			a == rhs.a);
	}
	//---------------------------------------------------------------------
	bool CGUIColor::operator!=(const CGUIColor& rhs) const
	{
		return !(*this == rhs);
	}
	//------------------------------------------------------------------------------ 

	//------------------------------------------------------------------------------ 
	CGUIColor operator * (const real fScalar, const CGUIColor& rkVector )
	{
		CGUIColor kProd;

		kProd.r = fScalar * rkVector.r;
		kProd.g = fScalar * rkVector.g;
		kProd.b = fScalar * rkVector.b;
		kProd.a = fScalar * rkVector.a;

		return kProd;
	}
	//------------------------------------------------------------------------------ 

}//namespace guiex

