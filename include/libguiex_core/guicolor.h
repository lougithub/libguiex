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
#include "guipropertyable.h"


//============================================================================//
// define
//============================================================================// 
namespace guiex
{
	typedef uint32 GUIARGB;
	typedef uint32 GUIRGBA;
}

//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/** 
	* @brief represent color value in this system
	*/
	class GUIEXPORT CGUIColor : public CGUIPropertyable
	{
	public:
		/** 
		* @brief default constructor
		*/
		CGUIColor();

		/** 
		* @brief copy constructor
		*/
		CGUIColor(const CGUIColor &rColor);

		/** 
		* @brief constructor
		* @param argb color value, a 4-bytes number
		*/
		CGUIColor(GUIARGB argb);

		/** 
		* @brief constructor
		* @param fRed red factor of color, from 0.0 to 1.0
		* @param fGreen green factor of color, from 0.0 to 1.0
		* @param fBlue blue factor of color, from 0.0 to 1.0
		* @param fAlpha alpha factor of color, from 0.0 to 1.0, default is 1.0f
		*/
		CGUIColor(real fRed, real fGreen, real fBlue, real fAlpha=1.0f );

		/** 
		* @brief get color value in ARGB
		* @return color value with 4-bytes number
		*/
		GUIARGB	GetARGB() const;

		/** 
		* @brief get color value in ARGB
		* @return color value with 4-bytes number
		*/
		GUIRGBA GetRGBA() const;
		
		/** 
		* @brief convert color from ARGB to RGBA
		* @return color value with 4-bytes number
		*/
		static GUIRGBA ARGB2RGBA( GUIARGB aColorARGB );

		/** 
		* @brief set color value by ARGB
		* @param GUIARGB color value,color value with 4-bytes number
		*/
		void	SetARGB(GUIARGB argb);


		/** 
		* @brief set value by color r,g,b,a
		* @param fRed red value, from 0.0 to 1.0
		* @param fGreen green value, from 0.0 to 1.0
		* @param fBlue blue value, from 0.0 to 1.0
		* @param fAlpha alpha value, from 0.0 to 1.0, default is 1.0f
		*/
		void SetColor(real fRed, real fGreen, real fBlue, real fAlpha = 1.0f);

		/** 
		* @brief set value by RGB
		* @param val reference of CGUIColor object
		*/
		void SetColor(const CGUIColor& rColor);


		/** 
		* @brief get alpha factor
		* @return alpha factor, from 0.0 to 1.0
		*/
		real	GetAlpha() const;

		/** 
		* @brief set alpha
		* @param fAlpha alhpa value , from 0.0 to 1.0
		*/
		void SetAlpha(real fAlpha);

		/** 
		* @brief get red factor of RGB
		* @return red factor, from 0.0 to 1.0
		*/
		real	GetRed() const;

		/** 
		* @brief set red factor of RGB
		* @paramfRred red value of RGB, from 0.0 to 1.0
		*/
		void SetRed(real fRed);

		/** 
		* @brief get green factor of RGB
		* @return green factor, from 0.0 to 1.0
		*/
		real	GetGreen() const;

		/** 
		* @brief set green factor of RGB
		* @param fGreen value of RGB, from 0.0 to 1.0
		*/
		void SetGreen(real fGreen);

		/** 
		* @brief get blue factor of RGB
		* @return blue factor, from 0.0 to 1.0
		*/
		real	GetBlue() const;

		/** 
		* @brief set blue factor of RGB
		* @param fBlue blue value of RGB, from 0.0 to 1.0
		*/
		void SetBlue(real fBlue);

		/** 
		* @brief invert color.
		*/
		void	InvertColor(void);

		/**
		* @brief invert color and alpha
		*/
		void	InvertColorWithAlpha(void);

		/** 
		* @brief assign operator
		* @param rColor reference of CGUIColor
		* @return reference of this object
		*/
		CGUIColor& operator=(const CGUIColor &rColor);

		/** 
		* @brief assign operator
		* @param argb color value, a 4-bytes number
		* @return reference of this object
		*/
		CGUIColor& operator=(GUIARGB argb);

		/** 
		* @brief & and assign operator
		* @param argb color value, a 4-bytes number
		* @return reference of this object
		*/
		CGUIColor& operator&=(GUIARGB argb);

		/** 
		* @brief & and assign operator
		* @param rColor reference of CGUIColor object.
		* @return reference of this object
		*/
		CGUIColor& operator&=(const CGUIColor& rColor);

		/** 
		* @brief & operator
		* @param argb color value, a 4-bytes number
		* @return a temporary object
		*/
		CGUIColor operator&(GUIARGB argb);

		/** 
		* @brief & operator
		* @param argb reference of CGUIColor object.
		* @return a temporary object
		*/
		CGUIColor operator&(const CGUIColor& rColor);

		/** 
		* @brief | and assign operator
		* @param argb color value, a 4-bytes number
		* @return reference of this object
		*/
		CGUIColor& operator|=(GUIARGB argb);

		/** 
		* @brief | and assign operator
		* @param rColor reference of CGUIColor object.
		* @return reference of this object
		*/
		CGUIColor& operator|=(const CGUIColor& rColor);

		/** 
		* @brief | operator
		* @param argb color value, a 4-bytes number
		* @return a temporary object
		*/
		CGUIColor operator|(GUIARGB argb);

		/** 
		* @brief | operator
		* @param rColor reference of CGUIColor object.
		* @return a temporary object
		*/
		CGUIColor operator|(const CGUIColor& rColor);

		/** 
		* @brief << and assign operator
		* @param nOffset offset
		* @return reference of this object
		*/
		CGUIColor& operator<<=(int32 nOffset);

		/** 
		* @brief >> and assign operator
		* @param nOffset offset
		* @return reference of this object
		*/
		CGUIColor& operator>>=(int32 nOffset);

		/** 
		* @brief << operator
		* @param nOffset offset
		* @return reference of this object
		*/
		CGUIColor operator<<(int32 nOffset);

		/** 
		* @brief >> operator
		* @param nOffset offset
		* @return reference of this object
		*/
		CGUIColor operator>>(int32 nOffset);

		/** 
		* @brief + operator
		* @param rColor reference of CGUIColor object.
		* @return a temporary object
		*/
		CGUIColor operator+(const CGUIColor& rColor) const;

		/** 
		* @brief + operator
		* @param argb color value, a 4-bytes number
		* @return a temporary object
		*/
		CGUIColor operator+(GUIARGB argb) const;

		/** 
		* @brief + and assign operator
		* @param rColor reference of CGUIColor object.
		* @return reference of this object
		*/
		CGUIColor& operator+=(const CGUIColor& rColor);

		/** 
		* @brief + and assign operator
		* @param argb color value, a 4-bytes number
		* @return reference of this object
		*/
		CGUIColor& operator+=(GUIARGB argb);

		/** 
		* @brief - operator
		* @param rColor reference of CGUIColor object.
		* @return a temporary object
		*/
		CGUIColor operator-(const CGUIColor& rColor) const;

		/** 
		* @brief - operator
		* @param argb color value, a 4-bytes number
		* @return a temporary object
		*/
		CGUIColor operator-(GUIARGB argb) const;

		/** 
		* @brief - and assign operator
		* @param rColor reference of CGUIColor object
		* @return reference of this object
		*/
		CGUIColor& operator-=(const CGUIColor& rColor);

		/** 
		* @brief - and assign operator
		* @param argb color value, a 4-bytes number
		* @return reference of this object
		*/
		CGUIColor& operator-=(GUIARGB argb);

		/** 
		* @brief * operator
		* @param fValue multiplication factor
		* @return a temporary object
		*/
		CGUIColor operator*(const real fValue) const;

		/** 
		* @brief * and assign operator
		* @param fValue multiplication factor
		* @return reference of this object
		*/
		CGUIColor& 	operator*=(const real fValue);

		/** 
		* @brief * operator
		* @param rColor reference of CGUIColor object.
		* @return a temporary object
		*/
		CGUIColor operator*(const CGUIColor& rColor) const;

		/** 
		* @brief * operator
		* @param argb color value, a 4-bytes number
		* @return a temporary object
		*/
		CGUIColor operator*(GUIARGB argb) const;

		/** 
		* @brief * and assign operator
		* @param rColor reference of CGUIColor object
		* @return reference of this object
		*/
		CGUIColor& operator*=(const CGUIColor& rColor);

		/** 
		* @brief * and assign operator
		* @param argb color value, a 4-bytes number
		* @return reference of this object
		*/
		CGUIColor& operator*=(GUIARGB argb);

		/** 
		* @brief compare operator
		* @param rColor reference of CGUIColor
		* @return true for equalization
		*/
		bool	operator==(const CGUIColor &rColor ) const;

		/** 
		* @brief compare operator
		* @param rColor reference of CGUIColor
		* @return true for not equalization
		*/
		bool	operator!=(const CGUIColor &rColor ) const;

		/** 
		* @brief less-than operator
		*/
		bool	operator<(const CGUIColor &rColor ) const;

		/** 
		* @brief greater-than operator
		*/
		bool	operator>(const CGUIColor &rColor ) const;

		virtual void SaveToProperty( CGUIProperty& rProperty ) const;
		virtual void LoadFromProperty( const CGUIProperty& rProperty );


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
	inline GUIARGB		CGUIColor::GetARGB() const
	{
		return m_nARGB;
	}
	//------------------------------------------------------------------------------ 
	inline GUIRGBA		CGUIColor::GetRGBA() const
	{
		return ((m_nARGB<<8) + ((m_nARGB>>24)&0xFF));
	}
	//------------------------------------------------------------------------------ 
	inline void		CGUIColor::SetARGB(GUIARGB argb)
	{
		m_nARGB = argb;
	}
	//------------------------------------------------------------------------------ 
	inline void 		CGUIColor::SetColor(real fRed, real fGreen, real fBlue, real fAlpha/* = 1.0f*/)
	{
		m_nARGB = ((uint32(fAlpha*255.0f)&0xFF)<<24) + ((uint32(fRed*255.0f)&0xFF)<<16) + ((uint32(fGreen*255.0f)&0xFF)<<8) + (uint32(fBlue*255.0f)&0xFF);
	}
	//------------------------------------------------------------------------------ 
	inline void 		CGUIColor::SetColor(const CGUIColor& rColor)
	{
		m_nARGB = rColor.m_nARGB;
	}
	//------------------------------------------------------------------------------ 
	inline real		CGUIColor::GetAlpha() const
	{
		return (real((m_nARGB>>24)&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void	 	CGUIColor::SetAlpha(real fAlpha)
	{
		m_nARGB = (m_nARGB&0x00FFFFFF)+((uint32(fAlpha*255.0f)&0xFF)<<24);
	}
	//------------------------------------------------------------------------------ 
	inline real		CGUIColor::GetRed() const
	{
		return (real((m_nARGB>>16)&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void 		CGUIColor::SetRed(real fRed)
	{
		m_nARGB = (m_nARGB&0xFF00FFFF)+((uint32(fRed*255.0f)&0xFF)<<16);
	}
	//------------------------------------------------------------------------------ 
	inline real		CGUIColor::GetGreen() const
	{
		return (real((m_nARGB>>8)&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void 		CGUIColor::SetGreen(real fGreen)
	{
		m_nARGB = (m_nARGB&0xFFFF00FF)+((uint32(fGreen*255.0f)&0xFF)<<8);
	}
	//------------------------------------------------------------------------------ 
	inline real		CGUIColor::GetBlue() const
	{
		return (real(m_nARGB&0xFF))/255.0f;
	}
	//------------------------------------------------------------------------------ 
	inline void 		CGUIColor::SetBlue(real fBlue)
	{
		m_nARGB = (m_nARGB&0xFFFFFF00)+(uint32(fBlue*255.0f)&0xFF);
	}
	//------------------------------------------------------------------------------ 
	inline void		CGUIColor::InvertColor(void)
	{
		m_nARGB = (m_nARGB&0xFF000000) + (0x00FFFFFF-(m_nARGB&0x00FFFFFF));
	}
	//----------------------------------------------------------------------------- 
	inline void		CGUIColor::InvertColorWithAlpha(void)
	{
		m_nARGB = 0xFFFFFFFF-m_nARGB;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator=(const CGUIColor &rColor)
	{
		SetColor(rColor);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator=(GUIARGB argb)
	{
		SetARGB( argb);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator&=(GUIARGB argb)
	{
		SetARGB(GetARGB() & argb);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator&=(const CGUIColor& rColor)
	{
		SetARGB(GetARGB() & rColor.GetARGB());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator&(GUIARGB argb)
	{
		return CGUIColor(GetARGB() & argb);
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator&(const CGUIColor& rColor)
	{
		return CGUIColor(GetARGB() & rColor.GetARGB());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator|=(GUIARGB argb)
	{
		SetARGB(GetARGB() | argb);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator|=(const CGUIColor& rColor)
	{
		SetARGB(GetARGB() | rColor.GetARGB());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator|(GUIARGB argb)
	{
		return CGUIColor(GetARGB() | argb );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator|(const CGUIColor& rColor)
	{
		return CGUIColor( GetARGB() | rColor.GetARGB());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator<<=(int32 nOffset)
	{
		SetARGB(GetARGB() << nOffset);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator>>=(int32 nOffset)
	{
		SetARGB(GetARGB() >> nOffset);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator<<(int32 nOffset)
	{
		return CGUIColor( GetARGB() << nOffset );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator>>(int32 nOffset)
	{
		return CGUIColor( GetARGB() >> nOffset );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator+(const CGUIColor& rColor) const
	{
		return CGUIColor( GetRed() 	+ rColor.GetRed(),
			GetGreen() 	+ rColor.GetGreen(),
			GetBlue() 	+ rColor.GetBlue(),
			GetAlpha() 	+ rColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator+(GUIARGB argb) const
	{
		CGUIColor aTempColor(argb);

		return CGUIColor( GetRed() 	+ aTempColor.GetRed(),
			GetGreen() 	+ aTempColor.GetGreen(),
			GetBlue() 	+ aTempColor.GetBlue(),
			GetAlpha() 	+ aTempColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator+=(const CGUIColor& rColor)
	{
		SetColor(
			GetRed() + rColor.GetRed(),
			GetGreen() + rColor.GetGreen(),
			GetBlue() + rColor.GetBlue(),
			GetAlpha() + rColor.GetAlpha());

		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator+=(GUIARGB argb)
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
	inline CGUIColor 	CGUIColor::operator-(const CGUIColor& rColor) const
	{
		return CGUIColor( GetRed() 	- rColor.GetRed(),
			GetGreen() 	- rColor.GetGreen(),
			GetBlue() 	- rColor.GetBlue(),
			GetAlpha() 	- rColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator-(GUIARGB argb) const
	{
		CGUIColor aTempColor(argb);
		return CGUIColor( GetRed() 	- aTempColor.GetRed(),
			GetGreen() 	- aTempColor.GetGreen(),
			GetBlue() 	- aTempColor.GetBlue(),
			GetAlpha() 	- aTempColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator-=(const CGUIColor& rColor)
	{
		SetColor(
			GetRed() - rColor.GetRed(),
			GetGreen() - rColor.GetGreen(),
			GetBlue() - rColor.GetBlue(),
			GetAlpha() - rColor.GetAlpha());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator-=(GUIARGB argb)
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
	inline CGUIColor 	CGUIColor::operator*(const real fValue) const
	{
		return CGUIColor( GetRed() 	* fValue,
			GetGreen() 	* fValue, 
			GetBlue() 	* fValue,
			GetAlpha() 	* fValue );
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator*=(const real fValue)
	{
		SetColor(
			GetRed() * fValue,
			GetGreen() * fValue,
			GetBlue() * fValue,
			GetAlpha() * fValue);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator*(const CGUIColor& rColor) const
	{
		return CGUIColor( GetRed() 	* rColor.GetRed(),
			GetGreen() 	* rColor.GetGreen(),
			GetBlue() 	* rColor.GetBlue(),
			GetAlpha() 	* rColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor 	CGUIColor::operator*(GUIARGB argb) const
	{
		CGUIColor aTempColor(argb);
		return CGUIColor( GetRed() 	* aTempColor.GetRed(),
			GetGreen() 	* aTempColor.GetGreen(),
			GetBlue() 	* aTempColor.GetBlue(),
			GetAlpha() 	* aTempColor.GetAlpha());
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator*=(const CGUIColor& rColor)
	{
		SetColor(
			GetRed() * rColor.GetRed(),
			GetGreen() * rColor.GetGreen(),
			GetBlue() * rColor.GetBlue(),
			GetAlpha() * rColor.GetAlpha());
		return *this;
	}
	//------------------------------------------------------------------------------ 
	inline CGUIColor& 	CGUIColor::operator*=(GUIARGB argb)
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
	inline bool		CGUIColor::operator==(const CGUIColor &rColor ) const
	{
		return GetARGB() == rColor.GetARGB();
	}
	//------------------------------------------------------------------------------ 
	inline bool		CGUIColor::operator!=(const CGUIColor &rColor ) const
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

