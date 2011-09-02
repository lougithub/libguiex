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
#include "guistringconvertor.h"
#include "guiinterfacemanager.h"
#include "guiinterfacestringconv.h"
#include <limits>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

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
			CGUIString::size_type ret = rString.find(",", idx);
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
	int AppWideByteToMultiChar( const CGUIStringW& rSrc, CGUIString& rDst )
	{
		IGUIInterfaceStringConv* pStringConv = CGUIInterfaceManager::Instance()->GetInterfaceStringConv();
		if( !pStringConv )
		{
			GUI_THROW( "[AppWideByteToMultiChar]: not found interface to convert string code");
			return -1;
		}
		return pStringConv->WCharToUtf8( rSrc, rDst );
	}
	//------------------------------------------------------------------------------
	CGUIString AppWideByteToMultiChar( const CGUIStringW& rSrc )
	{
		CGUIString strDst;
		AppWideByteToMultiChar( rSrc, strDst );
		return strDst;
	}
	//------------------------------------------------------------------------------
	int AppMultiByteToWideChar( const CGUIString& rSrc, CGUIStringW& rDst )
	{
		IGUIInterfaceStringConv* pStringConv = CGUIInterfaceManager::Instance()->GetInterfaceStringConv();
		if( !pStringConv )
		{
			GUI_THROW( "[AppMultiByteToWideChar]: not found interface to convert string code");
			return -1;
		}
		return pStringConv->Utf8ToWChar( rSrc, rDst );
	}
	//------------------------------------------------------------------------------
	CGUIStringW AppMultiByteToWideChar( const CGUIString& rSrc )
	{
		CGUIStringW strDst;
		AppMultiByteToWideChar( rSrc, strDst );
		return strDst;
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[bool]]: string value format is wrong! <%s>",
				rString.c_str()));
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
		unsigned long value = strtoul(rString.c_str(), 0, 10);
		if( value > std::numeric_limits<uint32>::max() )
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[uint32]]: Value <%s> exceeds range of destination type",
				rString.c_str()));
			return -1;
		}
		rValue = static_cast<uint32>(value);

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
		long value = strtol(rString.c_str(), 0, 10);
		if( value > (long)(std::numeric_limits<int32>::max()))
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[int32]]: Value <%s> exceeds range of destination type",
				rString.c_str()));
			return -1;
		}
		rValue = static_cast<int32>(value);
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
		unsigned long value = strtoul(rString.c_str(), 0, 10);
		if( value > std::numeric_limits<uint16>::max() )
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[uint16]]: Value <%s> exceeds range of destination type",
				rString.c_str()));
			return -1;
		}
		rValue = static_cast<uint16>(value);
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
		long value = strtol(rString.c_str(), 0, 10);
		if( value > (long)(std::numeric_limits<int16>::max()))
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[int16]]: Value <%s> exceeds range of destination type",
				rString.c_str()));
			return -1;
		}
		rValue = static_cast<int16>(value);
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
	//convert for uint8
	template< >
	CGUIString GetValueType<uint8>( )
	{
		return "uint8";
	}
	template<  >
	int32 StringToValue<uint8>( const CGUIString& rString, uint8& rValue)
	{
		unsigned long value = strtoul(rString.c_str(), 0, 10);
		if( value > std::numeric_limits<uint8>::max() )
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[uint8]]: Value <%s> exceeds range of destination type",
				rString.c_str()));
			return -1;
		}
		rValue = static_cast<uint8>(value);
		return 0;
	}

	template<  >
	int32 ValueToString<uint8>( const uint8& rValue, CGUIString& rString )
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[CGUIRect]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[CGUISize]]: string value format is wrong! <%s>",
				rString.c_str()));
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
	//convert for CGUIIntSize
	template< >
	CGUIString GetValueType<CGUIIntSize>( )
	{
		return "CGUIIntSize";
	}
	template<  >
	int32 StringToValue<CGUIIntSize>( const CGUIString& rString, CGUIIntSize& rValue)
	{
		//string should have format as "width,height"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 2 )
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[CGUIIntSize]]: string value format is wrong! <%s>",
				rString.c_str()));
			return -1;
		}

		StringToValue( aListString[0], rValue.m_uWidth );
		StringToValue( aListString[1], rValue.m_uHeight );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIIntSize>( const CGUIIntSize& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.m_uWidth<<','<<rValue.m_uHeight;
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str()));
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
	//convert for CGUIRotator
	template< >
	CGUIString GetValueType<CGUIRotator>( )
	{
		return "CGUIRotator";
	}
	template<  >
	int32 StringToValue<CGUIRotator>( const CGUIString& rString, CGUIRotator& rValue)
	{
		//string should have format as "pitch,yaw,roll"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 3 )
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str()));
			return -1;
		}

		StringToValue(aListString[0], rValue.Pitch);
		StringToValue(aListString[1], rValue.Yaw);
		StringToValue(aListString[2], rValue.Roll);
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIRotator>( const CGUIRotator& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.Pitch<<','<<rValue.Yaw<<','<<rValue.Roll;
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[CGUIColor]]: string value format is wrong! <%s>",
				rString.c_str()));
			return -1;
		}

		uint8 nColor;
		StringToValue(aListString[0], nColor );
		rValue.SetRed( nColor / 255.0f );
		StringToValue(aListString[1], nColor );
		rValue.SetGreen( nColor / 255.0f );
		StringToValue(aListString[2], nColor );
		rValue.SetBlue( nColor / 255.0f );
		StringToValue(aListString[3], nColor );
		rValue.SetAlpha( nColor / 255.0f );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIColor>( const CGUIColor& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << uint16( rValue.GetRed()*255 )
			<<','<<uint16( rValue.GetGreen()*255 )
			<<','<<uint16( rValue.GetBlue()*255 )
			<<','<<uint16( rValue.GetAlpha()*255 );
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[EImageOrientation]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[EImageOrientation]]: unknown image orientation <%d>",
				rValue));
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[EScreenValue]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[EScreenValue]]: unknown image orientation <%d>",
				rValue));
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[ETextAlignmentHorz]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[ETextAlignmentHorz]]: unknown ETextAlignmentHorz enum <%d>",
				rValue));
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[ETextAlignmentVert]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[ETextAlignmentVert]]: unknown ETextAlignmentVert enum <%d>",
				rValue));
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
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[EInterpolationType]]: string value format is wrong! <%s>",
				rString.c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[EInterpolationType]]: unknown EInterpolationType enum <%d>",
				rValue));
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EParticle2DSystemMode
	template< >
	CGUIString GetValueType<EParticle2DSystemMode>( )
	{
		return "EParticle2DSystemMode";
	}
	template<  >
	int32 StringToValue<EParticle2DSystemMode>( const CGUIString& rString, EParticle2DSystemMode& rValue)
	{
		if( rString == "eParticle2DSystemMode_Gravity" )
		{
			rValue = eParticle2DSystemMode_Gravity;
		}
		else if( rString == "eParticle2DSystemMode_Radius" )
		{
			rValue = eParticle2DSystemMode_Radius;
		}
		else
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[EParticle2DSystemMode]]: string value format is wrong! <%s>",
				rString.c_str()));
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EParticle2DSystemMode>( const EParticle2DSystemMode& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eParticle2DSystemMode_Gravity:
			rString = CGUIString("eParticle2DSystemMode_Gravity");
			break;
		case eParticle2DSystemMode_Radius:
			rString = CGUIString("eParticle2DSystemMode_Radius");
			break;
		default:
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[EParticle2DSystemMode]]: unknown EParticle2DSystemMode enum <%d>",
				rValue));
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EBlendFunc
	template< >
	CGUIString GetValueType<EBlendFunc>( )
	{
		return "EBlendFunc";
	}
	template<  >
	int32 StringToValue<EBlendFunc>( const CGUIString& rString, EBlendFunc& rValue)
	{
		if( rString == "eBlendFunc_ZERO" )
		{
			rValue = eBlendFunc_ZERO;
		}
		else if( rString == "eBlendFunc_ONE" )
		{
			rValue = eBlendFunc_ONE;
		}
		else if( rString == "eBlendFunc_SRC_COLOR" )
		{
			rValue = eBlendFunc_SRC_COLOR;
		}
		else if( rString == "eBlendFunc_ONE_MINUS_SRC_COLOR" )
		{
			rValue = eBlendFunc_ONE_MINUS_SRC_COLOR;
		}
		else if( rString == "eBlendFunc_SRC_ALPHA" )
		{
			rValue = eBlendFunc_SRC_ALPHA;
		}
		else if( rString == "eBlendFunc_ONE_MINUS_SRC_ALPHA" )
		{
			rValue = eBlendFunc_ONE_MINUS_SRC_ALPHA;
		}
		else if( rString == "eBlendFunc_DST_ALPHA" )
		{
			rValue = eBlendFunc_DST_ALPHA;
		}
		else if( rString == "eBlendFunc_ONE_MINUS_DST_ALPHA" )
		{
			rValue = eBlendFunc_ONE_MINUS_DST_ALPHA;
		}
		else
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[EBlendFunc]]: string value format is wrong! <%s>",
				rString.c_str()));
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EBlendFunc>( const EBlendFunc& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eBlendFunc_ZERO:
			rString = CGUIString("eBlendFunc_ZERO");
			break;
		case eBlendFunc_ONE:
			rString = CGUIString("eBlendFunc_ONE");
			break;
		case eBlendFunc_SRC_COLOR:
			rString = CGUIString("eBlendFunc_SRC_COLOR");
			break;
		case eBlendFunc_ONE_MINUS_SRC_COLOR:
			rString = CGUIString("eBlendFunc_ONE_MINUS_SRC_COLOR");
			break;
		case eBlendFunc_SRC_ALPHA:
			rString = CGUIString("eBlendFunc_SRC_ALPHA");
			break;
		case eBlendFunc_ONE_MINUS_SRC_ALPHA:
			rString = CGUIString("eBlendFunc_ONE_MINUS_SRC_ALPHA");
			break;
		case eBlendFunc_DST_ALPHA:
			rString = CGUIString("eBlendFunc_DST_ALPHA");
			break;
		case eBlendFunc_ONE_MINUS_DST_ALPHA:
			rString = CGUIString("eBlendFunc_ONE_MINUS_DST_ALPHA");
			break;
		default:
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[EBlendFunc]]: unknown EBlendFunc enum <%d>",
				rValue));
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EOrientation
	template< >
	CGUIString GetValueType<EOrientation>( )
	{
		return "EOrientation";
	}
	template<  >
	int32 StringToValue<EOrientation>( const CGUIString& rString, EOrientation& rValue)
	{
		if( rString == "eOrientation_Vertical" )
		{
			rValue = eOrientation_Vertical;
		}
		else if( rString == "eOrientation_Horizontal" )
		{
			rValue = eOrientation_Horizontal;
		}
		else
		{
			GUI_THROW( GUI_FORMAT(
				"[StringToValue[EOrientation]]: string value format is wrong! <%s>",
				rString.c_str()));
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EOrientation>( const EOrientation& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eOrientation_Vertical:
			rString = CGUIString("eOrientation_Vertical");
			break;
		case eOrientation_Horizontal:
			rString = CGUIString("eOrientation_Horizontal");
			break;
		default:
			GUI_THROW( GUI_FORMAT(
				"[ValueToString[EOrientation]]: unknown EOrientation enum <%d>",
				rValue));
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
 