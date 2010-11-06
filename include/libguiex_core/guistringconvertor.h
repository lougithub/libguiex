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
#include "guicolor.h"
#include "guiwidgetsystem.h"
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

	extern std::vector<CGUIString>	StringToVector(const CGUIString& rString );

	/**
	* @brief convert multi byte to wide char (utf16)
	* @param szFromCode the default will use the global code which was set before.
	* @return zero for success
	*/
	extern int MultiByteToWideChar( const CGUIString& rSrc, CGUIStringEx& rDst, const char* szFromCode = NULL );

	/**
	* @brief convert wide byte to multi char
	* @param szFromCode the default will use the global code which was set before.
	* @return zero for success
	*/
	extern int WideByteToMultiChar( const CGUIStringEx& rSrc, CGUIString& rDst,  const char* szFromCode = NULL );

	//------------------------------------------------------------------------------
	//convert for bool
	template< >
	int32 StringToValue<bool>( const CGUIString& rString, bool& rValue);
	template< >
	int32 ValueToString<bool>( const bool& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for uint32
	template< >
	int32 StringToValue<uint32>( const CGUIString& rString, uint32& rValue);
	template< >
	int32 ValueToString<uint32>( const uint32& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for int32
	template<  >
	int32 StringToValue<int32>( const CGUIString& rString, int32& rValue);
	template<  >
	int32 ValueToString<int32>( const int32& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for uint16
	template<  >
	int32 StringToValue<uint16>( const CGUIString& rString, uint16& rValue);
	template<  >
	int32 ValueToString<uint16>( const uint16& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for int16
	template<  >
	int32 StringToValue<int16>( const CGUIString& rString, int16& rValue);
	template<  >
	int32 ValueToString<int16>( const int16& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for real
	template<  >
	int32 StringToValue<real>( const CGUIString& rString, real& rValue);
	template<  >
	int32 ValueToString<real>( const real& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for CGUIRect
	template<  >
	int32 StringToValue<CGUIRect>( const CGUIString& rString, CGUIRect& rValue);
	template<  >
	int32 ValueToString<CGUIRect>( const CGUIRect& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for CGUISize
	template<  >
	int32 StringToValue<CGUISize>( const CGUIString& rString, CGUISize& rValue);
	template<  >
	int32 ValueToString<CGUISize>( const CGUISize& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for CGUIVector2
	template<  >
	int32 StringToValue<CGUIVector2>( const CGUIString& rString, CGUIVector2& rValue);
	template<  >
	int32 ValueToString<CGUIVector2>( const CGUIVector2& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for CGUIColor
	template<  >
	int32 StringToValue<CGUIColor>( const CGUIString& rString, CGUIColor& rValue);
	template<  >
	int32 ValueToString<CGUIColor>( const CGUIColor& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for EImageOperation
	template<  >
	int32 StringToValue<EImageOperation>( const CGUIString& rString, EImageOperation& rValue);
	template<  >
	int32 ValueToString<EImageOperation>( const EImageOperation& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for EScreenValue
	template<  >
	int32 StringToValue<EScreenValue>( const CGUIString& rString, EScreenValue& rValue);
	template<  >
	int32 ValueToString<EScreenValue>( const EScreenValue& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentHorz
	template<  >
	int32 StringToValue<ETextAlignmentHorz>( const CGUIString& rString, ETextAlignmentHorz& rValue);
	template<  >
	int32 ValueToString<ETextAlignmentHorz>( const ETextAlignmentHorz& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentVert
	template<  >
	int32 StringToValue<ETextAlignmentVert>( const CGUIString& rString, ETextAlignmentVert& rValue);
	template<  >
	int32 ValueToString<ETextAlignmentVert>( const ETextAlignmentVert& rValue, CGUIString& rString );
	//------------------------------------------------------------------------------

}	//namespace guiex

#endif //__GUI_STRING_CONVERTOR_200608030_H__
