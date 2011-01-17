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
	typedef uint32 GUIARGB;
	typedef uint32 GUIRGBA;
	typedef uint32 GUIABGR;
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
		CGUIColor();
		CGUIColor(const CGUIColor &rColor);
		CGUIColor(GUIARGB argb);
		CGUIColor(real fRed, real fGreen, real fBlue, real fAlpha=1.0f );

		void SetARGB(GUIARGB argb);
		GUIARGB	GetARGB() const;

		GUIRGBA GetRGBA() const;
		GUIABGR GetABGR() const;

		static GUIRGBA ARGB2RGBA( GUIARGB aColorARGB );

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

		void InvertColor(void);
		void InvertColorWithAlpha(void);

		CGUIColor& operator=(const CGUIColor &rColor);
		CGUIColor& operator=(GUIARGB argb);
		CGUIColor& operator&=(GUIARGB argb);
		CGUIColor& operator&=(const CGUIColor& rColor);
		CGUIColor operator&(GUIARGB argb);
		CGUIColor operator&(const CGUIColor& rColor);
		CGUIColor& operator|=(GUIARGB argb);
		CGUIColor& operator|=(const CGUIColor& rColor);
		CGUIColor operator|(GUIARGB argb);
		CGUIColor operator|(const CGUIColor& rColor);
		CGUIColor& operator<<=(int32 nOffset);
		CGUIColor& operator>>=(int32 nOffset);
		CGUIColor operator<<(int32 nOffset);
		CGUIColor operator>>(int32 nOffset);
		CGUIColor operator+(const CGUIColor& rColor) const;
		CGUIColor operator+(GUIARGB argb) const;
		CGUIColor& operator+=(const CGUIColor& rColor);
		CGUIColor& operator+=(GUIARGB argb);
		CGUIColor operator-(const CGUIColor& rColor) const;
		CGUIColor operator-(GUIARGB argb) const;
		CGUIColor operator/(real fScale) const;
		CGUIColor& operator-=(const CGUIColor& rColor);
		CGUIColor& operator-=(GUIARGB argb);
		CGUIColor operator*(const real fValue) const;
		CGUIColor& 	operator*=(const real fValue);
		CGUIColor operator*(const CGUIColor& rColor) const;
		CGUIColor operator*(GUIARGB argb) const;
		CGUIColor& operator*=(const CGUIColor& rColor);
		CGUIColor& operator*=(GUIARGB argb);
		bool operator==(const CGUIColor &rColor ) const;
		bool operator!=(const CGUIColor &rColor ) const;
		bool operator<(const CGUIColor &rColor ) const;
		bool operator>(const CGUIColor &rColor ) const;

	private:
		GUIARGB	m_nARGB;	///< color

	};//class CGUIColor


} //namespace guiex

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------ 
	/** 
	* @brief get color value in ARGB
	* @return color value with 4-bytes number
	*/
	inline GUIARGB CGUIColor::GetARGB() const
	{
		return m_nARGB;
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief get color value in ARGB
	* @return color value with 4-bytes number
	*/
	inline GUIRGBA CGUIColor::GetRGBA() const
	{
		return ((m_nARGB<<8) + ((m_nARGB>>24)&0xFF));
	}
	//------------------------------------------------------------------------------ 
	inline GUIABGR CGUIColor::GetABGR() const
	{
		return ((m_nARGB& 0xFF000000) | //A
			((m_nARGB & 0x00FF0000)>>16) | //R
			(m_nARGB & 0x0000FF00) | //G
			((m_nARGB & 0x000000FF)<<16) //B
			);
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief set color value by ARGB
	* @param GUIARGB color value,color value with 4-bytes number
	*/
	inline void CGUIColor::SetARGB(GUIARGB argb)
	{
		m_nARGB = argb;
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief set value by color r,g,b,a
	* @param fRed red value, from 0.0 to 1.0
	* @param fGreen green value, from 0.0 to 1.0
	* @param fBlue blue value, from 0.0 to 1.0
	* @param fAlpha alpha value, from 0.0 to 1.0, default is 1.0f
	*/
	inline void CGUIColor::SetColor(real fRed, real fGreen, real fBlue, real fAlpha/* = 1.0f*/)
	{
		m_nARGB = ((uint32(fAlpha*255.0f)&0xFF)<<24) + ((uint32(fRed*255.0f)&0xFF)<<16) + ((uint32(fGreen*255.0f)&0xFF)<<8) + (uint32(fBlue*255.0f)&0xFF);
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief set value by RGB
	* @param val reference of CGUIColor object
	*/
	inline void CGUIColor::SetColor(const CGUIColor& rColor)
	{
		m_nARGB = rColor.m_nARGB;
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief get alpha factor
	* @return alpha factor, from 0.0 to 1.0
	*/
	inline real CGUIColor::GetAlpha() const
	{
		return (real((m_nARGB>>24)&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief set alpha
	* @param fAlpha alhpa value , from 0.0 to 1.0
	*/
	inline void CGUIColor::SetAlpha(real fAlpha)
	{
		m_nARGB = (m_nARGB&0x00FFFFFF)+((uint32(fAlpha*255.0f)&0xFF)<<24);
	}
	//------------------------------------------------------------------------------ 
	inline real CGUIColor::GetRed() const
	{
		return (real((m_nARGB>>16)&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void CGUIColor::SetRed(real fRed)
	{
		m_nARGB = (m_nARGB&0xFF00FFFF)+((uint32(fRed*255.0f)&0xFF)<<16);
	}
	//------------------------------------------------------------------------------ 
	inline real CGUIColor::GetGreen() const
	{
		return (real((m_nARGB>>8)&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void CGUIColor::SetGreen(real fGreen)
	{
		m_nARGB = (m_nARGB&0xFFFF00FF)+((uint32(fGreen*255.0f)&0xFF)<<8);
	}
	//------------------------------------------------------------------------------ 
	inline real CGUIColor::GetBlue() const
	{
		return (real(m_nARGB&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void CGUIColor::SetBlue(real fBlue)
	{
		m_nARGB = (m_nARGB&0xFFFFFF00)+(uint32(fBlue*255.0f)&0xFF);
	}
	//------------------------------------------------------------------------------ 
	inline void CGUIColor::InvertColor(void)
	{
		m_nARGB = (m_nARGB&0xFF000000) + (0x00FFFFFF-(m_nARGB&0x00FFFFFF));
	}
	//----------------------------------------------------------------------------- 
	inline void CGUIColor::InvertColorWithAlpha(void)
	{
		m_nARGB = 0xFFFFFFFF-m_nARGB;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator=(const CGUIColor &rColor)
	{
		SetColor(rColor);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator=(GUIARGB argb)
	{
		SetARGB( argb);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator&=(GUIARGB argb)
	{
		SetARGB(GetARGB() & argb);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator&=(const CGUIColor& rColor)
	{
		SetARGB(GetARGB() & rColor.GetARGB());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator&(GUIARGB argb)
	{
		return CGUIColor(GetARGB() & argb);
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator&(const CGUIColor& rColor)
	{
		return CGUIColor(GetARGB() & rColor.GetARGB());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator|=(GUIARGB argb)
	{
		SetARGB(GetARGB() | argb);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator|=(const CGUIColor& rColor)
	{
		SetARGB(GetARGB() | rColor.GetARGB());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator|(GUIARGB argb)
	{
		return CGUIColor(GetARGB() | argb );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator|(const CGUIColor& rColor)
	{
		return CGUIColor( GetARGB() | rColor.GetARGB());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator<<=(int32 nOffset)
	{
		SetARGB(GetARGB() << nOffset);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator>>=(int32 nOffset)
	{
		SetARGB(GetARGB() >> nOffset);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator<<(int32 nOffset)
	{
		return CGUIColor( GetARGB() << nOffset );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator>>(int32 nOffset)
	{
		return CGUIColor( GetARGB() >> nOffset );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator+(const CGUIColor& rColor) const
	{
		return CGUIColor( GetRed() 	+ rColor.GetRed(),
			GetGreen() 	+ rColor.GetGreen(),
			GetBlue() 	+ rColor.GetBlue(),
			GetAlpha() 	+ rColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator+(GUIARGB argb) const
	{
		CGUIColor aTempColor(argb);

		return CGUIColor( GetRed() 	+ aTempColor.GetRed(),
			GetGreen() 	+ aTempColor.GetGreen(),
			GetBlue() 	+ aTempColor.GetBlue(),
			GetAlpha() 	+ aTempColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator+=(const CGUIColor& rColor)
	{
		SetColor(
			GetRed() + rColor.GetRed(),
			GetGreen() + rColor.GetGreen(),
			GetBlue() + rColor.GetBlue(),
			GetAlpha() + rColor.GetAlpha());

		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator+=(GUIARGB argb)
	{
		CGUIColor aTempColor(argb);
		SetColor(
			GetRed() + aTempColor.GetRed(),
			GetGreen() + aTempColor.GetGreen(),
			GetBlue() + aTempColor.GetBlue(),
			GetAlpha() + aTempColor.GetAlpha());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator-(const CGUIColor& rColor) const
	{
		return CGUIColor( GetRed() 	- rColor.GetRed(),
			GetGreen() 	- rColor.GetGreen(),
			GetBlue() 	- rColor.GetBlue(),
			GetAlpha() 	- rColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator/(real fScale) const
	{
		if( fScale <=0 )
		{
			return *this;
		}
		return CGUIColor(
			GetRed() / fScale,
			GetGreen() / fScale,
			GetBlue() / fScale,
			GetAlpha() / fScale);
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator-(GUIARGB argb) const
	{
		CGUIColor aTempColor(argb);
		return CGUIColor( GetRed() 	- aTempColor.GetRed(),
			GetGreen() 	- aTempColor.GetGreen(),
			GetBlue() 	- aTempColor.GetBlue(),
			GetAlpha() 	- aTempColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator-=(const CGUIColor& rColor)
	{
		SetColor(
			GetRed() - rColor.GetRed(),
			GetGreen() - rColor.GetGreen(),
			GetBlue() - rColor.GetBlue(),
			GetAlpha() - rColor.GetAlpha());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator-=(GUIARGB argb)
	{
		CGUIColor aTempColor(argb);
		SetColor(
			GetRed() - aTempColor.GetRed(),
			GetGreen() - aTempColor.GetGreen(),
			GetBlue() - aTempColor.GetBlue(),
			GetAlpha() - aTempColor.GetAlpha());
		return *this;

	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator*(const real fValue) const
	{
		return CGUIColor( GetRed() 	* fValue,
			GetGreen() 	* fValue, 
			GetBlue() 	* fValue,
			GetAlpha() 	* fValue );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator*=(const real fValue)
	{
		SetColor(
			GetRed() * fValue,
			GetGreen() * fValue,
			GetBlue() * fValue,
			GetAlpha() * fValue);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator*(const CGUIColor& rColor) const
	{
		return CGUIColor( GetRed() 	* rColor.GetRed(),
			GetGreen() 	* rColor.GetGreen(),
			GetBlue() 	* rColor.GetBlue(),
			GetAlpha() 	* rColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor CGUIColor::operator*(GUIARGB argb) const
	{
		CGUIColor aTempColor(argb);
		return CGUIColor( GetRed() 	* aTempColor.GetRed(),
			GetGreen() 	* aTempColor.GetGreen(),
			GetBlue() 	* aTempColor.GetBlue(),
			GetAlpha() 	* aTempColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator*=(const CGUIColor& rColor)
	{
		SetColor(
			GetRed() * rColor.GetRed(),
			GetGreen() * rColor.GetGreen(),
			GetBlue() * rColor.GetBlue(),
			GetAlpha() * rColor.GetAlpha());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& CGUIColor::operator*=(GUIARGB argb)
	{
		CGUIColor aTempColor(argb);
		SetColor(
			GetRed() * aTempColor.GetRed(),
			GetGreen() * aTempColor.GetGreen(),
			GetBlue() * aTempColor.GetBlue(),
			GetAlpha() * aTempColor.GetAlpha());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline bool CGUIColor::operator==(const CGUIColor &rColor ) const
	{
		return GetARGB() == rColor.GetARGB();
	}
	//------------------------------------------------------------------------------ 
	inline bool CGUIColor::operator!=(const CGUIColor &rColor ) const
	{
		return (!(*this == rColor));
	}
	//------------------------------------------------------------------------------ 
	inline bool	CGUIColor::operator<(const CGUIColor &rColor ) const
	{
		return GetARGB() < rColor.GetARGB();
	}
	//------------------------------------------------------------------------------ 
	inline bool	CGUIColor::operator>(const CGUIColor &rColor ) const
	{
		return GetARGB() > rColor.GetARGB();
	}
	//------------------------------------------------------------------------------ 
}
#endif//	__GUI_COLOR_20060322_H__

