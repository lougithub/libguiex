/** 
* @file guistringconvertor.cpp
* @brief convert string from other value or
* convert other value to string
* @author ken
* @date 2006-08-30
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacestringconv.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	std::vector<CGUIString>	StringToVector(const CGUIString& rString )
	{
		std::vector<CGUIString> aListString;
		CGUIString::size_type idx = 0;

		while( idx < rString.size())
		{
			CGUIString::size_type ret = rString.find_first_of(",", idx);
			aListString.push_back(rString.substr(idx, ret));
			if( ret == CGUIString::npos )
			{
				break;
			}
			else
			{
				idx = ret+1;
			}
		}
		return aListString;
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	int WideByteToMultiChar( const CGUIStringW& rSrc, CGUIString& rDst,  const char* szFromCode )
	{
		const char* pMultiByteCode = szFromCode ? szFromCode:CGUIWidgetSystem::Instance()->GetDefaultCode();
		if( strcmp( pMultiByteCode, "UTF-8" ) != 0 )
		{
			throw CGUIException(
				"[WideByteToMultiChar]: not supported multi byte code <%s>",
				pMultiByteCode );
			return -1;
		}

		IGUIInterfaceStringConv* pStringConv = CGUIInterfaceManager::Instance()->GetInterfaceStringConv();
		if( !pStringConv )
		{
			throw CGUIException(
				"[WideByteToMultiChar]: not found interface to convert string code");
			return -1;
		}
		return pStringConv->Utf16ToUtf8( rSrc, rDst );
	}
	//------------------------------------------------------------------------------
	int MultiByteToWideChar( const CGUIString& rSrc, CGUIStringW& rDst, const char* szFromCode)
	{
		const char* pMultiByteCode = szFromCode ? szFromCode:CGUIWidgetSystem::Instance()->GetDefaultCode();
		if( strcmp( pMultiByteCode, "UTF-8" ) != 0 )
		{
			throw CGUIException(
				"[MultiByteToWideChar]: not supported multi byte code <%s>",
				pMultiByteCode );
			return -1;
		}

		IGUIInterfaceStringConv* pStringConv = CGUIInterfaceManager::Instance()->GetInterfaceStringConv();
		if( !pStringConv )
		{
			throw CGUIException(
				"[MultiByteToWideChar]: not found interface to convert string code");
			return -1;
		}
		return pStringConv->Utf8ToUtf16( rSrc, rDst );
	}
	//------------------------------------------------------------------------------
	//convert for bool
	template< >
	CGUIString GetValueType<bool>( )
	{
		return "bool";
	}
	template< >
	int32 StringToValue<bool>( const CGUIString& rString, bool& rValue)
	{
		if( rString == "true")
		{
			rValue = true;
			return 0;
		}
		else if (  rString == "false")
		{
			rValue = false;
			return 0;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[bool]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
	}

	template< >
	int32 ValueToString<bool>( const bool& rValue, CGUIString& rString)
	{
		rString = rValue?"true":"false";
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for uint32
	template< >
	CGUIString GetValueType<uint32>( )
	{
		return "uint32";
	}
	template< >
	int32 StringToValue<uint32>( const CGUIString& rString, uint32& rValue)
	{
		rValue = static_cast<uint32>(strtoul(rString.c_str(), 0, 10));

		return 0;
	}

	template< >
	int32 ValueToString<uint32>( const uint32& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for int32
	template< >
	CGUIString GetValueType<int32>( )
	{
		return "int32";
	}
	template<  >
	int32 StringToValue<int32>( const CGUIString& rString, int32& rValue)
	{
		rValue = static_cast<int32>(strtol(rString.c_str(), 0, 10));

		return 0;
	}

	template<  >
	int32 ValueToString<int32>( const int32& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for uint16
	template< >
	CGUIString GetValueType<uint16>( )
	{
		return "uint16";
	}
	template<  >
	int32 StringToValue<uint16>( const CGUIString& rString, uint16& rValue)
	{
		rValue = static_cast<uint16>(strtoul(rString.c_str(), 0, 10));

		return 0;
	}

	template<  >
	int32 ValueToString<uint16>( const uint16& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for int16
	template< >
	CGUIString GetValueType<int16>( )
	{
		return "int16";
	}
	template<  >
	int32 StringToValue<int16>( const CGUIString& rString, int16& rValue)
	{
		rValue = static_cast<int16>(strtol(rString.c_str(), 0, 10));

		return 0;
	}

	template<  >
	int32 ValueToString<int16>( const int16& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for real
	template< >
	CGUIString GetValueType<real>( )
	{
		return "real";
	}
	template<  >
	int32 StringToValue<real>( const CGUIString& rString, real& rValue)
	{
		rValue = static_cast<real>(strtod(rString.c_str(), 0));

		return 0;
	}

	template<  >
	int32 ValueToString<real>( const real& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIRect
	template< >
	CGUIString GetValueType<CGUIRect>( )
	{
		return "CGUIRect";
	}
	template<  >
	int32 StringToValue<CGUIRect>( const CGUIString& rString, CGUIRect& rValue)
	{
		//string should have format as "left, top, right,bottom"
		std::vector<CGUIString> aListString= StringToVector(rString);
		if( aListString.size() != 4 )
		{
			throw CGUIException(
				"[StringToValue[CGUIRect]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue( aListString[0], rValue.m_fLeft );
		StringToValue( aListString[1], rValue.m_fTop );
		StringToValue( aListString[2], rValue.m_fRight );
		StringToValue( aListString[3], rValue.m_fBottom );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIRect>( const CGUIRect& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.m_fLeft<<','<<rValue.m_fTop<<','<<rValue.m_fRight<<','<<rValue.m_fBottom;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUISize
	template< >
	CGUIString GetValueType<CGUISize>( )
	{
		return "CGUISize";
	}
	template<  >
	int32 StringToValue<CGUISize>( const CGUIString& rString, CGUISize& rValue)
	{
		//string should have format as "width,height"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[StringToValue[CGUISize]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue( aListString[0], rValue.m_fWidth );
		StringToValue( aListString[1], rValue.m_fHeight );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUISize>( const CGUISize& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.m_fWidth<<','<<rValue.m_fHeight;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIVector2
	template< >
	CGUIString GetValueType<CGUIVector2>( )
	{
		return "CGUIVector2";
	}
	template<  >
	int32 StringToValue<CGUIVector2>( const CGUIString& rString, CGUIVector2& rValue)
	{
		//string should have format as "x,y"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue(aListString[0], rValue.x);
		StringToValue(aListString[1], rValue.y);
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIVector2>( const CGUIVector2& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.x<<','<<rValue.y;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIVector3
	template< >
	CGUIString GetValueType<CGUIVector3>( )
	{
		return "CGUIVector3";
	}
	template<  >
	int32 StringToValue<CGUIVector3>( const CGUIString& rString, CGUIVector3& rValue)
	{
		//string should have format as "x,y,z"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 3 )
		{
			throw CGUIException(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue(aListString[0], rValue.x);
		StringToValue(aListString[1], rValue.y);
		StringToValue(aListString[2], rValue.z);
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIVector3>( const CGUIVector3& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.x<<','<<rValue.y<<','<<rValue.z;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIColor
	template< >
	CGUIString GetValueType<CGUIColor>( )
	{
		return "CGUIColor";
	}
	template<  >
	int32 StringToValue<CGUIColor>( const CGUIString& rString, CGUIColor& rValue)
	{
		//string should have format as "r,g,b,a"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 4 )
		{
			throw CGUIException(
				"[StringToValue[CGUIColor]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		real fColor;
		StringToValue(aListString[0], fColor );
		rValue.SetRed( fColor );
		StringToValue(aListString[1], fColor );
		rValue.SetGreen( fColor );
		StringToValue(aListString[2], fColor );
		rValue.SetBlue( fColor );
		StringToValue(aListString[3], fColor );
		rValue.SetAlpha( fColor );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIColor>( const CGUIColor& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.GetRed()<<','<<rValue.GetGreen()<<','<<rValue.GetBlue()<<','<<rValue.GetAlpha();
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EImageOrientation
	template< >
	CGUIString GetValueType<EImageOrientation>( )
	{
		return "EImageOrientation";
	}
	template<  >
	int32 StringToValue<EImageOrientation>( const CGUIString& rString, EImageOrientation& rValue)
	{
		if( rString == "eImageOrientation_Normal" )
		{
			rValue = eImageOrientation_Normal;
		}
		else if( rString == "eImageOrientation_90CW" )
		{
			rValue = eImageOrientation_90CW;
		}
		else if( rString == "eImageOrientation_90CCW" )
		{
			rValue = eImageOrientation_90CCW;
		}
		else if( rString == "eImageOrientation_FlipHorizon" )
		{
			rValue = eImageOrientation_FlipHorizon;
		}
		else if( rString == "eImageOrientation_FlipVertical" )
		{
			rValue = eImageOrientation_FlipVertical;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[EImageOrientation]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EImageOrientation>( const EImageOrientation& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eImageOrientation_Normal:
			rString = CGUIString("eImageOrientation_Normal");
			break;
		case eImageOrientation_90CW:
			rString =  CGUIString("eImageOrientation_90CW");
			break;
		case eImageOrientation_90CCW:
			rString =  CGUIString("eImageOrientation_90CCW");
			break;
		case eImageOrientation_FlipHorizon:
			rString =  CGUIString("eImageOrientation_FlipHorizon");
			break;
		case eImageOrientation_FlipVertical:
			rString =  CGUIString("eImageOrientation_FlipVertical");
			break;
		default:
			throw CGUIException(
				"[ValueToString[EImageOrientation]]: unknown image orientation <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EScreenValue
	template< >
	CGUIString GetValueType<EScreenValue>( )
	{
		return "EScreenValue";
	}
	template<  >
	int32 StringToValue<EScreenValue>( const CGUIString& rString, EScreenValue& rValue)
	{
		if( rString == "eScreenValue_Pixel" )
		{
			rValue = eScreenValue_Pixel;
		}
		else if( rString == "eScreenValue_Percentage" )
		{
			rValue = eScreenValue_Percentage;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[EScreenValue]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EScreenValue>( const EScreenValue& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eScreenValue_Pixel:
			rString = CGUIString("eScreenValue_Pixel");
			break;
		case eScreenValue_Percentage:
			rString = CGUIString("eScreenValue_Percentage");
			break;
		default:
			throw CGUIException(
				"[ValueToString[EScreenValue]]: unknown image orientation <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentHorz
	template< >
	CGUIString GetValueType<ETextAlignmentHorz>( )
	{
		return "ETextAlignmentHorz";
	}
	template<  >
	int32 StringToValue<ETextAlignmentHorz>( const CGUIString& rString, ETextAlignmentHorz& rValue)
	{
		if( rString == "eTextAlignment_Horz_Left" )
		{
			rValue = eTextAlignment_Horz_Left;
		}
		else if( rString == "eTextAlignment_Horz_Right" )
		{
			rValue = eTextAlignment_Horz_Right;
		}
		else if( rString == "eTextAlignment_Horz_Center" )
		{
			rValue = eTextAlignment_Horz_Center;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[ETextAlignmentHorz]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<ETextAlignmentHorz>( const ETextAlignmentHorz& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eTextAlignment_Horz_Left:
			rString = CGUIString("eTextAlignment_Horz_Left");
			break;
		case eTextAlignment_Horz_Right:
			rString = CGUIString("eTextAlignment_Horz_Right");
			break;
		case eTextAlignment_Horz_Center:
			rString = CGUIString("eTextAlignment_Horz_Center");
			break;
		default:
			throw CGUIException(
				"[ValueToString[ETextAlignmentHorz]]: unknown ETextAlignmentHorz enum <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentVert
	template< >
	CGUIString GetValueType<ETextAlignmentVert>( )
	{
		return "ETextAlignmentVert";
	}
	template<  >
	int32 StringToValue<ETextAlignmentVert>( const CGUIString& rString, ETextAlignmentVert& rValue)
	{
		if( rString == "eTextAlignment_Vert_Up" )
		{
			rValue = eTextAlignment_Vert_Up;
		}
		else if( rString == "eTextAlignment_Vert_Down" )
		{
			rValue = eTextAlignment_Vert_Down;
		}
		else if( rString == "eTextAlignment_Vert_Center" )
		{
			rValue = eTextAlignment_Vert_Center;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[ETextAlignmentVert]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<ETextAlignmentVert>( const ETextAlignmentVert& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eTextAlignment_Vert_Up:
			rString = CGUIString("eTextAlignment_Vert_Up");
			break;
		case eTextAlignment_Vert_Down:
			rString = CGUIString("eTextAlignment_Vert_Down");
			break;
		case eTextAlignment_Vert_Center:
			rString = CGUIString("eTextAlignment_Vert_Center");
			break;
		default:
			throw CGUIException(
				"[ValueToString[ETextAlignmentVert]]: unknown ETextAlignmentVert enum <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EInterpolationType
	template< >
	CGUIString GetValueType<EInterpolationType>( )
	{
		return "EInterpolationType";
	}
	template<  >
	int32 StringToValue<EInterpolationType>( const CGUIString& rString, EInterpolationType& rValue)
	{
		if( rString == "eInterpolationType_Linear" )
		{
			rValue = eInterpolationType_Linear;
		}
		else if( rString == "eInterpolationType_EaseIn" )
		{
			rValue = eInterpolationType_EaseIn;
		}
		else if( rString == "eInterpolationType_EaseInOut" )
		{
			rValue = eInterpolationType_EaseInOut;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[EInterpolationType]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EInterpolationType>( const EInterpolationType& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eInterpolationType_Linear:
			rString = CGUIString("eInterpolationType_Linear");
			break;
		case eInterpolationType_EaseIn:
			rString = CGUIString("eInterpolationType_EaseIn");
			break;
		case eInterpolationType_EaseInOut:
			rString = CGUIString("eInterpolationType_EaseInOut");
			break;
		default:
			throw CGUIException(
				"[ValueToString[EInterpolationType]]: unknown EInterpolationType enum <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
 