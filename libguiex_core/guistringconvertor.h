/** 
* @file guistringconvertor.h
* @brief convert string from other value or
* convert other value to string
* @author ken
* @date 2006-08-30
*/

#ifndef __GUI_STRING_CONVERTOR_200608030_H__
#define __GUI_STRING_CONVERTOR_200608030_H__

//============================================================================//
// include
//============================================================================// 
#include "guistring.h"
#include "guistringex.h"
#include "guirect.h"
#include "guisize.h"
#include "guiintsize.h"
#include "guivector2.h"
#include "guivector3.h"
#include "guicolor.h"
#include "guisystem.h"
#include "guiexception.h"
#include <vector>
#include <sstream>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//return 0 for success.
	template< class ValueType >
	int32 StringToValue( const CGUIString& rString, ValueType& rValue)
	{
		throw CGUIException(
			"[StringToValue]: unknown value type for ! <%s>",
			rString.c_str());
		return -1;
	}
	
	//return 0 for success.
	template< class ValueType >
	int32 ValueToString( const ValueType& rValue, CGUIString& rString )
	{
		throw CGUIException(
			"[ValueToString]: unknown value type");
		return -1;
	}

	template< class ValueType >
	CGUIString GetValueType(  )
	{
		throw CGUIException(
			"[GetValueType]: unknown value type");
		return CGUIString();
	}

	extern std::vector<CGUIString>	StringToVector(const CGUIString& rString );

	/**
	* @brief convert multi byte to wide char (utf16)
	* @return zero for success
	*/
	extern int AppMultiByteToWideChar( const CGUIString& rSrc, CGUIStringW& rDst );
	extern CGUIStringW AppMultiByteToWideChar( const CGUIString& rSrc );

	/**
	* @brief convert wide byte to multi char
	* @return zero for success
	*/
	extern int AppWideByteToMultiChar( const CGUIStringW& rSrc, CGUIString& rDst );
	extern CGUIString AppWideByteToMultiChar( const CGUIStringW& rSrc );

	//------------------------------------------------------------------------------
	//convert for bool
	template< >
	int32 StringToValue<bool>( const CGUIString& rString, bool& rValue);
	template< >
	int32 ValueToString<bool>( const bool& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<bool>( );
		//------------------------------------------------------------------------------
	//convert for uint32
	template< >
	int32 StringToValue<uint32>( const CGUIString& rString, uint32& rValue);
	template< >
	int32 ValueToString<uint32>( const uint32& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<uint32>( );
	//------------------------------------------------------------------------------
	//convert for int32
	template<  >
	int32 StringToValue<int32>( const CGUIString& rString, int32& rValue);
	template<  >
	int32 ValueToString<int32>( const int32& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<int32>( );
	//------------------------------------------------------------------------------
	//convert for uint16
	template<  >
	int32 StringToValue<uint16>( const CGUIString& rString, uint16& rValue);
	template<  >
	int32 ValueToString<uint16>( const uint16& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<uint16>( );
	//------------------------------------------------------------------------------
	//convert for int16
	template<  >
	int32 StringToValue<int16>( const CGUIString& rString, int16& rValue);
	template<  >
	int32 ValueToString<int16>( const int16& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<int16>( );
	//------------------------------------------------------------------------------
	//convert for uint8
	template<  >
	int32 StringToValue<uint8>( const CGUIString& rString, uint8& rValue);
	template<  >
	int32 ValueToString<uint8>( const uint8& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<uint8>( );
	//------------------------------------------------------------------------------
	//convert for real
	template<  >
	int32 StringToValue<real>( const CGUIString& rString, real& rValue);
	template<  >
	int32 ValueToString<real>( const real& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<real>( );
	//------------------------------------------------------------------------------
	//convert for CGUIRect
	template<  >
	int32 StringToValue<CGUIRect>( const CGUIString& rString, CGUIRect& rValue);
	template<  >
	int32 ValueToString<CGUIRect>( const CGUIRect& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<CGUIRect>( );
	//------------------------------------------------------------------------------
	//convert for CGUISize
	template<  >
	int32 StringToValue<CGUISize>( const CGUIString& rString, CGUISize& rValue);
	template<  >
	int32 ValueToString<CGUISize>( const CGUISize& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<CGUISize>( );
	//------------------------------------------------------------------------------
	//convert for CGUIIntSize
	template<  >
	int32 StringToValue<CGUIIntSize>( const CGUIString& rString, CGUIIntSize& rValue);
	template<  >
	int32 ValueToString<CGUIIntSize>( const CGUIIntSize& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<CGUIIntSize>( );
	//------------------------------------------------------------------------------
	//convert for CGUIVector2
	template<  >
	int32 StringToValue<CGUIVector2>( const CGUIString& rString, CGUIVector2& rValue);
	template<  >
	int32 ValueToString<CGUIVector2>( const CGUIVector2& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<CGUIVector2>( );
	//------------------------------------------------------------------------------
	//convert for CGUIVector3
	template<  >
	int32 StringToValue<CGUIVector3>( const CGUIString& rString, CGUIVector3& rValue);
	template<  >
	int32 ValueToString<CGUIVector3>( const CGUIVector3& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<CGUIVector3>( );
	//------------------------------------------------------------------------------
	//convert for CGUIColor
	template<  >
	int32 StringToValue<CGUIColor>( const CGUIString& rString, CGUIColor& rValue);
	template<  >
	int32 ValueToString<CGUIColor>( const CGUIColor& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<CGUIColor>( );
	//------------------------------------------------------------------------------
	//convert for EImageOrientation
	template<  >
	int32 StringToValue<EImageOrientation>( const CGUIString& rString, EImageOrientation& rValue);
	template<  >
	int32 ValueToString<EImageOrientation>( const EImageOrientation& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<EImageOrientation>( );
	//------------------------------------------------------------------------------
	//convert for EScreenValue
	template<  >
	int32 StringToValue<EScreenValue>( const CGUIString& rString, EScreenValue& rValue);
	template<  >
	int32 ValueToString<EScreenValue>( const EScreenValue& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<EScreenValue>( );
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentHorz
	template<  >
	int32 StringToValue<ETextAlignmentHorz>( const CGUIString& rString, ETextAlignmentHorz& rValue);
	template<  >
	int32 ValueToString<ETextAlignmentHorz>( const ETextAlignmentHorz& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<ETextAlignmentHorz>( );
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentVert
	template<  >
	int32 StringToValue<ETextAlignmentVert>( const CGUIString& rString, ETextAlignmentVert& rValue);
	template<  >
	int32 ValueToString<ETextAlignmentVert>( const ETextAlignmentVert& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<ETextAlignmentVert>( );
	//------------------------------------------------------------------------------
	//convert for EInterpolationType
	template<  >
	int32 StringToValue<EInterpolationType>( const CGUIString& rString, EInterpolationType& rValue);
	template<  >
	int32 ValueToString<EInterpolationType>( const EInterpolationType& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<EInterpolationType>( );
	//------------------------------------------------------------------------------
	//convert for EParticle2DSystemMode
	template<  >
	int32 StringToValue<EParticle2DSystemMode>( const CGUIString& rString, EParticle2DSystemMode& rValue);
	template<  >
	int32 ValueToString<EParticle2DSystemMode>( const EParticle2DSystemMode& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<EParticle2DSystemMode>( );
	//------------------------------------------------------------------------------
	//convert for EBlendFunc
	template<  >
	int32 StringToValue<EBlendFunc>( const CGUIString& rString, EBlendFunc& rValue);
	template<  >
	int32 ValueToString<EBlendFunc>( const EBlendFunc& rValue, CGUIString& rString );
	template< >
	CGUIString GetValueType<EBlendFunc>( );
	//------------------------------------------------------------------------------
}	//namespace guiex

#endif //__GUI_STRING_CONVERTOR_200608030_H__
