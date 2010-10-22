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
#include <vector>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIStringConvertor
	* @brief utility class, convert value between string and other values.
	*/ 
	class GUIEXPORT CGUIStringConvertor
	{
	public:
		/// convert string to bool
		static bool					StringToBool( const CGUIString& rStringValue );

		/// convert bool to string
		static CGUIString			BoolToString( bool bValue );

		/// convert string to uint32
		static uint32				StringToUInt( const CGUIString& rStringValue );

		/// convert uint32 to string
		static CGUIString			UIntToString( uint32 nValue );


		/// convert string to int32
		static uint32				StringToInt( const CGUIString& rStringValue );

		/// convert int32 to string
		static CGUIString			IntToString( int32 nValue );

		/// convert string to real
		static real					StringToReal( const CGUIString& rStringValue );

		/// convert real to string
		static CGUIString			RealToString( real fValue );

		/**
		* @brief convert string to CGUIRect.
		* the format of string is "left, top,right,bottom"
		*/
		static CGUIRect				StringToRect( const CGUIString& rStringValue );

		/**
		* @brief convert CGUIRect to string.
		* the format of string is "left, top,right,bottom"
		*/
		static CGUIString			RectToString( const CGUIRect& rRect);



		/**
		* @brief convert string to CGUISize.
		* the format of string is "width,height"
		*/
		static CGUISize				StringToSize( const CGUIString& rStringValue );
		static void					StringToSize( const CGUIString& rStringValue, CGUISize& rSize );

		/**
		* @brief convert CGUISize to string.
		* the format of string is "width,height"
		*/
		static CGUIString			SizeToString( const CGUISize& rSize);


		/**
		* @brief convert string to CGUIVector2.
		* the format of string is "x,y"
		*/
		static CGUIVector2			StringToVector2( const CGUIString& rStringValue );

		/**
		* @brief convert CGUIVector2 to string.
		* the format of string is "x,y"
		*/
		static CGUIString			Vector2ToString( const CGUIVector2& rVector2);

		/**
		* @brief convert string to CGUIColor.
		* the format of string is "r,g,b,a"
		*/
		static CGUIColor			StringToColor( const CGUIString& rStringValue );

		/**
		* @brief convert CGUIColor to string.
		* the format of string is "r,g,b,a"
		*/
		static CGUIString			ColorToString( const CGUIColor& rColor);




		/**
		* @brief convert string to EImageOperation.
		* the format of string is "IMAGE_ROTATE90CW"
		*/
		static EImageOperation		StringToImageOperation( const CGUIString& rStringValue );

		/**
		* @brief convert EImageOperation to string.
		* the format of string is "IMAGE_ROTATE90CW"
		*/
		static CGUIString			ImageOperationToString( EImageOperation eOperation);


		/**
		* @brief convert string to EAlignMode.
		* the format of string is "eRelative"
		*/
		static EAlignMode			StringToAlignMode( const CGUIString& rStringValue );

		/**
		* @brief convert EAlignMode to string.
		* the format of string is "eRelative"
		*/
		static CGUIString			AlignModeToString( EAlignMode eAlignMode);




		/**
		 * @brief convert multi byte to wide char (utf16)
		 * @param szFromCode the default will use the global code which was set before.
		 * @return zero for success
		 */
		static int					MultiByteToWideChar( const CGUIString& rSrc, CGUIStringEx& rDst, const char* szFromCode = NULL );

		/**
		* @brief convert wide byte to multi char
		* @param szFromCode the default will use the global code which was set before.
		* @return zero for success
		*/
		static int					WideByteToMultiChar( const CGUIStringEx& rSrc, CGUIString& rDst,  const char* szFromCode = NULL );
	};

}	//namespace guiex

#endif //__GUI_STRING_CONVERTOR_200608030_H__
